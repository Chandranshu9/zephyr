/**
 *  @file  auth_lib.c
 *
 *  @brief  Authentication Library functions used to authenticate a
 *          connection between a client and server.
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <init.h>
#include <stdint.h>


#include <logging/log_ctrl.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(auth_lib, CONFIG_AUTH_LOG_LEVEL);

#include <auth/auth_lib.h>
#include "auth_internal.h"


#define AUTH_THRD_STACK_SIZE       (4096u)
#define AUTH_THRD_PRIORITY         (0u)



#if defined(AUTH_INSTANCE_1)
K_SEM_DEFINE(thrd_sem_1, 0, 1);
#endif

#if defined(AUTH_INSTANCE_2)
K_SEM_DEFINE(thrd_sem_2, 0, 1);
#endif

static void auth_thrd_entry(void *, void *, void *);

static struct auth_thread_params thrd_params[CONFIG_NUM_AUTH_INSTANCES] =
{
#if defined(AUTH_INSTANCE_1)
    {.thrd_sem = &thrd_sem_1},
#endif

#if defined(AUTH_INSTANCE_2)
    {.thrd_sem = &thrd_sem_2},
#endif
};

#if defined(AUTH_INSTANCE_1)
K_THREAD_DEFINE(auth_tid_1, AUTH_THRD_STACK_SIZE,
                auth_thrd_entry, &thrd_params[AUTH_INST_1_ID], NULL, NULL,
                AUTH_THRD_PRIORITY, 0, 0);
#endif


#if defined(AUTH_INSTANCE_2)
K_THREAD_DEFINE(auth_tid_2, AUTH_THRD_STACK_SIZE,
                auth_thrd_entry, &thrd_params[AUTH_INST_2_ID], NULL, NULL,
                AUTH_THRD_PRIORITY, 0, 0);
#endif


/**
 * Forward function declarations for authentication threads.
 */
void auth_dtls_thead(struct authenticate_conn *auth_conn);
void auth_chalresp_thread(struct authenticate_conn *auth_conn);


/* ========================== local functions ========================= */

/**
 * Check auth flags consistency.
 *
 * @param flags Flags to check.
 *
 * @return  true if flags are correct, else false.
 */
static bool auth_lib_checkflags(uint32_t flags)
{
    /* stub for now */
    /* TODO: Add code to check for conflicting flags */

    return true;
}


/**
 * Invokes the status callback from the system work queue
 *
 * @param work Pointer to wor item.
 */
static void auth_lib_status_work(struct k_work *work)
{
    struct authenticate_conn *auth_conn =
           CONTAINER_OF(work, struct authenticate_conn, auth_status_work);

    if(!auth_conn) {
        LOG_ERR("Failed to get auth conn struct.");
        return;
    }

    /* invoke callback */
    auth_conn->status_cb(auth_conn, auth_conn->instance, auth_conn->curr_status,
                         auth_conn->callback_context);
}


/**
 * Auth thread starts during boot and waits on semaphore.
 *
 * @param arg1  Pointer to struct auth_thread_parms.
 * @param arg2  Unused
 * @param arg3  Unused
 */
static void auth_thrd_entry(void *arg1, void *arg2, void *arg3)
{
    int ret;
    struct auth_thread_params *thrd_params = (struct auth_thread_params *)arg1;

    while(true) {

        ret = k_sem_take(thrd_params->thrd_sem, K_FOREVER);

        if(ret) {
            LOG_ERR("Failed to get semaphore for auth thread, err: %d", ret);
            LOG_ERR("Auth thread terminating, instance id: %d.", thrd_params->auth_conn->instance);
            return;
        }

        /* call auth thread function */
        thrd_params->auth_conn->auth_func(thrd_params->auth_conn);
    }
}



/* ========================= external API ============================ */


/**
 * @see auth_lib.h
 */
int auth_lib_init(struct authenticate_conn *auth_conn, enum auth_instance_id instance,
                  auth_status_cb_t status_func, void *context, struct auth_optional_param *opt_params,
                  uint32_t auth_flags)
{
    int err = 0;

    /* check input params */
    if(status_func == NULL) {
        LOG_ERR("Error, status function is NULL.");
        return AUTH_ERROR_INVALID_PARAM;
    }

    /* check auth flags */
    if(!auth_lib_checkflags(auth_flags)) {
        LOG_ERR("Invalid auth flags.");
        return AUTH_ERROR_INVALID_PARAM;
    }

    /* init the struct to zero */
    memset(auth_conn, 0, sizeof(struct authenticate_conn));

    /* setup the status callback */
    auth_conn->status_cb = status_func;
    auth_conn->callback_context = context;

    auth_conn->cancel_auth = false;
    auth_conn->instance = instance;

    /* init the work item used to post authentication status */
    k_work_init(&auth_conn->auth_status_work, auth_lib_status_work);

    auth_conn->is_client = (auth_flags & AUTH_CONN_CLIENT) ? true : false;

#if defined(CONFIG_AUTH_DTLS)

    /* Set the DTLS authentication thread */
    auth_conn->auth_func = auth_dtls_thead;
    {
        if(opt_params == NULL || opt_params->param_id != AUTH_TLS_PARAM) {
            LOG_ERR("Missing certificates for TLS/DTLS authentication.");
            return AUTH_ERROR_INVALID_PARAM;
        }

        struct auth_tls_certs *certs = &opt_params->param_body.tls_certs;

        // init TLS layer
        err = auth_init_dtls_method(auth_conn, certs);
    }

    if(err) {
        LOG_ERR("Failed to initialize MBed TLS, err: %d", err);
        return err;
    }
#endif

#if defined(CONFIG_AUTH_CHALLENGE_RESPONSE)

    /* Set the Challenge-Response authentication thread */
    auth_conn->auth_func = auth_chalresp_thread;

    if((opt_params != NULL) && (opt_params->param_id == AUTH_CHALRESP_PARAM)) {

        struct auth_challenge_resp *chal_resp = &opt_params->param_body.chal_resp;

        err = auth_init_chalresp_method(auth_conn, chal_resp);

        if(err) {
            LOG_ERR("Failed to set Challege-Response param, err: %d", err);
            return err;
        }
    }
#endif

    /* set auth connect into thread param instance */
    thrd_params[instance].auth_conn = auth_conn;

    return AUTH_SUCCESS;
}

