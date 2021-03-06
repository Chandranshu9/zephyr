/*
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_AUTH_CERTS_H_
#define ZEPHYR_INCLUDE_AUTH_CERTS_H_


#define AUTH_ROOTCA_CERT_PEM \
    "-----BEGIN CERTIFICATE-----\r\n" \
    "MIICeTCCAh+gAwIBAgIUPt+1qmulufx3ze5ZMsGpact9ekwwCgYIKoZIzj0EAwIw\r\n"   \
    "gYkxCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRIwEAYDVQQHDAlT\r\n"   \
    "YW4gRGllZ28xGjAYBgNVBAoMEUF1dGggQ29tcGFueSBUZXN0MRwwGgYDVQQLDBNB\r\n"   \
    "dXRoZW50aWNhdGlvbiBURVNUMRcwFQYDVQQDDA5BdXRoIFJvb3QgVEVTVDAeFw0y\r\n"   \
    "MDA4MjYxOTM5MDJaFw0zNDA1MDUxOTM5MDJaMIGJMQswCQYDVQQGEwJVUzETMBEG\r\n"   \
    "A1UECAwKQ2FsaWZvcm5pYTESMBAGA1UEBwwJU2FuIERpZWdvMRowGAYDVQQKDBFB\r\n"   \
    "dXRoIENvbXBhbnkgVGVzdDEcMBoGA1UECwwTQXV0aGVudGljYXRpb24gVEVTVDEX\r\n"   \
    "MBUGA1UEAwwOQXV0aCBSb290IFRFU1QwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNC\r\n"   \
    "AATCxBhg/MkpI+6PfUg6NUdk03pOd8x0Q8qcJngr4Gje+AzQLFoJHN0NE+PaTf2k\r\n"   \
    "ILZX1hT2l39oKFXxhQedPB/uo2MwYTAdBgNVHQ4EFgQU4wEI2+yUj/DsbPmqLakg\r\n"   \
    "jsuWUKkwHwYDVR0jBBgwFoAU4wEI2+yUj/DsbPmqLakgjsuWUKkwDwYDVR0TAQH/\r\n"   \
    "BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwCgYIKoZIzj0EAwIDSAAwRQIgT8TvcsCl\r\n"   \
    "adPrqWv50U87LON0QMsaTAqvVV6tFd06knICIQCxH+oRA9l0NSiyHJuPHRuKn6a9\r\n"   \
    "xfOTq1zTpr6fNu9vuw==\r\n"   \
    "-----END CERTIFICATE-----\r\n"


#define AUTH_INTERMEDIATE_CERT_PEM \
    "-----BEGIN CERTIFICATE-----\r\n"   \
    "MIICXTCCAgOgAwIBAgICEAAwCgYIKoZIzj0EAwIwgYkxCzAJBgNVBAYTAlVTMRMw\r\n"   \
    "EQYDVQQIDApDYWxpZm9ybmlhMRIwEAYDVQQHDAlTYW4gRGllZ28xGjAYBgNVBAoM\r\n"   \
    "EUF1dGggQ29tcGFueSBUZXN0MRwwGgYDVQQLDBNBdXRoZW50aWNhdGlvbiBURVNU\r\n"   \
    "MRcwFQYDVQQDDA5BdXRoIFJvb3QgVEVTVDAeFw0yMDA4MjYyMDEyMjVaFw0zMTA4\r\n"   \
    "MDkyMDEyMjVaMH0xCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRow\r\n"   \
    "GAYDVQQKDBFBdXRoIENvbXBhbnkgVGVzdDEcMBoGA1UECwwTQXV0aGVudGljYXRp\r\n"   \
    "b24gVEVTVDEfMB0GA1UEAwwWQXV0aCBJbnRlcm1lZGlhdGUgVEVTVDBZMBMGByqG\r\n"   \
    "SM49AgEGCCqGSM49AwEHA0IABICgo5Ku11qZi4vFoTb2HmCShvvpsCM/0U3SDdqF\r\n"   \
    "syBUT/cXdlYuqY+DdhM+GpQ0Qd4KNZEjwFWFEMXVH66gJgqjZjBkMB0GA1UdDgQW\r\n"   \
    "BBQ2H3A6aIoNgXkAIVzaz+199JaFKjAfBgNVHSMEGDAWgBTjAQjb7JSP8Oxs+aot\r\n"   \
    "qSCOy5ZQqTASBgNVHRMBAf8ECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBhjAKBggq\r\n"   \
    "hkjOPQQDAgNIADBFAiB3iPwLrBmMN6erAl+6w96pQJRH62R5s5T59nLP32d5QQIh\r\n"   \
    "AMx3N8ijw8cE35Jg5szKy3hYgiN/6VTFEYADSdi737Si\r\n"   \
    "-----END CERTIFICATE-----\r\n"

#define AUTH_SERVER_CERT_PEM \
    "-----BEGIN CERTIFICATE-----\r\n"   \
    "MIICNjCCAd2gAwIBAgICIAEwCgYIKoZIzj0EAwIwfTELMAkGA1UEBhMCVVMxEzAR\r\n"   \
    "BgNVBAgMCkNhbGlmb3JuaWExGjAYBgNVBAoMEUF1dGggQ29tcGFueSBUZXN0MRww\r\n"   \
    "GgYDVQQLDBNBdXRoZW50aWNhdGlvbiBURVNUMR8wHQYDVQQDDBZBdXRoIEludGVy\r\n"   \
    "bWVkaWF0ZSBURVNUMB4XDTIwMDgyNjIwMzgzOFoXDTIxMDkwNTIwMzgzOFowgYsx\r\n"   \
    "CzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRIwEAYDVQQHDAlTYW4g\r\n"   \
    "RGllZ28xGjAYBgNVBAoMEUF1dGggQ29tcGFueSBUZXN0MRwwGgYDVQQLDBNBdXRo\r\n"   \
    "ZW50aWNhdGlvbiBURVNUMRkwFwYDVQQDDBBBdXRoIFNlcnZlciBURVNUMFkwEwYH\r\n"   \
    "KoZIzj0CAQYIKoZIzj0DAQcDQgAEuHrjl6aMJTYsMqOP6cdzQ22pJwBl3Cy698Co\r\n"   \
    "SpH6Ek5NMTJbSphGi2QdSCaQFDcq+T5H51j9Ch6m7+sblSDupqM+MDwwCQYDVR0T\r\n"   \
    "BAIwADAfBgNVHSMEGDAWgBQ2H3A6aIoNgXkAIVzaz+199JaFKjAOBgNVHQ8BAf8E\r\n"   \
    "BAMCB4AwCgYIKoZIzj0EAwIDRwAwRAIgAtXzzTzbEc/5yI4AU6+cFoOwaf3RK7/E\r\n"   \
    "8kOsxGiemNECICLojqGP5O/tb5F/xfIYjuGvrkdAlY2ykb1btv9DoMUg\r\n"   \
    "-----END CERTIFICATE-----\r\n"


#define AUTH_CLIENT_CERT_PEM \
    "-----BEGIN CERTIFICATE-----\r\n"   \
    "MIICNzCCAd2gAwIBAgICIAAwCgYIKoZIzj0EAwIwfTELMAkGA1UEBhMCVVMxEzAR\r\n"   \
    "BgNVBAgMCkNhbGlmb3JuaWExGjAYBgNVBAoMEUF1dGggQ29tcGFueSBUZXN0MRww\r\n"   \
    "GgYDVQQLDBNBdXRoZW50aWNhdGlvbiBURVNUMR8wHQYDVQQDDBZBdXRoIEludGVy\r\n"   \
    "bWVkaWF0ZSBURVNUMB4XDTIwMDgyNjIwMzgxMFoXDTIxMDkwNTIwMzgxMFowgYsx\r\n"   \
    "CzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRIwEAYDVQQHDAlTYW4g\r\n"   \
    "RGllZ28xGjAYBgNVBAoMEUF1dGggQ29tcGFueSBUZXN0MRwwGgYDVQQLDBNBdXRo\r\n"   \
    "ZW50aWNhdGlvbiBURVNUMRkwFwYDVQQDDBBBdXRoIENsaWVudCBURVNUMFkwEwYH\r\n"   \
    "KoZIzj0CAQYIKoZIzj0DAQcDQgAEkwj+WKKVM3k9AEiS/efDhXoICq/e8rVKew2q\r\n"   \
    "pun6zie8xATw7pyte1Hf59Ga2Vmti/QTvzczZrvj/m0LbKzmn6M+MDwwCQYDVR0T\r\n"   \
    "BAIwADAfBgNVHSMEGDAWgBQ2H3A6aIoNgXkAIVzaz+199JaFKjAOBgNVHQ8BAf8E\r\n"   \
    "BAMCB4AwCgYIKoZIzj0EAwIDSAAwRQIgfynezJlsshDJnyVVEC/twUHJAyhpIrEU\r\n"   \
    "+qomFnsDB20CIQC9FxlDtKVrn4MOfI1yx9grPCN9ztwq1pRTLAh9LB/F9w==\r\n"   \
    "-----END CERTIFICATE-----\r\n"


/**
 * Sever and client private keys
 */

