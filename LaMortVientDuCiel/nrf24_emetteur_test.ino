
/*******************************************************************
 Chaque seconde, un nombre est émis par une carte Arduino munie
 d'un module nRF24L01.
 Ce message peut être capté par un autre Arduino, ou par un Raspberry Pi.
 http://electroniqueamateur.blogspot.ca/2017/02/communication-par-nrf24l01-entre-deux.html
********************************************************************/
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8);
const uint64_t addresse = 0x1111111111;
const int taille = 32;
byte message[taille + 1];


/*
void createMessage(char valeur1[], char valeur2[])
{
  
  int size1 = sizeof(mot1);
  int size2 = sizeof(mot2);
  
  for(int i = 0; i < size1-1 ; i++)
  {
    message[i] = valeur1[i];
  }

  message[size1-1] = ';';
  
  for(int j = 0; j <= size2; j++)
  {
    message[j+size1] = valeur2[j];
  }  

  

  Serial.println(message);  
}*/



void createMessageFromJoystick(byte valeur0, byte valeur1, byte valeur2, byte valeur3)
{
 message[0] = valeur0;
 message[1] = valeur1; 
 message[2] = valeur2;
 message[3] = valeur3;

}

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Emetteur de donnees");
  radio.begin();
  radio.openWritingPipe(addresse);
}

void loop(void)
{
 /* Serial.print("X = ");
  Serial.print(analogRead(A0));
  Serial.print("; Y = ");
  Serial.println(analogRead(A1));*/
  delay(10);
  createMessageFromJoystick(abs(analogRead(A0)/4), abs(analogRead(A1)/4), abs(analogRead(A2)/4), abs(analogRead(A3)/4));
  radio.write( message, taille );
  
  delay(100);
}

