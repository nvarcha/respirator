void electrovalvula () {

if (ValveSensor == HIGH) {
  ValveOut = HIGH;
  delay(1000);
  }
 else{
  ValveOut = LOW;
 }
}
