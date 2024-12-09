#include "80211_mac.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hardware.h"

mac_state_t mac_state = INACTIVE;

static bool network_found = false, network_authenticated = false, network_associated = false;
static volatile bool sb_wifi_connect = false;

char* TAG = "80211_mac";

// State transition function
void transition_state(mac_state_t cur_state){
    switch cur_state:

        case MAC_INACTIVE:
            if(sb_wifi_connect){
                ESP_LOGI(TAG, "Switching to scanning...");
                return MAC_SCAN;
            }
            break;
        
        case MAC_SCAN:
            if(network_found){
                ESP_LOGI(TAG, "In scanning mode. Transitioning to auth");
                return MAC_AUTHENTICATE;
            }
            break;

        case MAC_AUTHENTICATE:
            if(network_authenticated){
                ESP_LOGI(TAG, "In Authentication mode. Transitioning to association");
                return MAC_ASSOCIATE;
            }
            break;

        case MAC_ASSOCIATE:
            if(network_associated){
                ESP_LOGI(TAG, "In Association mode. Transitioning to connected...");
                return MAC_CONNECTED;
            }
            break;

        case MAC_CONNECTED:
            if(!sb_wifi_connect){
                ESP_LOGI(TAG, "In Connected mode. Transitioning to inactive");
                return MAC_INACTIVE;
            }
            break;

        // Return current state and proceed
        return cur_state;

}

// State function
void mac_handle_state(mac_state){
    case MAC_INACTIVE:
        // Do nothing
        break;
    case MAC_SCAN:
        // Set filters to start receiving beacons
        break;
    case MAC_AUTHENTICATE:
        // Set filters to stop receiving beacons
        // Send auth frame and wait
        break;
    case MAC_ASSOCIATE:
        // Send association frame and wait
        break;
    case MAC_CONNECTED:
        // Do nothing
        break;

    return;

}

// Starts the connection process
void mac_layer_init(){
    sb_wifi_connect = true;
}

// Stops the connection.
// Device stops sending out frames
void mac_layer_deinit(){
    sb_wifi_connect = false;
}

void mac_task(){
    ESP_LOGI(TAG, "Starting MAC task");
    while(1){
        // State machine. Monitor queue for events
        mac_state = transition_state(mac_state);

        mac_handle_state(mac_state);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}