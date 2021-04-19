
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
```shell
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

Using the OpenCV library the next frame from the camera is grabbed. For faster execution, some preprocessing is needed - the image is resized according to the *SCALE_FACTOR* parameter. It should be noted that OpenCV opens the camera stream as 640x480 by default and it it scaled down even more. The image is then converted to grayscale for dimensionality reduction and the histogram is equalised to normalise brightness and increase contrast of the image.
For display, the coloured frame is used, but all the calculations are performed on the grayscale image.

<p align="center">
<img src="https://user-images.githubusercontent.com/47836357/115252067-cecede80-a133-11eb-9c8f-5d54ea82f2a2.png" width="49%"></img> <img src="https://user-images.githubusercontent.com/47836357/115252074-cf677500-a133-11eb-8cff-c987f0d15cee.png" width="49%"></img> 
</p>

The face is detected using a [pre-trained Haar Cascade Classifier](https://github.com/opencv/opencv/blob/master/data/haarcascades/haarcascade_frontalface_alt.xml). The classifier is loaded from an *.xml* file and [detectMultiScale](https://docs.opencv.org/3.4/d1/de5/classcv_1_1CascadeClassifier.html#aaf8181cb63968136476ec4204ffca498) method is used on the frame and returns a boundary rectangle for the detected faces. Sometimes it can misclassify other objects as faces or detect other people's faces. In that case, only the the largest face is used. If no face is found, the system shows a visual indicator and just loops until a face is found.
<p align="center">
<img src="https://user-images.githubusercontent.com/47836357/115254862-70572f80-a136-11eb-8406-e6ae9fe84ab1.png" width="53%"></img> 
 <img src="https://user-images.githubusercontent.com/47836357/115257157-7d751e00-a138-11eb-9a94-bb6668111080.png" width="40%"></img> 
</p>

Then  dlib's shape predictor is used with a pre-trained model, which predicts the points of 68 facial landmarks.
<p align="center">
<img src="https://ibug.doc.ic.ac.uk/media/uploads/images/annotpics/figure_68_markup.jpg" width="39%" ></img >
<img src="https://user-images.githubusercontent.com/47836357/115262925-a51ab500-a13d-11eb-9f94-8d25aeb66359.png" width="56%"></img> 
</p>
Out of these points, only the points of the eyes are relevant, since they are used to determine the Eye Aspect Ratio (EAR). It is caluclated using the following formula:
<p align="center">
<img src="https://user-images.githubusercontent.com/47836357/115263908-7ea94980-a13e-11eb-9662-51dd8483cbfc.jpg" width="50%"></img> 
<br>
<img src="https://lh6.googleusercontent.com/qoeyiEcyQI4jfi3Bu2WTXX0rWsPYixvJjmqSjQ6ChvPpi2tCLBNXQCLedJNhaq4B-_U9vyk70e5vpOChxlPZNCUGAfv9A30pXsGXgarmUAmryM-M91hUS0Bgy2Yle1J7SX2NYSln" width="50%"></img> 
</p>
It can be deduced that the EAR is lower when the eyes are closed, however it is unclear by how much.

When starting the program, an audio instruction is played that says to look into the camera for a few seconds without blinking. The average EAR for N frames (specified in config as *FRAMES_FOR_CALIBRATION*) is taken and then the *THRESHOLD_SENSITIVITY* is subtracted from that to determine the EAR threshold. 

If the instantaneous EAR value is compared against the threshold, then the system is slightly unreliable as head movements or changes in lighting can bring the EAR slighly above or below the threshold for a few frames. This can result in the system detecting the person as awake when the alarm is playing and stopping it, even when the eyes remain closed. To make the system more robust, Exponential Moving Average is used to mitigate this issue.
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=EMA = \alpha \times EAR %2B (1-\alpha) \times EMA">
</p>
α should be between 0 and 1. As the α value becomes higher, more emphasis is placed on more recent samples. With α =  1.0, the system does not take past values into consideration and only acts on the most recent EAR value. With α= 1/N, the EMA approximates the Simple Moving Average for N samples. This method is more simple than storing past values to calculate SMA,  and puts more emphasis on recent values, which is prefect for this problem.

When the EMA is lower than the threshold for N seconds (specified by *TIME_THRESHOLD*), the system identifies this as sleepiness and shows this information on screen and sends a signal to play the alarm sound.
<p align="center">
<img src="https://user-images.githubusercontent.com/47836357/115269200-bf579180-a143-11eb-8a80-a7e4923ff11c.png" width="90%"></img> 
</p>
The alarm is stopped when the EMA goes above the set threshold. The alarm sample is only a single "beep" sound that is being looped to avoid loading a large file into memory. The calls to start or stop playing an audio sample are executed on different threads to prevent blocking.

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
