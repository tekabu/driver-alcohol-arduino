#include <nvs.h>
#include <nvs_flash.h>

#define NAMESPACE "storage"

void saveContacts() {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  const char* mobile_numbers[5] = {
    mobile1.c_str(),
    mobile2.c_str(),
    mobile3.c_str(),
    mobile4.c_str(),
    mobile5.c_str()
  };

  nvs_handle_t nvs_handle;

  err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
  if (err == ESP_OK) {
    for (int i = 0; i < 5; i++) {
      String key = "mobile_number_" + String(i + 1);
      err = nvs_set_str(nvs_handle, key.c_str(), mobile_numbers[i]);

      Serial.print(F("Saving mobile"));
      Serial.print(i + 1);
      Serial.print(F(": "));
      Serial.print(mobile_numbers[i]);

      if (err == ESP_OK) {
        Serial.println(F(", OK"));
      } else {
        Serial.println(F(", Failed"));
      }
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
      Serial.println(F("Error committing data"));
    }

    nvs_close(nvs_handle);
  } else {
    Serial.println(F("Error opening NVS"));
  }
}

void loadContacts() {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  String mobile_numbers[] = {"", "", "", "", ""};
  char retrieved_number[20];
  nvs_handle_t nvs_read_handle;

  err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_read_handle);
  if (err == ESP_OK) {
    for (int i = 0; i < 5; i++) {
      String key = "mobile_number_" + String(i + 1);
      size_t required_size = sizeof(retrieved_number);
      err = nvs_get_str(nvs_read_handle, key.c_str(), retrieved_number, &required_size);
      
      Serial.print(F("Retrieve mobile"));
      Serial.print(i + 1);
      Serial.print(F(": "));
      
      if (err == ESP_OK) {
        mobile_numbers[i] = retrieved_number;  
        Serial.print(mobile_numbers[i]);
        Serial.println(F(", OK"));
      } else {
        mobile_numbers[i] = "";
        Serial.println(F(", Failed"));
      }
    }

    nvs_close(nvs_read_handle);
  } else {
    Serial.println("Error opening NVS for reading");
  }

  mobile1 = mobile_numbers[0];
  mobile2 = mobile_numbers[1];
  mobile3 = mobile_numbers[2];
  mobile4 = mobile_numbers[3];
  mobile5 = mobile_numbers[4];
}
