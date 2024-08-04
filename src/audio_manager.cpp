#include "audio_manager.h"
#include <Audio.h>
#include <SD.h>
#include <SPI.h>
#include "esp_log.h"

static const char *TAG = "audio";

#define SD_CS 21
#define MAX98357A_I2S_DOUT 4
#define MAX98357A_I2S_BCLK 5
#define MAX98357A_I2S_LRC  6

Audio audio;

bool init_audio(void)
{
    audio.setPinout(MAX98357A_I2S_BCLK, MAX98357A_I2S_LRC, MAX98357A_I2S_DOUT);
    audio.setVolume(100);
    return true;
}

bool init_sd_card(void)
{
    if (!SD.begin(SD_CS)) {
        ESP_LOGE(TAG, "Error inicializando la tarjeta SD!");
        return false;
    }
    ESP_LOGI(TAG, "Tarjeta SD inicializada correctamente.");
    return true;
}

void play_audio(const char* filename)
{
    if (audio.connecttoFS(SD, filename)) {
        ESP_LOGI(TAG, "Reproduciendo Audio: %s", filename);
        while (audio.isRunning()) {
            audio.loop();
        }
    } else {
        ESP_LOGE(TAG, "Error al reproducir Audio: %s", filename);
    }
}