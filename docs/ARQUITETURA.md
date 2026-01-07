# Arquitetura do Sistema - Caixinha do Poder

## Visão Geral da Arquitetura

A Caixinha do Poder utiliza uma arquitetura distribuída de dois microcontroladores comunicando-se via I2C, seguindo o padrão Master-Slave.

---

## Diagrama de Arquitetura Geral

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          CAIXINHA DO PODER - ARQUITETURA                    │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐   │
│  │                      CAMADA DE APRESENTAÇÃO                          │   │
│  │  ┌───────────────────┐    ┌────────────────────┐                    │   │
│  │  │   Display TFT     │    │    Touchscreen     │                    │   │
│  │  │    320x240        │    │    Resistivo       │                    │   │
│  │  │   (ILI9341)       │    │                    │                    │   │
│  │  └─────────┬─────────┘    └──────────┬─────────┘                    │   │
│  │            │                         │                               │   │
│  │            └────────────┬────────────┘                               │   │
│  │                         │                                            │   │
│  └─────────────────────────┼────────────────────────────────────────────┘   │
│                            ▼                                                 │
│  ┌─────────────────────────────────────────────────────────────────────┐   │
│  │                      CAMADA DE CONTROLE                              │   │
│  │                                                                      │   │
│  │  ┌─────────────────────────────────────────────────────────────┐    │   │
│  │  │                    ARDUINO MEGA 2560                         │    │   │
│  │  │                      (MASTER)                                │    │   │
│  │  │                                                              │    │   │
│  │  │  ┌────────────┐  ┌─────────────┐  ┌─────────────────────┐   │    │   │
│  │  │  │   Timer    │  │   Estado    │  │   Lógica de         │   │    │   │
│  │  │  │  Manager   │  │   Machine   │  │   Poderes           │   │    │   │
│  │  │  └────────────┘  └─────────────┘  └─────────────────────┘   │    │   │
│  │  │                                                              │    │   │
│  │  └──────────────────────────┬──────────────────────────────────┘    │   │
│  │                             │                                        │   │
│  └─────────────────────────────┼────────────────────────────────────────┘   │
│                                │ I2C (Wire)                                 │
│                                ▼                                            │
│  ┌─────────────────────────────────────────────────────────────────────┐   │
│  │                       CAMADA DE ÁUDIO                                │   │
│  │                                                                      │   │
│  │  ┌─────────────────────────────────────────────────────────────┐    │   │
│  │  │                     ARDUINO UNO                              │    │   │
│  │  │                      (SLAVE)                                 │    │   │
│  │  │                                                              │    │   │
│  │  │  ┌────────────────────┐    ┌──────────────────────────┐     │    │   │
│  │  │  │   I2C Receiver     │───►│   Audio Controller       │     │    │   │
│  │  │  │   (Address: 8)     │    │   (DFPlayer Mini)        │     │    │   │
│  │  │  └────────────────────┘    └────────────┬─────────────┘     │    │   │
│  │  │                                         │                    │    │   │
│  │  └─────────────────────────────────────────┼────────────────────┘    │   │
│  │                                            ▼                          │   │
│  │  ┌─────────────────────────────────────────────────────────────┐    │   │
│  │  │                    ALTO-FALANTE                              │    │   │
│  │  └─────────────────────────────────────────────────────────────┘    │   │
│  │                                                                      │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
│  ┌─────────────────────────────────────────────────────────────────────┐   │
│  │                     CAMADA DE ARMAZENAMENTO                          │   │
│  │                                                                      │   │
│  │  ┌─────────────────────┐         ┌─────────────────────┐            │   │
│  │  │   SD Card           │         │   SD Card           │            │   │
│  │  │   (Imagens BMP)     │         │   (Áudios MP3)      │            │   │
│  │  │   SPI Interface     │         │   DFPlayer Native   │            │   │
│  │  └─────────────────────┘         └─────────────────────┘            │   │
│  │                                                                      │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Componentes do Sistema

