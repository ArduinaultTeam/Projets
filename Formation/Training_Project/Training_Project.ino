//-----------------------------------------------------------------------------------
//Projet formation

//Concept :
//1 bouton ON/OFF
//1 bouton affichage 7segments
//1 bouton allumage led si valeur différente

//Materiel :
//2 photoresistances : 10Kohm pour chacune
//LED : 220ohm(152ohm)
//3 Boutons : 10Kohm pour chacun
//1 afficheur 7segments
//-----------------------------------------------------------------------------------

#define BUTTON_1  2   //ON/OFF
#define PHOTO_R1  A0  //Analog Photoresistance 1
#define PHOTO_R2  A1  //Analog Photoresistance 2
#define LED 4

int Photoresistance1 = 0;
int Photoresistance2 = 0;
int ActiveLED = 0;

String Valeur1 = " PR1 : "; //Chaine de caractère pour faire afficher sur la console
String Valeur2 = " PR2 : ";

void setup() // fonction setup - début de l'exécution du programme
{
  pinMode(BUTTON_1, INPUT);
  pinMode(PHOTO_R1, INPUT);
  pinMode(PHOTO_R2, INPUT);
  pinMode(LED, OUTPUT);


  Serial.begin(9600);

}

void loop() // fonction loop - est exécutée en boucle
// Une fois que la fonction setup a été exécutée
{
  //Lecture des valeurs analogique des photoresistances
  Photoresistance1 = analogRead(PHOTO_R1);
  Photoresistance2 = analogRead(PHOTO_R2);

  // Condition à respecter pour activer la LED
  if ( abs(Photoresistance1 - Photoresistance2) > 100) // abs pour avoir une valeur absolue, pour ne pas avoir de valeurs négatives.
  {
    ActiveLED = 1;
  }
  else
  {
    ActiveLED = 0;
    digitalWrite(LED, LOW); //Eteint la LED
  }

  if (digitalRead(BUTTON_1)) // Par défaut le button = 0 pour le mettre = 1 par défaut, utiliser "!" --> (digitalRead(!BUTTON_1))
  {
    /*
        //Test du bouton :
        Serial.print(" BUTTON_1 ON "); //ON/OFF

    */

    if (ActiveLED)
      digitalWrite(LED, HIGH); //Allume la LED
      
    Serial.print(Valeur1 + Photoresistance1);
    Serial.print("\t"); // Tabulation
    Serial.println(Valeur2 + Photoresistance2); //println : ln = Ligne Nouvelle

    delay(500);

  }
  else
  {
    Serial.println(" Bouton OFF ");
    delay(1500);


  }
}

