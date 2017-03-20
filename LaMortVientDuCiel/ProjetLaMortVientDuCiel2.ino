#include "I2Cdev.h"

//GESTION INCLUDE DES 4 MOTEURS
#include <Servo.h>
int value_Moteur_Avant_Droite = 0;
int value_Moteur_Avant_Gauche = 0;
int value_Moteur_Derriere_Droite = 0;
int value_Moteur_Derriere_Gauche = 0;

int pitchRef;
int rollRef;
int yawRef;
int altitudeRef;

int pitchDesire;
int rollDesire;
int yawDesire;
int altitudeDesire;

Servo Moteur_Avant_Droite;
Servo Moteur_Avant_Gauche;
Servo Moteur_Derriere_Droite;
Servo Moteur_Derriere_Gauche;





#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL


#define MOTEUR_VARIATION 10
#define THRESHOLD 10
#define ANGLE_VARIATION 20

#define MOTEUR_AVANT_DROITE 3
#define MOTEUR_AVANT_GAUCHE 5
#define MOTEUR_DERRIERE_DROITE 6
#define MOTEUR_DERRIERE_GAUCHE 9

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void gestionAnglesDesires(byte xJoystickGauche, byte yJoystickGauche, byte xJoystickDroit, byte yJoystickDroit){
  if (xJoystickGauche > 250) {
    rollDesire = rollRef - ANGLE_VARIATION;
  } else if (xJoystickGauche < 5) {
     rollDesire = rollRef + ANGLE_VARIATION;
  }
  
  if (yJoystickGauche > 250) {
    pitchDesire = pitchRef - ANGLE_VARIATION;
  } else if (yJoystickGauche < 5) {
    pitchDesire = pitchRef + ANGLE_VARIATION;
  }
  if (xJoystickDroit > 250) {
    yawDesire = yawRef - ANGLE_VARIATION;
  } else if (xJoystickDroit < 5) {
    yawDesire = yawRef - ANGLE_VARIATION;
  }
  if (yJoystickDroit > 250) {
    altitudeDesire = altitudeRef + 1;
  } else if (yJoystickDroit < 5) {
    altitudeDesire = altitudeRef -1;
  }
}


void affichageInput() {
  Serial.print(" On a roll Desiré a ");
  Serial.println(rollDesire);
  Serial.print(" On a pitch Désiré a ");
  Serial.println(pitchDesire);
  Serial.print(" On a yaw Desiré a ");
  Serial.println(yawDesire);
  Serial.print(" On a altitude Désiré a ");
  Serial.println(altitudeDesire);
 
}



void gestionStabilite(int yaw, int roll, int pitch, int yawDesire,  int rollDesire, int pitchDesire, int altitudeDesire){
// Gestion selon changement du ROLL
  if (roll < rollRef - THRESHOLD) {
    value_Moteur_Avant_Droite += MOTEUR_VARIATION;
    value_Moteur_Avant_Gauche -= MOTEUR_VARIATION;
    value_Moteur_Derriere_Droite += MOTEUR_VARIATION;
    value_Moteur_Derriere_Gauche -= MOTEUR_VARIATION;
  }

// Gestion selon changement du ROLL
  if (roll > rollRef + THRESHOLD) {
    value_Moteur_Avant_Droite -= MOTEUR_VARIATION;
    value_Moteur_Avant_Gauche += MOTEUR_VARIATION;
    value_Moteur_Derriere_Droite -= MOTEUR_VARIATION;
    value_Moteur_Derriere_Gauche += MOTEUR_VARIATION;
  }

// Gestion selon changement du PITCH
  if (pitch < pitchRef - THRESHOLD) {
    value_Moteur_Avant_Droite += MOTEUR_VARIATION;
    value_Moteur_Avant_Gauche += MOTEUR_VARIATION;
    value_Moteur_Derriere_Droite -= MOTEUR_VARIATION;
    value_Moteur_Derriere_Gauche -= MOTEUR_VARIATION;
  }

// Gestion selon changement du PITCH
  if (pitch > pitchRef + THRESHOLD) {
    value_Moteur_Avant_Droite -= MOTEUR_VARIATION;
    value_Moteur_Avant_Gauche -=  MOTEUR_VARIATION;
    value_Moteur_Derriere_Droite += MOTEUR_VARIATION;
    value_Moteur_Derriere_Gauche += MOTEUR_VARIATION;
  }
  
// Gestion de l'altitude
 if (altitudeDesire == 1) {
    value_Moteur_Avant_Droite += MOTEUR_VARIATION;
    value_Moteur_Avant_Gauche +=  MOTEUR_VARIATION;
    value_Moteur_Derriere_Droite += MOTEUR_VARIATION;
    value_Moteur_Derriere_Gauche += MOTEUR_VARIATION;
 } else if (altitudeDesire == -1) {
    value_Moteur_Avant_Droite -= MOTEUR_VARIATION;
    value_Moteur_Avant_Gauche -=  MOTEUR_VARIATION;
    value_Moteur_Derriere_Droite -= MOTEUR_VARIATION;
    value_Moteur_Derriere_Gauche -= MOTEUR_VARIATION;
 }
}






// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
// SET UP MOTEUR
    Moteur_Avant_Droite.attach(MOTEUR_AVANT_DROITE);
    Moteur_Avant_Gauche.attach(MOTEUR_AVANT_GAUCHE);
    Moteur_Derriere_Droite.attach(MOTEUR_DERRIERE_DROITE);
    Moteur_Derriere_Gauche.attach(MOTEUR_DERRIERE_GAUCHE);



  
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    pitchDesire = pitchRef;
    rollDesire = rollRef;
    yawDesire = yawRef;
    altitudeDesire = altitudeRef;


    gestionAnglesDesires(message[0], message[1], message[2], message[3]);
    affichageInput();
  

  
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;


        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
        #endif


        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);


        //On affecte les nouvelles valeurs calculées aux moteurs
        Moteur_Avant_Droite.writeMicroseconds(value_Moteur_Avant_Droite);
        Moteur_Avant_Gauche.writeMicroseconds(value_Moteur_Avant_Gauche);
        Moteur_Derriere_Droite.writeMicroseconds(value_Moteur_Derriere_Droite);
        Moteur_Derriere_Gauche.writeMicroseconds(value_Moteur_Derriere_Gauche);
    }
}
