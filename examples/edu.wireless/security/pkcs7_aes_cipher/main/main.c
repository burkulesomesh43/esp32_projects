/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/security/pkcs7_aes_cipher/main/main.c

  @brief
    Basic use PKCS7 Padding and AES Cipher for Encryption and Decryption with CBC Mode

  @details

  @note

  @author
    Somesh Burkule

  @date
    17/12/23
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"

/* wolfSSL */
#include <wolfssl/wolfcrypt/settings.h>
#include <user_settings.h>
#include <wolfssl/version.h>
#ifndef WOLFSSL_ESPIDF
#warning "problem with wolfSSL user settings. Check components/wolfssl/include"
#endif
#include "wolfssl/wolfcrypt/aes.h"

/* Use tag to print logs */
#define TAG "CRYPTOGRAPHY"

/* Different Key sizes to choose for AES Cipher */
#define AES_BLOCK_SIZE_16  16

/* AES 16 byte key */
#define KEY_16             "DEVICESECURITY12"

/* 16 byte = 128 bits of Initialization Vector */
#define IV                 "IV-edu.wireless1"
/* plaintext should be same or multiple of key size, if not then padding is needed */
#define PLAINTEXT          "This is a test message from edu.wireless"

#define MAX_PT_PAD_LEN     100

static void print_hex( char *label, unsigned char *data, int len );
static void aes_cipher_cbc_mode_test( void );


static void print_hex( char *label, unsigned char *data, int len )
{
  printf( "%s:", label );
  for( int i = 0; i < len; ++i )
  {
    printf( " %02X", data[i] );
  }
  printf( "\n" );
}

static void aes_cipher_cbc_mode_test( void )
{
  Aes enc, dec;

  ESP_LOGI( TAG, "Entering aes_cipher_cbc_mode_test" );
  /* 1 extra byte for null char */
  byte key[17] = { 0 };
  byte iv[17] = { 0 };
  byte plain[MAX_PT_PAD_LEN] = { 0 }; /* considering max 6 blocks of 16 bytes can be stored*/
  byte cipher[MAX_PT_PAD_LEN] = { 0 };

  /* initialize encrypt context */
  wc_AesInit( &enc, NULL, INVALID_DEVID );
  wc_AesInit( &dec, NULL, INVALID_DEVID );

  /* same IV size for different key size option */
  memcpy( iv, IV, strlen( IV ) );

  ESP_LOGI( TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### AES CBC-Mode ENC/DEC with 16 byte = 128 bit Key ###" );
  ESP_LOGI( TAG, "---------------------------------------------" );
  memcpy( key, KEY_16, strlen( KEY_16 ) );

  /* if plaintext is not in multiple of 1 block = 16 byte, need to do padding using pkcs7
   * wolfSSL already take care of it using direct api's */
  memcpy( plain, PLAINTEXT, strlen( PLAINTEXT ) );

  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

  /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_16, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesEncryptDirect( &enc, cipher, plain );

  print_hex( "CRYPTOGRAPHY: CipherText", cipher, strlen( ( char *)cipher ) );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_16, iv, AES_DECRYPTION );

  /* AES Decryption with CBC mode & 128 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesDecryptDirect( &dec, plain, cipher );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  wc_AesFree(&enc);
  wc_AesFree(&dec);

  ESP_LOGI( TAG, "Leaving aes_cipher_cbc_mode_test" );
}

void app_main()
{
  aes_cipher_cbc_mode_test();
}
