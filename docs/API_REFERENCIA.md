# Referência da API - Caixinha do Poder

## Documentação Técnica do Código

Este documento fornece uma referência completa das funções, variáveis e estruturas utilizadas no projeto.

---

## Arquivos do Projeto

| Arquivo | Descrição | Tipo |
|---------|-----------|------|
| `Apresentaçao.ino` | Código principal (Display + Controle) | Main |
| `Audio_Cabo.ino` | Controlador de áudio (I2C Slave) | Module |
| `Arduino_Tela_Cabo.ino` | Versão alternativa simplificada | Alt |
| `Tela_pronta1.1.ino` | Versão básica dos poderes | Alt |
| `Tela_Teclado.ino` | Interface de calculadora | Test |
| `Teste_ImagemSD.ino` | Teste de imagens BMP | Test |
| `touchscreendemo.ino` | Demonstração do touchscreen | Test |
| `btSerial.ino` | Teste de Bluetooth | Test |

---

## Apresentaçao.ino - Código Principal

### Bibliotecas Utilizadas

```cpp
#include <Adafruit_GFX.h>      // Biblioteca gráfica base
#include <MCUFRIEND_kbv.h>     // Driver específico do display
#include <TouchScreen.h>       // Entrada touchscreen
#include <Wire.h>              // Comunicação I2C
#include <SoftwareSerial.h>    // Serial por software (Bluetooth)
#include <SPI.h>               // Interface SPI
#include <SdFat.h>             // Sistema de arquivos SD (otimizado)
```

### Constantes de Configuração

```cpp
// Touch Screen
#define MINPRESSURE 100        // Pressão mínima para detectar toque
#define MAXPRESSURE 1000       // Pressão máxima válida

// Pinos do Touch
const int XP = 8;              // X+
const int XM = A2;             // X-
const int YP = A3;             // Y+
const int YM = 9;              // Y-

// Calibração do Touch
const int TS_LEFT = 125;       // Borda esquerda
const int TS_RT = 965;         // Borda direita
const int TS_TOP = 905;        // Borda superior
const int TS_BOT = 85;         // Borda inferior

// SD Card
#define SD_CS 10               // Chip Select do SD
#define PALETTEDEPTH 8         // Profundidade de paleta (256 cores)
#define BUFFPIXEL 20           // Buffer de pixels para BMP
```

### Definições de Cores (RGB565)

```cpp
#define BLACK   0x0000         // Preto
#define BLUE    0x001F         // Azul
#define RED     0xF800         // Vermelho
#define GREEN   0x07E0         // Verde (NOTA: código original tem erro)
#define CYAN    0x07FF         // Ciano
#define MAGENTA 0xF81F         // Magenta
#define YELLOW  0xFFE0         // Amarelo
#define WHITE   0xFFFF         // Branco
#define PURPLE  0x780F         // Roxo
```

### Variáveis Globais

```cpp
MCUFRIEND_kbv tft;             // Objeto do display TFT
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  // Touch com 300Ω
TSPoint tp;                    // Ponto de toque atual

int NumPoder;                  // Código do poder atual
int Orientation = 1;           // Orientação do display (1=landscape)
int pixel_x, pixel_y;          // Coordenadas do toque mapeadas

long int Num1, Number;         // Valores do teclado numérico
long int NumT = 1;             // Contador do timer

SoftwareSerial HC05(50, 51);   // Bluetooth nos pinos 50(RX), 51(TX)
SdFatSoftSpi<12, 11, 13> SD;   // SD com bit-bang SPI

char namebuf[32] = "/";        // Buffer para nomes de arquivo
File root;                     // Diretório raiz do SD
int pathlen;                   // Comprimento do path
```

---

## Funções Principais

### Touch_getXY()

```cpp
/**
 * @brief Lê as coordenadas do touchscreen e mapeia para pixels
 * @return true se houver toque válido, false caso contrário
 *
 * @details
 * - Lê o ponto de toque bruto
 * - Restaura pinos compartilhados com TFT
 * - Valida a pressão (MINPRESSURE < z < MAXPRESSURE)
 * - Mapeia coordenadas brutas para pixels do display
 *
 * @example
 * if (Touch_getXY()) {
 *     Serial.print("Toque em: ");
 *     Serial.print(pixel_x);
 *     Serial.print(", ");
 *     Serial.println(pixel_y);
 * }
 */
bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}
```

### DisplayResult()