### 1. Arduino Mega 2560 (Master)

O Arduino Mega atua como controlador principal do sistema.

#### Responsabilidades:
- Gerenciamento da interface touchscreen
- Renderização de gráficos no display TFT
- Leitura de imagens BMP do cartão SD
- Controle da máquina de estados
- Gerenciamento de timers
- Comunicação I2C com o slave

#### Bibliotecas Utilizadas:

```cpp
#include <Adafruit_GFX.h>      // Primitivas gráficas
#include <MCUFRIEND_kbv.h>     // Driver do display
#include <TouchScreen.h>       // Entrada touch
#include <Wire.h>              // Comunicação I2C
#include <SdFat.h>             // Sistema de arquivos SD
#include <SPI.h>               // Interface SPI
#include <SoftwareSerial.h>    // Serial para Bluetooth
```

#### Mapa de Pinos:

```
┌─────────────────────────────────────────────────────────────┐
│                    ARDUINO MEGA 2560                        │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  DISPLAY TFT (Shield)        TOUCH SCREEN                   │
│  ─────────────────           ───────────────                │
│  LCD_CS    → A3              XP → 8                         │
│  LCD_CD    → A2              XM → A2                        │
│  LCD_WR    → A1              YP → A3                        │
│  LCD_RD    → A0              YM → 9                         │
│  LCD_RST   → A4                                             │
│                                                             │
│  SD CARD (SPI Bit-Bang)      I2C                            │
│  ──────────────────          ────                           │
│  SD_CS     → 10              SDA → 20                       │
│  SD_MOSI   → 12              SCL → 21                       │
│  SD_MISO   → 11                                             │
│  SD_SCK    → 13              BLUETOOTH (Opcional)           │
│                              ────────────────────           │
│  SERIAL DEBUG                HC05_RX → 50                   │
│  ────────────                HC05_TX → 51                   │
│  TX → 1                                                     │
│  RX → 0                                                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 2. Arduino Uno (Slave - Áudio)

O Arduino Uno atua como controlador dedicado de áudio.

#### Responsabilidades:
- Receber comandos via I2C
- Controlar o módulo DFPlayer Mini
- Gerenciar reprodução de áudios MP3

#### Bibliotecas Utilizadas:

```cpp
#include <SoftwareSerial.h>        // Comunicação com DFPlayer
#include <DFRobotDFPlayerMini.h>   // Controle do MP3
#include <Wire.h>                   // Comunicação I2C
```

#### Mapa de Pinos:

```
┌─────────────────────────────────────────────────────────────┐
│                      ARDUINO UNO                            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  DFPLAYER MINI               I2C                            │
│  ─────────────               ────                           │
│  pinRx → 10                  SDA → A4                       │
│  pinTx → 11                  SCL → A5                       │
│                                                             │
│  SERIAL DEBUG                                               │
│  ────────────                                               │
│  TX → 1                                                     │
│  RX → 0                                                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## Protocolos de Comunicação

### I2C (Wire)

```
┌──────────────┐                    ┌──────────────┐
│  MASTER      │   SDA ────────────►│  SLAVE       │
│  (Mega)      │   SCL ────────────►│  (Uno)       │
│              │   GND ────────────►│  Endereço: 8 │
└──────────────┘                    └──────────────┘
```

#### Estrutura de Mensagens:

| Código | Significado | Ação |
|--------|-------------|------|
| 1 | Inicialização | Som de boot |
| 10-15 | Visão | Áudios do poder amarelo |
| 20-25 | Invisibilidade | Áudios do poder azul |
| 30-35 | Velocidade | Áudios do poder verde |
| 40-45 | Super Força | Áudios do poder roxo |

#### Código de Transmissão (Master):

```cpp
Wire.beginTransmission(8);  // Endereço do slave
Wire.write(NumPoder);        // Código do poder
Wire.endTransmission();
```

