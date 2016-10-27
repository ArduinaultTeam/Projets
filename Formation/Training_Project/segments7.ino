void update_tab7seg (int centaine, int dizaine, int unite,int number)
{
    tabSeg[3] = display.encodeDigit(unite);
    tabSeg[2] = display.encodeDigit(dizaine);
    tabSeg[1] = display.encodeDigit(centaine);
    tabSeg[0] = display.encodeDigit(number);
}

void decompo_7seg(int value)
{
  unite = value %10;
  dizaine = ((value %100)- unite)/10;
  centaine = (value - value %100)/100; 
}

void affichage_7seg()
{
     decompo_7seg(Photoresistance1);
     update_tab7seg (centaine,dizaine,unite,1);

    display.setSegments(tabSeg);

    delay(1000);
    
    decompo_7seg(Photoresistance2);
    update_tab7seg(centaine,dizaine,unite,2);

    display.setSegments(tabSeg);
}