```cpp
/**
 * @brief Atualiza o visor do teclado numérico
 *
 * @details
 * - Limpa a área do visor (retângulo ciano)
 * - Exibe o valor atual de 'Number'
 * - Usa fonte tamanho 4, cor preta
 */
void DisplayResult() {
    tft.fillRect(0, 0, 240, 80, CYAN);
    tft.setCursor(10, 20);
    tft.setTextSize(4);
    tft.setTextColor(BLACK);
    tft.println(Number);
}
```

### Tela_timer()

```cpp
/**
 * @brief Exibe e gerencia o teclado numérico
 *
 * @details
 * - Desenha grid 4x4 com números 0-9, C (clear), = (enter)
 * - Loop infinito aguardando toques
 * - Atualiza 'Number' conforme teclas pressionadas
 * - Ao pressionar '=', chama Tela_herois()
 *
 * Layout:
 * [7][8][9][ ]
 * [4][5][6][ ]
 * [1][2][3][ ]
 * [C][0][=][ ]
 */
void Tela_timer();
```

### Tela_herois()

```cpp
/**
 * @brief Exibe a tela de seleção dos 4 poderes
 *
 * @details
 * - Divide a tela em 4 quadrantes coloridos
 * - Envia código 1 via I2C para som inicial
 * - Detecta qual quadrante foi tocado
 * - Chama a função de tela correspondente
 *
 * Layout:
 * [VISÃO - Ciano   ][INVISÍVEL - Branco]
 * [VELOZ - Branco  ][FORTE - Ciano     ]
 *
 * Mapeamento:
 * - Superior esquerdo → telaAmarelo() (Visão)
 * - Superior direito  → telaAzul() (Invisibilidade)
 * - Inferior esquerdo → telaVerde() (Velocidade)
 * - Inferior direito  → telaRoxo() (Super Força)
 */
void Tela_herois(void);
```

### telaAmarelo() / telaAzul() / telaVerde() / telaRoxo()

```cpp
/**
 * @brief Exibe a tela de seleção de personagem para o poder
 * @param Cada função é específica para um poder
 *
 * @details
 * - Envia códigos I2C para áudio de apresentação
 * - Carrega imagem BMP do poder (ex: Visao2.bmp)
 * - Aguarda toque para seleção M/F
 * - Superior = Masculino, Inferior = Feminino
 *
 * Códigos I2C enviados (exemplo Amarelo/Visão):
 * - 12: Apresentação do poder
 * - 11: Poder selecionado
 */
void telaAmarelo(void);
void telaAzul(void);
void telaVerde(void);
void telaRoxo(void);
```

### poderAmarelo() / poderAmareloF() / etc.

```cpp
/**
 * @brief Ativa o poder e executa countdown
 * @details
 * - Carrega imagem do personagem (M ou F)
 * - Exibe texto "Aplicando"
 * - Executa countdown: NumT = Num1 * 60 segundos
 * - Ao final, envia código de desativação via I2C
 *
 * @note BUG: O código de desativação está dentro do loop
 *       onde NumT > 0, então nunca é executado
 *
 * Funções M (Masculino):
 * - poderAmarelo()  → VisaoM.bmp, código 15
 * - poderBlue()     → InvisibilidadeM.bmp, código 25
 * - poderVerde()    → SuperVelocM.bmp, código 35
 * - poderPurple()   → SuperF_M.bmp, código 45
 *
 * Funções F (Feminino):
 * - poderAmareloF() → VisaoF.bmp
 * - poderAzulF()    → InvisibilidadeF.bmp
 * - poderVerdeF()   → SuperVelocF.bmp
 * - poderPurpleF()  → SuperF_F.bmp
 */
void poderAmarelo(void);
void poderAmareloF(void);
// ... etc
```

### showBMP()

```cpp
/**
 * @brief Carrega e exibe uma imagem BMP do cartão SD
 *
 * @param nm Nome do arquivo (ex: "Visao2.bmp")
 * @param x Coordenada X inicial
 * @param y Coordenada Y inicial
 * @return uint8_t Código de retorno:
 *         0 = sucesso
 *         1 = fora da tela
 *         2 = ID BMP inválido
 *         3 = muitos planos
 *         4 = formato não suportado
 *         5 = profundidade de paleta inválida
 *
 * @details
 * - Suporta BMP 1, 4, 8, 16, 24 bits
 * - Suporta formato 565 comprimido
 * - Lê arquivo linha por linha para economizar RAM
 * - Converte paleta para RGB565
 *
 * @example
 * uint8_t result = showBMP("hero.bmp", 0, 0);
 * if (result != 0) {
 *     Serial.print("Erro ao carregar BMP: ");
 *     Serial.println(result);
 * }
 */
uint8_t showBMP(char *nm, int x, int y);
```

