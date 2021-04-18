
// Indicates whether the alarm sample has been initialised
extern bool alarmReady;
// Indicates whether the alarm is currently on
extern bool alarmON;

int init_alarm();
int start_alarm();
int play_calibration_start();
int play_calibartion_completed();
int stop_playing();
int init_playback();