/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/rtos_notif/main/main.c
    
  @brief
    rtos task notifications feature main file

  @details 
	  send task notifications between tasks.

  @note
    https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html

  @author
    Somesh Burkule

  @date
    01/12/2023
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Initialise task Handle */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

#define TASK1_NOTIFY_VALUE 1

void task1( void *pvParameter )
{
    printf( "task1  started\n" );

    while( 1 )
    {
      // xTaskNotify( task2Handle, TASK1_NOTIFY_VALUE, eSetBits );
      xTaskNotifyGive( task2Handle );
      vTaskDelay( 1000/portTICK_PERIOD_MS );
    }
}

void task2(void *pvParameter)
{
    printf( "task2  started\n" );
    uint32_t result = 0;;

    while( 1 )
    {
      // if( xTaskNotifyWait( 0, 0, &result, ( TickType_t )portMAX_DELAY ) == pdTRUE )
      //  printf( "result: %d\n", result);
      result = ulTaskNotifyTake( pdTRUE, ( TickType_t )portMAX_DELAY );
      
      if( result > 0 )
        printf( "result: %d\n", result);

      vTaskDelay( 10/portTICK_PERIOD_MS ); 
    }
}

void app_main()
{	

  xTaskCreate( &task1, "task1", 2048, NULL, 5, &task1Handle );
  xTaskCreate( &task2, "task2", 2048, NULL, 5, &task2Handle );     
}
