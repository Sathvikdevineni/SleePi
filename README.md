<!-- PROJECT LOGO -->
<br />
<div align="center">
    <img src="https://user-images.githubusercontent.com/73529936/114861442-2f34e780-9de5-11eb-8a82-76c7db7b4bd9.png" alt="Paris" height="320">
  </a>
</div>
<br />

## Table of contents
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
         
## About the project         
SleePi is a real-time alert system and its function is to record the driver's drowsiness by using a camera mounted on raspberry pi. SleePi is a low-cost prototype which will observe the driver’s eyes, then alert them if they feel sleepy and also closing the eyes for long. As it is a real-time project which enhances safety-critical applications. Hence different methodologies are used on deduction times and exhibit the viability of ongoing observing dependent on out-of-test information to alert a sleepy driver. So we can say that SleePi step towards the pragmatic profound learning applications, possibly forestalling miniature dozes and reduces the accidents.

## Hardware
- Raspberry Pi
- 5MP Camera Module OV5647

## Software
- linux



## Methodology
The first and most important step is to obtain a video from the Raspberry Pi's camera. We resized the obtained video to 640x480 pixels and converted it to grayscale, making it suitable for ongoing operation. From there on, the facial parts like eyebrows, nose, mouth, eyes and facial structure were captured.The eyes were then confined to ascertain the PERCLOS, which depends on the Eye Aspect Ratio (EAR). The EAR was calculated during the calibration process, which was used to decide the drowsiness limit. In order to evaluate the driver's condition, the EAR for each of the driver's eyes, for each tip, will be determined.. Furthermore, the presence of the driver's head is used to decide if the driver is conscious or not. An alarm is activated when the careless identification and the languor exploration work together when either:

   1.	The EAR surpasses the sluggishness edge.
   2.	The driver isn't looking forward.
<p align="center">
   </br>
   <img src="https://user-images.githubusercontent.com/73529936/115017058-d8471500-9ead-11eb-8e0a-109c558ce478.PNG" alt="Paris" height="320">
    

## License
Distributed under the GPL-3.0 License.

## Social Media 

Email-id -  Sleep_Pi@outlook.com 

Facebook - https://www.facebook.com/RTPSleePI/

Instagram - https://www.instagram.com/sleep.pi_uofg/

Twitter - https://twitter.com/PiSleep

Youtube - https://www.youtube.com/channel/UCE3p9DPSaK3XjFBGdwhIPAQ/featured


## Contact
Team 27 in ENG5220: Real Time Embedded Programming

👤 **Sai Sathvik Devineni (2532243d)**

👤 **Tomas Simutis (2603015s)**

👤 **Muhammad Hassan Shahbaz (2619717s)**



