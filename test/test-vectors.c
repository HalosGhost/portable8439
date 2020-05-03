#include "../src/chacha-portable/chacha-portable.h"
#include "../src/poly1305-donna/poly1305-donna.h"
#include "../src/portable8439.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/**************************
 * This file contains test vectors extracted from RFC 8439
 **************************/


static void print_hex(const uint8_t *dt, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02X", dt[i]);
    }
    printf("\n");
}

#define UNEXPECTED_RESULT(a,b,s) \
    printf("Expected: \n"); \
    print_hex(a, s); \
    printf("Got: \n"); \
    print_hex(b, s);

#define MAX_TEST_SIZE (512)

struct chacha_test_vector {
    char* title;
    uint8_t key[CHACHA20_KEY_SIZE];
    uint8_t nonce[CHACHA20_NONCE_SIZE];
    uint32_t counter;
    uint8_t plain_text[MAX_TEST_SIZE];
    uint8_t cipher_text[MAX_TEST_SIZE];
    size_t size;
};

#define CHACHA_TEST_VECTORS (2)

static struct chacha_test_vector chacha_rfc_tests[CHACHA_TEST_VECTORS] = {
    {
        "RFC8439 2.4.2",
        {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x00},
        1,
        {
            0x4c,0x61,0x64,0x69,0x65,0x73,0x20,0x61,0x6e,0x64,0x20,0x47,0x65,0x6e,0x74,0x6c,
            0x65,0x6d,0x65,0x6e,0x20,0x6f,0x66,0x20,0x74,0x68,0x65,0x20,0x63,0x6c,0x61,0x73,
            0x73,0x20,0x6f,0x66,0x20,0x27,0x39,0x39,0x3a,0x20,0x49,0x66,0x20,0x49,0x20,0x63,
            0x6f,0x75,0x6c,0x64,0x20,0x6f,0x66,0x66,0x65,0x72,0x20,0x79,0x6f,0x75,0x20,0x6f,
            0x6e,0x6c,0x79,0x20,0x6f,0x6e,0x65,0x20,0x74,0x69,0x70,0x20,0x66,0x6f,0x72,0x20,
            0x74,0x68,0x65,0x20,0x66,0x75,0x74,0x75,0x72,0x65,0x2c,0x20,0x73,0x75,0x6e,0x73,
            0x63,0x72,0x65,0x65,0x6e,0x20,0x77,0x6f,0x75,0x6c,0x64,0x20,0x62,0x65,0x20,0x69,
            0x74,0x2e
        },
        {
            0x6e,0x2e,0x35,0x9a,0x25,0x68,0xf9,0x80,0x41,0xba,0x07,0x28,0xdd,0x0d,0x69,0x81,
            0xe9,0x7e,0x7a,0xec,0x1d,0x43,0x60,0xc2,0x0a,0x27,0xaf,0xcc,0xfd,0x9f,0xae,0x0b,
            0xf9,0x1b,0x65,0xc5,0x52,0x47,0x33,0xab,0x8f,0x59,0x3d,0xab,0xcd,0x62,0xb3,0x57,
            0x16,0x39,0xd6,0x24,0xe6,0x51,0x52,0xab,0x8f,0x53,0x0c,0x35,0x9f,0x08,0x61,0xd8,
            0x07,0xca,0x0d,0xbf,0x50,0x0d,0x6a,0x61,0x56,0xa3,0x8e,0x08,0x8a,0x22,0xb6,0x5e,
            0x52,0xbc,0x51,0x4d,0x16,0xcc,0xf8,0x06,0x81,0x8c,0xe9,0x1a,0xb7,0x79,0x37,0x36,
            0x5a,0xf9,0x0b,0xbf,0x74,0xa3,0x5b,0xe6,0xb4,0x0b,0x8e,0xed,0xf2,0x78,0x5e,0x42,
            0x87,0x4d
        },
        114
    },
    {
        "RFC8439 A.2 #2",
        {
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
        },
        { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02 },
        1,
        {
            0x41,0x6e,0x79,0x20,0x73,0x75,0x62,0x6d,0x69,0x73,0x73,0x69,0x6f,0x6e,0x20,0x74,
            0x6f,0x20,0x74,0x68,0x65,0x20,0x49,0x45,0x54,0x46,0x20,0x69,0x6e,0x74,0x65,0x6e,
            0x64,0x65,0x64,0x20,0x62,0x79,0x20,0x74,0x68,0x65,0x20,0x43,0x6f,0x6e,0x74,0x72,
            0x69,0x62,0x75,0x74,0x6f,0x72,0x20,0x66,0x6f,0x72,0x20,0x70,0x75,0x62,0x6c,0x69,
            0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x61,0x73,0x20,0x61,0x6c,0x6c,0x20,0x6f,0x72,
            0x20,0x70,0x61,0x72,0x74,0x20,0x6f,0x66,0x20,0x61,0x6e,0x20,0x49,0x45,0x54,0x46,
            0x20,0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,0x74,0x2d,0x44,0x72,0x61,0x66,0x74,0x20,
            0x6f,0x72,0x20,0x52,0x46,0x43,0x20,0x61,0x6e,0x64,0x20,0x61,0x6e,0x79,0x20,0x73,
            0x74,0x61,0x74,0x65,0x6d,0x65,0x6e,0x74,0x20,0x6d,0x61,0x64,0x65,0x20,0x77,0x69,
            0x74,0x68,0x69,0x6e,0x20,0x74,0x68,0x65,0x20,0x63,0x6f,0x6e,0x74,0x65,0x78,0x74,
            0x20,0x6f,0x66,0x20,0x61,0x6e,0x20,0x49,0x45,0x54,0x46,0x20,0x61,0x63,0x74,0x69,
            0x76,0x69,0x74,0x79,0x20,0x69,0x73,0x20,0x63,0x6f,0x6e,0x73,0x69,0x64,0x65,0x72,
            0x65,0x64,0x20,0x61,0x6e,0x20,0x22,0x49,0x45,0x54,0x46,0x20,0x43,0x6f,0x6e,0x74,
            0x72,0x69,0x62,0x75,0x74,0x69,0x6f,0x6e,0x22,0x2e,0x20,0x53,0x75,0x63,0x68,0x20,
            0x73,0x74,0x61,0x74,0x65,0x6d,0x65,0x6e,0x74,0x73,0x20,0x69,0x6e,0x63,0x6c,0x75,
            0x64,0x65,0x20,0x6f,0x72,0x61,0x6c,0x20,0x73,0x74,0x61,0x74,0x65,0x6d,0x65,0x6e,
            0x74,0x73,0x20,0x69,0x6e,0x20,0x49,0x45,0x54,0x46,0x20,0x73,0x65,0x73,0x73,0x69,
            0x6f,0x6e,0x73,0x2c,0x20,0x61,0x73,0x20,0x77,0x65,0x6c,0x6c,0x20,0x61,0x73,0x20,
            0x77,0x72,0x69,0x74,0x74,0x65,0x6e,0x20,0x61,0x6e,0x64,0x20,0x65,0x6c,0x65,0x63,
            0x74,0x72,0x6f,0x6e,0x69,0x63,0x20,0x63,0x6f,0x6d,0x6d,0x75,0x6e,0x69,0x63,0x61,
            0x74,0x69,0x6f,0x6e,0x73,0x20,0x6d,0x61,0x64,0x65,0x20,0x61,0x74,0x20,0x61,0x6e,
            0x79,0x20,0x74,0x69,0x6d,0x65,0x20,0x6f,0x72,0x20,0x70,0x6c,0x61,0x63,0x65,0x2c,
            0x20,0x77,0x68,0x69,0x63,0x68,0x20,0x61,0x72,0x65,0x20,0x61,0x64,0x64,0x72,0x65,
            0x73,0x73,0x65,0x64,0x20,0x74,0x6f
        },
        {
            0xa3,0xfb,0xf0,0x7d,0xf3,0xfa,0x2f,0xde,0x4f,0x37,0x6c,0xa2,0x3e,0x82,0x73,0x70,
            0x41,0x60,0x5d,0x9f,0x4f,0x4f,0x57,0xbd,0x8c,0xff,0x2c,0x1d,0x4b,0x79,0x55,0xec,
            0x2a,0x97,0x94,0x8b,0xd3,0x72,0x29,0x15,0xc8,0xf3,0xd3,0x37,0xf7,0xd3,0x70,0x05,
            0x0e,0x9e,0x96,0xd6,0x47,0xb7,0xc3,0x9f,0x56,0xe0,0x31,0xca,0x5e,0xb6,0x25,0x0d,
            0x40,0x42,0xe0,0x27,0x85,0xec,0xec,0xfa,0x4b,0x4b,0xb5,0xe8,0xea,0xd0,0x44,0x0e,
            0x20,0xb6,0xe8,0xdb,0x09,0xd8,0x81,0xa7,0xc6,0x13,0x2f,0x42,0x0e,0x52,0x79,0x50,
            0x42,0xbd,0xfa,0x77,0x73,0xd8,0xa9,0x05,0x14,0x47,0xb3,0x29,0x1c,0xe1,0x41,0x1c,
            0x68,0x04,0x65,0x55,0x2a,0xa6,0xc4,0x05,0xb7,0x76,0x4d,0x5e,0x87,0xbe,0xa8,0x5a,
            0xd0,0x0f,0x84,0x49,0xed,0x8f,0x72,0xd0,0xd6,0x62,0xab,0x05,0x26,0x91,0xca,0x66,
            0x42,0x4b,0xc8,0x6d,0x2d,0xf8,0x0e,0xa4,0x1f,0x43,0xab,0xf9,0x37,0xd3,0x25,0x9d,
            0xc4,0xb2,0xd0,0xdf,0xb4,0x8a,0x6c,0x91,0x39,0xdd,0xd7,0xf7,0x69,0x66,0xe9,0x28,
            0xe6,0x35,0x55,0x3b,0xa7,0x6c,0x5c,0x87,0x9d,0x7b,0x35,0xd4,0x9e,0xb2,0xe6,0x2b,
            0x08,0x71,0xcd,0xac,0x63,0x89,0x39,0xe2,0x5e,0x8a,0x1e,0x0e,0xf9,0xd5,0x28,0x0f,
            0xa8,0xca,0x32,0x8b,0x35,0x1c,0x3c,0x76,0x59,0x89,0xcb,0xcf,0x3d,0xaa,0x8b,0x6c,
            0xcc,0x3a,0xaf,0x9f,0x39,0x79,0xc9,0x2b,0x37,0x20,0xfc,0x88,0xdc,0x95,0xed,0x84,
            0xa1,0xbe,0x05,0x9c,0x64,0x99,0xb9,0xfd,0xa2,0x36,0xe7,0xe8,0x18,0xb0,0x4b,0x0b,
            0xc3,0x9c,0x1e,0x87,0x6b,0x19,0x3b,0xfe,0x55,0x69,0x75,0x3f,0x88,0x12,0x8c,0xc0,
            0x8a,0xaa,0x9b,0x63,0xd1,0xa1,0x6f,0x80,0xef,0x25,0x54,0xd7,0x18,0x9c,0x41,0x1f,
            0x58,0x69,0xca,0x52,0xc5,0xb8,0x3f,0xa3,0x6f,0xf2,0x16,0xb9,0xc1,0xd3,0x00,0x62,
            0xbe,0xbc,0xfd,0x2d,0xc5,0xbc,0xe0,0x91,0x19,0x34,0xfd,0xa7,0x9a,0x86,0xf6,0xe6,
            0x98,0xce,0xd7,0x59,0xc3,0xff,0x9b,0x64,0x77,0x33,0x8f,0x3d,0xa4,0xf9,0xcd,0x85,
            0x14,0xea,0x99,0x82,0xcc,0xaf,0xb3,0x41,0xb2,0x38,0x4d,0xd9,0x02,0xf3,0xd1,0xab,
            0x7a,0xc6,0x1d,0xd2,0x9c,0x6f,0x21,0xba,0x5b,0x86,0x2f,0x37,0x30,0xe3,0x7c,0xfd,
            0xc4,0xfd,0x80,0x6c,0x22,0xf2,0x21
        },
        375
    }
};

