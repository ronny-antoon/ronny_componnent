#include "metric_tracker.hpp"

#include "wifi_setup.hpp"
#include "lightAccessory.hpp"

extern "C" void app_main()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(__FILENAME__, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    metric_tracker_init("http://192.168.3.196:3001/metrics", "DevKitModule", 800);

    /* Send the metrics to the server automatically every 30 seconds */
    metric_tracker_auto_send(30, true, true);

    LightAccessory *lightAccessory = new LightAccessory(GPIO_NUM_5, GPIO_NUM_2);
    ESP_LOGI(__FILENAME__, "LightAccessory created");
}