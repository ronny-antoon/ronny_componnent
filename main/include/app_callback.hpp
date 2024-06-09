#ifndef APP_CALLBACK_HPP
#define APP_CALLBACK_HPP

#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_matter.h>

#include <BaseDevice.hpp>

void app_event_cb(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg) {
  switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
      ESP_LOGW(__FILENAME__, "Interface IP Address changed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
      ESP_LOGW(__FILENAME__, "Commissioning complete");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
      ESP_LOGW(__FILENAME__, "Commissioning failed, fail safe timer expired");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
      ESP_LOGW(__FILENAME__, "Commissioning session started");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
      ESP_LOGW(__FILENAME__, "Commissioning session stopped");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
      ESP_LOGW(__FILENAME__, "Commissioning window opened");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
      ESP_LOGW(__FILENAME__, "Commissioning window closed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFabricRemoved: {
      ESP_LOGW(__FILENAME__, "Fabric removed successfully");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
        chip::CommissioningWindowManager &commissionMgr =
            chip::Server::GetInstance().GetCommissioningWindowManager();
        constexpr auto kTimeoutSeconds = chip::System::Clock::Seconds16(300);
        if (!commissionMgr.IsCommissioningWindowOpen()) {
          CHIP_ERROR err = commissionMgr.OpenBasicCommissioningWindow(
              kTimeoutSeconds, chip::CommissioningWindowAdvertisement::kDnssdOnly);
          if (err != CHIP_NO_ERROR) {
            ESP_LOGE(__FILENAME__, "Failed to open commissioning window: %s", chip::ErrorStr(err));
          }
        }
      }
      break;
    }

    case chip::DeviceLayer::DeviceEventType::kFabricWillBeRemoved:
      ESP_LOGW(__FILENAME__, "Fabric will be removed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFabricUpdated:
      ESP_LOGW(__FILENAME__, "Fabric is updated");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFabricCommitted:
      ESP_LOGW(__FILENAME__, "Fabric is committed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;
    default:
      break;
  }
}

esp_err_t app_identification_cb(esp_matter::identification::callback_type type, uint16_t endpoint_id,
                                uint8_t effect_id, uint8_t effect_variant, void *priv_data) {
  if (type == esp_matter::identification::callback_type_t::START) {
    if (priv_data != nullptr) {
      BaseDevice *device = static_cast<BaseDevice *>(priv_data);
      if (device != nullptr) {
        device->identify();
      }
    }
  }
  return ESP_OK;
}

esp_err_t app_attribute_cb(esp_matter::attribute::callback_type type, uint16_t endpoint_id,
                           uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val,
                           void *priv_data) {
  if (type == esp_matter::attribute::callback_type_t::POST_UPDATE) {
    if (priv_data != nullptr) {
      BaseDevice *device = static_cast<BaseDevice *>(priv_data);
      if (device != nullptr) {
        device->updateAccessory();
      }
    }
    return ESP_OK;
  }

  return ESP_OK;
}

#endif  // APP_CALLBACK_HPP