static int test_chacha20() {
    printf("Testing chacha20\n");
    uint8_t buffer[MAX_TEST_SIZE] = {0};
    for (int i = 0; i < CHACHA_TEST_VECTORS; i++) {
        struct chacha_test_vector t = chacha_rfc_tests[i];
        printf("- %s: ", t.title);
        chacha20_xor_stream(buffer, t.plain_text, t.size, t.key, t.nonce, t.counter);
        if (memcmp(buffer, t.cipher_text, t.size) != 0) {
            printf("failed\n");
            return -1;
        }
        else {
            printf("success\n");
        }
    }
    return 0;
}


struct poly_test_vector {
    char* title;
    uint8_t key[32];
    uint8_t msg[512];
    size_t size;
    uint8_t tag[16];
};

#define POLY_TEST_VECTORS (2)

static struct poly_test_vector poly_rfc_tests[POLY_TEST_VECTORS] = {
    {
        "RFC8439 2.5.2",
        {0x85,0xd6,0xbe,0x78,0x57,0x55,0x6d,0x33,0x7f,0x44,0x52,0xfe,0x42,0xd5,0x06,0xa8,0x01,0x03,0x80,0x8a,0xfb,0x0d,0xb2,0xfd,0x4a,0xbf,0xf6,0xaf,0x41,0x49,0xf5,0x1b},
        {
            0x43,0x72,0x79,0x70,0x74,0x6f,0x67,0x72,0x61,0x70,0x68,0x69,0x63,0x20,0x46,0x6f,
            0x72,0x75,0x6d,0x20,0x52,0x65,0x73,0x65,0x61,0x72,0x63,0x68,0x20,0x47,0x72,0x6f,
            0x75,0x70
        },
        34,
        {0xa8,0x06,0x1d,0xc1,0x30,0x51,0x36,0xc6,0xc2,0x2b,0x8b,0xaf,0x0c,0x01,0x27,0xa9}
    },
    {
        "RFC8439 A.3",
        {
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x36,0xe5,0xf6,0xb5,0xc5,0xe0,0x60,0x70,0xf0,0xef,0xca,0x96,0x22,0x7a,0x86,0x3e
        },
        {
            0x41,0x6e,0x79,0x20,0x73,0x75,0x62,0x6d,0x69,0x73,0x73,0x69,0x6f,0x6e,0x20,0x74,
            0x6f,0x20,0x74,0x68,0x65,0x20,0x49,0x45,0x54,0x46,0x20,0x69,0x6e,0x74,0x65,0x6e,
            0x64,0x65,0x64,0x20,0x62,0x79,0x20,0x74,0x68,0x65,0x20,0x43,0x6f,0x6e,0x74,0x72,
            0x69,0x62,0x75,0x74,0x6f,0x72,0x20,0x66,0x6f,0x72,0x20,0x70,0x75,0x62,0x6c,0x69,
            0x63,0x61,0x74,0x69,0x6f,0x6e,0x20,0x61,0x73,0x20,0x61,0x6c,0x6c,0x20,0x6f,0x72,
            0x20,0x70,0x61,0x72,0x74,0x20,0x6f,0x66,0x20,0x61,0x6e,0x20,0x49,0x45,0x54,0x46,
            0x20,0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,0x74,0x2d,0x44,0x72,0x61,0x66,0x74,0x20,
            0x6f,0x72,0x20,0x52,0x46,0x43,0x20,0x61,0x6e,0x64,0x20,0x61,0x6e,0x79,0x20,0x73,
            0x74,0x61,0x74,0x65,0x6d,0x65,0x6e,0x74,0x20,0x6d,0x61,0x64,0x65,0x20,0x77,0x69,
            0x74,0x68,0x69,0x6e,0x20,0x74,0x68,0x65,0x20,0x63,0x6f,0x6e,0x74,0x65,0x78,0x74,
            0x20,0x6f,0x66,0x20,0x61,0x6e,0x20,0x49,0x45,0x54,0x46,0x20,0x61,0x63,0x74,0x69,
            0x76,0x69,0x74,0x79,0x20,0x69,0x73,0x20,0x63,0x6f,0x6e,0x73,0x69,0x64,0x65,0x72,
            0x65,0x64,0x20,0x61,0x6e,0x20,0x22,0x49,0x45,0x54,0x46,0x20,0x43,0x6f,0x6e,0x74,
            0x72,0x69,0x62,0x75,0x74,0x69,0x6f,0x6e,0x22,0x2e,0x20,0x53,0x75,0x63,0x68,0x20,
            0x73,0x74,0x61,0x74,0x65,0x6d,0x65,0x6e,0x74,0x73,0x20,0x69,0x6e,0x63,0x6c,0x75,
            0x64,0x65,0x20,0x6f,0x72,0x61,0x6c,0x20,0x73,0x74,0x61,0x74,0x65,0x6d,0x65,0x6e,
            0x74,0x73,0x20,0x69,0x6e,0x20,0x49,0x45,0x54,0x46,0x20,0x73,0x65,0x73,0x73,0x69,
            0x6f,0x6e,0x73,0x2c,0x20,0x61,0x73,0x20,0x77,0x65,0x6c,0x6c,0x20,0x61,0x73,0x20,
            0x77,0x72,0x69,0x74,0x74,0x65,0x6e,0x20,0x61,0x6e,0x64,0x20,0x65,0x6c,0x65,0x63,
            0x74,0x72,0x6f,0x6e,0x69,0x63,0x20,0x63,0x6f,0x6d,0x6d,0x75,0x6e,0x69,0x63,0x61,
            0x74,0x69,0x6f,0x6e,0x73,0x20,0x6d,0x61,0x64,0x65,0x20,0x61,0x74,0x20,0x61,0x6e,
            0x79,0x20,0x74,0x69,0x6d,0x65,0x20,0x6f,0x72,0x20,0x70,0x6c,0x61,0x63,0x65,0x2c,
            0x20,0x77,0x68,0x69,0x63,0x68,0x20,0x61,0x72,0x65,0x20,0x61,0x64,0x64,0x72,0x65,
            0x73,0x73,0x65,0x64,0x20,0x74,0x6f
        },
        375,
        {
            0x36,0xe5,0xf6,0xb5,0xc5,0xe0,0x60,0x70,0xf0,0xef,0xca,0x96,0x22,0x7a,0x86,0x3e
        }
    }
};

