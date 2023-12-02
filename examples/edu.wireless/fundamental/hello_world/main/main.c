/*===============================================================================================*/
/*         >>> Copyright (C) edu.wireless All Rights Reserved. <<<          */
/*!
  @file
    examples/edu.wireless/fundamental/hello_world/main/main.c
    
  @brief
    Hello world source code.file

  @details 
	  Print Hello world message on terminal

  @note

  @author
    Somesh Burkule

  @date
    01/12/2023
*/

/* Include the header files using preprocessor */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

/*
 * Starting point of code
*/
void app_main(void)
{
	
  printf( "Hello world!\n" );

  for ( int i = 10; i >= 0; i-- ) 
	{
    printf("Restarting in %d seconds...\n", i);
		
    /* Add delay to run at specific interval */
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
	
  printf("Restarting now.\n");
    
	/* Reset the esp32 device. */
  esp_restart();
}
