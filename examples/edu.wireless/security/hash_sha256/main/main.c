/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/security/hash_sha256/main/main.c

  @brief
    Basic use of sha256 hashing algorithm

  @details

  @note

  @author
    Somesh Burkule

  @date
    20/12/23
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
#include <wolfssl/wolfcrypt/sha256.h>

/* Use tag to print logs */
#define TAG "CRYPTOGRAPHY"

/* test to be hashed */
#define PLAINTEXT          "This is test message from edu.wireless"

#define MAX_PLAINTEXT_SIZE 1024


static void hash_sha256_test( void );
static void print_hex( char *label, unsigned char *data, int len );


static void print_hex( char *label, unsigned char *data, int len )
{
  printf( "%s:", label );
  for( int i = 0; i < len; ++i )
  {
    printf( " %02X", data[i] );
  }
  printf( "\n" );
}

/* sha256 hashing algorithm generate 256bits
 * unique data for the input provided of any size */
static void hash_sha256_test( void )
{
  Sha256 sha256;
  byte digest[SHA256_DIGEST_SIZE];

  ESP_LOGI( TAG, "Entering hash_sha256_test" );

  char *plaintext = ( char * )malloc( MAX_PLAINTEXT_SIZE );
  if( plaintext == NULL )
  {
    ESP_LOGI( TAG, "malloc failed" );
    return;
  }

  memset( plaintext, 0, MAX_PLAINTEXT_SIZE );
  memset( digest, 0, SHA256_DIGEST_SIZE );
  memcpy( plaintext, PLAINTEXT, strlen( PLAINTEXT ) );

  /* Initialise sha256 context */
  wc_InitSha256( &sha256 );

  /* Fill the plaintext data in sha256 context*/
  wc_Sha256Update( &sha256, ( const byte * )plaintext, strlen( plaintext ) );

  /* covert plaintext to hash digest */
  wc_Sha256Final( &sha256, digest );

  print_hex( "CRYPTOGRAPHY: Digest", digest, SHA256_DIGEST_SIZE );

  ESP_LOGI( TAG, "Leaving hash_sha256_test" );
}

void app_main()
{
  hash_sha256_test();
}
