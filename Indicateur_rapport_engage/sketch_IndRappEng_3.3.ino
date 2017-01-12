/* 
Projet : Indicateur de rapport engagé
Développeur : Super Benji

But du projet : A l'aide de 2 contacteurs magnétiques on détecte le passage de rapport d'une moto
Un reset est effectué à l'aide du voyant du neutre sur le tableau de bord

Comment : 2 contacteurs magnétiques sont liés aux interruptions de la carte Arduino
Le voyant est sur une entrée numérique
Un afficheur 7 segments permet d'afficher le rapport engagé

*/

/***** Liste des bibliothèques *****/

// Bibliothèque pour les interruptions
  #include <PinChangeInt.h>


/***** Variables globales *****/

// Variables compteur et neutre.  
  int compteur = 0;
  int neutre = 0;

// Variables de Update 7 segment.
  const int A = 5;
  const int B = 6;
  const int C = 7;
  const int D = 8;
  const int E = 9;
  const int F = 10;
  const int G = 11;


/***** Fonctions d'interruptions *****/

// Signal du neutre: neutre=0 && compteur=0.  
  void ISR3()
  {
    neutre = 0;
    compteur = 0;
  }
     
// Contacteur haut: si neutre>=0 & compteur<=1 -> Compteur=2, Sinon incremente compteur sauf si compteur=5.
  void ISR1()
  {
  if (neutre >= 0 && compteur <= 1)
  {
    compteur = 2;
    neutre = 1;
  }

  else if (compteur == 5)
  {
  }
  
  else 
  {
    compteur ++;
    neutre = 1;
  } 
  }

// Contacteur bas: si neutre=0 & compteur=0 -> Compteur=1, Sinon decremente compteur sauf si compteur=1.
  void ISR2()
  {
  if (neutre == 0 && compteur == 0) 
  {
    compteur = 1; neutre = 1;
  }
  
  else if (compteur == 1)
  {
  }
         
  else 
  {
    compteur --;
    neutre = 1;
  }    
  }


/***** Fonction setup de l'Arduino *****/

void setup() {
  inithardware();  

  PCintPort::attachInterrupt (2,ISR1,RISING);
  PCintPort::attachInterrupt (3,ISR2,RISING);
  PCintPort::attachInterrupt (4,ISR3,RISING);
}


/***** Fonction loop de l'Arduino *****/

void loop() {
 
  Serial.print("compteur : ");
  Serial.print(compteur);
  Serial.print("\t");
  Serial.print("neutre : ");
  Serial.println(neutre);
   
// Update 7 segments via fonction segupdate pour affichage des rapport en fonction du compteur.
segupdate (); 
}
