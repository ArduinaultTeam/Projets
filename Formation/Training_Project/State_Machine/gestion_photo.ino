int activate()  
{
  Photoresistance1 = analogRead(PHOTO_R1);
  Photoresistance2 = analogRead(PHOTO_R2);

  // Condition à respecter pour activer la LED
  if ( abs(Photoresistance1 - Photoresistance2) > 100) // abs pour avoir une valeur absolue, pour ne pas avoir de valeurs négative.
  {
    ActiveLED = 1;
  }
  else
  {
    ActiveLED = 0;
    digitalWrite(LED, LOW); //Eteint la LED
  }
return ActiveLED;
}




void affichage(int PR1, int PR2)
{
    Serial.print(Valeur1 + PR1);
    Serial.print("\t"); // Tabulation
    Serial.println(Valeur2 + PR2); //println : ln = Ligne Nouvelle
}