#### Código de Recepção (Slave):

```cpp
void receberPoder(int NumPoder) {
    NumPoder = Wire.read();
    tocarMusica(NumPoder);
}
```

---

## Máquina de Estados

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         MÁQUINA DE ESTADOS                                  │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│                           ┌─────────────┐                                   │
│                           │    INIT     │                                   │
│                           │  (setup)    │                                   │
│                           └──────┬──────┘                                   │
│                                  │                                          │
│                                  ▼                                          │
│                    ┌─────────────────────────────┐                          │
│           ┌───────►│        TECLADO              │◄──────────┐              │
│           │        │   (Tela_timer)              │           │              │
│           │        │   Entrada numérica          │           │              │
│           │        └─────────────┬───────────────┘           │              │
│           │                      │ ENTER                     │              │
│           │                      ▼                           │              │
│           │        ┌─────────────────────────────┐           │              │
│           │        │     SELEÇÃO PODER           │           │              │
│           │        │    (Tela_herois)            │           │              │
│           │        │   4 opções coloridas        │           │              │
│           │        └──────┬──────┬──────┬────────┘           │              │
│           │               │      │      │                    │              │
│           │        ┌──────┘      │      └──────┐             │              │
│           │        ▼             ▼              ▼             │              │
│           │   ┌─────────┐  ┌─────────┐   ┌─────────┐         │              │
│           │   │ Amarelo │  │  Azul   │   │  Verde  │  ...    │              │
│           │   │ (Visão) │  │(Invisi) │   │ (Veloc) │         │              │
│           │   └────┬────┘  └────┬────┘   └────┬────┘         │              │
│           │        │            │             │              │              │
│           │        ▼            ▼             ▼              │              │
│           │   ┌─────────────────────────────────────┐        │              │
│           │   │        SELEÇÃO PERSONAGEM           │        │              │
│           │   │   Masculino (M) / Feminino (F)      │        │              │
│           │   └────────────────┬────────────────────┘        │              │
│           │                    │                             │              │
│           │                    ▼                             │              │
│           │   ┌─────────────────────────────────────┐        │              │
│           │   │        APLICANDO PODER              │        │              │
│           │   │   - Exibe imagem                    │        │              │
│           │   │   - Reproduz áudio                  │        │              │
│           │   │   - Countdown timer                 │        │              │
│           │   │   - Envia código I2C                │        │              │
│           │   └────────────────┬────────────────────┘        │              │
│           │                    │ Timer = 0                   │              │
│           │                    ▼                             │              │
│           │   ┌─────────────────────────────────────┐        │              │
│           │   │        DESATIVAÇÃO                  │────────┘              │
│           │   │   - Envia código de desativação     │                       │
│           │   │   - Retorna ao início               │                       │
│           │   └─────────────────────────────────────┘                       │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Fluxo de Dados

### Entrada de Usuário → Feedback Visual/Sonoro

