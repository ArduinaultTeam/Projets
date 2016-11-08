#include <TM1637Display.h>
#include <LiquidCrystal.h>

#define BUTTON_1  10   //ON/OFF
#define BUTTON_2  7   //Affichage
#define BUTTON_3  8   //Reglage
#define PHOTO_R1  A0  //Analog Photoresistance 1
#define PHOTO_R2  A1  //Analog Photoresistance 2
#define LED 6

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int Photoresistance1 = 0;
int Photoresistance2 = 0;
int ActiveLED = 0;
int active = 0;
int state = 0;

int previous_bouton1 = 0;
int actual_bouton1 = 0;
int previous_bouton2 = 0;
int actual_bouton2 = 0;
int previous_bouton3 = 0;
int actual_bouton3 = 0;

int sequenceur = 0;
int sequenceur2 = 0;
int sequenceur3 = 0;

int v_reglage_photo = 100;
String Reg = "Reglage : ";

String Valeur1 = " PR1 : "; //Chaine de caractère pour faire afficher sur la console
String Valeur2 = " PR2 : ";

void setup() // fonction setup - début de l'exécution du programme
{
  
  init_hardware();
  Serial.begin(9600);

   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
}

void loop() 
{

//  // set the cursor to column 0, line 1
//  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(0, 1);
//  // print the number of seconds since reset:
//  lcd.print(millis() / 1000);


  
  loadButtons();
  
  if((actual_bouton1 == 1) && (previous_bouton1 == 0)) // Etat 0 -> 1
  {
    sequenceur = !sequenceur;
  }
 
  if (sequenceur)
  {
   state = whichState();

   Serial.println(state);

   switch(state)
   {
   case 1 : // IDLE
    if (activate())
      
      digitalWrite(LED, HIGH);
    break;
    case 2 : // Affichage
    affichage_lcd();
    break;
    case 3 : // Réglage
    affichage_lcd_2lines("Ah que kikou", Reg + v_reglage_photo);
      if((actual_bouton2 == 1) && (previous_bouton2 == 0)) // Etat 0 -> 1
      {
        lcd.setCursor(9 , 1);
        lcd.print("       ");
        v_reglage_photo += 10;
      }
      if((actual_bouton3 == 1) && (previous_bouton3 == 0)) // Etat 0 -> 1
      {
        lcd.setCursor(9 , 1);
        lcd.print("       ");
        v_reglage_photo -= 10;
      }
    break;
    }
    delay(100);
  }
  else // ETAT 0
  {
    state0();
  }
  updateVar();
}
