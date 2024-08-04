#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool init_audio(void);
bool init_sd_card(void);
void play_audio(const char* filename);

#ifdef __cplusplus
}
#endif