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

/*// Signal du neutre: si neutre<=1 -> neutre=0 & compteur=0.  
  void ISR3()
{
  if (neutre <= 1)
  {
    neutre = 0;
    compteur = 0;
  }
  }*/
     
// Contacteur haut: si neutre>=0 & compteur<=1 -> Compteur=2, Sinon incremente compteur sauf si compteur=5.
  void ISR1()
  {
    if (neutre == 0 && compteur <= 1)
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

  PCintPort::attachInterrupt (12,ISR1,RISING);
  PCintPort::attachInterrupt (3,ISR2,RISING);
  /*PCintPort::attachInterrupt (4,ISR3,FALLING);*/
}

void loop() {

 int delay = 0;   //pour savoir si on est resté 1 seconde au neutre
  unsigned long int c_t = 0;  //pour stocker la valeur de millis pour faire le delay de 1 seconde
  int bp4 = 0;  //stocker la valeur du voyant
// SI NEUTRE > 0,5 SECONDE -> COMPTEUR = 0 & NEUTRE = 0  
bp4 = digitalRead(4);

if(bp4==0)     //A partir du moment où le signal est à l'état bas on lance le délai

{
  delay=1;  //On passe la variable de délai à 1, si elle reste à 1 pendant 
  
  c_t = millis(); //On stocke le temps actuel dans une variable

  while(millis()-c_t <= 500) //On lance la boucle de 1 seconde
    {
    bp4=digitalRead(4); //A chaque pas de temps on regarde l'état du voyant
    if(bp4==1)      // Si le signal est à l'état haut alors on casse la boucle
      
      {
      delay=0;
      break;          
      }
            
    }
     if (delay==0)
      delay = 1;
    
  else
    {neutre = 0;compteur = 0;}
  
}
  
  Serial.print("compteur : ");
  Serial.print(compteur);
  Serial.print("\t");
  Serial.print("neutre : ");
  Serial.println(neutre);
    
   
// Update 7 segments via fonction segupdate pour affichage des rapport en fonction du compteur.
segupdate (); 
}
