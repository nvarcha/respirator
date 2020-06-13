void autocal () {

  while (inicio == false) {
     
    lcd.print("Calibrando por favor espere....");     // print a simple message
    
    digitalWrite(dirPin, LOW);

    for (int i = 0; i < 100 * stepsPerRevolution; i++) {

      digitalWrite(stepPin, HIGH);
      delayMicroseconds(freq);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(freq);
     
      //ZeroCal es la variable digital del sensor HALL que da HIGH cuando se activa.
      
      if (ZeroCal == HIGH) {
        inicio = true;
      }
    }
  }
}