static int test_poly() {
    printf("Testing poly1305\n");
    uint8_t tag[16] = {0};
    for (int i = 0; i <POLY_TEST_VECTORS; i++) {
        struct poly_test_vector t = poly_rfc_tests[i];
        printf("- %s: ", t.title);
        poly1305_auth(tag, t.msg, t.size, t.key);
        if (memcmp(tag, t.tag, 16) != 0) {
            printf("failed\n");
            return -1;
        }
        else {
            printf("success\n");
        }
    }
    for (int i = 0; i <POLY_TEST_VECTORS; i++) {
        struct poly_test_vector t = poly_rfc_tests[i];
        printf("- %s streaming: ", t.title);
        poly1305_context ctx;
        poly1305_init(&ctx, t.key);
        poly1305_update(&ctx, t.msg, t.size);
        poly1305_finish(&ctx, tag);
        if (memcmp(tag, t.tag, 16) != 0) {
            printf("failed\n");
            return -1;
        }
        else {
            printf("success\n");
        }
    }
    return 0;
}


struct aead_test_vector {
    char* title;
    uint8_t key[RFC_8439_KEY_SIZE];
    uint8_t nonce[RFC_8439_NONCE_SIZE];
    uint8_t ad[MAX_TEST_SIZE];
    size_t ad_size;
    uint8_t plain_text[MAX_TEST_SIZE];
    uint8_t cipher_text[MAX_TEST_SIZE];
    size_t size;
    uint8_t mac[RFC_8439_TAG_SIZE];
};

