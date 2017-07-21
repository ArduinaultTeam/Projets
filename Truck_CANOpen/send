// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


/////
struct message {
  int arbitrage;
  int commande;
  int donnees;
};

int arbitrage = 2;
byte commande;
int donnees = 0;

struct message envoyer = {arbitrage, commande, donnees};
/////


void setup() {
  
  Serial.begin(9600);

  while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
  
  Serial.println("CAN BUS Shield init ok!");
}

void loop() {
  
  // Champ de commande
  /***************************************** 
     Premier bit : 0 (trame standard)
     Deuxième bit n/a
     4 autres bits Data Length Code (DLC)
  *****************************************/

  envoyer.commande = 0b000010;
  
  bool ext = envoyer.commande>>5;
  
  int mask = 0b001111;
  
  int len = mask & envoyer.commande; // 0b000010 = 2
  // Fin champ de commande
  
  CAN.sendMsgBuf(envoyer.arbitrage,ext,len,envoyer.donnees);
  
  delay(100);  
  
  envoyer.donnees++;

  Serial.print("Arbitrage : ");
  Serial.println(envoyer.arbitrage);

  Serial.print("Commande : ");
  Serial.println(envoyer.commande);

  Serial.print("Donnees : ");
  Serial.println(envoyer.donnees); 
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
