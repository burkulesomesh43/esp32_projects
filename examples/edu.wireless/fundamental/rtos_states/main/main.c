/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/rtos_queue/main/main.c
    
  @brief
    rtos states main file

  @details 
	  multiple task states

  @note
    https://docs.espressif.com/projects/esp-idf/en/v4.1.2/api-reference/system/freertos.html#task-api

  @author
    Somesh Burkule

  @date
    01/12/2023
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;
TaskHandle_t task4Handle = NULL;

void task4( void *pvParameter )
{
  printf( "task4  started\n" );

  while( 1 )
  {     
    vTaskDelay( 500/portTICK_PERIOD_MS );
    printf( "task4  running\n" );
  }
}

void task3( void *pvParameter )
{
  printf( "task3  started\n" );

  while( 1 )
  {     
    vTaskDelay( 5000/portTICK_PERIOD_MS );
    
    /* suspend task 4 for next 5 sec */
    printf( "task4  suspended\n" );
    vTaskSuspend( task4Handle );

    vTaskDelay( 5000/portTICK_PERIOD_MS ); 

    /* resume task 4 for next 5 sec */
    printf( "task4  resumed\n" );
    vTaskResume( task4Handle );
  }
}

void task2( void *pvParameter )
{
  printf( "task2  started\n" );

  while( 1 )
  {     
    vTaskDelay( 5000/portTICK_PERIOD_MS );

    printf( "task2 deleting...\n" );
    vTaskDelete( task2Handle );
    task2Handle = NULL;
  }
}

void task1(void *pvParameter)
{
  printf( "task1 started\n" );

  while( 1 )
  {
    if( task2Handle != NULL )
      printf( "task2 is alive\n" );
    else
      printf( "task2 is deleted\n" );
    vTaskDelay( 1000/portTICK_PERIOD_MS ); 
  }
}

void app_main()
{	
  /* task1 and task2 to understand terminate state */
  // xTaskCreate( &task1, "task1", 2048, NULL, 5, &task1Handle );
  // xTaskCreate( &task2, "task2", 2048, NULL, 5, &task2Handle );

  /* task3 and task4 to understand suspend and resume state */
  xTaskCreate( &task3, "task3", 2048, NULL, 5, &task3Handle );
  xTaskCreate( &task4, "task4", 2048, NULL, 5, &task4Handle );

  /* use scheduler to suspend and resume state */
  //vTaskStartScheduler();    
}
