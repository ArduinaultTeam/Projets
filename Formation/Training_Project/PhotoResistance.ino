
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

int unite1 = 0;
int dizaine1 = 0;
int centaine1 = 0;

int unite2 = 0;
int dizaine2 = 0;
int centaine2 = 0;

String Valeur1 = " PR1 : "; //Chaine de caractère pour faire afficher sur la console
String Valeur2 = " PR2 : ";

void setup() // fonction setup - début de l'exécution du programme
{
  init_hardware();
  Serial.begin(9600);
}

void loop() 
{
  if (digitalRead(BUTTON_1)) // Par défaut le button = 0 pour le mettre = 1 par défaut, utiliser "!" --> (digitalRead(!BUTTON_1))
  {
    
    display.setBrightness(0x0f);
    
    if (activate())
      digitalWrite(LED, HIGH); //Allume la LED
    affichage(Photoresistance1,Photoresistance2);

    // 7seg

    
    int unite1 = Photoresistance1 %10;
    int dizaine1 = ((Photoresistance1 %100)- unite1)/10;
    int centaine1 = (Photoresistance1 - Photoresistance1 %100)/100; 

    Serial.print(centaine1);
    Serial.print(dizaine1);
    Serial.println(unite1);

    tabSeg[3] = display.encodeDigit(unite1);
    tabSeg[2] = display.encodeDigit(dizaine1);
    tabSeg[1] = display.encodeDigit(centaine1);
    tabSeg[0] = display.encodeDigit(1);

    display.setSegments(tabSeg);

    delay(2000);

    int unite2 = Photoresistance2 %10;
    int dizaine2 = ((Photoresistance2 %100)- unite2)/10;
    int centaine2 = (Photoresistance2 - Photoresistance2 %100)/100; 

    tabSeg[3] = display.encodeDigit(unite2);
    tabSeg[2] = display.encodeDigit(dizaine2);
    tabSeg[1] = display.encodeDigit(centaine2);
    tabSeg[0] = display.encodeDigit(2);

    display.setSegments(tabSeg);
    
    //


   
    delay(2000);
  }
  else
  {
    Serial.println(" Bouton OFF ");
    delay(1500);
    digitalWrite(LED,LOW);
    display.setBrightness(0);
    display.setSegments(tabSeg);
  }
}
