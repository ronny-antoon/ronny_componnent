#include "metric_tracker.hpp"

#include "matter_setup.hpp"
#include "lightAccessory.hpp"
#include <endpoint/root_node.hpp>
#include <endpoint/aggregator.hpp>
#include <endpoint/on_off_light.hpp>

extern "C" void app_main()
{
    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    /* Initialize the Matter Callbacks */
    esp_matter::attribute::set_callback(app_attribute_update_cb);
    esp_matter::identification::set_callback(app_identification_cb);

    /* Create a Matter root node on endpoint 0 */
    metahouse::endpoint::root_node::config_t node_config;
    esp_matter::node_t *node = metahouse::endpoint::root_node::create(&node_config);

    /* Create a Matter aggregator endpoint */
    metahouse::endpoint::aggregator::config_t aggregator_config;
    esp_matter::endpoint_t *aggregator = metahouse::endpoint::aggregator::create(node, &aggregator_config);

    /* Create a Matter on/off light endpoint */
    metahouse::endpoint::on_off_light::config_t on_off_light_config;
    esp_matter::endpoint_t *on_off_light = metahouse::endpoint::on_off_light::create(node, &on_off_light_config, aggregator);

    /* Matter start */
    esp_matter::start(app_event_cb);

    metric_tracker_init("http://192.168.3.196:3001/metrics", "DevKitModule", 800);

    /* Send the metrics to the server automatically every 30 seconds */
    metric_tracker_auto_send(30, true, true);

    LightAccessory *lightAccessory = new LightAccessory(GPIO_NUM_5, GPIO_NUM_2);
    ESP_LOGI(__FILENAME__, "LightAccessory created.");
}