#pragma once

#include <esp_err.h>
#include <esp_log.h>
#include <nvs.h>

#include "myConfig.hpp"

bool checkProgramMode() {
  // check if the device is in ProgramMode
  nvs_handle_t nvs_handle;

  // open the nvs
  esp_err_t err = nvs_open_from_partition(M_NVS_PARTITION, M_NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
  if (err == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGI(__FILENAME__, "NVS not found, defaulting to program mode");
    return true;
  } else {
    if (err != ESP_OK) {
      ESP_LOGE(__FILENAME__, "Failed to open NVS");
      return true;
    }
  }

  uint8_t val;
  // check if the key exists
  if (nvs_get_u8(nvs_handle, M_NVS_KEY_PROGRAM_MODE, &val) != ESP_OK) {
    ESP_LOGI(__FILENAME__, "Program Mode is not set, defaulting to program mode");
    nvs_close(nvs_handle);
    return true;
  }

  // check if the value is 1
  if (val == 1) {
    ESP_LOGI(__FILENAME__, "Program Mode is set to true");
    nvs_close(nvs_handle);
    return true;
  }

  nvs_close(nvs_handle);
  return false;
}

esp_err_t setProgramMode(bool mode) {
  // set the program mode
  nvs_handle_t nvs_handle;

  // open the nvs
  if (nvs_open_from_partition(M_NVS_PARTITION, M_NVS_NAMESPACE, NVS_READWRITE, &nvs_handle) != ESP_OK) {
    ESP_LOGE(__FILENAME__, "Failed to open NVS");
    return ESP_FAIL;
  }

  // set the value
  if (nvs_set_u8(nvs_handle, M_NVS_KEY_PROGRAM_MODE, mode) != ESP_OK) {
    ESP_LOGE(__FILENAME__, "Failed to set program mode");
    nvs_close(nvs_handle);
    return ESP_FAIL;
  }

  // commit the changes
  if (nvs_commit(nvs_handle) != ESP_OK) {
    ESP_LOGE(__FILENAME__, "Failed to commit changes");
    nvs_close(nvs_handle);
    return ESP_FAIL;
  }

  nvs_close(nvs_handle);
  return ESP_OK;
}
