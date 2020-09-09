
/* main.c - Application main entry point */

/*
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/l2cap.h>

#include <net/tls_credentials.h>


#include <logging/log.h>
#include <logging/log_ctrl.h>

#include <auth/auth_lib.h>


LOG_MODULE_REGISTER(periph_auth, CONFIG_AUTH_LOG_LEVEL);

#if defined(CONFIG_AUTH_DTLS)
#include "../../../certs/auth_certs.h"
#endif


static void client_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value);

/* AUTH Service Declaration */
BT_GATT_SERVICE_DEFINE(auth_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_AUTH_SVC),

    /**
     *    Central (client role) bt_gatt_write()  ---> server characteristic --> bt_gatt_read() Peripheral (server role)
     *
     *                Central    <---  Notification (client characteristic)  <--- Peripheral
     *
     */

    /**
     * Client characteristic, used by the peripheral (server role) to write messages authentication messages
     * to the central (client role).  The peripheral needs to alert the central a message is
     * ready to be read.
     */
    BT_GATT_CHARACTERISTIC(BT_UUID_AUTH_SVC_CLIENT_CHAR, BT_GATT_CHRC_INDICATE,
            (BT_GATT_PERM_READ|BT_GATT_PERM_WRITE), NULL, NULL, NULL),
    BT_GATT_CCC(client_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    /**
     * Server characteristic, used by the central (client role) to write authentication messages to.
     * to the server (peripheral)
     */
    BT_GATT_CHARACTERISTIC(BT_UUID_AUTH_SVC_SERVER_CHAR, BT_GATT_CHRC_WRITE,
           (BT_GATT_PERM_READ|BT_GATT_PERM_WRITE), NULL, auth_xp_bt_central_write, NULL),
);



struct bt_conn *default_conn;

static bool is_connected = false;

static struct authenticate_conn auth_conn;

#if defined(CONFIG_AUTH_DTLS)

/* The Root and Intermediate Certs in a single CA chain.
 * plus the server cert. All in PEM format.*/
static const uint8_t auth_cert_ca_chain[] = AUTH_ROOTCA_CERT_PEM AUTH_INTERMEDIATE_CERT_PEM;
static const uint8_t auth_dev_server_cert[] = AUTH_SERVER_CERT_PEM;
static const uint8_t auth_server_privatekey[] = AUTH_SERVER_PRIVATE_KEY_PEM;
#endif

/**
 * Set up the advertising data
 */
static const struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x10, 0x30 ),  //BT_UUID_AUTH_SVC
        // "Auth Svc"
        BT_DATA_BYTES(BT_DATA_NAME_SHORTENED, 0x41, 0x75, 0x74, 0x68, 0x20, 0x53, 0x76, 0x63),
};

/**
 *
 * @param conn
 * @param err
 */
static void connected(struct bt_conn *conn, u8_t err)
{
    int ret;
    struct auth_xport_evt conn_evt;

    if (err) {
        printk("Connection failed (err 0x%02x)\n", err);
    } else {
        default_conn = bt_conn_ref(conn);
        printk("Connected\n");

        struct auth_xp_bt_params xport_param = { .conn = conn, .is_central = false,
                                                 .client_attr = &auth_svc.attrs[1] };

        ret = auth_xport_init(&auth_conn.xport_hdl, auth_conn.instance, &xport_param);

        if(ret) {
            printk("Failed to initialize BT transport, err: %d", ret);
            return;
        }

	// DAG DEBUG BEG
	// Need to figure out how the peripheral get's notified
	// of the MTU size change
	printk("*** MTU size is: %d\n", bt_gatt_get_mtu(conn));
	// DAG DEBUG END

        is_connected = true;

        /* send connection event to BT transport */
        conn_evt.event = XP_EVT_CONNECT;
        auth_xport_event(auth_conn.xport_hdl, &conn_evt);

        /* Start authentication */
        int ret = auth_lib_start(&auth_conn);

        if(ret) {
            printk("Failed to start authentication, err: %d\n", ret);
        }
    }
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
    struct auth_xport_evt conn_evt;

    printk("Disconnected (reason 0x%02x)\n", reason);

    is_connected = false;

    /* Send disconnect event to BT transport. */
    conn_evt.event = XP_EVT_DISCONNECT;
    auth_xport_event(auth_conn.xport_hdl, &conn_evt);

    /* Deinit lower transport */
    auth_xport_deinit(auth_conn.xport_hdl);
    auth_conn.xport_hdl = NULL;

    if (default_conn) {
        bt_conn_unref(default_conn);
        default_conn = NULL;
    }
}

