/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/uart_echo/main/main.c
  @brief
    Inbuilt uart echo source code.file

  @details
    The esp32 send a string and receives the same data through uart.

  @note
    API Refernces:
    https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html

  @author
    Somesh Burkule

  @date
    01/12/2023
*/


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"

/*
 * - Port: UART2
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below
 */

#define ECHO_TEST_TXD  (17)
#define ECHO_TEST_RXD  (16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)
#define UART_PORT      (UART_NUM_2)

/* buffer size */
#define BUF_SIZE (1024)

uart_config_t uart_config = {
                              .baud_rate = 115200,
                              .data_bits = UART_DATA_8_BITS,
                              .parity    = UART_PARITY_DISABLE,
                              .stop_bits = UART_STOP_BITS_1,
                              .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
                              }; 


static void echo_task(void *pvParameter)
{
  /* Configure a temporary buffer for the incoming data */
  uint8_t *data = ( uint8_t * ) malloc( BUF_SIZE );
  int length = 0;

  while ( 1 ) 
  {
    /* Read data from the UART */
    length = uart_read_bytes( UART_PORT,  data, BUF_SIZE, 20 / portTICK_RATE_MS );
    if( length != 0 )
    {
      /* Write data back to the UART */
      uart_write_bytes( UART_PORT, ( const char * ) data, length ); 
    }
    else
    {
      printf( "uart failed\n" );
    }

    vTaskDelay( 10 / portTICK_PERIOD_MS );
  }    
}

void app_main()
{
  /* Configure parameters of an UART driver,
  * communication pins and install the driver */

  uart_param_config( UART_PORT, &uart_config );
  uart_set_pin( UART_PORT, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS );
  uart_driver_install( UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0 );

  xTaskCreate( echo_task, "uart_echo_task", 1024, NULL, 10, NULL );
}