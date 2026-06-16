# esp32cam-quick-shutter-and-deepsleep


This Arduino sketch is designed for an ESP32-CAM dev board. Its a simple click a still shot and deep sleep sketch. When a gpio button is pressed, it simply wakes up from a deep sleep, quickly takes a single photo, saves it to a MicroSD card with an incrementing file name, and immediately go back to sleep to conserve battery

It was made in haste because I needed a point and shoot the very next morning. I didnt have time to solder a pushbutton, so I just use the reset pin as a shutter button. The code is supposed to just take a still image, blink the builtin led thrice, update the image count and go back to sleep, resetting the board essentially acts like a wake up button and runs the program once before going back to sleep so its very battery efficient 

# Related Youtube Video
<p align="left">
  <a href="https://www.youtube.com/watch?v=CD7sIbKeu5s" target="_blank">
    <img src="https://img.youtube.com/vi/CD7sIbKeu5s/maxresdefault.jpg" alt="Watch the video" width="640" height="360" border="10" />
  </a>
</p>

# Circuit Diagram 

<img width="828" height="368" alt="Untitled design (2)" src="https://github.com/user-attachments/assets/3d67959a-f454-4ffd-8754-f4cf8c584c97" />

Components used- 

1. 3.7v LiPo Battery

2. 3.7v to 5v buck boost convertor

3. tp4056 charge module

4. esp32cam dev board