### Funções Auxiliares de BMP

```cpp
/**
 * @brief Lê valor de 16 bits little-endian do arquivo
 */
uint16_t read16(File& f);

/**
 * @brief Lê valor de 32 bits little-endian do arquivo
 */
uint32_t read32(File& f);
```

---

## Audio_Cabo.ino - Controlador de Áudio

### Configuração

```cpp
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>

#define pinRx 10               // RX do DFPlayer
#define pinTx 11               // TX do DFPlayer
#define volumeMP3 25           // Volume (0-30)

SoftwareSerial playerMP3Serial(pinRx, pinTx);
DFRobotDFPlayerMini playerMP3;
```

### Funções

```cpp
/**
 * @brief Callback chamado quando dados I2C são recebidos
 * @param NumPoder Número de bytes recebidos (não usado)
 *
 * @details
 * - Lê o código do poder do buffer I2C
 * - Chama tocarMusica() com o código
 */
void receberPoder(int NumPoder) {
    NumPoder = Wire.read();
    Serial.println(NumPoder);
    tocarMusica(NumPoder);
}

/**
 * @brief Reproduz o áudio correspondente ao código do poder
 * @param NumPoder Código do poder/áudio
 *
 * @details
 * - Usa playFolder(1, NumPoder) para tocar
 * - Arquivos devem estar em /01/XXX.mp3
 * - XXX = NumPoder com 3 dígitos (ex: 011.mp3)
 */
void tocarMusica(int NumPoder) {
    playerMP3.playFolder(1, NumPoder);
}
```

---

## Mapa de Códigos I2C

### Tabela de Códigos

| Código | Poder | Ação | Áudio |
|--------|-------|------|-------|
| 1 | Geral | Inicialização | 001.mp3 |
| 10 | Visão | Intro | 010.mp3 |
| 11 | Visão | Ativo | 011.mp3 |
| 12 | Visão | Efeito | 012.mp3 |
| 15 | Visão | Desativar | 015.mp3 |
| 20 | Invisibilidade | Intro | 020.mp3 |
| 21 | Invisibilidade | Ativo | 021.mp3 |
| 23 | Invisibilidade | Efeito | 023.mp3 |
| 25 | Invisibilidade | Desativar | 025.mp3 |
| 30 | Velocidade | Intro | 030.mp3 |
| 31 | Velocidade | Ativo | 031.mp3 |
| 33 | Velocidade | Efeito | 033.mp3 |
| 35 | Velocidade | Desativar | 035.mp3 |
| 40 | Super Força | Intro | 040.mp3 |
| 41 | Super Força | Ativo | 041.mp3 |
| 43 | Super Força | Efeito | 043.mp3 |
| 45 | Super Força | Desativar | 045.mp3 |

---

## Fluxo de Execução

