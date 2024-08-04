#include "qr_code_reader.h"
#include "esp_camera.h"
#include "quirc.h"
#include "esp_log.h"
#include "audio_manager.h"

static const char *TAG = "qr_reader";

void qr_code_reader_task(void *pvParameters)
{
    struct quirc *q = NULL;
    uint8_t *image = NULL;
    camera_fb_t *fb = NULL;
    struct quirc_code code;
    struct quirc_data data;
    quirc_decode_error_t err;

    while (1) {
        q = quirc_new();
        if (q == NULL) {
            ESP_LOGE(TAG, "Can't create quirc object");
            continue;
        }

        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            quirc_destroy(q);
            continue;
        }

        quirc_resize(q, fb->width, fb->height);
        image = quirc_begin(q, NULL, NULL);
        memcpy(image, fb->buf, fb->len);
        quirc_end(q);

        int count = quirc_count(q);
        if (count > 0) {
            quirc_extract(q, 0, &code);
            err = quirc_decode(&code, &data);

            if (err) {
                ESP_LOGE(TAG, "Decoding FAILED");
            } else {
                ESP_LOGI(TAG, "Decoding successful:");
                ESP_LOGI(TAG, "Payload: %s", data.payload);

                if (strcmp((const char *)data.payload, "encender") == 0) {
                    play_audio("/sound2.wav");
                } else if (strcmp((const char *)data.payload, "apagar") == 0) {
                    play_audio("/sound1.wav");
                } else if (strcmp((const char *)data.payload, "api") == 0) {
                    play_audio("/arduino_rec.wav");
                }
            }
        } else {
            ESP_LOGI(TAG, "No QR code detected.");
        }

        esp_camera_fb_return(fb);
        quirc_destroy(q);

        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100ms
    }
}