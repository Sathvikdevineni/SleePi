# SleePi
Real Time Embedded Project
## ABOUT
SleePi is a real-time alert system and its function is to reocord the driver's drowsiness detection by using a camera mounted on raspberry pi. SleePi is a low-cost prototype which will observe the driver’s eyes, then alert them if they feel sleepy and also closing the eyes for long. As it is a real-time project which enhances safety-critical applications. Hence different methodologies are used on deduction times and exhibit the viability of ongoing observing dependent on out-of-test information to alert a sleepy driver. So we can say that SleePi step towards the pragmatic profound learning applications, possibly forestalling miniature dozes and reduces the accidents.

## HARDWARE
- Raspberry Pi
- 5MP Camera Module OV5647

## SOFTWARE




## METHODOLOGY
The first and foremost step is to acquires a video from the camera connected with the Raspberry Pi. We resized the obtained video from its current size to 640x480, and we convert it to grayscale, appropriate for ongoing handling. From there on, the facial tourist spots like eyebrows, nose, mouth, eyes and facial structure were gotten. The eyes were then confined to ascertain the PERCLOS, which depends on the Eye Aspect Ratio (EAR). The EAR was obtained once during the calibration time frame, at the framework set-up time, which was utilized as the laziness limit in the system. Afterward, during the excursion, the EAR for both the driver's eyes will be determined, for each edge, to decide the driver's condition. Additionally, the head present is obtained to decide whether the driver is mindful or oblivious. The languor discovery and the careless identification works in equal and an alert will be set off when either:
1. The EAR surpasses the sluggishness edge, or
2. The driver head present isn't looking forward.
<p align="center">
   </br>
   <a href="https://drive.google.com/file/d/1GYySi0OvmA4BjbVHNCkL5UcyptOgmjX4/view?usp=sharing"><img  src="Downloads/fb_logo.png"  width="44" height="44">

## Author
>Team 27 in ENG5220: Real Time Embedded Programming

👤 **Sai Sathvik Devineni (2532243d)**

👤 **Tomas Simutis (2603015s)**

👤 **Muhammad Hassan Shahbaz (2619717s)**



