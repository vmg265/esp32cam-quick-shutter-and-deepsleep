#include "esp_camera.h"
#include "FS.h"                
#include "SD_MMC.h"            
#include "soc/soc.h"           
#include "soc/rtc_cntl_reg.h"  

#define LED_GPIO_NUM      33  
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

camera_config_t config;

void blinkStatus(int times) {
  pinMode(LED_GPIO_NUM, OUTPUT);
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_GPIO_NUM, LOW);  
    delay(150);
    digitalWrite(LED_GPIO_NUM, HIGH); 
    delay(150);
  }
}

int getNextPhotoNumber() {
  int count = 1;
  fs::FS &fs = SD_MMC;
  
  // Look for count.txt inside the esp32cam folder
  if (fs.exists("/esp32cam/count.txt")) {
    File file = fs.open("/esp32cam/count.txt", FILE_READ);
    if (file) {
      String content = file.readString();
      count = content.toInt();
      file.close();
    }
  }
  
  // Save next number back to the same folder
  File file = fs.open("/esp32cam/count.txt", FILE_WRITE);
  if (file) {
    file.print(count + 1);
    file.close();
  }
  return count;
}

void configInitCamera(){
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;

  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  esp_camera_init(&config);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  configInitCamera();
  
  if(!SD_MMC.begin()){
    blinkStatus(10); 
    esp_deep_sleep_start();
  }

  int currentNumber = getNextPhotoNumber();

  camera_fb_t * fb = esp_camera_fb_get(); 
  esp_camera_fb_return(fb); 
  fb = esp_camera_fb_get(); 

  if(fb) {
    // Save image to the /esp32cam/ folder
    char path[30];
    sprintf(path, "/esp32cam/%04d.jpg", currentNumber);

    File file = SD_MMC.open(path, FILE_WRITE);
    if(file){
      file.write(fb->buf, fb->len);
      file.close();
    }
    esp_camera_fb_return(fb); 
  }

  blinkStatus(3);
  esp_deep_sleep_start();
}

void loop() {}
