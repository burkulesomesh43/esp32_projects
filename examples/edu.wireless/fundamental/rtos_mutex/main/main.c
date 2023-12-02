/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/rtos_mutex/main/main.c
    
  @brief
    rtos mutex feature main file

  @details 
	  accessing shared resource between tasks

  @note
    https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#semaphore-api

  @author
    Somesh Burkule

  @date
    01/12/2023
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Initialise mutex Handle */
SemaphoreHandle_t xMutex = NULL;

char data[50] = {0};

void task2( void *pvParameter )
{
    printf( "task2  started\n" );
    char msg[] = "potatoes";

    while( 1 )
    {
      /* critical section */
      if( xSemaphoreTake( xMutex, portMAX_DELAY ) == 1 )
      {
        printf( "task2 mutex locked\n" );
        for( int i = 0; i < 9; i++ )
        {
          data[i] = msg[i];
          vTaskDelay( 50/portTICK_PERIOD_MS );
        }
        data[9] = '\0';

        printf( "task2 data: %s\n", data );

        xSemaphoreGive( xMutex );
        printf( "task2 mutex unlocked\n" );
      }

      vTaskDelay( 100/portTICK_PERIOD_MS );
    }
}

void task1(void *pvParameter)
{
    printf( "task1  started\n" );
    char msg[] = "tomatoes";

    while( 1 )
    {
       /* critical section */
      if( xSemaphoreTake( xMutex, portMAX_DELAY ) == 1 )
      {
        printf( "task1 mutex locked\n" );
        for( int i = 0; i < 9; i++ )
        {
          data[i] = msg[i];
          vTaskDelay( 50/portTICK_PERIOD_MS );
        }
        data[9] = '\0';

        printf( "task1 data: %s\n", data );

        xSemaphoreGive( xMutex );
        printf( "task1 mutex unlocked\n" );
      }

      vTaskDelay( 100/portTICK_PERIOD_MS );
    }
}

void app_main()
{	
    /* Create mutex */
    xMutex = xSemaphoreCreateMutex();

    vTaskDelay( 100/portTICK_PERIOD_MS );

    /* check if mutex is created successfully */
    if( xMutex != NULL )
    {
        printf( "Mutex is created\n" );

        xTaskCreate( &task1, "task1", 2048, NULL, 5, NULL );
        xTaskCreate( &task2, "task2", 2048, NULL, 5, NULL );   
    }
    else
    {
        printf( "Mutex creation failed" );
    }    
}
