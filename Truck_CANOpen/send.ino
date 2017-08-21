// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

#define BLANC 4
#define JAUNE 5
#define VERT 6
#define ROUGE 7
#define LUM A0

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    
    for (int i = 4; i <= 7; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);        
    }
}

unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int pb = 0;


void loop()
{
    // ENVOYER
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    Serial.println(analogRead(LUM));
    if (pb == 0)
    {
        stmp[7] = stmp[7]+1;
        
        if(stmp[7] == 100)
        {
            stmp[7] = 0;
            stmp[6] = stmp[6] + 1;
            
            if(stmp[6] == 100)
            {
                stmp[6] = 0;
                stmp[5] = stmp[6] + 1;
            }
        
        }
    CAN.sendMsgBuf(0x00, 0, 8, stmp);
    delay(100);                       // send data per 100ms    unsigned char len = 0;
    }
    

    
    // RECEVOIR
    unsigned char buf[8];
    unsigned char len = 0;
    
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned int canId = CAN.getCanId();
        
        Serial.println("-----------------------------");
        Serial.print("Get data from ID: ");
        Serial.println(canId, HEX);
        
        if (buf[0] == 2)
        {
            for(int i = 0; i<len; i++)    // print the data
            {
                Serial.print(buf[i], HEX);
                Serial.print("\t");
            }
            Serial.println();
        }
        
        else if (buf[0] == 100)
        {
            if (buf [7] == 9)
            {
                pb = 1;
                digitalWrite(ROUGE, HIGH);
                digitalWrite(VERT, LOW);
            }
            
            if (buf [7] == 1)
            {
                pb = 0;
                digitalWrite(ROUGE, LOW);
                digitalWrite(VERT, HIGH); 
            }
        }
    }
       
    if (buf[6]%2 == 0)
    {
        digitalWrite(JAUNE, HIGH);
        digitalWrite(BLANC, LOW); 
    }

    else
    {
        digitalWrite(JAUNE, LOW);
        digitalWrite(BLANC, HIGH); 
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
