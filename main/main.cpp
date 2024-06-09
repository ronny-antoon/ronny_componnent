#include <esp_matter.h>
#include <nvs_flash.h>

#include <ButtonDevice.hpp>
#include <FanDevice.hpp>
#include <LightDevice.hpp>
#include <PlugInDevice.hpp>
#include <WindowDevice.hpp>

#include "app_callback.hpp"
#include "metric_tracker.hpp"

extern "C" void app_main() {
  /* Initialize the ESP NVS layer */
  nvs_flash_init();

  /* Initialize the Matter stack */
  esp_matter::node::config_t node_config;
  esp_matter::endpoint_t *node =
      esp_matter::node::create(&node_config, app_attribute_cb, app_identification_cb);

  esp_matter::endpoint::aggregator::config_t aggregator_config;
  esp_matter::endpoint_t *aggregator = esp_matter::endpoint::aggregator::create(
      node, &aggregator_config, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);

  /* Create the LightDevice */
  LightDevice *lightDevice = new LightDevice("LightDevice", GPIO_NUM_23, GPIO_NUM_34, aggregator);

  /* Create the FanDevice */
  FanDevice *fanDevice = new FanDevice("FanDevice", GPIO_NUM_22, GPIO_NUM_35, aggregator);

  /* Create the PlugInDevice */
  PlugInDevice *plugInDevice = new PlugInDevice("PlugInDevice", GPIO_NUM_21, GPIO_NUM_32, aggregator);

  /* Create the ButtonDevice */
  ButtonDevice *buttonDevice = new ButtonDevice("ButtonDevice", GPIO_NUM_18, aggregator);

  /* Create the WindowDevice */
  WindowDevice *windowDevice =
      new WindowDevice("WindowDevice", GPIO_NUM_2, GPIO_NUM_17, GPIO_NUM_5, GPIO_NUM_26, 10, 10, aggregator);

  esp_matter::start(app_event_cb);

  /* Initialize the metric tracker */
  metric_tracker_init("http://192.168.3.196:3001/metrics", "DevKitModule", 800);

  /* Send the metrics to the server automatically every 30 seconds */
  metric_tracker_auto_send(30, true, true);
}