/*===============================================================================================*/
/*!
  @file
    examples/edu.wireless/continuous_ble_scanner/main/main.c

  @brief
    continuous ble scan with bluedroid source code.file

  @details 
    https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/bluetooth/bt_le.html

  @note

  @author
    Somesh Burkule

  @date
    07/08/2023
*/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "esp_bt.h"
#include "nvs_flash.h"
#include "esp_bt_device.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#define BLE_TAG                   "BLE_SCAN"

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);

static esp_ble_scan_params_t ble_scan_params = {
    .scan_type              = BLE_SCAN_TYPE_PASSIVE,
    .own_addr_type          = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy     = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval          = 0x00A0,  					//100ms
    .scan_window            = 0x00A0,  					//100ms
    .scan_duplicate         = BLE_SCAN_DUPLICATE_DISABLE
};

uint32_t duration = 1;  // scan duration = 1 sec

unsigned int scan_count = 0;

static void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{

    esp_err_t err;

    switch(event){
    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT: {

        if((err = param->scan_param_cmpl.status) != ESP_BT_STATUS_SUCCESS){
            ESP_LOGE(BLE_TAG, "Scan param set failed: %s", esp_err_to_name(err));
            break;
        }

        esp_ble_gap_start_scanning(duration);
        break;
    }
    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:

    	if ((err = param->scan_start_cmpl.status) != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(BLE_TAG, "Scan start failed: %s", esp_err_to_name(err));
            break;
        }

        ESP_LOGI(BLE_TAG, "Scan start successed");
        break;
    case ESP_GAP_BLE_SCAN_STOP_COMPLETE_EVT:

        if ((err = param->scan_stop_cmpl.status) != ESP_BT_STATUS_SUCCESS) {
            ESP_LOGE(BLE_TAG, "Scan stop failed: %s", esp_err_to_name(err));
            break;
        }
        ESP_LOGI(BLE_TAG, "Scan stop successed");

        break;
    case ESP_GAP_BLE_SCAN_RESULT_EVT: {

        esp_ble_gap_cb_param_t *scan_result = (esp_ble_gap_cb_param_t *)param;

        if( scan_result->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT ){

            ESP_LOGI(BLE_TAG, "<<<-----Scan Timeout----->>>Total Scan Count: %u\r\n",scan_count);
			esp_ble_gap_start_scanning(duration);
			scan_count = 0;
	    }
        else{

			scan_count ++;

			printf("{MAC: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X,",
					scan_result->scan_rst.bda[0],
					scan_result->scan_rst.bda[1],
					scan_result->scan_rst.bda[2],
					scan_result->scan_rst.bda[3],
					scan_result->scan_rst.bda[4],
					scan_result->scan_rst.bda[5]);
			printf("ADV_DATA:");
			for(uint8_t i = 0; i< scan_result->scan_rst.adv_data_len; i++)
				printf("%.2X",scan_result->scan_rst.ble_adv[i]);

			printf(",RSSI:%d}-->>SCAN COUNT: %u\r\n\r\n",scan_result->scan_rst.rssi,scan_count);
        }

        break;
    }

    default:
        break;
    }
}

void ble_init(void)
{
    esp_err_t status;
    char err_msg[20];

    ESP_LOGI(BLE_TAG, "register callback");

    //register the scan callback function to the gap module
    if ((status = esp_ble_gap_register_callback(esp_gap_cb)) != ESP_OK) {
        ESP_LOGE(BLE_TAG, "gap register error: %s", esp_err_to_name_r(status, err_msg, sizeof(err_msg)));
        return;
    }
}

void app_main()
{
	esp_err_t status;
	ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

	esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    nvs_flash_init();

    status = esp_bt_controller_init(&bt_cfg);
	if (status) {

	   ESP_LOGE(BLE_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(status));
	   return;
	}

	status = esp_bt_controller_enable(ESP_BT_MODE_BLE);
	if (status) {

	   ESP_LOGE(BLE_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(status));
	   return;
	}

	ESP_LOGI(BLE_TAG, "%s init bluetooth\n", __func__);
	status = esp_bluedroid_init();
	if (status) {

	   ESP_LOGE(BLE_TAG, "%s init bluetooth failed: %s\n", __func__, esp_err_to_name(status));
	   return;
	}
	status = esp_bluedroid_enable();
	if (status) {

	   ESP_LOGE(BLE_TAG, "%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(status));
	   return;
	}

	esp_ble_gap_set_scan_params(&ble_scan_params);

    ble_init();
}
