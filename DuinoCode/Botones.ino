void botones() {
  
  adc_key_in = analogRead(0);      // Leemos A0
    // Valores analogos de los botones:  0, 145, 329,507,743
    // se comparan y asi se obtiene el boton que se apreto
    
    if (adc_key_in > 900)  return btnNONE = true;     // Ningun boton pulsado 
    if (adc_key_in < 50)   return btnRIGHT = true; 
    if (adc_key_in < 250)  return btnUP = true;
    if (adc_key_in < 450)  return btnDOWN = true;
    if (adc_key_in < 650)  return btnLEFT= true;
    if (adc_key_in < 850)  return btnSELECT = true; 

    return btnNONE = true;  // Por si todo falla
}
