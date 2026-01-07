#include <SoftwareSerial.h>
SoftwareSerial BTSerial(0, 1);
void setup() {
  Serial.begin(9600);
  BTSerial.begin(38400);
}
void loop() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    Serial.print("Dados recebidos: ");
    Serial.println(data);
  }
}