```
┌──────────────────────────────────────────────────────────────────────────┐
│                        FLUXO DE DADOS                                    │
├──────────────────────────────────────────────────────────────────────────┤
│                                                                          │
│  ┌─────────┐     Toque      ┌─────────────┐                              │
│  │ USUÁRIO │───────────────►│ TOUCHSCREEN │                              │
│  └─────────┘                └──────┬──────┘                              │
│                                    │ Coordenadas (x, y)                  │
│                                    ▼                                     │
│                          ┌─────────────────┐                             │
│                          │  Touch_getXY()  │                             │
│                          │  Mapeia coords  │                             │
│                          └────────┬────────┘                             │
│                                   │                                      │
│                                   ▼                                      │
│                          ┌─────────────────┐                             │
│                          │ Detecção de     │                             │
│                          │ Botão/Área      │                             │
│                          └────────┬────────┘                             │
│                                   │                                      │
│              ┌────────────────────┼────────────────────┐                 │
│              ▼                    ▼                    ▼                 │
│   ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐         │
│   │ Atualiza Estado │  │ Carrega Imagem  │  │ Envia Comando   │         │
│   │ da Máquina      │  │ do SD Card      │  │ via I2C         │         │
│   └────────┬────────┘  └────────┬────────┘  └────────┬────────┘         │
│            │                    │                    │                   │
│            ▼                    ▼                    ▼                   │
│   ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐         │
│   │ Redesenha Tela  │  │ showBMP()       │  │ Wire.write()    │         │
│   │ TFT             │  │ Renderiza BMP   │  │                 │         │
│   └────────┬────────┘  └────────┬────────┘  └────────┬────────┘         │
│            │                    │                    │                   │
│            ▼                    ▼                    ▼                   │
│   ┌─────────────────────────────────────────────────────────────┐       │
│   │                    DISPLAY TFT                               │       │
│   │                   (Feedback Visual)                          │       │
│   └─────────────────────────────────────────────────────────────┘       │
│                                                                          │
│            ┌────────────────────────────────────────────────────┐        │
│            │                    I2C BUS                          │        │
│            │              (Comando de Áudio)                     │        │
│            └─────────────────────────┬──────────────────────────┘        │
│                                      ▼                                   │
│                          ┌─────────────────┐                             │
│                          │  ARDUINO UNO    │                             │
│                          │  receberPoder() │                             │
│                          └────────┬────────┘                             │
│                                   ▼                                      │
│                          ┌─────────────────┐                             │
│                          │  tocarMusica()  │                             │
│                          │  DFPlayer       │                             │
│                          └────────┬────────┘                             │
│                                   ▼                                      │
│                          ┌─────────────────┐                             │
│                          │  ALTO-FALANTE   │                             │
│                          │ (Feedback Audio)│                             │
│                          └─────────────────┘                             │
│                                                                          │
└──────────────────────────────────────────────────────────────────────────┘
```

---

## Sistema de Arquivos

### Cartão SD do Display (Imagens)

```
/                           (raiz)
│
├── Visao2.bmp              # Seleção - Visão
├── VisaoM.bmp              # Aplicando - Masculino
├── VisaoF.bmp              # Aplicando - Feminino
│
├── Invisibilidade2.bmp     # Seleção - Invisibilidade
├── InvisibilidadeM.bmp     # Aplicando - Masculino
├── InvisibilidadeF.bmp     # Aplicando - Feminino
│
├── Velocidade2.bmp         # Seleção - Velocidade
├── SuperVelocM.bmp         # Aplicando - Masculino
├── SuperVelocF.bmp         # Aplicando - Feminino
│
├── SuperF2.bmp             # Seleção - Super Força
├── SuperF_M.bmp            # Aplicando - Masculino
└── SuperF_F.bmp            # Aplicando - Feminino
```

#### Especificações das Imagens BMP:

| Propriedade | Valor |
|-------------|-------|
| Formato | BMP (Windows Bitmap) |
| Resolução | 320 x 240 pixels |
| Profundidade | 24-bit (recomendado) |
| Suportado | 1, 4, 8, 16, 24-bit |
| Compressão | Não comprimido ou 565 |

### Cartão SD do DFPlayer (Áudios)

```
/
└── 01/                     # Pasta 01 (obrigatória)
    ├── 001.mp3             # Áudio inicial/boot
    │
    ├── 010.mp3             # Visão - Apresentação
    ├── 011.mp3             # Visão - Ativando
    ├── 012.mp3             # Visão - Efeito
    ├── 015.mp3             # Visão - Desativando
    │
    ├── 020.mp3             # Invisibilidade - Apresentação
    ├── 021.mp3             # Invisibilidade - Ativando
    ├── 023.mp3             # Invisibilidade - Efeito
    ├── 025.mp3             # Invisibilidade - Desativando
    │
    ├── 030.mp3             # Velocidade - Apresentação
    ├── 031.mp3             # Velocidade - Ativando
    ├── 033.mp3             # Velocidade - Efeito
    ├── 035.mp3             # Velocidade - Desativando
    │
    ├── 040.mp3             # Super Força - Apresentação
    ├── 041.mp3             # Super Força - Ativando
    ├── 043.mp3             # Super Força - Efeito
    └── 045.mp3             # Super Força - Desativando
```

