int activate()  
{
  Photoresistance1 = analogRead(PHOTO_R1);
  Photoresistance2 = analogRead(PHOTO_R2);

  // Condition à respecter pour activer la LED
  if ( abs(Photoresistance1 - Photoresistance2) > v_reglage_photo) // abs pour avoir une valeur absolue, pour ne pas avoir de valeurs négative.
  {
    ActiveLED = 1;
  }
  else
  {
    ActiveLED = 0;
    digitalWrite(LED, LOW); //Eteint la LED
  }
  Serial.println(ActiveLED);
return ActiveLED;
}




void affichage(int PR1, int PR2)
{
    Serial.print(Valeur1 + PR1);
    Serial.print("\t"); // Tabulation
    Serial.println(Valeur2 + PR2); //println : ln = Ligne Nouvelle

    
}

void affichage_lcd()
{

  Photoresistance1 = analogRead(PHOTO_R1);
  Photoresistance2 = analogRead(PHOTO_R2);
  
  lcd.setCursor(0, 0);

  String Photo1 = "Photo 1 : ";
  String Photo2 = "Photo 2 : ";

  lcd.print(Photo1 + Photoresistance1);

  lcd.setCursor(0, 1);

  lcd.print(Photo2 + Photoresistance2);
}

void affichage_lcd_2lines(String line1, String line2)
{
  lcd.setCursor(0, 0);

  lcd.print(line1);

  lcd.setCursor(0, 1);

  lcd.print(line2);
}


