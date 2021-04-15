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
The first step is to record a video from the camera which is connected to the Raspberry Pi. We resized the obtained video from its current size to 640x480, and converted to grayscale, which made it appropriate for ongoing handling. From there on, the facial parts like eyebrows, nose, mouth, eyes and facial structure were captured.The eyes were then confined to ascertain the PERCLOS, which depends on the Eye Aspect Ratio (EAR). The EAR was calculated during the calibration time frame, which was measured to know the drowsiness limit. During the EAR for both the driver's eyes will be determined, for each edge, to decide the driver's condition. Additionally, the head present is obtained to decide whether the driver is mindful or oblivious. The languor discovery and the careless identification works in equal and an alert will be set off when either:
1. The EAR surpasses the sluggishness edge, or
2. The driver head present isn't looking forward.
<p align="center">
   </br>
   <a href="https://drive.google.com/file/d/1GYySi0OvmA4BjbVHNCkL5UcyptOgmjX4/view?usp=sharing"><img  src="Downloads/fb_logo.png"  width="44" height="44">

## License
Distributed under the GPL-3.0 License.

## Social Media 
<ul>
<li>Email -  Sleep_Pi@outlook.com </li>
<li>Facebook - https://www.facebook.com/RTPSleepPI </li>
<li>Instagram - https://www.instagram.com/sleep.pi_uofg/</li>
<li>Twitter - https://twitter.com/PiSleep </li>
</ul>

## Contact
Team 27 in ENG5220: Real Time Embedded Programming

👤 **Sai Sathvik Devineni (2532243d)**

👤 **Tomas Simutis (2603015s)**

👤 **Muhammad Hassan Shahbaz (2619717s)**



