void sequence()
{
  if((actual_bouton2 == 1) && (previous_bouton2 == 0)) // Etat 1 -> 2
  {
    sequenceur2 = !sequenceur2;
  }

  if((actual_bouton3 == 1) && (previous_bouton3 == 0)) // Etat 1 -> 3
  {
    sequenceur3 = !sequenceur3;
  }
}

int whichState() // sequenceur,sequenceur2,sequenceur3,state
{
  sequence();
  
  if (!sequenceur)
    return 0;

  if(sequenceur && sequenceur2 && state != 3)
    return 2;

  if(sequenceur && sequenceur3 && state != 2)
    return 3;

  if(sequenceur && state != 2 && state !=3)
    return 1;
}

void state0()
{
    state = 0;
    sequenceur2=0;
    sequenceur3=0;
    
    Serial.println(state);
    delay(100);
    digitalWrite(LED,LOW);
    
    display.setBrightness(0);
    display.setSegments(tabSeg);

    
}

void loadButtons()
{
  actual_bouton1 = digitalRead(BUTTON_1);
  actual_bouton2 = digitalRead(BUTTON_2);
  actual_bouton3 = digitalRead(BUTTON_3);
}

void updateVar()
{
  previous_bouton1 = actual_bouton1;
  previous_bouton2 = actual_bouton2;
  previous_bouton3 = actual_bouton3;
}
