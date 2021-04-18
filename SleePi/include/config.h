// These parameters can be adjusted by user before compiling. They are unchangable constants and are not modified at compile time.

#ifndef CONFIG_H
#define CONFIG_H

// Pre-processing and performance

/*  Defines how much the image should be resized
For example, setting it to 0.5 would mean that the orginal video resolution is halved.
Processing at full resolution (1.0) is slow, so to increase processing speed per frame tou should reduce it. */
const float SCALE_FACTOR = 0.5;
// If true then prints to console how many frames it processes per second
// Useful for optimising parameters such as scaling and thresholds for different hardware
const bool PRINT_FPS = true;

// Thresholding parameters

// How many frames should be used to get the average EAR during calibration
const int FRAMES_FOR_CALIBRATION = 50;
// When setting the threshold, frames below this EAR value are ignored (eyes are likely to be closed)
const float MIN_EAR_THRESH = 0.22;
// How much lower than the average EAR during calibration should the threshold be to detect sleepiness?
// Formula: average_EAR (in FRAMES_FOR_CALIBRATION frames) - THRESHOLD_SENSITIVITY
const float THRESHOLD_SENSITIVITY = 0.05;
/* This is the alpha value for calculating the Exponential Moving Average (EMA) of the EAR values.
The formula is EMA = ALPHA * avg_EAR + (1.0 - ALPHA) * EMA;
Alpha should be between 0 and 1. As the alpha value becomes higher, more emphasis is placed on more recent samples.
With alpha 1.0, the system does not take past values into consideration and only acts on the most recent EAR value.
With alpha = 1/N, the EMA approximates the Simple moving average for N samples.
*/
const float ALPHA = 1 / 15.0;
// For how long should the EMA be below the threshold to identify the person as sleepy
const float TIME_THRESHOLD = 1.0;

// Video output options

// Displays video output in a window
const bool SHOW_VIDEO_OUTPUT = true;
// Saves the output video to a file
const bool SAVE_TO_FILE = true;
// If both SHOW_VIDEO_OUTPUT and SAVE_TO_FILE are false, the following parameters have no effect.
// Shows a rectangle around the detected face
const bool SHOW_FACE_DETECTION = true;
// Shows all 68 facial landmarks on the face
const bool SHOW_FACIAL_LANDMARKS = false;
// Draws a contour around the eyes
const bool SHOW_EYE_CONTOURS = true;
// Displays the current EAR value
const bool SHOW_EAR = true;

// Audio configuration

// Location of audio samples
static constexpr const char *ALARM_LOC = "../static/alarm.wav";
static constexpr const char *CALIBRATION_START_LOC = "../static/calibration_start.wav";
static constexpr const char *CALIBRATION_END_LOC = "../static/calibration_complete.wav";

// Audio decoder settings
#define SAMPLE_FORMAT ma_format_f32
#define CHANNEL_COUNT 2
#define SAMPLE_RATE 48000

#endif