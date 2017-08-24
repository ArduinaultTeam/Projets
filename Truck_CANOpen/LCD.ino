/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Libraire Ecran LCD
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

// Caractéristiques des messages
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variable
int valeur_lum = 0;
int valeur_tmp = 0;
int tmp = 0;

// Déclaration temporisation
unsigned long int temps_cycle = 100;
unsigned long int t = 0;

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

  // Init ecran LCD
  lcd.begin(16,2);
} // Fin setup

unsigned char stmp[LONGUEUR_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buf[LONGUEUR_DATA];

/**************************************************************************************/
/*                                BOUCLE INFINIE                                      */
/**************************************************************************************/

void loop() {

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

    if(buf[0] == 2) { // Réception infos des capteurs
      // Code Lumière
      valeur_lum = map(buf[7], 0, 255, 0, 1023);
      Serial.println(valeur_lum);
    
      // Code Température
      valeur_tmp = map(buf[6], 0, 255, 0, 1750);
      tmp = map(valeur_tmp, 0, 1750, -50, 125);
      Serial.println(tmp);
  
      // Code Ecran LCD
      lcd.setCursor(0,0);  // Curseur à gauche sur la première ligne
      if(millis() - t > temps_cycle) {
        if(valeur_lum < 750) {
          lcd.print("Nuit");
          stmp[4] = 1;
          t = millis();
        }
      
        else {
          lcd.print("Jour");
          stmp[4] = 0;
          t = millis();
        }
      }  
      
      lcd.setCursor(0,1);  // Curseur à gauche sur la deuxième ligne
      lcd.print(valeur_lum);
      lcd.setCursor(9,0);  // Curseur à la neuvième case de la première ligne
      lcd.print("Tmp :");
      lcd.setCursor(9,1);  // Curseur à la neuvième case de la seconde ligne
      lcd.print(tmp);

      // Acknowledgment
      stmp[0] = 3;
      CAN.sendMsgBuf(0x03, 0, LENGTH, stmp);
      stmp[0] = 0;
      
    } // Fin if
  } // Fin CAN receive
  delay(100);
} // Fin loop

// --- Fin du programme ---