---

## Diagrama de Tempo

```
┌────────────────────────────────────────────────────────────────────────────┐
│                         DIAGRAMA DE TEMPO                                  │
├────────────────────────────────────────────────────────────────────────────┤
│                                                                            │
│  Tempo →     0s      1s      2s      3s    ...    Ns     N+1s              │
│              │       │       │       │            │       │                │
│  ─────────────────────────────────────────────────────────────────────     │
│                                                                            │
│  USUÁRIO:    │                                                             │
│  ────────    ▼                                                             │
│           [Toque]                              [Timer=0]                   │
│              │                                     │                       │
│  ─────────────────────────────────────────────────────────────────────     │
│                                                                            │
│  DISPLAY:    │                                     │                       │
│  ────────    ▼                                     ▼                       │
│         ┌─────────┐  ┌──────────────────────┐  ┌─────────┐                 │
│         │ Carrega │  │   "Aplicando..."     │  │ Retorna │                 │
│         │ Imagem  │  │   Countdown: N-1...0 │  │ Início  │                 │
│         └─────────┘  └──────────────────────┘  └─────────┘                 │
│              │                                     │                       │
│  ─────────────────────────────────────────────────────────────────────     │
│                                                                            │
│  I2C:        │                                     │                       │
│  ────        ▼                                     ▼                       │
│         ┌─────────┐                           ┌─────────┐                  │
│         │ Código  │                           │ Código  │                  │
│         │ Ativação│                           │ Desativ.│                  │
│         │ (11,21.)│                           │(15,25..)│                  │
│         └─────────┘                           └─────────┘                  │
│              │                                     │                       │
│  ─────────────────────────────────────────────────────────────────────     │
│                                                                            │
│  ÁUDIO:      │                                     │                       │
│  ──────      ▼                                     ▼                       │
│         ┌─────────────────────────────────┐   ┌─────────┐                  │
│         │ Reproduzindo áudio do poder     │   │ Som de  │                  │
│         │ (efeitos sonoros, narração)     │   │ Desativ.│                  │
│         └─────────────────────────────────┘   └─────────┘                  │
│                                                                            │
└────────────────────────────────────────────────────────────────────────────┘
```

---

## Memória e Recursos

### Arduino Mega 2560

| Recurso | Disponível | Uso Estimado |
|---------|------------|--------------|
| Flash | 256 KB | ~45 KB (18%) |
| SRAM | 8 KB | ~2 KB (25%) |
| EEPROM | 4 KB | Não utilizado |

### Arduino Uno

| Recurso | Disponível | Uso Estimado |
|---------|------------|--------------|
| Flash | 32 KB | ~8 KB (25%) |
| SRAM | 2 KB | ~0.5 KB (25%) |
| EEPROM | 1 KB | Não utilizado |

---

## Considerações de Segurança

1. **Alimentação**: Utilizar fonte estável 9V/2A
2. **Proteção I2C**: Resistores pull-up de 4.7kΩ
3. **SD Card**: Formatação FAT16/FAT32
4. **DFPlayer RX**: Resistor de 1kΩ para proteção

---

## Extensibilidade

O sistema foi projetado para permitir:

1. **Novos Poderes**: Adicionar mais opções de poderes
2. **Bluetooth**: Controle remoto via app
3. **WiFi**: Upgrade para ESP32
4. **Múltiplos Módulos**: Conexão de mais slaves I2C
5. **Sensores**: Integração com acelerômetro, LED RGB, etc.