/**
 * @see auth_lib.h
 */
int auth_lib_deinit(struct authenticate_conn *auth_conn)
{
    /* Free any resources, nothing for now, but maybe
     * needed in the future */
    return AUTH_SUCCESS;
}

/**
 * @see auth_lib.h
 */
int auth_lib_start(struct authenticate_conn *auth_conn)
{
    /* Start the authentication thread */
    k_sem_give(thrd_params[auth_conn->instance].thrd_sem);

    return AUTH_SUCCESS;
}

/**
 * @see auth_lib.h
 */
int auth_lib_cancel(struct authenticate_conn *auth_conn)
{
    auth_conn->cancel_auth = true;

    auth_lib_set_status(auth_conn, AUTH_STATUS_CANCELED);

    return AUTH_SUCCESS;
}

/**
 * @see auth_lib.h
 */
const char *auth_lib_getstatus_str(enum auth_status status)
{
    switch(status) {
        case AUTH_STATUS_STARTED:
            return "Authentication started";
            break;

        case AUTH_STATUS_IN_PROCESS:
            return "In process";
            break;

        case AUTH_STATUS_CANCELED:
            return "Canceled";
            break;

        case AUTH_STATUS_FAILED:
            return "Failure";
            break;

        case AUTH_STATUS_AUTHENTICATION_FAILED:
            return "Authentication Failed";
            break;

        case AUTH_STATUS_SUCCESSFUL:
            return "Authentication Successful";
            break;

        default:
            break;
    }

    return "unknown";
}

/**
 * @see auth_lib.h
 */
enum auth_status auth_lib_get_status(struct authenticate_conn *auth_conn)
{
    return auth_conn->curr_status;
}

/**
 * @see auth_lib.h
 */
void auth_lib_set_status(struct authenticate_conn *auth_conn, enum auth_status status)
{
    auth_conn->curr_status = status;

    if(auth_conn->status_cb) {

        /* submit work item */
        k_work_submit(&auth_conn->auth_status_work);
    }
}


/* ============ Simple ring buffer routines */

/**
 * @note:  The ring buffer uses a simple head/tail index and
 * puts/gets one byte at a time.  This enables the use of atomic vars
 * which can be use safely in an ISR.  For UART input, the received bytes
 * are put into this ring buffer, a separate kernel thread reads
 */
void auth_ringbuf_init(struct auth_ringbuf *ringbuf)
{
    k_sem_init(&ringbuf->rx_sem, 0, AUTH_RING_BUFLEN);

    auth_ringbuf_reset(ringbuf);
}

void auth_ringbuf_reset(struct auth_ringbuf *ringbuf)
{
    atomic_set(&ringbuf->head_idx, 0);
    atomic_set(&ringbuf->did_overflow, 0);
    atomic_set(&ringbuf->tail_idx, 0);

    k_sem_reset(&ringbuf->rx_sem);
}


void auth_ringbuf_put_byte(struct auth_ringbuf *ringbuf, uint8_t one_byte)
{
    ringbuf->buf[atomic_inc(&ringbuf->head_idx)] = one_byte;

    /* check if head index beyond fx buffer */
    atomic_cas(&ringbuf->head_idx, AUTH_RING_BUFLEN, 0);

    /* Did an overflow occur? At this point we haven't put a byte into
     * the same place as the tail index (causing the overflow) but the buffer is
     * full enough that we will set the overflow flag. This k*/
    if (atomic_get(&ringbuf->head_idx) == atomic_get(&ringbuf->tail_idx)) {
        atomic_set(&ringbuf->did_overflow, 1);
    }

    /* inc semaphore */
    k_sem_give(&ringbuf->rx_sem);
}

/**
 * Returns true if more bytes avail
 *
 * @param ringbuf
 * @param byte
 * @param wait_mec, num milliseconds, or K_FOREVER, or K_NO_WAIT
 *
 * @return
 */
bool auth_ringbuf_get_byte(struct auth_ringbuf *ringbuf, uint8_t *one_byte, uint32_t wait_msec)
{
    /* inc semaphore */
    int ret = k_sem_take(&ringbuf->rx_sem, K_MSEC(wait_msec));

    /* Timed out or some other error. */
    if(ret) {
        return false;
    }

    if (atomic_get(&ringbuf->head_idx) == atomic_get(&ringbuf->tail_idx)) {
        return false;
    }

    *one_byte = ringbuf->buf[atomic_inc(&ringbuf->tail_idx)];

    atomic_cas(&ringbuf->tail_idx, AUTH_RING_BUFLEN, 0);

    return true;
}


bool auth_ringbuf_overflow(struct auth_ringbuf *ringbuf)
{
    return atomic_get(&ringbuf->did_overflow) == 0 ? true : false;
}




