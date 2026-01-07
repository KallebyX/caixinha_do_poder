
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 100
#define MAXPRESSURE 1000

int Orientation = 1;
const int XP = 8, XM = A2, YP = A3, YM = 9; //ID=0x9341
const int TS_LEFT = 108, TS_RT =930, TS_TOP = 897, TS_BOT = 82;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

Adafruit_GFX_Button White_btn, Green_btn, Black_btn, Yellow_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
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
#define DARK_GREEN   0x03E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define MAROON  0x7800
#define WHITE   0xFFFF
#define PURPLE  0x780F

void show_tft(void){
    tft.fillRect(0, 0, 160,120, MAROON);
    tft.fillRect(0,120,160,120, DARK_GREEN);
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
        if (tp.x > 0 && tp.x < 465  && tp.y > 86 && tp.y < 445) {poderVerde();break;} // Verde
        if (tp.x > 0 && tp.x < 465  && tp.y > 445 && tp.y < 897){ poderPurple(); break;} // Purple
        if (tp.x > 465 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderAmarelo(); break;} // amarelo
        if (tp.x > 465 && tp.x < 930  && tp.y > 445 && tp.y < 897) {poderBlue(); break;}; // BLue
      }
  }
void poderVerde(void){
  tft.fillScreen(DARK_GREEN);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  }
void poderAmarelo(void){
  tft.fillScreen(MAROON);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  }
void poderBlue(void){
  tft.fillScreen(BLUE);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  }
void poderPurple(void){
  tft.fillScreen(PURPLE);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  }





void setup(void)
{
    Serial.begin(9600);
    tft.reset();  
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);         
    show_tft();
    //on_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "ON", 2);
    //off_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
    //on_btn.drawButton(false);
    //off_btn.drawButton(false);
    
}

/* two buttons are quite simple
 */
void loop(void)
{
   
}
