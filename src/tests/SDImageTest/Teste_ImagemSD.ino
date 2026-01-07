#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 100
#define MAXPRESSURE 1000

//CAbo
#include <Wire.h>


//BLuethooth -Fail
#include <SoftwareSerial.h>
SoftwareSerial HC05(50,51);

//Imagem SD
#include <SPI.h>             // f.k. for Arduino-1.5.2
#define USE_SDFAT
#include <SdFat.h>           // Use the SdFat library
SdFatSoftSpi<12, 11, 13> SD; //Bit-Bang on the Shield pins

#define SD_CS     10
#define NAMEMATCH ""         // "" matches any name
//#define NAMEMATCH "tiger"    // *tiger*.bmp
#define PALETTEDEPTH   8     // support 256-colour Palette

char namebuf[32] = "/";   //BMP files in root directory
//char namebuf[32] = "/bitmaps/";  //BMP directory e.g. files in /bitmaps/*.bmp

File root;
int pathlen;

//Fim imagem

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
#define GREEN   0x07E01
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PURPLE  0x780F
#define ORANGE  

String symbol[4][4] = {
{ "7", "8", "9", " " },
{ "4", "5", "6", " " },
{ "1", "2", "3", " " },
{ "C", "0", "=", " " }
};
long int Num1,Number;
long int NumT;
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
    NumPoder = 1;
    Serial.println(NumPoder);
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(NumPoder);
    Wire.endTransmission();
    tft.setRotation(Orientation);
    tft.fillRect(0, 0, 160,120, CYAN);
    tft.fillRect(0,120,160,120, WHITE);
    tft.fillRect(160,0, 160,120, WHITE);
    tft.fillRect(160,120, 160,120, CYAN);
    // tft.fillRect (0,0,60,60,RED);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.setCursor(50,55);
    tft.print("Visao");
    tft.setCursor(195,55);
    tft.print("Invisivel");
    tft.setCursor(55,175);
    tft.print("Veloz");
    tft.setCursor(210,175);
    tft.print("Forte");
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

void telaAmarelo(void){
  // tft.fillRect(0,0,160,240, YELLOW);
  // tft.fillRect(160,0,160,240, WHITE);
  NumPoder = 12;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  delay(100);
  showBMP("Visao2.bmp",0,0);
  NumPoder = 11;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  showBMP("Visao2.bmp",0,0);
  delay(1000);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderAmarelo();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){poderAmareloF(); break;} // BLue
    }
}

void telaAzul(void){
  // tft.fillRect(0,0,160,240, BLUE);
  // tft.fillRect(160,0,160,240, WHITE);
  NumPoder = 23;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  showBMP("Invisibilidade2.bmp",0,0);
  delay(1700);
  NumPoder = 21;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  showBMP("Invisibilidade2.bmp",0,0);
  delay(1000);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderBlue();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){poderAzulF(); break;} // BLue
    }
}

void telaVerde(void){
  // NumPoder = 33;
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  // showBMP("Velocidade2.bmp",0,0);
  NumPoder = 31;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  showBMP("Velocidade2.bmp",0,0);
  delay(1000);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderVerde();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){poderVerdeF(); break;} // BLue
    }
}

void telaRoxo(void){
  // tft.fillRect(0,0,160,240, PURPLE);
  // tft.fillRect(160,0,160,240, WHITE);
  NumPoder = 43;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  showBMP("SuperF2.bmp",0,0);
  NumPoder = 41;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(NumPoder);
  Wire.endTransmission();
  showBMP("SuperF2.bmp",0,0);
  delay(1000);
  while (1) {
    tp = ts.getPoint();
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);
      if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
      if (tp.x > 0 && tp.x < 930  && tp.y > 86 && tp.y < 445) {poderPurple();break;} // Verde
      if (tp.x > 0 && tp.x < 930  && tp.y > 445 && tp.y < 897){poderPurpleF(); break;} // BLue
    }
}


