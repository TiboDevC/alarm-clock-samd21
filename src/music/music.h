#ifndef ALARM_CLOCK_MUSIC_H
#define ALARM_CLOCK_MUSIC_H

#define PIN_SWITCH_SPEAKER 21

int music_init(uint8_t volume_percent);
void music_deinit();
void music_play();
void music_stop();

#endif // ALARM_CLOCK_MUSIC_H
