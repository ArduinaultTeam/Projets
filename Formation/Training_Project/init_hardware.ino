void init_hardware()
{
  display.setBrightness(0x0f);
  display.setSegments(tabSeg);
  delay(2000);

  
  pinMode(BUTTON_1, INPUT);
  pinMode(PHOTO_R1, INPUT);
  pinMode(PHOTO_R2, INPUT);
  pinMode(LED, OUTPUT);
}

