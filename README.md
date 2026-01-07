# Caixinha do Poder

![Arduino](https://img.shields.io/badge/Arduino-Mega-00979D?logo=arduino)
![Status](https://img.shields.io/badge/Status-Em%20Desenvolvimento-yellow)
![Licença](https://img.shields.io/badge/Licença-MIT-green)

## Simulador Interativo de Poderes de Super-Heróis

A **Caixinha do Poder** é um projeto educacional e interativo baseado em Arduino que simula a ativação de poderes de super-heróis. Utilizando uma tela touchscreen colorida, áudio imersivo e comunicação sem fio, o sistema oferece uma experiência gamificada onde usuários podem selecionar e "ativar" diferentes poderes.

---

## Índice

- [Visão Geral](#visão-geral)
- [Características](#características)
- [Hardware Necessário](#hardware-necessário)
- [Arquitetura do Sistema](#arquitetura-do-sistema)
- [Instalação](#instalação)
- [Uso](#uso)
- [Os Poderes](#os-poderes)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Documentação](#documentação)
- [Contribuição](#contribuição)
- [Licença](#licença)

---

## Visão Geral

```
┌─────────────────────────────────────────────────────────────────┐
│                      CAIXINHA DO PODER                          │
│                                                                 │
│    ┌─────────────┐    I2C     ┌─────────────┐                  │
│    │   Arduino   │◄──────────►│   Arduino   │                  │
│    │    Mega     │            │   (Audio)   │                  │
│    │  + Display  │            │  + DFPlayer │                  │
│    └─────────────┘            └─────────────┘                  │
│          │                          │                          │
│          ▼                          ▼                          │
│    ┌───────────┐              ┌───────────┐                    │
│    │   Tela    │              │   Alto    │                    │
│    │   Touch   │              │  Falante  │                    │
│    │  320x240  │              │           │                    │
│    └───────────┘              └───────────┘                    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Características

### Interface Visual
- Display TFT 3.2" colorido (320x240 pixels)
- Tela touchscreen capacitiva
- Imagens BMP personalizadas para cada poder
- Animações e contagem regressiva visual

### Sistema de Áudio
- Módulo DFPlayer Mini para reprodução MP3
- Efeitos sonoros para cada poder
- Feedback auditivo de ações

### Comunicação
- Protocolo I2C entre módulos
- Suporte a Bluetooth HC-05 (expansível)
- Debug via Serial

### Poderes Disponíveis
| Poder | Cor | Descrição |
|-------|-----|-----------|
| Visão | Amarelo | Poder de visão especial |
| Invisibilidade | Azul | Tornar-se invisível |
| Super Velocidade | Verde | Mover-se em alta velocidade |
| Super Força | Roxo | Força sobre-humana |

---

## Hardware Necessário

### Componentes Principais

| Componente | Quantidade | Descrição |
|------------|------------|-----------|
| Arduino Mega 2560 | 1 | Controlador principal |
| Arduino Uno/Nano | 1 | Controlador de áudio |
| Display TFT 3.2" | 1 | ILI9341 com touchscreen |
| DFPlayer Mini | 1 | Módulo MP3 |
| Módulo HC-05 | 1 | Bluetooth (opcional) |
| Cartão SD | 2 | Imagens e áudio |
| Alto-falante | 1 | 3W, 8Ω |

### Lista de Materiais Completa

```
□ Arduino Mega 2560
□ Arduino Uno (para módulo de áudio)
□ Shield TFT 3.2" ILI9341
□ Módulo DFPlayer Mini
□ Cartão MicroSD 8GB (x2)
□ Alto-falante 3W 8Ω
□ Módulo Bluetooth HC-05
□ Jumpers diversos
□ Cabo USB (x2)
□ Fonte 9V 2A
□ Caixa/Case (impressão 3D recomendada)
```

---

## Arquitetura do Sistema

```
                    ┌──────────────────┐
                    │   CARTÃO SD      │
                    │  (Imagens BMP)   │
                    └────────┬─────────┘
                             │ SPI
                             ▼
┌─────────────┐         ┌─────────────────────┐
│   USUÁRIO   │◄───────►│    ARDUINO MEGA     │
│   (Touch)   │         │                     │
└─────────────┘         │  - Adafruit_GFX     │
                        │  - MCUFRIEND_kbv    │
                        │  - TouchScreen      │
                        │  - Wire (I2C)       │
                        │  - SdFat            │
                        └──────────┬──────────┘
                                   │ I2C (Endereço 8)
                                   ▼
                        ┌─────────────────────┐
                        │    ARDUINO UNO      │
                        │    (Áudio Slave)    │
                        │                     │
                        │  - DFPlayerMini     │
                        │  - Wire (I2C)       │
                        └──────────┬──────────┘
                                   │ Serial
                                   ▼
                        ┌─────────────────────┐
                        │    DFPLAYER MINI    │
                        │    + CARTÃO SD      │
                        │    (Áudios MP3)     │
                        └──────────┬──────────┘
                                   │
                                   ▼
                        ┌─────────────────────┐
                        │    ALTO-FALANTE     │
                        └─────────────────────┘
```

---

## Instalação

### 1. Preparar o Ambiente

```bash
# Instalar Arduino IDE 2.x
# https://www.arduino.cc/en/software

# Instalar bibliotecas necessárias via Library Manager:
# - Adafruit GFX Library
# - MCUFRIEND_kbv
# - SdFat
# - DFRobotDFPlayerMini
```

### 2. Bibliotecas Necessárias

| Biblioteca | Versão | Propósito |
|------------|--------|-----------|
| Adafruit_GFX | 1.11+ | Gráficos base |
| MCUFRIEND_kbv | 2.9+ | Driver display |
| TouchScreen | 1.1+ | Input touch |
| SdFat | 2.1+ | Sistema de arquivos |
| DFRobotDFPlayerMini | 1.0+ | Controle MP3 |
| Wire | (built-in) | Comunicação I2C |

### 3. Preparar Cartões SD

#### Cartão SD do Display (Imagens)
```
/
├── Visao2.bmp
├── VisaoM.bmp
├── VisaoF.bmp
├── Invisibilidade2.bmp
├── InvisibilidadeM.bmp
├── InvisibilidadeF.bmp
├── Velocidade2.bmp
├── SuperVelocM.bmp
├── SuperVelocF.bmp
├── SuperF2.bmp
├── SuperF_M.bmp
└── SuperF_F.bmp
```

#### Cartão SD do DFPlayer (Áudios)
```
/01/
├── 001.mp3   (Som ativação geral)
├── 010.mp3   (Visão - intro)
├── 011.mp3   (Visão - ativo)
├── 012.mp3   (Visão - efeito)
├── 015.mp3   (Visão - desativar)
├── 020.mp3   (Invisibilidade - intro)
├── 021.mp3   (Invisibilidade - ativo)
├── ...
```

### 4. Upload do Código

```bash
# Arduino Mega (Display + Controle)
1. Abrir Apresentaçao.ino
2. Selecionar: Tools > Board > Arduino Mega 2560
3. Selecionar porta COM correta
4. Upload

# Arduino Uno (Áudio)
1. Abrir Audio_Cabo.ino
2. Selecionar: Tools > Board > Arduino Uno
3. Selecionar porta COM correta
4. Upload
```

---

## Uso

### Fluxo de Operação

```
┌──────────────────────────────────────────────────────────────┐
│                    FLUXO DE USO                              │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  1. INICIALIZAÇÃO                                            │
│     ┌─────────────┐                                          │
│     │   Ligar     │                                          │
│     │   Sistema   │                                          │
│     └──────┬──────┘                                          │
│            ▼                                                 │
│  2. TECLADO NUMÉRICO                                         │
│     ┌─────────────────────────────┐                          │
│     │  Digite a duração (0-9)     │                          │
│     │  Pressione ENTER            │                          │
│     │  (duração = número × 60s)   │                          │
│     └──────────────┬──────────────┘                          │
│                    ▼                                         │
│  3. SELEÇÃO DE PODER                                         │
│     ┌─────────────────────────────┐                          │
│     │  ┌───────┐  ┌───────┐       │                          │
│     │  │ VISÃO │  │INVISI-│       │                          │
│     │  │(Amar.)│  │BILID. │       │                          │
│     │  └───────┘  └───────┘       │                          │
│     │  ┌───────┐  ┌───────┐       │                          │
│     │  │VELOCI-│  │ SUPER │       │                          │
│     │  │ DADE  │  │ FORÇA │       │                          │
│     │  └───────┘  └───────┘       │                          │
│     └──────────────┬──────────────┘                          │
│                    ▼                                         │
│  4. ESCOLHA PERSONAGEM                                       │
│     ┌─────────────────────────────┐                          │
│     │  Masculino (toque superior) │                          │
│     │  Feminino (toque inferior)  │                          │
│     └──────────────┬──────────────┘                          │
│                    ▼                                         │
│  5. ATIVAÇÃO DO PODER                                        │
│     ┌─────────────────────────────┐                          │
│     │  - Exibe imagem do poder    │                          │
│     │  - Reproduz áudio           │                          │
│     │  - Contagem regressiva      │                          │
│     │  - "Aplicando..." na tela   │                          │
│     └──────────────┬──────────────┘                          │
│                    ▼                                         │
│  6. DESATIVAÇÃO                                              │
│     ┌─────────────────────────────┐                          │
│     │  Timer finaliza             │                          │
│     │  Poder desativado           │                          │
│     └─────────────────────────────┘                          │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### Códigos dos Poderes (I2C)

| Poder | Ativação | Efeito | Desativação |
|-------|----------|--------|-------------|
| Visão | 10, 11, 12 | 1x | 15 |
| Invisibilidade | 20, 21, 23 | 2x | 25 |
| Velocidade | 30, 31, 33 | 3x | 35 |
| Super Força | 40, 41, 43 | 4x | 45 |

---

## Os Poderes

### Visão (Amarelo)
```
┌──────────────────────┐
│        VISÃO         │
│    ════════════      │
│  Poder de ver além   │
│  do alcance normal   │
│                      │
│  Cor: AMARELO        │
│  Código: 1x          │
│  Imagens:            │
│   - Visao2.bmp       │
│   - VisaoM.bmp       │
│   - VisaoF.bmp       │
└──────────────────────┘
```

### Invisibilidade (Azul)
```
┌──────────────────────┐
│   INVISIBILIDADE     │
│    ════════════      │
│  Capacidade de se    │
│  tornar invisível    │
│                      │
│  Cor: AZUL           │
│  Código: 2x          │
│  Imagens:            │
│   - Invisibilidade2  │
│   - InvisibilidadeM  │
│   - InvisibilidadeF  │
└──────────────────────┘
```

### Super Velocidade (Verde)
```
┌──────────────────────┐
│  SUPER VELOCIDADE    │
│    ════════════      │
│  Mover-se em         │
│  velocidade extrema  │
│                      │
│  Cor: VERDE          │
│  Código: 3x          │
│  Imagens:            │
│   - Velocidade2.bmp  │
│   - SuperVelocM.bmp  │
│   - SuperVelocF.bmp  │
└──────────────────────┘
```

### Super Força (Roxo)
```
┌──────────────────────┐
│    SUPER FORÇA       │
│    ════════════      │
│  Força física        │
│  sobre-humana        │
│                      │
│  Cor: ROXO           │
│  Código: 4x          │
│  Imagens:            │
│   - SuperF2.bmp      │
│   - SuperF_M.bmp     │
│   - SuperF_F.bmp     │
└──────────────────────┘
```

---

## Estrutura do Projeto

```
caixinha_do_poder/
├── README.md                    # Este arquivo
├── docs/                        # Documentação completa
│   ├── ARQUITETURA.md          # Arquitetura do sistema
│   ├── MANUAL_HARDWARE.md      # Manual de montagem
│   ├── MANUAL_USUARIO.md       # Manual do usuário
│   ├── MELHORIAS.md            # Sugestões de melhoria
│   ├── JOGOS_INTERATIVOS.md    # Modos de jogo
│   ├── API_REFERENCIA.md       # Referência do código
│   ├── TROUBLESHOOTING.md      # Solução de problemas
│   ├── imagens/                # Diagramas e imagens
│   └── diagramas/              # Esquemáticos
├── src/                         # Código fonte organizado
│   ├── main/                   # Código principal
│   │   └── Apresentacao/       # Projeto principal
│   ├── modules/                # Módulos separados
│   │   └── Audio_Controller/   # Controlador de áudio
│   └── tests/                  # Códigos de teste
│       ├── TouchDemo/          # Demo touchscreen
│       └── SDImageTest/        # Teste cartão SD
├── assets/                      # Recursos
│   ├── audio/                  # Arquivos MP3
│   └── images/                 # Imagens BMP
├── examples/                    # Exemplos
└── hardware/                    # Esquemáticos e 3D
    ├── schematics/             # Circuitos
    └── 3d_models/              # Modelos para impressão
```

---

## Documentação

| Documento | Descrição |
|-----------|-----------|
| [ARQUITETURA.md](docs/ARQUITETURA.md) | Arquitetura técnica detalhada |
| [MANUAL_HARDWARE.md](docs/MANUAL_HARDWARE.md) | Guia de montagem do hardware |
| [MANUAL_USUARIO.md](docs/MANUAL_USUARIO.md) | Manual de uso do sistema |
| [MELHORIAS.md](docs/MELHORIAS.md) | Sugestões de melhorias futuras |
| [JOGOS_INTERATIVOS.md](docs/JOGOS_INTERATIVOS.md) | Modos de jogo e atividades |
| [API_REFERENCIA.md](docs/API_REFERENCIA.md) | Documentação das funções |
| [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) | Solução de problemas comuns |

---

## Conexões de Hardware

### Arduino Mega - Display TFT

| Pino Display | Pino Arduino | Função |
|--------------|--------------|--------|
| LCD_CS | A3 | Chip Select LCD |
| LCD_CD | A2 | Command/Data |
| LCD_WR | A1 | Write |
| LCD_RD | A0 | Read |
| LCD_RST | A4 | Reset |
| SD_CS | 10 | Chip Select SD |
| SD_MOSI | 12 | SPI MOSI |
| SD_MISO | 11 | SPI MISO |
| SD_SCK | 13 | SPI Clock |

### Comunicação I2C

| Arduino Mega | Arduino Uno |
|--------------|-------------|
| SDA (20) | SDA (A4) |
| SCL (21) | SCL (A5) |
| GND | GND |

### Arduino Uno - DFPlayer

| DFPlayer | Arduino Uno |
|----------|-------------|
| RX | Pin 11 (via resistor 1K) |
| TX | Pin 10 |
| VCC | 5V |
| GND | GND |
| SPK_1 | Alto-falante (+) |
| SPK_2 | Alto-falante (-) |

---

## Calibração Touch

Se necessário, ajuste as constantes de calibração em `Apresentaçao.ino`:

```cpp
const int TS_LEFT = 125;   // Borda esquerda
const int TS_RT = 965;     // Borda direita
const int TS_TOP = 905;    // Borda superior
const int TS_BOT = 85;     // Borda inferior
```

Use o arquivo `touchscreendemo.ino` para obter os valores corretos do seu display.

---

## Contribuição

Contribuições são bem-vindas! Por favor:

1. Fork o repositório
2. Crie sua feature branch (`git checkout -b feature/NovaFuncionalidade`)
3. Commit suas mudanças (`git commit -m 'Adiciona NovaFuncionalidade'`)
4. Push para a branch (`git push origin feature/NovaFuncionalidade`)
5. Abra um Pull Request

---

## Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

## Autores

- **Desenvolvedor Principal** - *Desenvolvimento inicial*

---

## Agradecimentos

- Comunidade Arduino
- Adafruit Industries pelas bibliotecas gráficas
- DFRobot pelo módulo DFPlayer Mini

---

<p align="center">
  <b>Caixinha do Poder</b> - Transformando crianças em super-heróis!
</p>
