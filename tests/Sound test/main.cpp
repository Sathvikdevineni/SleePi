#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "play_audio.cpp"
using namespace std;

int main(int argc, char **argv)
{
    // Load audio files and initialise playback
    init_playback();
    // Detaching because the capture can start as the instruction is being played
    std::thread thread1(play_calibration_start);
    thread1.detach();
    // simulate calibration
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // Play the sound of calibration completion
    std::thread thread2(play_calibartion_completed);
    thread2.detach();
    // Initialise the alarm file for playback
    std::thread thread3(init_alarm);
    thread3.detach();
    int a;
    // Enter 1 to start alarm and 2 to stop
    while (1)
    {
        cin >> a;
        if (a == 1)
        {
            std::thread thread_start(start_alarm);
            thread_start.detach();
        }
        else if (a == 2)
        {
            stop_playing();
        }
    }

    return 0;
}