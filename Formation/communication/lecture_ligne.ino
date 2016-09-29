String readString = "";

void setup()
{
    Serial.begin(9600);

}

void loop()
{
while (Serial.available()) 
{
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() >0) 
    {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
}

  if (readString.length() >0) 
      Serial.println(readString); //see what was received
  
  readString="";

}