void poderAmarelo(void){
  // tft.fillScreen(YELLOW);
  showBMP("VisaoM.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=15;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 11;
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();

  //HC05.println(NumPoder);
  }

void poderAmareloF(void){
  // tft.fillScreen(WHITE);
  showBMP("VisaoF.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=15;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
}
  // NumPoder = 11;
  // // HC05.println(NumPoder);
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();

void poderBlue(void){
  // tft.fillScreen(BLUE);
  showBMP("InvisibilidadeM.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=25;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 21;
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  
  //HC05.println(NumPoder);
  }
void poderAzulF(void){
  // tft.fillScreen(WHITE);
  showBMP("InvisibilidadeF.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=25;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 21;
  // // HC05.println(NumPoder);
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  }

void poderVerde(void){
  showBMP("SuperVelocM.bmp",0,0);
  // tft.fillScreen(GREEN);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=35;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 31;
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  //HC05.println(NumPoder);
  }

void poderVerdeF(void){
  // tft.fillScreen(WHITE);
  showBMP("SuperVelocF.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=35;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 31;
  // // HC05.println(NumPoder);
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  }
  
void poderPurple(void){
  // tft.fillScreen(PURPLE);
  showBMP("SuperF_M.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=45;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 41;
  // // HC05.println(NumPoder);
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  //HC05.println(NumPoder);
  }

void poderPurpleF(void){
  // tft.fillScreen(WHITE);
  showBMP("SuperF_F.bmp",0,0);
  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(95,100);
  tft.print("Aplicando");
  NumT = Num1 * 60;
  for(NumT; NumT>=0;NumT--){
    Serial.print(NumT);
    delay(1000);
    if(NumT == 0){
      NumPoder=45;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write(NumPoder);
      Wire.endTransmission();
    }
  }
  // NumPoder = 41;
  // // HC05.println(NumPoder);
  // Wire.beginTransmission(8); // transmit to device #8
  // Wire.write(NumPoder);
  // Wire.endTransmission();
  }

void setup(void)
{
    Wire.begin();
    Serial.begin(9600);
    //HC05.begin(38400);
    tft.reset();
    uint16_t ID = tft.readID();
    tft.begin(ID);
    bool good = SD.begin(SD_CS);
    if (!good) {
        Serial.print(F("cannot start SD"));
        while (1);
    }
    root = SD.open(namebuf);
    pathlen = strlen(namebuf);
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

#define BMPIMAGEOFFSET 54

#define BUFFPIXEL      20

uint16_t read16(File& f) {
    uint16_t result;         // read little-endian
    f.read(&result, sizeof(result));
    return result;
}

uint32_t read32(File& f) {
    uint32_t result;
    f.read(&result, sizeof(result));
    return result;
}

uint8_t showBMP(char *nm, int x, int y)
{
    File bmpFile;
    int bmpWidth, bmpHeight;    // W+H in pixels
    uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
    uint32_t bmpImageoffset;    // Start of image data in file
    uint32_t rowSize;           // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], *palette = NULL;
    uint8_t bitmask, bitshift;
    boolean flip = true;        // BMP is stored bottom-to-top
    int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
    uint32_t pos;               // seek position
    boolean is565 = false;      //

    uint16_t bmpID;
    uint16_t n;                 // blocks read
    uint8_t ret;

    if ((x >= tft.width()) || (y >= tft.height()))
        return 1;               // off screen

    bmpFile = SD.open(nm);      // Parse BMP header
    bmpID = read16(bmpFile);    // BMP signature
    (void) read32(bmpFile);     // Read & ignore file size
    (void) read32(bmpFile);     // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    (void) read32(bmpFile);     // Read & ignore DIB header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    n = read16(bmpFile);        // # planes -- must be '1'
    bmpDepth = read16(bmpFile); // bits per pixel
    pos = read32(bmpFile);      // format
    if (bmpID != 0x4D42) ret = 2; // bad ID
    else if (n != 1) ret = 3;   // too many planes
    else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
    else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette 
    else {
        bool first = true;
        is565 = (pos == 3);               // ?already in 16-bit format
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
        if (bmpHeight < 0) {              // If negative, image is in top-down order.
            bmpHeight = -bmpHeight;
            flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w) >= tft.width())       // Crop area to be loaded
            w = tft.width() - x;
        if ((y + h) >= tft.height())      //
            h = tft.height() - y;

        if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
            //bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
            bmpFile.seek(bmpImageoffset - (4<<bmpDepth)); //54 for regular, diff for colorsimportant
            bitmask = 0xFF;
            if (bmpDepth < 8)
                bitmask >>= bmpDepth;
            bitshift = 8 - bmpDepth;
            n = 1 << bmpDepth;
            lcdbufsiz -= n;
            palette = lcdbuffer + lcdbufsiz;
            for (col = 0; col < n; col++) {
                pos = read32(bmpFile);    //map palette to 5-6-5
                palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
            }
        }

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
        for (row = 0; row < h; row++) { // For each scanline...
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            uint8_t r, g, b, *sdptr;
            int lcdidx, lcdleft;
            if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else        // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
            if (bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
            }

            for (col = 0; col < w; ) {  //pixels in row
                lcdleft = w - col;
                if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                    uint16_t color;
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(sdbuffer)) { // Indeed
                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
                        buffidx = 0; // Set index to beginning
                        r = 0;
                    }
                    switch (bmpDepth) {          // Convert pixel from BMP to TFT format
                        case 24:
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            color = tft.color565(r, g, b);
                            break;
                        case 16:
                            b = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            if (is565)
                                color = (r << 8) | (b);
                            else
                                color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                            break;
                        case 1:
                        case 4:
                        case 8:
                            if (r == 0)
                                b = sdbuffer[buffidx++], r = 8;
                            color = palette[(b >> bitshift) & bitmask];
                            r -= bmpDepth;
                            b <<= bmpDepth;
                            break;
                    }
                    lcdbuffer[lcdidx] = color;

                }
                tft.pushColors(lcdbuffer, lcdidx, first);
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}