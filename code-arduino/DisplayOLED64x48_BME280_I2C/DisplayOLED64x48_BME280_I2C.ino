/* 
 * Chuong trinh hien thi thong so nhiet do, do am va ap suat cua cam bien BME280 len OLED 64x48 (I2C address 0x3C) cung giao tiep theo chuan I2C
 * Thu vien can cai dat:
 * 1. https://github.com/mcauser/Adafruit_SSD1306/tree/esp8266-64x48
 * 2. https://github.com/adafruit/Adafruit-GFX-Library
 * 3. http://static.cactus.io/downloads/library/bme280/cactus_io_BME280_I2C.zip
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "cactus_io_BME280_I2C.h"

#define OLED_RESET 0

#define SSD1306_LCDHEIGHT 48
#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(OLED_RESET);

//BME280_I2C bme;              // I2C using default 0x77 
BME280_I2C bme(0x76);         // I2C using address 0x76

// Cac chuong trinh con duoc dinh nghia
void DisplayBME280();       // Do gia tri va hien thi len OLED
void oled64x48(float T, float H, float P);    // Hien thi so lieu len OLED

void setup(){
  Serial.begin(9600);
  // Khoi tao voi dia chi I2C 0x3C (loai OLED 64x48) 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay(); // Xoa bo nho  
  if (!bme.begin()){
    while (1){
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,20);
    display.println("Not find!");   
    display.display();
    }
  }
  bme.setTempCal(-1);   //Calib: -1
  display.clearDisplay(); // Xoa bo nho
}


void loop(){
  DisplayBME280();        // Hien thi gia tri tu cam bien len OLED
}

void DisplayBME280(){   // Hien thi gia tri tu cam bien len OLED
  float T, H, P;
  
  bme.readSensor();  
  T = bme.getTemperature_C();     //Nhiet do (C)
  H = bme.getHumidity();          // Do am   (%)
  P = bme.getPressure_MB();       // Ap suat (milibar-mB)
  
  oled64x48(T, H, P);             // Hien thi gia tri do duoc len OLED 64x48
  
  delay(2000);
  display.clearDisplay();         // Xoa bo nho
}

void oled64x48(float T, float H, float P){    // Hien thi gia tri do duoc len OLED 64x48
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("T:");  display.print(T);  display.println("*C");

  display.setCursor(0,10);
  display.print("H:");  display.print(H);  display.println("%");

  display.setCursor(0,20);
  display.print("P:");  display.println(P);
  display.setCursor(40,30); display.println("mB");
  
  display.display();
}
