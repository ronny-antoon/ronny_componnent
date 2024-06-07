#include "metric_tracker.hpp"

#include <nvs_flash.h>
#include <mh_matter.hpp>

extern "C" void app_main()
{
    /* Initialize the ESP NVS layer */
    nvs_flash_init();

    metric_tracker_init("http://192.168.3.196:3001/metrics", "DevKitModule", 800);

    /* Send the metrics to the server automatically every 30 seconds */
    metric_tracker_auto_send(30, true, true);
}