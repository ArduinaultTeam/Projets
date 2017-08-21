// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
#define SPI_CS_PIN 9;

// Set CS pin
MCP_CAN CAN(SPI_CS_PIN);

// Set Pin value for button
#define BOUTON 3

// Set photoresistance values
int lum = 0; // lum value receive from CAN
int valeur_lum = 0; // lum value after map operation. From 0..127 to 0..1023

// Set values for problem detection
unsigned long int t = 90;
int flag = 1;



void setup() {
    
  Serial.begin(115200);
  
  // init can bus : baudrate = 500k
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
  }
  
  Serial.println("CAN BUS Shield init ok!");

  pinMode(BOUTON, INPUT);
}

unsigned char stmp[8] = {2, 0, 0, 0, 0, 0, 0, 0};


void loop() {
  
  // RECEVOIR
  unsigned char len = 0;
  unsigned char buf[8];
  
  // check if data coming
  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    // read data,  len: data length, buf: data buf
    CAN.readMsgBuf(&len, buf);

    unsigned int canId = CAN.getCanId();
    
    Serial.println("-----------------------------");
    Serial.print("Get data from ID: ");
    Serial.println(canId, HEX);
    
    // print the data
    for(int i = 0; i<len; i++) {
      Serial.print(buf[i], DEC);
      Serial.print("\t");
    }
    
    Serial.println();
    
    lum = buf[0];
    valeur_lum = map(lum, 0, 127, 0, 1023);
    Serial.println(valeur_lum);
  }


  // ENVOYER
  // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
/*  stmp[0] = (stmp[0] == 2 ) ? 3 : 2;
    
  if (digitalRead(BOUTON) || t < 20) {
    for (int i = 1; i < 8; i++) {
      stmp[i]=9;
    }

    stmp[0] = 100;
    
    if (flag  == 1) {
      t = 0;
      flag = 0;
    }

    else {
      t++;
    }
  }

  else if (t == 20) {
    for (int i = 0; i < 8; i++) {
      stmp[i]=1;
    }
    
    stmp [0] = 100;
    t = 90;
    flag = 1;
    CAN.sendMsgBuf(0x00, 0, 8, stmp);
    
    for (int i = 0; i < 8; i++) {
      stmp[i]=0;
    }
      
    stmp [0] = 2;
  }
  
  else {
    stmp[7] = stmp[7]+2;
    if(stmp[7] == 100) {
      stmp[7] = 0;
      stmp[6] = stmp[6] + 1;
      
      if(stmp[6] == 100) {
        stmp[6] = 0;
        stmp[5] = stmp[6] + 1;
      }
    }
  }
*/
  
  CAN.sendMsgBuf(0x00, 0, 8, stmp);
  // send data per 100ms    unsigned char len = 0;
  delay(100);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
