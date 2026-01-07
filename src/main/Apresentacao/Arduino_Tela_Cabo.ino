#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 100
#define MAXPRESSURE 1000

#include <Wire.h>

#include <SoftwareSerial.h>
SoftwareSerial HC05(50,51);

int NumPoder;
int Orientation = 1;
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 125, TS_RT =965, TS_TOP = 905, TS_BOT = 85;
//direita / Esquerda / Cima / baixo
//127 930 897 82
//deve ser 125 965 905 85

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

Adafruit_GFX_Button White_btn, Green_btn, Black_btn, Yellow_btn;

int pixel_x, pixel_y;
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PURPLE  0x780F

String symbol[4][4] = {
{ "7", "8", "9", " " },
{ "4", "5", "6", " " },
{ "1", "2", "3", " " },
{ "C", "0", "=", " " }
};
long int Num1,Number;
char action;
boolean result = false;

void DisplayResult()
{
tft.fillRect(0, 0, 240, 80, CYAN); //clear result box
tft.setCursor(10, 20);
tft.setTextSize(4);
tft.setTextColor(BLACK);
tft.println(Number); //update new value
}

void Tela_timer()
{
tft.setRotation(0);
const int TS_LEFT = 125, TS_RT =965, TS_TOP = 905, TS_BOT = 85;
//Draw the Result Box
tft.fillRect(0, 0, 240, 80, CYAN);
//Draw First Column
tft.fillRect (0,260,60,60,RED);
tft.fillRect (0,200,60,60,BLACK);
tft.fillRect (0,140,60,60,BLACK);
tft.fillRect (0,80,60,60,BLACK);
//Draw Third Column
tft.fillRect (120,260,60,60,GREEN);
tft.fillRect (120,200,60,60,BLACK);
tft.fillRect (120,140,60,60,BLACK);
tft.fillRect (120,80,60,60,BLACK);
//Draw Secound & Fourth Column
for (int b=260; b>=80; b-=60)
{ tft.fillRect (180,b,60,60,BLUE);
tft.fillRect (60,b,60,60,BLACK);}
//Draw Horizontal Lines
for (int h=80; h<=320; h+=60)
tft.drawFastHLine(0, h, 240, WHITE);
//Draw Vertical Lines
for (int v=0; v<=240; v+=60)
tft.drawFastVLine(v, 80, 240, WHITE);
//Display keypad lables
for (int j=0;j<4;j++) {
  for (int i=0;i<4;i++) {
    tft.setCursor(22 + (60*i), 100 + (60*j));
    tft.setTextSize(3);
    tft.setTextColor(WHITE);
    tft.println(symbol[j][i]);
  }
}

Num1 = 0;
//void DetectButtons(){
DisplayResult();
while (1) {
    tp = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    //if (tp.x > 0 && tp.x < 465  && tp.y > 445 && tp.y < 897){poderPurple(); break;} // Purple
    if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
    if (tp.x > 120 && tp.x < 300) {
        if (tp.y > 760 && tp.y < 900){
          delay(200);
          Serial.println("Button Clear");
          Number=Num1=0;
          DisplayResult();
          continue;}
        if (tp.y > 600 && tp.y < 760){
            delay(200);
            Serial.println ("Button 1");
            if (Number==0){Number=1;}
            else{Number = (Number*10) + 1;}//11
            DisplayResult();
            continue;
        }
        if (tp.y > 425 && tp.y < 600){
            Serial.println ("Button 4");
            delay(200);
            if (Number==0){Number=4;}
            else{Number = (Number*10) + 4;} //44
            DisplayResult();
            continue;
        }
        if (tp.y > 270 && tp.y < 425)
        {
        delay(200);
        Serial.println ("Button 7");
        if (Number==0){Number=7;}
        else{Number = (Number*10) + 7;}//77o
        DisplayResult();
        continue;
        }
      }
    if (tp.x >320 && tp.x<520){
        if (tp.y > 760 && tp.y < 900){
            delay(200);
            Serial.println ("Button 0");
            if (Number==0){Number=0;}
            else{Number = (Number*10) + 0;}
            DisplayResult();
            continue;
            
        }
        if (tp.y > 600 && tp.y < 760){
            delay(200);
            Serial.println ("Button 2");
            if (Number==0){Number=2;}
            else{Number = (Number*10) + 2;}
            DisplayResult();
            continue;  
        }
        if (tp.y > 425 && tp.y < 600){
            delay(200);
            Serial.println ("Button 5");
            if (Number==0){Number=5;}
            else{Number = (Number*10) + 5;}
            DisplayResult();
            continue;
        }
        if (tp.y > 270 && tp.y < 425){
            delay(200);
            Serial.println ("Button 8");
            if (Number==0){Number=8;}
            else{Number = (Number*10) + 8;}
            DisplayResult();
            continue;
        }
      }
    if (tp.x > 520 && tp.x < 720){
        if (tp.y > 760 && tp.y < 900){
            delay(200);
            Serial.println ("Button Enter");
            DisplayResult();
            Num1=Number;
            Serial.println(Num1);
            Tela_herois();
            break;
        }
        if (tp.y > 600 && tp.y < 760){
            delay(200);
            Serial.println ("Button 3");
            if (Number==0){Number=3;}
            else{Number = (Number*10) + 3;}
            DisplayResult();
            continue;
        }
        if (tp.y > 425 && tp.y < 600){
            delay(200);
            Serial.println ("Button 6");
            if (Number==0){Number=6;}
            else{Number = (Number*10) + 6;}
            DisplayResult();
            continue;
        }
        if (tp.y > 270 && tp.y < 425){
            delay(200);
            Serial.println ("Button 9");
            if (Number==0){Number=9;}
            else{Number = (Number*10) + 9;}
            DisplayResult();
            continue;
          }
        }
    if (tp.x > 720 && tp.x < 920){
        if(tp.y > 760 && tp.y < 900)
            {Serial.println ("Ação 1");
        }
        if(tp.y > 600 && tp.y < 760)
            {Serial.println ("Ação 2");}
        if(tp.y > 425 && tp.y < 600)
            {Serial.println ("Ação 3");}
        if(tp.y > 270 && tp.y < 425)
            {Serial.println ("Ação 4");
            if(HC05.available()){
            HC05.write((byte*)&Num1, sizeof(Num1));
            Serial.write(Num1);
            Tela_herois();
            }
          }
        }
    }
}

