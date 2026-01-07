// INCLUSÃO DE BIBLIOTECAS
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>

// SoftwareSerial HC05(5,6);
long NumPoder;
// DEFINIÇÕES DE PINOS
#define pinRx 10 //informe aqui qual porta Rx está sendo utilizada.
#define pinTx 11 //informe aqui qual porta Tx está sendo utilizada.
#define button 1 //definimos a saída digital 1 com o nome de Button.
#define button2 2 //definimos a saída digital 2 com o nome de Button2.
// DEFINIÇÕES
#define volumeMP3 25//definimos o volume, entre 0 e 30.
#define DEBUG
// INSTANCIANDO OBJETOS
SoftwareSerial playerMP3Serial(pinRx, pinTx);
DFRobotDFPlayerMini playerMP3;
// DECLARAÇÃO DE FUNÇÕES
// DECLARAÇÃO DE VARIÁVEIS
void setup() {
  Serial.begin(9600);
  playerMP3Serial.begin(9600);
  Serial.println();
  Serial.println(F("Iniciando DFPlayer ... (Espere 3~5 segundos)"));
  if (!playerMP3.begin(playerMP3Serial)) {  // COMUNICAÇÃO REALIZADA VIA SOFTWARE SERIAL
    Serial.println(F("Falha:"));
    Serial.println(F("1.conexões!"));
    Serial.println(F("2.cheque o cartão SD!"));
    while(true){
      delay(0);
    }
  }
  Serial.println(F("DFPlayer iniciado!"));
  playerMP3.volume(volumeMP3);

  #ifdef DEBUG
    Serial.println("o Setup acabou");
  #endif
  Wire.begin(8);
  Wire.onReceive(receberPoder);
}

int newPoder;

void receberPoder(int NumPoder){
  if (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  NumPoder = Wire.read();    // receive byte as an integer
  Serial.println(NumPoder);         // print the integer
  tocarMusica(NumPoder);
}
void tocarMusica(int NumPoder){
  Serial.println(NumPoder);
  playerMP3.playFolder(1, NumPoder);
  Serial.println("Tocando pasta 01, Audio: ");
  Serial.println(NumPoder);
}

void loop() {
  delay(100);
}