```
┌────────────────────────────────────────────────────────────────┐
│                      FLUXOGRAMA DO CÓDIGO                      │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│  setup()                                                       │
│     │                                                          │
│     ├──► Wire.begin()           // Inicia I2C como master      │
│     ├──► Serial.begin(9600)     // Debug serial                │
│     ├──► tft.reset()            // Reset do display            │
│     ├──► tft.begin(ID)          // Inicia display              │
│     ├──► SD.begin(SD_CS)        // Inicia cartão SD            │
│     └──► Tela_timer()           // Vai para tela inicial       │
│                                                                │
│  Tela_timer()                                                  │
│     │                                                          │
│     └──► Loop: Detecta toques                                  │
│            │                                                   │
│            ├──► Tecla 0-9: Number = Number*10 + tecla          │
│            ├──► Tecla C: Number = 0                            │
│            └──► Tecla =: Num1 = Number → Tela_herois()         │
│                                                                │
│  Tela_herois()                                                 │
│     │                                                          │
│     ├──► Wire.write(1)          // Som de início               │
│     ├──► Desenha 4 quadrantes                                  │
│     └──► Loop: Detecta qual quadrante                          │
│            │                                                   │
│            ├──► Amarelo → telaAmarelo()                        │
│            ├──► Azul    → telaAzul()                           │
│            ├──► Verde   → telaVerde()                          │
│            └──► Roxo    → telaRoxo()                           │
│                                                                │
│  telaXXX() [Amarelo/Azul/Verde/Roxo]                           │
│     │                                                          │
│     ├──► Wire.write(código)     // Som de apresentação         │
│     ├──► showBMP("imagem.bmp")  // Carrega imagem              │
│     └──► Loop: Detecta M ou F                                  │
│            │                                                   │
│            ├──► Superior → poderXXX()   (Masculino)            │
│            └──► Inferior → poderXXXF()  (Feminino)             │
│                                                                │
│  poderXXX() / poderXXXF()                                      │
│     │                                                          │
│     ├──► showBMP("imagemM/F.bmp")                              │
│     ├──► Exibe "Aplicando"                                     │
│     └──► for(NumT = Num1*60; NumT > 0; NumT--)                 │
│            │                                                   │
│            ├──► delay(1000)                                    │
│            └──► Serial.print(NumT)                             │
│                                                                │
│  loop()                                                        │
│     │                                                          │
│     └──► (vazio - todo controle está em Tela_timer)            │
│                                                                │
└────────────────────────────────────────────────────────────────┘
```

---

## Exemplo de Código Corrigido

### Correção do Bug do Timer

```cpp
// VERSÃO CORRIGIDA de poderAmarelo()
void poderAmarelo(void) {
    showBMP("VisaoM.bmp", 0, 0);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.setCursor(95, 100);
    tft.print("Aplicando");

    NumT = Num1 * 60;

    for(; NumT > 0; NumT--) {
        Serial.print(NumT);
        delay(1000);

        // Atualizar display do timer
        tft.fillRect(150, 140, 80, 30, WHITE);
        tft.setCursor(150, 140);
        tft.print(NumT);
        tft.print("s");
    }

    // CORREÇÃO: Enviar código de desativação APÓS o loop
    NumPoder = 15;
    Wire.beginTransmission(8);
    Wire.write(NumPoder);
    Wire.endTransmission();

    delay(2000);  // Aguardar áudio de desativação

    // Voltar para tela inicial
    Tela_timer();
}
```

---

## Extensão: Adicionando Novo Poder

```cpp
// Exemplo: Adicionar poder de VOO (código 5x)

// 1. Adicionar cor
#define SKYBLUE 0x867D

// 2. Criar função de tela
void telaCeu(void) {
    NumPoder = 53;
    Wire.beginTransmission(8);
    Wire.write(NumPoder);
    Wire.endTransmission();

    NumPoder = 51;
    Wire.beginTransmission(8);
    Wire.write(NumPoder);
    Wire.endTransmission();

    showBMP("Voo2.bmp", 0, 0);
    delay(1000);

    while (1) {
        tp = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
        if (tp.x > 0 && tp.x < 930 && tp.y > 86 && tp.y < 445) {
            poderVoo();
            break;
        }
        if (tp.x > 0 && tp.x < 930 && tp.y > 445 && tp.y < 897) {
            poderVooF();
            break;
        }
    }
}

// 3. Criar função do poder
void poderVoo(void) {
    showBMP("VooM.bmp", 0, 0);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.setCursor(95, 100);
    tft.print("Voando!");

    NumT = Num1 * 60;
    for(; NumT > 0; NumT--) {
        delay(1000);
    }

    NumPoder = 55;
    Wire.beginTransmission(8);
    Wire.write(NumPoder);
    Wire.endTransmission();

    Tela_timer();
}

// 4. Adicionar arquivos de áudio ao SD do DFPlayer:
// /01/050.mp3 (intro)
// /01/051.mp3 (ativo)
// /01/053.mp3 (efeito)
// /01/055.mp3 (desativar)

// 5. Adicionar imagens ao SD do display:
// Voo2.bmp, VooM.bmp, VooF.bmp
```

---

## Referências

- [Adafruit GFX Library](https://learn.adafruit.com/adafruit-gfx-graphics-library)
- [MCUFRIEND_kbv Library](https://github.com/prenticedavid/MCUFRIEND_kbv)
- [DFRobot DFPlayer Mini Wiki](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299)
- [Arduino Wire Library](https://www.arduino.cc/en/Reference/Wire)
- [SdFat Library](https://github.com/greiman/SdFat)
