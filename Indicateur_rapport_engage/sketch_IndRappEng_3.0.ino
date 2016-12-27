// DECLARATION DE VARIABLES:  
  int compteur = 0;
  int neutre = 0;

// BP1A = CONT HAUT(Bouton rouge), BP2A = CONT BAS(Bouton bleu), BP3A = CONT NEUTRE (Bouton blanc)
  int BP1A = 0,BP2A = 0,BP3A = 0;
  int BP1O = 0,BP2O = 0,BP3O = 0;

// GROS ENTIER
  unsigned long currentTime = 0;
  int state = 0;

// Update 7 segment: (Variables)
  const int A = 5;
  const int B = 6;
  const int C = 7;
  const int D = 8;
  const int E = 9;
  const int F = 10;
  const int G = 11;

//Interruption:
  #include <PinChangeInt.h>
      
  void ISR1()
  {
  compteur++;
  neutre = 0;
}
  
  void ISR2()
  {
  compteur--;
  neutre = 0;
}
  
  void ISR3()
  {
  neutre = 0;
}
  
void setup() {
inithardware();  

  PCintPort::attachInterrupt (2,ISR1,RISING);
  PCintPort::attachInterrupt (3,ISR2,RISING);
  PCintPort::attachInterrupt (4,ISR3,RISING);
}

void loop() {
/*
  BP1A = digitalRead (2);
  
// SI CONT HAUT = 1 & NEUTRE = 1 & COMPT <= 4 -> COMPT + (Vitesse 2,3,4,5)
  if (BP1A == 1 && BP1O == 0 && neutre == 1 && compteur <= 4)
  { compteur++ ; neutre = 1 ; }
  
// SI CONT HAUT = 1 & NEUTRE = 0 & COMPT = 0 -> COMPT = 2 (Vitesse 2)
  if (BP1A == 1 && BP1O == 0 && neutre == 0 && compteur == 0)
  { compteur=2 ; neutre = 1 ; }

  BP2A = digitalRead (3);
  
// SI CONT BAS = 1 & NEUTRE = 1 & COMPT >= 2 -> COMPT - (Vitesse 1,2,3,4)
  if(BP2A == 1 && BP2O == 0 && neutre == 1 && compteur >= 2)  
  { compteur-- ; neutre=1 ; }
  
// SI CONT BAS = 1 & NEUTRE = 0 & COMPT = 0 -> COMPT = 1 (Vitesse 1)
  if(BP2A == 1 && BP2O == 0 && neutre == 0 && compteur == 0)  
  { compteur=1 ; neutre=1 ; }

// SI NEUTRE > 1 SECONDE -> COMPTEUR = 0 & NEUTRE = 0  
  BP3A = digitalRead (4);
  if (BP3A == 1 && BP3O == 0)
  {
    currentTime = millis();
    // while = TANT QUE    
    while(millis()-currentTime < 1000)
    {
      if (digitalRead (4) == 0)
      {
        state = 1;
        break;
      }
    }
    if (state==1)
      state = 0;
//RESET COMPT & NEUTRE
    else
    {neutre = 0;compteur = 0;}
     
  }    */
 
  Serial.print("compteur : ");
  Serial.print(compteur);
  Serial.print("\t");
  Serial.print("neutre : ");
  Serial.println(neutre);
  
  /*
  BP1O = BP1A;
  BP2O = BP2A;
  BP3O = BP3A;
  */
  
// Update 7 segments via fonction segupdate: AFFICHAGE DES RAPPORTS
segupdate (); 
}