#define AUTH_SERVER_PRIVATE_KEY_PEM     \
    "-----BEGIN EC PRIVATE KEY-----\r\n"    \
    "MHcCAQEEIP8LVrSz0CD/KvTTo5/qmOKIFtepGBTDG2odkSnYx/ssoAoGCCqGSM49\r\n"    \
    "AwEHoUQDQgAEuHrjl6aMJTYsMqOP6cdzQ22pJwBl3Cy698CoSpH6Ek5NMTJbSphG\r\n"    \
    "i2QdSCaQFDcq+T5H51j9Ch6m7+sblSDupg==\r\n"    \
    "-----END EC PRIVATE KEY-----\r\n"


#define AUTH_CLIENT_PRIVATE_KEY_PEM \
    "-----BEGIN EC PRIVATE KEY-----\r\n"    \
    "MHcCAQEEIJ87D6q+Z4ulXG2B9lQblbgCQh4xhMgImQpyKxUCtP3soAoGCCqGSM49\r\n"    \
    "AwEHoUQDQgAEkwj+WKKVM3k9AEiS/efDhXoICq/e8rVKew2qpun6zie8xATw7pyt\r\n"    \
    "e1Hf59Ga2Vmti/QTvzczZrvj/m0LbKzmnw==\r\n"    \
    "-----END EC PRIVATE KEY-----\r\n"

#endif  // ZEPHYR_INCLUDE_AUTH_CERTS_H_