void Tela_herois(void){
    tft.setRotation(Orientation);
    tft.fillRect(0, 0, 160,120, YELLOW);
    tft.fillRect(0,120,160,120, GREEN);
    tft.fillRect(160,0, 160,120, BLUE);
    tft.fillRect(160,120, 160,120, PURPLE);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(50,55);
    tft.print("Poder1");
    tft.setCursor(210,55);
    tft.print("Poder2");
    tft.setCursor(55,175);
    tft.print("Poder3");
    tft.setCursor(210,175);
    tft.print("Poder4");
    while (1) {
      tp = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
        if (tp.x > 0 && tp.x < 465  && tp.y > 86 && tp.y < 445) {telaVerde();break;} // Verde
        if (tp.x > 0 && tp.x < 465  && tp.y > 445 && tp.y < 897){telaRoxo(); break;} // Purple
        if (tp.x > 465 && tp.x < 930  && tp.y > 86 && tp.y < 445){telaAmarelo(); break;} // amarelo
        if (tp.x > 465 && tp.x < 930  && tp.y > 445 && tp.y < 897){telaAzul(); break;} // BLue
      }
}

void telaVerde(void){
  tft.fillRect(0,0,160,240, GREEN);
  tft.fillRect(160,0,160,240, WHITE);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderVerde();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){coringa(); break;} // BLue
    }
}

void telaRoxo(void){
  tft.fillRect(0,0,160,240, PURPLE);
  tft.fillRect(160,0,160,240, WHITE);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderPurple();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){coringa(); break;} // BLue
    }
}

void telaAmarelo(void){
  tft.fillRect(0,0,160,240, YELLOW);
  tft.fillRect(160,0,160,240, WHITE);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderAmarelo();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){coringa(); break;} // BLue
    }
}

void telaAzul(void){
  tft.fillRect(0,0,160,240, BLUE);
  tft.fillRect(160,0,160,240, WHITE);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderBlue();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){coringa(); break;} // BLue
    }
}

void coringa(void){
  tft.fillScreen(WHITE);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumPoder = 1;
  // HC05.println(NumPoder);
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  }

void poderVerde(void){
  tft.fillScreen(GREEN);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumPoder = 2;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();

  //HC05.println(NumPoder);
  }
void poderAmarelo(void){
  tft.fillScreen(YELLOW);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumPoder = 3;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();

  //HC05.println(NumPoder);
  }
void poderBlue(void){
  tft.fillScreen(BLUE);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumPoder = 4;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  
  //HC05.println(NumPoder);
  }
void poderPurple(void){
  tft.fillScreen(PURPLE);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumPoder = 5;
  HC05.println(NumPoder);
  }

void setup(void)
{
    Wire.begin();
    Serial.begin(9600);
    //HC05.begin(38400);
    tft.reset();
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.fillScreen(WHITE);
    Tela_timer();

}

void loop(void)
{
  
  // if (HC05.available())
  // {
  //   while (HC05.available()){
  //       HC05.write(NumPoder);
  //       Serial.println(NumPoder);
  //   }
  //   HC05.flush();
  // }
}
