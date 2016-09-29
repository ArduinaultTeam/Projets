#include <SPI.h>
#include <Wire.h>

#include "Adafruit_STMPE610.h"

#define STMPE_CS 8
Adafruit_STMPE610 touch = Adafruit_STMPE610(STMPE_CS);

void setup() { 
  Serial.begin(9600);
  Serial.println("Adafruit STMPE610 example");
  Serial.flush();
  
  if (! touch.begin()) {
    Serial.println("STMPE not found!");
    while(1);
  }
  Serial.println("Waiting for touch sense");
}

void loop() {
  uint16_t x, y;
  uint8_t z;

  if (touch.touched()) {
    
    while (! touch.bufferEmpty()) {
      Serial.print(touch.bufferSize());
      touch.readData(&x, &y, &z);
      Serial.print("->("); 
      Serial.print(x); Serial.print(", "); 
      Serial.print(y); Serial.print(", "); 
      Serial.print(z);
      Serial.println(")");
    }
    touch.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
  }
  delay(10);
}

