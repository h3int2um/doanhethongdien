/* 
 * Chuong trinh hien thi thong so nhiet do, do am va ap suat cua cam bien BME280 len:
 *  + OLED 64x48 (I2C address 0x3C) cung giao tiep theo chuan I2C.
 *  + App Blynk - http://www.blynk.cc/
 *  
 * Thu vien can cai dat:
 * 1. https://github.com/mcauser/Adafruit_SSD1306/tree/esp8266-64x48
 * 2. https://github.com/adafruit/Adafruit-GFX-Library
 * 3. http://static.cactus.io/downloads/library/bme280/cactus_io_BME280_I2C.zip
 * 4. https://github.com/jfturcot/SimpleTimer
 */

#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "cactus_io_BME280_I2C.h"
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

char ssid[] = "Tam7";       //  your network SSID (name) 
char pass[] = "21019400";   //  your network password

// You should get Auth Token in the Blynk App.
char auth[] = "2d74c3d3c9c241a49ee23022dbbeda75";

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

#define SSD1306_LCDHEIGHT 48
#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//BME280_I2C bme;              // I2C using default 0x77 
BME280_I2C bme(0x76);         // I2C using address 0x76

SimpleTimer timer;

// Cac chuong trinh con duoc dinh nghia

// Do gia tri va hien thi len OLED va Blynk
void DisplayBME280();

// Hien thi so lieu len OLED
void oled64x48(float T, float H, float P);

// Khoi tao Blynk
void init_Blynk();

// Gui du lieu len Blynk
void send_data_Blynk(float T, float H, float P);

void setup(){  
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

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, DisplayBME280);
}

void loop(){
  DisplayBME280();        // Hien thi gia tri tu cam bien len OLED
  init_Blynk();                 //Gui gui lieu len Blynk
}

void DisplayBME280(){   // Hien thi gia tri tu cam bien len OLED
  float T, H, P;
  
  bme.readSensor();  
  T = bme.getTemperature_C();     //Nhiet do (C)
  H = bme.getHumidity();          // Do am   (%)
  P = bme.getPressure_MB();       // Ap suat (milibar-mB)

  oled64x48(T, H, P);            // Hien thi gia tri do duoc len OLED

  Blynk.virtualWrite(V0, T);  // Virtual 0 - Temperature
  Blynk.virtualWrite(V1, H);  // Virtual 1 - Humidity
  Blynk.virtualWrite(V2, P);  // Virtual 2 - Pressure
    
  delay(2000);                 // updates every 60 seconds.
  display.clearDisplay();       // Xoa bo nho
}

// Hien thi gia tri do duoc len OLED 64x48
void oled64x48(float T, float H, float P){
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

void init_Blynk(){
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
