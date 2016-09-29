#include <TM1637Display.h>

const int CLK = 2;
const int DIO = 3;

TM1637Display display(CLK,DIO);

uint8_t tabSeg[] = { 0xff, 0xff, 0xff, 0xff };

void setup() {
}

void loop() {
  display.setBrightness(0x0f);
  display.setSegments(tabSeg);
  delay(2000);
  
 for (int i=0;i<4;i++)
 {
  tabSeg[i] = display.encodeDigit(i+1);
 }  
    display.setSegments(tabSeg);
}
