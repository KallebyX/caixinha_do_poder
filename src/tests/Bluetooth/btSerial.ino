#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"


SoftwareSerial btSerial(10, 11); // Pinos RX, TX do HC-05

String Arroz = "";

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial com o monitor serial
  btSerial.begin(38400); // Inicia a comunicação serial com o HC-05
}

void receberPoder(int NumPoder){
  if (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  NumPoder = Wire.read();    // receive byte as an integer
  Serial.println(NumPoder);         // print the integer
  tocarMusica(NumPoder);
}

void bt_receberPoder(int NumPoder){
  if(1 < btSerial.available()){
    char c = vtbtSerial.read();
    Serial.print(c);
  }
  NumPoder = Wire.read();    // receive byte as an integer
  Serial.println(NumPoder);         // print the integer
  tocarMusica(NumPoder);
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
