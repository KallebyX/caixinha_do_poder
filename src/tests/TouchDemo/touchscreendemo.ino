// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// This demo code returns raw readings, public domain

#include <stdint.h>
#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PURPLE  0x780F

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

String symbol[4][4] = {
{ "7", "8", "9", "/" },
{ "4", "5", "6", "*" },
{ "1", "2", "3", "-" },
{ "C", "0", "=", "+" }
};
long Num1,Number;
char action;
boolean result = false;

void Tela_timer()
{
tft.setRotation(0);
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

while(1){ // a point object holds x y and z coordinates
  TSPoint p = ts.getPoint();
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold) {
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
  }

  delay(100);}
}

void setup(void) {
  Serial.begin(9600);
  tft.reset();  
  uint16_t ID = tft.readID();
  tft.begin(ID);
  Tela_timer();
}


void loop(void) {

}