/**
 * Connect callbacks
 */
static struct bt_conn_cb conn_callbacks = {
        .connected = connected,
        .disconnected = disconnected,
};

/**
 * If the paring security (pincode, etc..) failed.
 * @param conn
 */
static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
        .cancel = auth_cancel,
};

/**
 * Called after the BT module has initialized or not (error occurred).
 *
 * @param err
 */
static void bt_ready(int err)
{
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");

    /* Start advertising after BT module has initialized OK */
    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }

    printk("Advertising successfully started\n");
}


static void auth_status(struct authenticate_conn *auth_conn, enum auth_instance_id instance,
                        enum auth_status status, void *context)
{
    /* print out auth status */
    printk("Authentication status: %s\n", auth_lib_getstatus_str(status));
}


/**
 * Called when client notification is (dis)enabled by the Central
 *
 * @param attr    GATT attribute.
 * @param value   BT_GATT_CCC_NOTIFY if changes are notified.
 */
static void client_ccc_cfg_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    ARG_UNUSED(attr);

    bool notif_enabled = (value == BT_GATT_CCC_NOTIFY) ? true : false;

    LOG_INF("Client notifications %s", notif_enabled ? "enabled" : "disabled");
}


static void process_log_msgs(void)
{
    while(log_process(false)) {
        ;  /* intentionally empty statement */
    }
}


void main(void)
{
    int err = 0;

    log_init();

    uint32_t auth_flags = AUTH_CONN_SERVER;


#if defined(CONFIG_AUTH_DTLS)
    auth_flags |= AUTH_CONN_DTLS_AUTH_METHOD;

    /**
    * Add certificates to authentication instance.
    */

    /* Add cert chain and end server cert. */
    if( (err = tls_credential_add(AUTH_CERT_CA_CHAIN_TAG, TLS_CREDENTIAL_CA_CERTIFICATE,
		                     auth_cert_ca_chain, sizeof(auth_cert_ca_chain))) != 0 ||
        (err = tls_credential_add(AUTH_DEVICE_CERT_TAG, TLS_CREDENTIAL_SERVER_CERTIFICATE,
		                     auth_dev_server_cert, sizeof(auth_dev_server_cert))) != 0)
    {
        printk("Failed to add certs, err: %d\n", err);
        return;
    }

    /* Add server cert private key. */
    if((err = tls_credential_add(AUTH_DEVICE_CERT_TAG, TLS_CREDENTIAL_PRIVATE_KEY,
		                     auth_server_privatekey, sizeof(auth_server_privatekey))) != 0)
	{
        printk("Failed to add server private key, err: %d\n", err);
        return;
	}

#endif

#if defined(CONFIG_AUTH_CHALLENGE_RESPONSE)
    auth_flags |= AUTH_CONN_CHALLENGE_AUTH_METHOD;
#endif


    err = auth_lib_init(&auth_conn, auth_status, AUTH_INST_1_ID, NULL, auth_flags);

    if(err){
        printk("Failed to init authentication service.\n");
        return;
    }


    err = bt_enable(bt_ready);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    bt_conn_cb_register(&conn_callbacks);
    bt_conn_auth_cb_register(&auth_cb_display);


    printk("Peripheral Auth started\n");

    while(true) {

        process_log_msgs();

        /* give the handshake thread a chance to run */
        k_yield();
    }
}