#define AEAD_TEST_VECTORS (2)

static struct aead_test_vector aead_rfc_tests[AEAD_TEST_VECTORS] = {
    {
        "RFC8437 2.8.2",
        {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
            0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f
        },
        {
            0x07,0x00,0x00,0x00,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47
        },
        {
            0x50,0x51,0x52,0x53,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7
        },
        12,
        {
            0x4c,0x61,0x64,0x69,0x65,0x73,0x20,0x61,0x6e,0x64,0x20,0x47,0x65,0x6e,0x74,0x6c,
            0x65,0x6d,0x65,0x6e,0x20,0x6f,0x66,0x20,0x74,0x68,0x65,0x20,0x63,0x6c,0x61,0x73,
            0x73,0x20,0x6f,0x66,0x20,0x27,0x39,0x39,0x3a,0x20,0x49,0x66,0x20,0x49,0x20,0x63,
            0x6f,0x75,0x6c,0x64,0x20,0x6f,0x66,0x66,0x65,0x72,0x20,0x79,0x6f,0x75,0x20,0x6f,
            0x6e,0x6c,0x79,0x20,0x6f,0x6e,0x65,0x20,0x74,0x69,0x70,0x20,0x66,0x6f,0x72,0x20,
            0x74,0x68,0x65,0x20,0x66,0x75,0x74,0x75,0x72,0x65,0x2c,0x20,0x73,0x75,0x6e,0x73,
            0x63,0x72,0x65,0x65,0x6e,0x20,0x77,0x6f,0x75,0x6c,0x64,0x20,0x62,0x65,0x20,0x69,
            0x74,0x2e
        },
        {
            0xd3,0x1a,0x8d,0x34,0x64,0x8e,0x60,0xdb,0x7b,0x86,0xaf,0xbc,0x53,0xef,0x7e,0xc2,
            0xa4,0xad,0xed,0x51,0x29,0x6e,0x08,0xfe,0xa9,0xe2,0xb5,0xa7,0x36,0xee,0x62,0xd6,
            0x3d,0xbe,0xa4,0x5e,0x8c,0xa9,0x67,0x12,0x82,0xfa,0xfb,0x69,0xda,0x92,0x72,0x8b,
            0x1a,0x71,0xde,0x0a,0x9e,0x06,0x0b,0x29,0x05,0xd6,0xa5,0xb6,0x7e,0xcd,0x3b,0x36,
            0x92,0xdd,0xbd,0x7f,0x2d,0x77,0x8b,0x8c,0x98,0x03,0xae,0xe3,0x28,0x09,0x1b,0x58,
            0xfa,0xb3,0x24,0xe4,0xfa,0xd6,0x75,0x94,0x55,0x85,0x80,0x8b,0x48,0x31,0xd7,0xbc,
            0x3f,0xf4,0xde,0xf0,0x8e,0x4b,0x7a,0x9d,0xe5,0x76,0xd2,0x65,0x86,0xce,0xc6,0x4b,
            0x61,0x16
        },
        114,
        { 
            0x1a,0xe1,0x0b,0x59,0x4f,0x09,0xe2,0x6a,0x7e,0x90,0x2e,0xcb,0xd0,0x60,0x06,0x91 
        }
    },

