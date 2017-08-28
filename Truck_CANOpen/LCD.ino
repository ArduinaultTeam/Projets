/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Librairie Ecran LCD
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

// Caractéristiques des messages
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variables
int valeur_lum = 0;
int valeur_tmp = 0;
int tmp = 0;

// Déclaration temporisations
unsigned long int temps_cycle = 100;
unsigned long int t = 0;

// Détection des erreurs
int nombre_erreurs_max = 10;
int compteur_erreur_led_blanche = 0;

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

  lcd.begin(16,2); // Init ecran LCD
} // Fin setup

unsigned char stmp[LONGUEUR_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buf[LONGUEUR_DATA];

/**************************************************************************************/
/*                                BOUCLE INFINIE                                      */
/**************************************************************************************/

void loop() {

  unsigned char len = 0;

  // Code jour/nuit **********
  if(millis() - t > temps_cycle) {
    stmp[0] = 3;
    if(valeur_lum < 800) {
      stmp[4] = 1;
      t = millis();
      compteur_erreur_led_blanche++;
    } // Fin if
    else {
      stmp[4] = 0;
      t = millis();
      compteur_erreur_led_blanche++;
    } // Fin else
    CAN.sendMsgBuf(0x03, 0, LENGTH, stmp);    
  } // Fin if code jour/nuit


  // Code erreur **********
  else if (compteur_erreur_led_blanche >= nombre_erreurs_max) {
    stmp[0] = 102; // Ce code serait donc une erreur de communication

    if(compteur_erreur_led_blanche > nombre_erreurs_max) stmp[4] = 1; // Erreur communication lumière
    else stmp[4] = 0;

    CAN.sendMsgBuf(0x03, 0, LENGTH, stmp);
  } // Fin else if code erreur
  Serial.println(compteur_erreur_potar);
    
  
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

 
    // Code réception infos des capteurs  **********
    if(buf[0] == 2) {
      // Code Lumière **********
      valeur_lum = map(buf[7], 0, 255, 0, 1023);
    
      // Code Température **********
      valeur_tmp = map(buf[6], 0, 255, 0, 1750);
      tmp = map(valeur_tmp, 0, 1750, -50, 125);
  
      // Code Ecran LCD **********
      lcd.setCursor(0,0);  // Curseur à gauche sur la première ligne
      if(valeur_lum < 800) lcd.print("Nuit");   
      else lcd.print("Jour");
      lcd.setCursor(0,1); // Curseur à gauche sur la deuxième ligne
      lcd.print(valeur_lum);
      lcd.setCursor(9,0); // Curseur à la neuvième case de la première ligne
      lcd.print("Tmp :");
      lcd.setCursor(9,1); // Curseur à la neuvième case de la seconde ligne
      lcd.print(tmp);
      stmp[0] = 2;

      // Acknowledgment **********
      for(int i =0; i< LONGUEUR_DATA;i++)  {
        stmp[i] = buf[i];
      } // Fin for
      stmp[1] = 1;
      CAN.sendMsgBuf(0x03, 0, LENGTH, stmp);
      stmp[0] = 0;
    } // Fin if
  } // Fin CAN receive

  // Traitement des acknowledgment **********
  if(buf[0] == 3 && buf[1] == 1) { // Dès que l'on reçoit l'acknowledgment capteurs de la part du LCD on peut reset les compteurs d'erreur
    compteur_erreur_led_blanche = 0;
  } // Fin if buf[0] = 2
  // Fin du traitement des acknowledgement
  
  delay(100);
} // Fin loop

// --- Fin du programme ---
