#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "camera_config.h"
#include "qr_code_reader.h"
#include "audio_manager.h"

static const char *TAG = "main";

extern "C" void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    
    ESP_LOGI(TAG, "Iniciando aplicación");

    if (!init_camera()) {
        ESP_LOGE(TAG, "Error iniciando la cámara!");
        return;
    }

    if (!init_audio()) {
        ESP_LOGE(TAG, "Error iniciando el audio!");
        return;
    }

    if (!init_sd_card()) {
        ESP_LOGE(TAG, "Error iniciando la tarjeta SD!");
        return;
    }

    xTaskCreate(qr_code_reader_task, "QRCodeReader_Task", 10000, NULL, 5, NULL);
}