    {
        "RFC8439 A.5",
        {
            0x1c,0x92,0x40,0xa5,0xeb,0x55,0xd3,0x8a,0xf3,0x33,0x88,0x86,0x04,0xf6,0xb5,0xf0,
            0x47,0x39,0x17,0xc1,0x40,0x2b,0x80,0x09,0x9d,0xca,0x5c,0xbc,0x20,0x70,0x75,0xc0
        },
        {
            0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08
        },
        {
            0xf3,0x33,0x88,0x86,0x00,0x00,0x00,0x00,0x00,0x00,0x4e,0x91
        },
        12,
        {
            0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,0x74,0x2d,0x44,0x72,0x61,0x66,0x74,0x73,0x20,
            0x61,0x72,0x65,0x20,0x64,0x72,0x61,0x66,0x74,0x20,0x64,0x6f,0x63,0x75,0x6d,0x65,
            0x6e,0x74,0x73,0x20,0x76,0x61,0x6c,0x69,0x64,0x20,0x66,0x6f,0x72,0x20,0x61,0x20,
            0x6d,0x61,0x78,0x69,0x6d,0x75,0x6d,0x20,0x6f,0x66,0x20,0x73,0x69,0x78,0x20,0x6d,
            0x6f,0x6e,0x74,0x68,0x73,0x20,0x61,0x6e,0x64,0x20,0x6d,0x61,0x79,0x20,0x62,0x65,
            0x20,0x75,0x70,0x64,0x61,0x74,0x65,0x64,0x2c,0x20,0x72,0x65,0x70,0x6c,0x61,0x63,
            0x65,0x64,0x2c,0x20,0x6f,0x72,0x20,0x6f,0x62,0x73,0x6f,0x6c,0x65,0x74,0x65,0x64,
            0x20,0x62,0x79,0x20,0x6f,0x74,0x68,0x65,0x72,0x20,0x64,0x6f,0x63,0x75,0x6d,0x65,
            0x6e,0x74,0x73,0x20,0x61,0x74,0x20,0x61,0x6e,0x79,0x20,0x74,0x69,0x6d,0x65,0x2e,
            0x20,0x49,0x74,0x20,0x69,0x73,0x20,0x69,0x6e,0x61,0x70,0x70,0x72,0x6f,0x70,0x72,
            0x69,0x61,0x74,0x65,0x20,0x74,0x6f,0x20,0x75,0x73,0x65,0x20,0x49,0x6e,0x74,0x65,
            0x72,0x6e,0x65,0x74,0x2d,0x44,0x72,0x61,0x66,0x74,0x73,0x20,0x61,0x73,0x20,0x72,
            0x65,0x66,0x65,0x72,0x65,0x6e,0x63,0x65,0x20,0x6d,0x61,0x74,0x65,0x72,0x69,0x61,
            0x6c,0x20,0x6f,0x72,0x20,0x74,0x6f,0x20,0x63,0x69,0x74,0x65,0x20,0x74,0x68,0x65,
            0x6d,0x20,0x6f,0x74,0x68,0x65,0x72,0x20,0x74,0x68,0x61,0x6e,0x20,0x61,0x73,0x20,
            0x2f,0xe2,0x80,0x9c,0x77,0x6f,0x72,0x6b,0x20,0x69,0x6e,0x20,0x70,0x72,0x6f,0x67,
            0x72,0x65,0x73,0x73,0x2e,0x2f,0xe2,0x80,0x9d
        },
        {
            0x64,0xa0,0x86,0x15,0x75,0x86,0x1a,0xf4,0x60,0xf0,0x62,0xc7,0x9b,0xe6,0x43,0xbd,
            0x5e,0x80,0x5c,0xfd,0x34,0x5c,0xf3,0x89,0xf1,0x08,0x67,0x0a,0xc7,0x6c,0x8c,0xb2,
            0x4c,0x6c,0xfc,0x18,0x75,0x5d,0x43,0xee,0xa0,0x9e,0xe9,0x4e,0x38,0x2d,0x26,0xb0,
            0xbd,0xb7,0xb7,0x3c,0x32,0x1b,0x01,0x00,0xd4,0xf0,0x3b,0x7f,0x35,0x58,0x94,0xcf,
            0x33,0x2f,0x83,0x0e,0x71,0x0b,0x97,0xce,0x98,0xc8,0xa8,0x4a,0xbd,0x0b,0x94,0x81,
            0x14,0xad,0x17,0x6e,0x00,0x8d,0x33,0xbd,0x60,0xf9,0x82,0xb1,0xff,0x37,0xc8,0x55,
            0x97,0x97,0xa0,0x6e,0xf4,0xf0,0xef,0x61,0xc1,0x86,0x32,0x4e,0x2b,0x35,0x06,0x38,
            0x36,0x06,0x90,0x7b,0x6a,0x7c,0x02,0xb0,0xf9,0xf6,0x15,0x7b,0x53,0xc8,0x67,0xe4,
            0xb9,0x16,0x6c,0x76,0x7b,0x80,0x4d,0x46,0xa5,0x9b,0x52,0x16,0xcd,0xe7,0xa4,0xe9,
            0x90,0x40,0xc5,0xa4,0x04,0x33,0x22,0x5e,0xe2,0x82,0xa1,0xb0,0xa0,0x6c,0x52,0x3e,
            0xaf,0x45,0x34,0xd7,0xf8,0x3f,0xa1,0x15,0x5b,0x00,0x47,0x71,0x8c,0xbc,0x54,0x6a,
            0x0d,0x07,0x2b,0x04,0xb3,0x56,0x4e,0xea,0x1b,0x42,0x22,0x73,0xf5,0x48,0x27,0x1a,
            0x0b,0xb2,0x31,0x60,0x53,0xfa,0x76,0x99,0x19,0x55,0xeb,0xd6,0x31,0x59,0x43,0x4e,
            0xce,0xbb,0x4e,0x46,0x6d,0xae,0x5a,0x10,0x73,0xa6,0x72,0x76,0x27,0x09,0x7a,0x10,
            0x49,0xe6,0x17,0xd9,0x1d,0x36,0x10,0x94,0xfa,0x68,0xf0,0xff,0x77,0x98,0x71,0x30,
            0x30,0x5b,0xea,0xba,0x2e,0xda,0x04,0xdf,0x99,0x7b,0x71,0x4d,0x6c,0x6f,0x2c,0x29,
            0xa6,0xad,0x5c,0xb4,0x02,0x2b,0x02,0x70,0x9b
        },
        265,
        {
            0xee,0xad,0x9d,0x67,0x89,0x0c,0xbb,0x22,0x39,0x23,0x36,0xfe,0xa1,0x85,0x1f,0x38
        }
    }
};

static int test_aead() {
    printf("Testing chacha20-poly1305\n");
    uint8_t buffer[MAX_TEST_SIZE + RFC_8439_TAG_SIZE] = {0};
    for (int i = 0; i <AEAD_TEST_VECTORS; i++) {
        struct aead_test_vector t = aead_rfc_tests[i];
        printf("- %s: ", t.title);
        portable_chacha20_poly1305_encrypt(buffer, t.key, t.nonce, t.ad, t.ad_size, t.plain_text, t.size);
        if (memcmp(buffer, t.cipher_text, t.size) != 0) {
            printf("failed encryption\n");
            UNEXPECTED_RESULT(t.cipher_text, buffer, t.size)
            return -1;
        }
        else if (memcmp(buffer + t.size, t.mac, RFC_8439_TAG_SIZE) != 0) {
            printf("failed mac\n");
            UNEXPECTED_RESULT(t.mac, buffer, 16)
            return -1;
        }
        else {
            printf("success\n");
        }
    }
    return 0;
}


int main() {
    int result = 0;
    result += test_chacha20();
    result += test_poly();
    result += test_aead();
    if (result != 0) {
        printf("%d test failed\n", result * -1);
    }
    else {
        printf("All test succeeded\n");
    }
    return result;
}
