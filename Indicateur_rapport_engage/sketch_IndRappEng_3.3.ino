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

// Interruption:
  #include <PinChangeInt.h>

// Signal du neutre: si neutre<=1 -> neutre=0 & compteur=0.  
  void ISR3()
{
  if (neutre <= 1)
  {
    neutre = 0;
    compteur = 0;
  }
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
void setup() {
inithardware();  

  PCintPort::attachInterrupt (2,ISR1,RISING);
  PCintPort::attachInterrupt (3,ISR2,RISING);
  PCintPort::attachInterrupt (4,ISR3,RISING);
}

void loop() {
 
  Serial.print("compteur : ");
  Serial.print(compteur);
  Serial.print("\t");
  Serial.print("neutre : ");
  Serial.println(neutre);
   
// Update 7 segments via fonction segupdate pour affichage des rapport en fonction du compteur.
segupdate (); 
}
