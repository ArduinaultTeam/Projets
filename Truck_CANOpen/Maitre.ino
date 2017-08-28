/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Affectations
#define LUM A0
#define TMP A1
#define POTAR A2
#define BOUTON 4

// Caractéristiques des messages
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variables
int valeur_lum = 0;

int premier_cycle = 1;
int valeur_tension = 0;
int valeur_tmp = 0;
int tmp = 0;

int interrupteur = 0;
int old_bouton = 0;

int valeur_potar = 0;

// Déclaration temporisations
unsigned long int temps_cycle1 = 1000; // Lumière
unsigned long int t1 = 0;
unsigned long int temps_cycle2 = 10000; // Température
unsigned long int t2 = 0;
unsigned long int temps_cycle3 = 100; // Bouton
unsigned long int t3 = 0;
unsigned long int temps_cycle4 = 100; // Potar
unsigned long int t4 = 0;

// Détection des erreurs
int nombre_erreurs_max = 10;
int compteur_erreur_lum = 0;
int compteur_erreur_temp = 0;
int compteur_erreur_bouton = 0;
int compteur_erreur_potar = 0;

bool message_pret = false;

/**************************************************************************************/
/*                                INITIALISATION                                      */
/**************************************************************************************/

void setup() {
  
  Serial.begin(115200);  // Init communication série à 115200 Bauds
  
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {  // Init can bus : baudrate = 500k
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
  } // Fin while
  
  Serial.println("CAN BUS Shield init ok!");  

  pinMode(BOUTON, INPUT);
} // Fin setup

unsigned char stmp[LONGUEUR_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buf[LONGUEUR_DATA];

/**************************************************************************************/
/*                                BOUCLE INFINIE                                      */
/**************************************************************************************/

void loop() {

  if((compteur_erreur_lum < nombre_erreurs_max) || (compteur_erreur_temp < nombre_erreurs_max) || (compteur_erreur_bouton < nombre_erreurs_max) || (compteur_erreur_potar < nombre_erreurs_max)) // Si cette condition est respectée tout va bien
  {
    // Code lumière **********
    if(millis() - t1 > temps_cycle1) {
      valeur_lum = map(analogRead(LUM), 0, 1023, 0, 255);  // Transforme la valeur pour le CAN
      stmp[0] = 2;
      stmp[7] = valeur_lum;
      t1 = millis();    
      message_pret = true;
      compteur_erreur_lum++;
    } // Fin if lumière
      
    
    // Code Température **********
    if((millis() - t2 > temps_cycle2) || premier_cycle == 1) {
      valeur_tension = map(analogRead(TMP), 0, 1023, 0, 5000);  // Transforme la valeur lue en tension
      valeur_tmp = map(valeur_tension, 0, 1750, 0, 255);  // Transforme la tension pour le CAN
      stmp[0] = 2; //
      stmp[6] = valeur_tmp;
      t2 = millis();
      message_pret = true;
      premier_cycle = 0;
      compteur_erreur_temp++;
    } // Fin if température
  
  
    // Code interrupteur **********
    if(digitalRead(BOUTON) && (old_bouton==0) && (millis() - t3 > temps_cycle3)) { // On lit l'état du bouton et on teste si le bouton est pressé
      interrupteur = !interrupteur; // Si oui on change l'état de l'interrupteur
      stmp[0] = 100;
      stmp[5] = interrupteur;
      t3 = millis();
      message_pret = true;
      compteur_erreur_bouton++;
    } // Fin if interrupteur
    old_bouton = digitalRead(BOUTON);
    
    
    // Code potar **********
    if(interrupteur && (millis() - t4 > temps_cycle4) && (millis() - t3 > temps_cycle4)) {
      valeur_potar = map(analogRead(POTAR), 0, 1023, 0, 255);
      stmp[0] = 20;
      stmp[1] = 0;
      stmp[3] = valeur_potar; 
      t4 = millis();
      message_pret = true;
      compteur_erreur_potar++;
    } // Fin if potar
  } // Fin if erreurs compteur

  
  // Code erreur **********
  else if ((compteur_erreur_temp >= nombre_erreurs_max) || (compteur_erreur_lum >= nombre_erreurs_max) || (compteur_erreur_bouton >= nombre_erreurs_max) || (compteur_erreur_potar >= nombre_erreurs_max)) {
    stmp[0] = 102; // Ce code serait donc une erreur de communication

    if(compteur_erreur_lum > nombre_erreurs_max) stmp[7] = 1; // Erreur communication lumière
    else stmp[7] = 0;
    
    if(compteur_erreur_temp > nombre_erreurs_max) stmp[6] = 1; // Erreur communication température
    else stmp[6] = 0;

    if(compteur_erreur_bouton > nombre_erreurs_max) stmp[5] = 1; // Erreur communication température
    else stmp[5] = 0;

    if(compteur_erreur_potar > nombre_erreurs_max) stmp[3] = 1; // Erreur communication température
    else stmp[3] = 0;
    
    CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
  } // Fin else if
  Serial.println(compteur_erreur_lum);
  Serial.println(compteur_erreur_temp);
  Serial.println(compteur_erreur_bouton);
  Serial.println(compteur_erreur_potar);

  // Code envoyer message **********
  if(message_pret) {
      CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
      message_pret = false;
    } // Fin if message_pret


  unsigned char len = 0;

  
  // Check if data coming **********
  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    // Read data,  len: data length, buf: data buf
    CAN.readMsgBuf(&len, buf);

    unsigned int canId = CAN.getCanId();
    
    if(buf[0] != 0) {    
      Serial.println("-----------------------------");
      Serial.print("Get data from ID: ");
      Serial.println(canId, DEC);

      // Print the data
      for(int i = 0; i<len; i++) {
        Serial.print(buf[i], DEC);
        Serial.print("\t");
      } // Fin for
    Serial.println("");
    } // Fin if
  } // Fin CAN receive
  

  // Traitement des acknowledgment **********
  if(buf[0] == 2 && buf[1] == 1) { // Dès que l'on reçoit l'acknowledgment capteurs de la part du LCD on peut reset les compteurs d'erreur
    compteur_erreur_lum = 0;
    compteur_erreur_temp = 0;
  }  // Fin if buf[0] = 2

  if(buf[0] == 20 && buf[1] == 1) { // Dès que l'on reçoit l'acknowledgment potar de la part de l'esclave on peut reset les compteurs d'erreur
    compteur_erreur_potar = 0;
  }  // Fin if buf[0] = 20

  if(buf[0] == 100 && buf[1] == 1) { // Dès que l'on reçoit l'acknowledgment potar de la part de l'esclave on peut reset les compteurs d'erreur
    compteur_erreur_bouton = 0;
  }  // Fin if buf[0] = 100
  // Fin du traitement des acknowledgement

  
  delay(100);
}  // Fin loop

// --- Fin du programme ---
