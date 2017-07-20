struct message {
  byte SOF;
  short int arbitrage;
  char commande;
  double donnees;
  short int CRC;
  byte ACK:2;
  char EOF;
};

struct message createMessage

