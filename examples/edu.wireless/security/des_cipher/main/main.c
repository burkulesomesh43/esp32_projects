/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/security/des_cipher/main/main.c

  @brief
    Basic use of DES & DES3 Cipher for Encryption and Decryption with CBC Mode

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
#include "wolfssl/wolfcrypt/des3.h"

/* Use tag to print logs */
#define TAG "CRYPTOGRAPHY"

/* Key size for DES Cipher */
#define DES_BLOCK_SIZE_08   8
/* Key size for 3DES Cipher */
#define DES_BLOCK_SIZE_24  24

/* key for DES Cipher */
#define KEY_08             "SECURE09"
/* key for 3DES Cipher */
#define KEY_24             "SECURE090807060504030201"

/* Initialization Vector for DES Cipher */
#define IV_08              "IV-edu45"
/* Initialization Vector for 3DES Cipher */
#define IV_24              "IV-edu.wireless123456789"

/* plaintext for DES/3DES should be same or multiple of key size considered as block/blocks */
#define PLAINTEXT_08       "edu.wire"
#define PLAINTEXT_24       "edu.wireless-des3-cbcmod"

static void des_cipher_cbc_mode_test( void );
static void des3_cipher_cbc_mode_test( void );

static void des_cipher_cbc_mode_test( void )
{
  Des enc, dec;
  ESP_LOGI( TAG, "Entering des_cipher_cbc_mode_test" );
  /* 1 extra byte for null char */
  byte key[9] = { 0 };
  byte iv[9] = { 0 };     /* IV length same as Key */
  byte plain[9] = { 0 };
  byte cipher[9] = { 0 };

  memcpy( iv, IV_08, strlen( IV_08 ) );

  ESP_LOGI( TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### DES CBC-Mode ENC/DEC with 64 bit Key ###" );
  ESP_LOGI( TAG, "---------------------------------------------" );
  memcpy( key, KEY_08, strlen( KEY_08 ) );
  memcpy( plain, PLAINTEXT_08, strlen( PLAINTEXT_08 ) );
  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

  /* initialize encrypt context with set key and iv */
  wc_Des_SetKey( &enc, key, iv, DES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_Des_CbcEncrypt( &enc, cipher, plain, DES_BLOCK_SIZE_08 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_Des_SetKey( &dec, key, iv, DES_DECRYPTION );

  /* DES Decryption with CBC mode & 64 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_Des_CbcDecrypt( &dec, plain, cipher, DES_BLOCK_SIZE_08 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  ESP_LOGI( TAG, "Leaving des_cipher_cbc_mode_test" );
}

static void des3_cipher_cbc_mode_test( void )
{
  Des3 enc, dec;
  ESP_LOGI( TAG, "Entering des3_cipher_cbc_mode_test" );
  /* 1 extra byte for null char */
  byte key[25] = { 0 };
  byte iv[25] = { 0 };     /* IV length same as Key */
  byte plain[25] = { 0 };
  byte cipher[25] = { 0 };

  memcpy( iv, IV_24, strlen( IV_24 ) );

  ESP_LOGI( TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### DES3 CBC-Mode ENC/DEC with 192 bit Key ###" );
  ESP_LOGI( TAG, "---------------------------------------------" );
  memcpy( key, KEY_24, strlen( KEY_24 ) );
  memcpy( plain, PLAINTEXT_24, strlen( PLAINTEXT_24 ) );
  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

  /* initialize encrypt context with set key and iv */
  wc_Des3_SetKey( &enc, key, iv, DES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_Des3_CbcEncrypt( &enc, cipher, plain, DES_BLOCK_SIZE_24 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_Des3_SetKey( &dec, key, iv, DES_DECRYPTION );

  /* DES3 Decryption with CBC mode & 192 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_Des3_CbcDecrypt( &dec, plain, cipher, DES_BLOCK_SIZE_24 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  ESP_LOGI( TAG, "Leaving des3_cipher_cbc_mode_test" );
}


void app_main()
{
  des_cipher_cbc_mode_test();
  des3_cipher_cbc_mode_test();
}
