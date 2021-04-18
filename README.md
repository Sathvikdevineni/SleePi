<!-- PROJECT LOGO -->
<br />
<div align="center">
    <img src="https://user-images.githubusercontent.com/73529936/114861442-2f34e780-9de5-11eb-8a82-76c7db7b4bd9.png" alt="Paris" height="320">
  </a>
</div>
<br />

# Table of contents
<ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
      <li><a href="#Hardware">Hardware</a</li>
      <li><a href="#Software">Software</a</li>
      </ul>
    <li><a href="#Methodology">Methodology</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
</ol>
         
# About the project         
SleePi is a real-time alert system and its function is to record the driver's drowsiness by using a camera mounted on raspberry pi. SleePi is a low-cost prototype which will observe the driver’s eyes, then alert them if they feel sleepy and also closing the eyes for long. As it is a real-time project which enhances safety-critical applications. Hence different methodologies are used on deduction times and exhibit the viability of ongoing observing dependent on out-of-test information to alert a sleepy driver. So we can say that SleePi step towards the pragmatic profound learning applications, possibly forestalling miniature dozes and reduces the accidents.

# Hardware
- Raspberry Pi
- 5MP Camera Module OV5647

# Software
- linux

# Getting Started

This project was developed for Raspberry Pi, however it is cross-platform and can be theoretically can be run from any Windows or Linux device as long as the video capture device can be opened by OpenCV libraries.

## Raspberry Pi (or other Linux devices)

