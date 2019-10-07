/*
https://github.com/MKme/Wemos-D1-Subscriber-Counter-MAX7219-LED-Matrix/blob/master/D1MiniSubscriberCounterMAX2719/D1MiniSubscriberCounterMAX2719.ino

Brian Lough's library contains rotate: https://github.com/witnessmenow/MAX7219LedMatrix
Brians doesnt work with Wemos D1 mini unless you comment out this line https://github.com/witnessmenow/MAX7219LedMatrix/blob/master/LedMatrix.cpp#L34
Done in the cpp file contained with this sketch
Eric originally followed this tutorial here for the YouTube API info etc: http://www.joeybabcock.me/blog/projects/arduino-esp8266-live-subscriber-display/
Wiring for Wemos D1 Mini:
| Pin On 7219| Pin on Wemos D1 Mini |
| ------- |----------------|
| GND     | G |
| VCC     | 3V3 |
| DIN     | D7 |
| CS      | D4 |
| CLK     | D5 |


*/


#include <Arduino.h>

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include "LedMatrix.h"

#define NUMBER_OF_DEVICES 1
#define CS_PIN 2

const char *ssid     = "<SSID>";
const char *password = "<PASSWORD>";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

const boolean withWifi = false;

void ntpInit(){
  WiFi.begin(ssid, password);
  WiFi.hostname("Electronica_watch");

    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
    }

    timeClient.begin();
}

void ntpLoop(){
 
      timeClient.update();
      Serial.println(timeClient.getFormattedTime());
  
}

void matrixInit(){
    ledMatrix.init();
    ledMatrix.setRotation(true);
    ledMatrix.setIntensity(1); // range is 0-15
    ledMatrix.setText("MAX7219 Demo");
}

void matrixLoop(){
    ledMatrix.clear();
    ledMatrix.scrollTextLeft();
    ledMatrix.drawText();
    ledMatrix.commit(); // commit transfers the byte buffer to the displays
    delay(200); 
}


void setup(){
  Serial.begin(115200);

  if (withWifi){
    ntpInit();
  }

  matrixInit();
}

void loop() {
  if (withWifi){
    ntpLoop();
  } 
  
  matrixLoop();

  delay(1000);
}


