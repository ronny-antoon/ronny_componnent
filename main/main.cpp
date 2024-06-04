#include "metric_tracker.hpp"

#include "matter_setup.hpp"
#include "lightAccessory.hpp"

extern "C" void app_main()
{
    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    /* Create a Matter node and add the mandatory Root Node device type on endpoint 0 */
    esp_matter::node::config_t node_config;

    // node handle can be used to add/modify other endpoints.
    esp_matter::node_t *node = esp_matter::node::create(&node_config, app_attribute_update_cb, app_identification_cb);

    esp_matter::endpoint::on_off_light::config_t on_off_light_config;
    esp_matter::endpoint::on_off_light::create(node, &on_off_light_config, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);

    /* Matter start */
    esp_matter::start(app_event_cb);

    metric_tracker_init("http://192.168.3.196:3001/metrics", "DevKitModule", 800);

    /* Send the metrics to the server automatically every 30 seconds */
    metric_tracker_auto_send(30, true, true);

    LightAccessory *lightAccessory = new LightAccessory(GPIO_NUM_5, GPIO_NUM_2);
    ESP_LOGI(__FILENAME__, "LightAccessory created");
}