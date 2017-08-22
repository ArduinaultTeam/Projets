/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Affectation
#define VERT 4
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variable
int led_verte = 0;

/**************************************************************************************/
/*                                INITIALISATION                                      */
/**************************************************************************************/

void setup() {
  
  Serial.begin(115200); // Init communication série à 115200 Bauds
  
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {  // Init can bus : baudrate = 500k
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
  } // Fin while
  
  Serial.println("CAN BUS Shield init ok!");

  pinMode(VERT, OUTPUT);
  digitalWrite(VERT, LOW);        
} // Fin setup

unsigned char stmp[LONGUEUR_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buf[LONGUEUR_DATA];

/**************************************************************************************/
/*                                BOUCLE INFINIE                                      */
/**************************************************************************************/

void loop() {

  // ENVOYER
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
} // Fin loop

// --- Fin du programme ---