If you are using the official camera module you need to enable it first (more detailed instructions [here](https://projects.raspberrypi.org/en/projects/getting-started-with-picamera/1)).
Enter into terminal:
```bash
sudo raspi-config
```
Go to _Interfacing Options_ select _Camera_ and follow the prompts to enable it. Your Raspberry Pi will reboot.

To select the audio device you need to select ....


### Pre-Requisites

- [CMake](https://cmake.org/) (Tested using 3.20.1)
- [OpenCV](https://opencv.org/) (Tested using 4.1.1 and 4.5.1)

For CMake you can just follow the recommended [instructions](https://cmake.org/install/):
Download the latest souce, unzip and enter:
```bash
./bootstrap
make
make install
```

For OpenCV you need to install dependencies
```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install cmake gfortran
sudo apt-get install libjpeg-dev libtiff-dev libgif-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install libgtk2.0-dev libcanberra-gtk*
sudo apt-get install libxvidcore-dev libx264-dev libgtk-3-dev
sudo apt-get install libtbb2 libtbb-dev libdc1394-22-dev libv4l-dev
sudo apt-get install libopenblas-dev libatlas-base-dev libblas-dev
sudo apt-get install libjasper-dev liblapack-dev libhdf5-dev
sudo apt-get install protobuf-compiler
```
Then download source (you can use more recent version if available) unzip and rename for conveniance
```bash
cd ~
wget -O https://github.com/opencv/opencv/archive/refs/tags/4.5.1.zip
unzip opencv.zip
mv opencv-4.5.0 opencv
```
Create and build build directory
```bash
cd ~/opencv/
mkdir build
cd build
```
Then compile. The V4L flags are needed to enable Video4Linux, which increases camera module support.
```bash
cmake -D CMAKE_BUILD_TYPE=RELEASE \
		-D CMAKE_INSTALL_PREFIX=/usr/local \
		-D WITH_V4L=ON \
		-D WITH_LIBV4L=ON
```
if you are using Raspberry Pi 2 or newer you can also add these flags to add optimisations:
```bash
-D ENABLE_NEON=ON \
-D ENABLE_VFPV3=ON \
```
Before installing, you need to increase the swap size. Otherwise the install process may be stuck because it does not have enough memory space
```bash
sudo nano /etc/dphys-swapfile
```
Go to _CONF_SWAPSIZE_ and change from 100 to 2048. Restart the swap service to make sure changes take effect.
```bash
sudo /etc/init.d/dphys-swapfile stop
sudo /etc/init.d/dphys-swapfile start
```
Run make, with the number after j being the number of cores you have (Pi 1 has 1 core and the newer versions have 4 cores)
```bash
make -j4
```
Be aware that this process takes a very long time. On the tested Rasberry Pi 1 it took around 24 hours.
On Raspberry Pi 4 it should take about 2 hours.
To complete, run
```bash
sudo make install
```
Now you should change the swap file size back to 100 and restarting the swap service using the same steps as before.

If this did not work or for more detailed information you can check out [this guide](https://qengineering.eu/install-opencv-4.5-on-raspberry-pi-4.html)
 
## Windows

You can also run the code on Windows using your webcam. It was tested using MinGW compiler and pre-compiled
OpenCV libraries.
You need to install [CMake](https://cmake.org/download/)
Then install the [MinGW-64 compiler](https://sourceforge.net/projects/mingw-w64/) (use x86_64 architecture and posix threads)
Add _"YOUR_INSTALL_DIRECTORY"\mingw64\bin_ to your system PATH.
Download pre-compiled OpenCV libraries for MinGW from [here](https://github.com/huihut/OpenCV-MinGW-Build) (Latest x64 version is 4.1.1 at the time of writing)
Extract the archive on your pc and add _"EXTRACTED_LOCATION"\x64\mingw\bin_ to system PATH.

If CMake still can't find your OpenCV installation, then you can change this line in CMakeLists.txt
```cpp
SET("OpenCV_DIR" "C:/OpenCV/")
```
to reflect your OpenCV location.

## Running the code
To run the code you can either build it yourself by running this from the build directory:
```bash
cmake ..
cmake --build .
```
The exacutable will then appear in the bin folder from where you can run it. (Note that you need
to run it from the bin directory, because the sound and prediction files are loaded using relative imports.

Or you can use the provided scripts *Build_and_run_Windows.bat* or *build_and_run_linux.sh* to do this automatically.



## Configuration

Before running the code, you can configure a lot of the parameters to adjust the code's behaviour to your needs.
These parameters are stored in the config.h, which is in the include folder.
The description of what each parameter does is put in the file itselffolder.

For example if you want to save a video output at full size and show only Face detection and EAR, you could define:
```cpp
const float SCALE_FACTOR = 1.0;
const bool SHOW_VIDEO_OUTPUT = false;
const bool SHOW_FACE_DETECTION = true;
const bool SHOW_FACIAL_LANDMARKS = false;
const bool SHOW_EYE_CONTOURS = false;
const bool SHOW_EAR = true;
const bool SAVE_TO_FILE = true;
```
Or if you want to have use 60 frames for calibration, set the threshold 0.12 below the average value and sound the alarm if the instantaneous EAR (not using EMA) is below threshold for more than 3 seconds, you could define:
```cpp
const float TIME_THRESHOLD = 3.0;
const float ALPHA = 1.0;
const float THRESHOLD_SENSITIVITY = 0.12;
const int FRAMES_FOR_CALIBRATION = 60;
```

Note that after changing parameters you need to recompile the code.
## Methodology
The first and most important step is to obtain a video from the Raspberry Pi's camera. We resized the obtained video to 640x480 pixels and converted it to grayscale, making it suitable for ongoing operation. From there on, the facial parts like eyebrows, nose, mouth, eyes and facial structure were captured.The eyes were then confined to ascertain the PERCLOS, which depends on the Eye Aspect Ratio (EAR). The EAR was calculated during the calibration process, which was used to decide the drowsiness limit. In order to evaluate the driver's condition, the EAR for each of the driver's eyes, for each tip, will be determined.. Furthermore, the presence of the driver's head is used to decide if the driver is conscious or not. An alarm is activated when the careless identification and the languor exploration work together when either:

   1.	The EAR surpasses the sluggishness edge.
   2.	The driver isn't looking forward.
<p align="center">
   </br>
   <img src="https://user-images.githubusercontent.com/73529936/115017058-d8471500-9ead-11eb-8e0a-109c558ce478.PNG" alt="Paris" height="320">
    

# License
Distributed under the GPL-3.0 License.

# Social Media 

Email -  Sleep_Pi@outlook.com 

Facebook - https://www.facebook.com/RTPSleepPI

Instagram - https://www.instagram.com/sleep.pi_uofg/

Twitter - https://twitter.com/PiSleep


# Contact
Team 27 in ENG5220: Real Time Embedded Programming

👤 **Sai Sathvik Devineni (2532243d)**

👤 **Tomas Simutis (2603015s)**

👤 **Muhammad Hassan Shahbaz (2619717s)**
