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
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variable
int valeur_lum = 0;
int valeur_tension = 0;
int valeur_tmp = 0;
int led_verte = 0;
int etat_bouton = 0;
int tmp = 0;

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
  
  // ENVOYER
  // Code lumière
  valeur_lum = map(analogRead(LUM), 0, 1023, 0, 255);
  stmp[0] = 2;
  stmp[7] = valeur_lum;
  CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
  delay(200);
  
  // Code Température
  valeur_tension = map(analogRead(TMP), 0, 1023, 0, 5000);  // Transforme la valeur lue en tension
  valeur_tmp = map(valeur_tension, 0, 1750, 0, 255);  // Transforme la tension pour le CAN
  stmp[0] = 2;
  stmp[6] = valeur_tmp;
  CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
  delay(400);

/*  etat_bouton = digitalRead(BOUTON);
  if(etat_bouton == HIGH) {
    stmp[0] = 100;
    stmp[6] = 1;
  }
  else {
    stmp[0] = 0;
    stmp[6] = 0;
  }
  
  // Code Bouton poussoir
  if(digitalRead(BOUTON) && etat_bouton == 0) {
    etat_bouton = 1;
  } // Fin if
  
  else if(digitalRead(BOUTON) && etat_bouton == 1) {
    etat_bouton = 0;
  } // Fin else if
  */

  for(int i = 0; i < 8; i++) { // Initialisation du message {0, 0, 0, 0, 0, 0, 0, 0}
    stmp[i] = 0;
  }

  CAN.sendMsgBuf(0x01, 0, LENGTH, stmp);
  delay(100);
  
  // RECEVOIR
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
}  // Fin loop

// --- Fin du programme ---
