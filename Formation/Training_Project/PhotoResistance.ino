#include <TM1637Display.h>

#define BUTTON_1  5   //ON/OFF
#define PHOTO_R1  A0  //Analog Photoresistance 1
#define PHOTO_R2  A1  //Analog Photoresistance 2
#define LED 4
#define CLK 2
#define DIO 3

TM1637Display display(CLK,DIO);

uint8_t tabSeg[] = { 0xff, 0xff, 0xff, 0xff };

int Photoresistance1 = 0;
int Photoresistance2 = 0;
int ActiveLED = 0;
int active = 0;
int previous_bouton = 0;
int actual_bouton = 0;
int allume = 0;

int unite = 0;
int dizaine = 0;
int centaine = 0;

String Valeur1 = " PR1 : "; //Chaine de caractère pour faire afficher sur la console
String Valeur2 = " PR2 : ";

void setup() // fonction setup - début de l'exécution du programme
{
  init_hardware();
  Serial.begin(9600);
}

void loop() 
{
  actual_bouton = digitalRead(BUTTON_1);

  if((actual_bouton == 1) && (previous_bouton == 0))
  {
      allume = !allume;
  }
  
  if (allume)
  {
    
    display.setBrightness(0x0f);
    
    if (activate())
      digitalWrite(LED, HIGH); //Allume la LED
    affichage(Photoresistance1,Photoresistance2);
    
    affichage_7seg();
   
    delay(1000);
  }
  else
  {
    Serial.println(" Bouton OFF ");
    delay(100);
    digitalWrite(LED,LOW);
    
    display.setBrightness(0);
    display.setSegments(tabSeg);
  }

  previous_bouton = actual_bouton;
}
