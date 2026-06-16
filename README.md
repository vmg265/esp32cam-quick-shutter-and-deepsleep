# esp32cam-quick-shutter-and-deepsleep

This Arduino sketch is designed for an ESP32-CAM dev board. Its a simple click a still shot and deep sleep sketch. When a gpio button is pressed, it simply wakes up from a deep sleep, quickly takes a single photo, saves it to a MicroSD card with an incrementing file name, and immediately go back to sleep to conserve battery
