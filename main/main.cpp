#include <esp_matter.h>
#include <nvs_flash.h>

#include <ButtonModule.hpp>
#include <LightAccessory.hpp>
#include <LightDevice.hpp>
#include <RelayModule.hpp>

#include "accesspoint.hpp"
#include "app_callback.hpp"
#include "database_helper.hpp"
#include "metric_tracker.hpp"
#include "myConfig.hpp"
#include "webserver.hpp"

extern "C" void app_main() {
  /* Initialize the ESP NVS layer */
  nvs_flash_init();

  /* Check if the device is in ProgramMode */
  if (checkProgramMode()) {
    // If the device is in ProgramMode, start the program mode
    ESP_LOGI(__FILENAME__, "Starting Program Mode");

    // Init access point
    metahouse::accesspoint::init_accesspoint();

    /* Start the webserver */
    httpd_handle_t server = metahouse::accesspoint::webserver::start_webserver();
    if (server == NULL) {
      ESP_LOGE(__FILENAME__, "Failed to start the webserver");
      return;
    }
    ESP_LOGI(__FILENAME__, "Webserver started on port: %d", M_WEB_PORT);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return;
  }

  /* Initialize the Matter stack */
  esp_matter::node::config_t node_config;
  esp_matter::endpoint_t *node =
      esp_matter::node::create(&node_config, app_attribute_cb, app_identification_cb);

  esp_matter::endpoint::aggregator::config_t aggregator_config;
  esp_matter::endpoint_t *aggregator = esp_matter::endpoint::aggregator::create(
      node, &aggregator_config, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);

  /* Create the LightDevice */
  ButtonModule *buttonModule = new ButtonModule(GPIO_NUM_5);
  RelayModule *relayModule = new RelayModule(GPIO_NUM_2);
  LightAccessory *lightAccessory = new LightAccessory(relayModule, buttonModule);
  LightDevice *lightDevice = new LightDevice("LightDevice", lightAccessory, aggregator);

  esp_matter::start(app_event_cb);

  // /* Initialize the metric tracker */
  // metric_tracker_init("http://192.168.3.196:3001/metrics", "DevKitModule", 800);

  // /* Send the metrics to the server automatically every 30 seconds */
  // metric_tracker_auto_send(30, true, true);
}