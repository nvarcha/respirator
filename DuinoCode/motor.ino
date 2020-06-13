void motor() {

  //El motor siempre debe girar CONTRA RELOJ.
  //EL RESPIRADOR NO FUNCIONA EN DIRECCION DE LAS AGUJAS DEL RELOJ
  //dirPin en LOW para CONTRA RELOJ
   
  if (inicio == true) {
    
  digitalWrite(dirPin, LOW);

    // Loop de for para hacer los pasos del motor

    for (int i = 0; i < 100 * stepsPerRevolution; i++) {

   lcd.setCursor(0,0);
   lcd.print("En Funcionamiento");

      // Estas 4 lineas hace 1 PASO
      // El delay da la frecuencia (rapidez) de giro del motor

      digitalWrite(stepPin, HIGH);    
      delayMicroseconds(freq);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(freq);
      electrovalvula();
    }
  }
}
