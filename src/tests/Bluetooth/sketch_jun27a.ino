#include <SoftwareSerial.h>

SoftwareSerial btSerial(10, 11); // Pinos RX, TX do HC-05

String Arroz = "";

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial com o monitor serial
  btSerial.begin(38400); // Inicia a comunicação serial com o HC-05
}

void loop() {
  if (btSerial.available()>0){
    int receivedNumber = btSerial.read();
    Arroz = Arroz + receivedNumber;
    // Imprimir o número recebido
    Serial.println(receivedNumber);
    Serial.println(Arroz);
    if (Arroz == "1200"){
      Serial.println("Arroz");
      Arroz = "";
    }

  }
  
}