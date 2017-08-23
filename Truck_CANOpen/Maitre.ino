/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Affectation
#define LUM A0
#define TMP A1
#define BOUTON 4

// Caractéristiques des messages
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variable
int premier_cycle = 1;
int valeur_lum = 0;
int valeur_tension = 0;
int valeur_tmp = 0;
int tmp = 0;
int interrupteur = 0;
int bascule = 0;
int etat_bouton = 0;

// Déclaration temporisation
unsigned long int temps_cycle1 = 1000; // Lumière
unsigned long int t1 = 0;
unsigned long int temps_cycle2 = 10000; // Température
unsigned long int t2 = 0;
unsigned long int temps_cycle3 = 125; // Bouton
unsigned long int t3 = 0;

// Detection des erreurs
int nombre_erreurs_max = 10;
int compteur_erreur_lum = 0;
int compteur_erreur_temp = 0;

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

  
  if((compteur_erreur_lum < nombre_erreurs_max) || (compteur_erreur_temp < nombre_erreurs_max)) // Si cette condition est respectée tout va bien
  {
    // Code lumière
    if(millis() - t1 > temps_cycle1) {
      valeur_lum = map(analogRead(LUM), 0, 1023, 0, 255);
      stmp[0] = 2;
      stmp[7] = valeur_lum;
      t1 = millis();
    
      message_pret = true;
    
      compteur_erreur_lum++;
    } // Fin if lumière
    
  
    // Code Température
    if(millis() - t2 > temps_cycle2) {
      valeur_tension = map(analogRead(TMP), 0, 1023, 0, 5000);  // Transforme la valeur lue en tension
      valeur_tmp = map(valeur_tension, 0, 1750, 0, 255);  // Transforme la tension pour le CAN
      stmp[0] = 2;
      stmp[6] = valeur_tmp;
      t2 = millis();
      
      message_pret = true;
      
      premier_cycle = 0;
      
      compteur_erreur_temp++;
    } // Fin if température
    
  } // Fin if erreurs compteur
  
  else if ((compteur_erreur_temp >= nombre_erreurs_max) || (compteur_erreur_lum >= nombre_erreurs_max))
  {
    stmp[0] = 102; // Ce code serait donc une erreur de communication
    
    if(compteur_erreur_lum > nombre_erreurs_max)
      stmp[7] = 1; // erreur communication lumière
    else
      stmp[7] = 0;
    
    if(compteur_erreur_temp > nombre_erreurs_max)
      stmp[6] = 1; // erreur communication température
    else
      stmp[6] = 0;
    
    CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
  }

  if(message_pret)
  {
    CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
    message_pret = false;
  }
    
  // Code interrupteur
  etat_bouton = digitalRead(BOUTON);
  if (etat_bouton && millis() - t3 > temps_cycle3) { // On lit l'état du bouton et on teste si le bouton est pressé
    if (bascule == 1) { // On regarde s'il y a un changement d'état
      interrupteur = !interrupteur; // Si oui on change l'état de l'interrupteur
      bascule = 0; // On réinitialise la bascule
      stmp[0] = 100;
      stmp[5] = interrupteur;
      t3 = millis();
      CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
    } // Fin if     
    
    else {
      bascule = 1; // Si le bouton n'est pas pressé, on active la bascule
      stmp[0] = 100;
      stmp[5] = interrupteur;
      t3 = millis();
      CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
    } // Fin else
  } // Fin if

  unsigned char len = 0;
  
  // Check if data coming
  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    // Read data,  len: data length, buf: data buf
    CAN.readMsgBuf(&len, buf);

    unsigned int canId = CAN.getCanId();
    
    if (buf[0] != 0) {    
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
  
  
  
  
  
  // Traitement des acknowledgment
  if(buf[0] == 3) // Dès que l'on reçoit l'acknowledgment capteurs de la part du LCD on peut reset les compteurs d'erreur
  {
    compteur_erreur_lum = 0;
    compteur_erreur_temp = 0;
  }
  
  
}  // Fin loop

// --- Fin du programme ---
