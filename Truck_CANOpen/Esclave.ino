/**************************************************************************************/
/*                                 DECLARATIONS                                       */
/**************************************************************************************/

// Librairie CAN
#include <mcp_can.h>
#include <SPI.h>
#define SPI_CS_PIN 9
MCP_CAN CAN(SPI_CS_PIN);

// Librairie servomoteur
#include <Servo.h>

// Affectations
#define BLANC 3
#define ROUGE 4
#define PINSERVO 5

// Caractéristiques des messages
#define LONGUEUR_DATA 8
#define LENGTH 8

// Déclaration variables
int commande_moteur = 0; // Variable qui stock la position du servomoteur

bool ack_pret = false;

// Création d'un objet servo pour le contrôler
Servo servo;

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
  for(int i = 3; i < 5; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); 
  }

  servo.attach(PINSERVO); // On attache la pin
       
} // Fin setup

unsigned char stmp[LONGUEUR_DATA] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char buf[LONGUEUR_DATA];

/**************************************************************************************/
/*                                BOUCLE INFINIE                                      */
/**************************************************************************************/

void loop() {
  
  unsigned char len = 0;
  
  // Check if data coming **********
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


    // Code réception jour/nuit **********
    if(buf[0] == 3) {
      if(buf[4]) digitalWrite(BLANC, HIGH);
      else digitalWrite(BLANC, LOW);
      ack_pret = true;
    } // Fin if


    // Code réception servomoteur **********
    if(buf[0] == 20) {
      commande_moteur = map(buf[3], 0, 255, 0, 179);
      servo.write(commande_moteur);
      ack_pret = true;
    } // Fin if


    // Code réception interrupteur **********
    if(buf[0] == 100) {
      if (buf[5]) digitalWrite(ROUGE, HIGH);
      else digitalWrite(ROUGE, LOW);
      ack_pret = true;
    } // Fin if
  } // Fin CAN receive


  // Acknowledgment **********
  if(ack_pret) {
    for(int i = 0; i < LONGUEUR_DATA; i++)  {
      stmp[i] = buf[i];
    } // Fin for
    stmp[1] = 1;
    CAN.sendMsgBuf(0x02, 0, LENGTH, stmp);
    stmp[0] = 0;
    ack_pret = false;
  } // Fin if
      
  delay(10);
} // Fin loop

// --- Fin du programme ---
