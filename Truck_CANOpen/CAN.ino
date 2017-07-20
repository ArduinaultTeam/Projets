// Les defines suivants sont à redéfinir
# define arbitrage 0
# define commande 0
# define donnees 0

struct message envoie = {arbitrage, commande, donnees};

// Le code suivant ne marche pas !

void setup() {
  Serial.begin(9600);
}

void loop() {
  
byte envoie.DLC = 0b000010;

bool ext = envoie.DLC>>5;

int mask = 0b001111;

int len = mask & envoie.DLC; // 0b000010 = 2

CAN.sendMsgbuf(envoie.arbitrage,ext,len,envoie.donnees);

envoie.donnees++;

delay(100);
}
