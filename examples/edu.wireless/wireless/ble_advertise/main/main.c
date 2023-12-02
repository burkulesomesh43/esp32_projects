/*===============================================================================================*/
/*!
  @file
    examples/edu.wireless/ble_advertise/main/main.c

  @brief
    ble advertising with bluedroid source code.file

  @details 
    https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/bluetooth/bt_le.html

  @note

  @author
    Somesh Burkule

  @date
    07/08/2023
*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"


#define TAG "BLE_ADVERTISING"

static esp_ble_adv_params_t ble_adv_params = {
	
	.adv_int_min = 0x20,                     /* 32 * 0.625 = 20ms */
	.adv_int_max = 0x40,                     /* 64 * 0.625 = 40ms */
	.adv_type = ADV_TYPE_NONCONN_IND,
	.own_addr_type  = BLE_ADDR_TYPE_PUBLIC,
	.channel_map = ADV_CHNL_ALL,
	.adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static uint8_t adv_raw_data[] = { 0x02,   /* AD struct 1, Length: 2 */
                                  0x01,   /* AD struct 1, Type: Flag */
								  0x06,   /* AD struct 1, Data: General Discoverable, BrEdr Not Supported */
								  0x0D,   /* AD struct 2, Length: 13 */
								  0x09,   /* AD struct 2, Type: Complete Local Name */
								  0x65,0x64,0x75,0x2E,0x77,0x69,0x72,0x65,0x6c,0x65,0x73,0x73, /* AD struct 2, Data: edu.wireless */
								  0x02,   /* AD struct 3, Length: 2 */
								  0x0A,   /* AD struct 3, Type: Tx Power Level */
								  0x04 }; /* AD struct 3, Data: 4 dbm */

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch ( event )
	{
		case ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT: 

			ESP_LOGI( TAG,"ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT\n" );
			esp_ble_gap_start_advertising( &ble_adv_params );
			break;			

		case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:

			ESP_LOGI( TAG,"ESP_GAP_BLE_ADV_START_COMPLETE_EVT\n" );
			if( param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS ) {
				ESP_LOGI( TAG,"Advertising started\n\n" );
			}
			else ESP_LOGI( TAG,"Unable to start advertising process, error code %d\n\n", param->scan_start_cmpl.status );
			break;

		default:
			ESP_LOGI( TAG,"Event %d unhandled\n\n", event );
			break;
	}
}


void app_main()
{

	ESP_ERROR_CHECK( nvs_flash_init() );

	/* release memory reserved for classic BT (not used) */
	ESP_ERROR_CHECK( esp_bt_controller_mem_release( ESP_BT_MODE_CLASSIC_BT ) );

	/* initialize the BT controller with the default config */
	esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init( &bt_cfg );

	/* enable the BT controller in BLE mode */
    esp_bt_controller_enable( ESP_BT_MODE_BLE );

	/* initialize Bluedroid library */
	esp_bluedroid_init();
    esp_bluedroid_enable();

	/* register GAP callback function */
	ESP_ERROR_CHECK( esp_ble_gap_register_callback( esp_gap_cb ) );
	
	/* configure the adv data */
	ESP_ERROR_CHECK( esp_ble_gap_config_adv_data_raw( adv_raw_data, 17 ) );
}
