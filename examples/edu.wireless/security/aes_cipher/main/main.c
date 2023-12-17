/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/security/aes_cipher/main/main.c

  @brief
    Basic use of AES Cipher for Encryption and Decryption with CBC Mode

  @details

  @note

  @author
    Somesh Burkule

  @date
    07/12/23
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
#define AES_BLOCK_SIZE_24  24
#define AES_BLOCK_SIZE_32  32
/* Bigger the key size( 128/192/256 bits ), more the processing time. */
#define KEY_16             "DEVICESECURITY12"
#define KEY_24             "DEVICESECURITY1234567890"
#define KEY_32             "DEVICESECURITY123456789012345678"
/* 16 byte = 128 bits of Initialization Vector for all modes and all key sizes */
#define IV                 "IV-edu.wireless1"
/* plaintext should be same or multiple of key size considered as block/blocks */
#define PLAINTEXT_16       "edu.wireless-aes"
#define PLAINTEXT_24       "edu.wireless-aes-cbc-mod"
#define PLAINTEXT_32       "edu.wireless-aes-cbc-mode-32-byt"

static void aes_cipher_cbc_mode_test( void );
static void aes_cipher_ctr_mode_test( void );

static void aes_cipher_cbc_mode_test( void )
{
  Aes enc, dec;
  ESP_LOGI( TAG, "Entering aes_cipher_cbc_mode_test" );
  /* 1 extra byte for null char */
  byte key[33] = { 0 };
  byte iv[17] = { 0 };
  byte plain[33] = { 0 };
  byte cipher[33] = { 0 };

  /* initialize encrypt context */
  wc_AesInit( &enc, NULL, INVALID_DEVID );
  wc_AesInit( &dec, NULL, INVALID_DEVID );

  /* same IV size for different key size option */
  memcpy( iv, IV, strlen( IV ) );

  ESP_LOGI( TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### AES CBC-Mode ENC/DEC with 128 bit Key ###" );
  ESP_LOGI( TAG, "---------------------------------------------" );
  memcpy( key, KEY_16, strlen( KEY_16 ) );
  memcpy( plain, PLAINTEXT_16, strlen( PLAINTEXT_16 ) );
  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

  /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_16, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesCbcEncrypt( &enc, cipher, plain, AES_BLOCK_SIZE_16 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_16, iv, AES_DECRYPTION );

  /* AES Decryption with CBC mode & 128 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesCbcDecrypt( &dec, plain, cipher, AES_BLOCK_SIZE_16 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  ESP_LOGI( TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### AES CBC-Mode ENC/DEC with 192 bit Key ###" );
  ESP_LOGI( TAG, "---------------------------------------------" );
  memset( key, 0, sizeof( key ) );
  memset( plain, 0, sizeof( plain ) );

  memcpy( key, KEY_24, strlen( KEY_24 ) );
  memcpy( plain, PLAINTEXT_24, strlen( PLAINTEXT_24 ) );
  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

    /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_24, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesCbcEncrypt( &enc, cipher, plain, AES_BLOCK_SIZE_24 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_24, iv, AES_DECRYPTION );

  /* AES Decryption with CBC mode & 128 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesCbcDecrypt( &dec, plain, cipher, AES_BLOCK_SIZE_24 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  ESP_LOGI( TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### AES CBC-Mode ENC/DEC with 256 bit Key ###" );
  ESP_LOGI( TAG, "---------------------------------------------" );
  memset( key, 0, sizeof( key ) );
  memset( plain, 0, sizeof( plain ) );

  memcpy( key, KEY_32, strlen( KEY_32 ) );
  memcpy( plain, PLAINTEXT_32, strlen( PLAINTEXT_32 ) );
  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

  /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_32, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesCbcEncrypt( &enc, cipher, plain, AES_BLOCK_SIZE_32 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_32, iv, AES_DECRYPTION );

  /* AES Decryption with CBC mode & 256 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesCbcDecrypt( &dec, plain, cipher, AES_BLOCK_SIZE_32 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  wc_AesFree(&enc);
  wc_AesFree(&dec);

  ESP_LOGI( TAG, "Leaving aes_cipher_cbc_mode_test" );
}

static void aes_cipher_ctr_mode_test( void )
{
  Aes enc, dec;
  ESP_LOGI( TAG, "Entering aes_cipher_ctr_mode_test" );
  /* 1 extra byte for null char */
  byte key[33] = { 0 };
  byte iv[17] = { 0 };
  byte plain[33] = { 0 };
  byte cipher[33] = { 0 };

  /* initialize encrypt context */
  wc_AesInit( &enc, NULL, INVALID_DEVID );
  wc_AesInit( &dec, NULL, INVALID_DEVID );

  /* same IV size for different key size option */
  memcpy( iv, IV, strlen( IV ) );

  /* ###################### AES CTR-Mode ENC/DEC with 128 bit Key ###################### */
  ESP_LOGI(TAG, "---------------------------------------------" );
  ESP_LOGI(TAG, "### AES CTR-Mode ENC/DEC with 128 bit Key ###" );
  ESP_LOGI(TAG, "---------------------------------------------" );
  memcpy( key, KEY_16, strlen( KEY_16 ) );
  memcpy( plain, PLAINTEXT_16, strlen( PLAINTEXT_16 ) );
  ESP_LOGI(TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI(TAG, "I/P Key        : %s", key );
  ESP_LOGI(TAG, "I/P IV         : %s", iv );

  /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_16, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesCtrEncrypt( &enc, cipher, plain, AES_BLOCK_SIZE_16 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_16, iv, AES_DECRYPTION );

  /* AES Decryption with CTR mode & 128 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesCtrEncrypt( &dec, plain, cipher, AES_BLOCK_SIZE_16 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  /* ###################### AES CTR-Mode ENC/DEC with 192 bit Key ###################### */
  ESP_LOGI(TAG, "---------------------------------------------" );
  ESP_LOGI(TAG, "### AES CTR-Mode ENC/DEC with 192 bit Key ###" );
  ESP_LOGI(TAG, "---------------------------------------------" );
  memset( key, 0, sizeof( key ) );
  memset( plain, 0, sizeof( plain ) );

  memcpy( key, KEY_24, strlen( KEY_24 ) );
  memcpy( plain, PLAINTEXT_24, strlen( PLAINTEXT_24 ) );
  ESP_LOGI(TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI(TAG, "I/P Key        : %s", key );
  ESP_LOGI(TAG, "I/P IV         : %s", iv );

  /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_24, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesCtrEncrypt( &enc, cipher, plain, AES_BLOCK_SIZE_24 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_24, iv, AES_DECRYPTION );

  /* AES Decryption with CTR mode & 128 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesCtrEncrypt( &dec, plain, cipher, AES_BLOCK_SIZE_24 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  /* ###################### AES CTR-Mode ENC/DEC with 256 bit Key ###################### */
  ESP_LOGI(TAG, "---------------------------------------------" );
  ESP_LOGI( TAG, "### AES CTR-Mode ENC/DEC with 256 bit Key ###" );
  ESP_LOGI(TAG, "---------------------------------------------" );
  memset( key, 0, sizeof( key ) );
  memset( plain, 0, sizeof( plain ) );

  memcpy( key, KEY_32, strlen( KEY_32 ) );
  memcpy( plain, PLAINTEXT_32, strlen( PLAINTEXT_32 ) );
  ESP_LOGI( TAG, "I/P Plain Text : %s", plain );
  ESP_LOGI( TAG, "I/P Key        : %s", key );
  ESP_LOGI( TAG, "I/P IV         : %s", iv );

  /* set key and iv to encrypt context */
  wc_AesSetKey( &enc, key, AES_BLOCK_SIZE_32, iv, AES_ENCRYPTION );

  ESP_LOGI( TAG, "Encrypting..." );
  wc_AesCtrEncrypt( &enc, cipher, plain, AES_BLOCK_SIZE_32 );

  ESP_LOGI( TAG, "-> O/P Cipher Text: %s", cipher );

  /* set Key and IV to Decrypt context */
  wc_AesSetKey( &dec, key, AES_BLOCK_SIZE_32, iv, AES_DECRYPTION );

  /* AES Decryption with CTR mode & 256 bit key size. */
  ESP_LOGI( TAG, "Decrypting..." );
  wc_AesCtrEncrypt( &dec, plain, cipher, AES_BLOCK_SIZE_32 );

  ESP_LOGI( TAG, "-> O/P Plain Text : %s", plain );

  wc_AesFree(&enc);
  wc_AesFree(&dec);

  ESP_LOGI( TAG, "Leaving aes_cipher_ctr_mode_test" );
}

void app_main()
{
  aes_cipher_cbc_mode_test();
  aes_cipher_ctr_mode_test();
}
