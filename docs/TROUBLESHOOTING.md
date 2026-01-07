# Troubleshooting - Caixinha do Poder

## Guia de Solução de Problemas

Este documento ajuda a identificar e resolver problemas comuns no projeto.

---

## Diagnóstico Rápido

```
┌────────────────────────────────────────────────────────────────┐
│                  CHECKLIST DE DIAGNÓSTICO                      │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│  DISPLAY:                                                      │
│  □ Tela liga?                    SIM → Próximo                 │
│                                  NÃO → Ver seção "Display"     │
│                                                                │
│  □ Exibe conteúdo?               SIM → Próximo                 │
│                                  NÃO → Ver seção "Display"     │
│                                                                │
│  TOUCH:                                                        │
│  □ Responde ao toque?            SIM → Próximo                 │
│                                  NÃO → Ver seção "Touch"       │
│                                                                │
│  □ Toque está calibrado?         SIM → Próximo                 │
│                                  NÃO → Ver seção "Calibração"  │
│                                                                │
│  SD CARD (Display):                                            │
│  □ Cartão é detectado?           SIM → Próximo                 │
│                                  NÃO → Ver seção "SD Card"     │
│                                                                │
│  □ Imagens carregam?             SIM → Próximo                 │
│                                  NÃO → Ver seção "Imagens BMP" │
│                                                                │
│  ÁUDIO:                                                        │
│  □ Som é reproduzido?            SIM → Sistema OK!             │
│                                  NÃO → Ver seção "Áudio"       │
│                                                                │
└────────────────────────────────────────────────────────────────┘
```

---

## Problemas com Display

### Tela Não Liga

```
SINTOMA: Display completamente preto/sem luz

CAUSAS POSSÍVEIS:
┌─────────────────────────────────────────────────────────────┐
│ 1. Shield mal encaixado                                     │
│ 2. Arduino não está alimentado                              │
│ 3. Shield defeituoso                                        │
│ 4. Código não foi carregado                                 │
└─────────────────────────────────────────────────────────────┘

SOLUÇÕES:
1. Remova e reencaixe o shield firmemente
2. Verifique LED de power do Arduino
3. Teste com outro shield (se disponível)
4. Faça upload do código novamente

TESTE:
- Abra Serial Monitor (9600 baud)
- Se aparecer texto → Arduino OK, problema no shield
- Se não aparecer nada → Verificar USB/código
```

### Tela Branca ou Com Ruído

```
SINTOMA: Tela acende mas mostra só branco ou pixels aleatórios

CAUSAS POSSÍVEIS:
┌─────────────────────────────────────────────────────────────┐
│ 1. Driver incorreto                                         │
│ 2. ID do display não reconhecido                            │
│ 3. Inicialização falhou                                     │
└─────────────────────────────────────────────────────────────┘

SOLUÇÕES:
1. Verificar ID do display:
   - Adicione no setup():
     Serial.print("Display ID: 0x");
     Serial.println(tft.readID(), HEX);
   - ID esperado: 0x9341 (ILI9341)

2. Se ID diferente, pode precisar de biblioteca específica

3. Código para debug:
   void setup() {
       Serial.begin(9600);
       tft.reset();
       uint16_t ID = tft.readID();
       Serial.print("ID = 0x");
       Serial.println(ID, HEX);
       if (ID == 0x0 || ID == 0xFFFF) {
           Serial.println("ERRO: Display nao detectado!");
       }
       tft.begin(ID);
   }
```

### Cores Invertidas ou Erradas

```
SINTOMA: Cores aparecem trocadas (vermelho=azul, etc.)

SOLUÇÕES:
1. Verificar definições de cores:
   #define RED     0xF800  // Vermelho correto
   #define BLUE    0x001F  // Azul correto
   #define GREEN   0x07E0  // Verde correto (não 0x07E01!)

2. Testar inversão:
   tft.invertDisplay(true);  // ou false
```

---

## Problemas com Touch Screen

### Touch Não Responde

```
SINTOMA: Toques não são detectados

CAUSAS POSSÍVEIS:
┌─────────────────────────────────────────────────────────────┐
│ 1. Pinos compartilhados não restaurados                     │
│ 2. Pressão insuficiente                                     │
│ 3. Touch defeituoso                                         │
│ 4. Constantes de pressão incorretas                         │
└─────────────────────────────────────────────────────────────┘

SOLUÇÕES:
1. Verificar restauração de pinos após leitura:
   TSPoint p = ts.getPoint();
   pinMode(YP, OUTPUT);      // IMPORTANTE!
   pinMode(XM, OUTPUT);      // IMPORTANTE!
   digitalWrite(YP, HIGH);
   digitalWrite(XM, HIGH);

2. Ajustar sensibilidade:
   #define MINPRESSURE 50    // Reduzir (era 100)
   #define MAXPRESSURE 1200  // Aumentar (era 1000)

3. Código de teste:
   void loop() {
       TSPoint p = ts.getPoint();
       Serial.print("X="); Serial.print(p.x);
       Serial.print(" Y="); Serial.print(p.y);
       Serial.print(" Z="); Serial.println(p.z);
       delay(100);
   }
```

### Touch Descalibrado

```
SINTOMA: Toque detectado em posição errada

PROCEDIMENTO DE CALIBRAÇÃO:

1. Faça upload do touchscreendemo.ino

2. Abra o Serial Monitor (9600 baud)

3. Toque nos 4 cantos e anote os valores:

   ┌─────────────────────────────────────────┐
   │                                         │
   │  CANTO SUPERIOR ESQUERDO                │
   │  X= _____ Y= _____                      │
   │                                         │
   │                 CANTO SUPERIOR DIREITO  │
   │                 X= _____ Y= _____       │
   │                                         │
   │  CANTO INFERIOR ESQUERDO                │
   │  X= _____ Y= _____                      │
   │                                         │
   │                 CANTO INFERIOR DIREITO  │
   │                 X= _____ Y= _____       │
   │                                         │
   └─────────────────────────────────────────┘

4. Calcule os valores de calibração:
   TS_LEFT = mínimo X encontrado
   TS_RT   = máximo X encontrado
   TS_TOP  = máximo Y encontrado
   TS_BOT  = mínimo Y encontrado

5. Atualize no código:
   const int TS_LEFT = ???;
   const int TS_RT = ???;
   const int TS_TOP = ???;
   const int TS_BOT = ???;

VALORES TÍPICOS:
┌──────────┬────────────────┐
│ Constante│ Valor Típico   │
├──────────┼────────────────┤
│ TS_LEFT  │ 100-200        │
│ TS_RT    │ 900-1000       │
│ TS_TOP   │ 850-950        │
│ TS_BOT   │ 50-150         │
└──────────┴────────────────┘
```

---

## Problemas com SD Card

### Cartão Não Detectado

```
SINTOMA: Mensagem "cannot start SD" no Serial Monitor

CAUSAS POSSÍVEIS:
┌─────────────────────────────────────────────────────────────┐
│ 1. Cartão não inserido corretamente                         │
│ 2. Cartão não formatado corretamente                        │
│ 3. Pino CS incorreto                                        │
│ 4. Cartão incompatível                                      │
│ 5. Conflito de pinos SPI                                    │
└─────────────────────────────────────────────────────────────┘

SOLUÇÕES:

1. Verificar inserção física do cartão

2. Formatar cartão:
   - Windows: FAT32, tamanho de alocação 32KB
   - Máximo recomendado: 32GB

3. Verificar pino CS:
   #define SD_CS 10  // Deve ser 10 para shields padrão

4. Testar com outro cartão

5. Código de debug:
   void setup() {
       Serial.begin(9600);
       Serial.println("Iniciando SD...");

       if (!SD.begin(SD_CS)) {
           Serial.println("FALHA no SD!");
           Serial.println("Verifique:");
           Serial.println("1. Cartao inserido?");
           Serial.println("2. Formatacao FAT32?");
           Serial.println("3. Pino CS = 10?");
           while(1);
       }
       Serial.println("SD OK!");
   }
```

### Imagens BMP Não Carregam

```
SINTOMA: Cartão detectado mas imagens não aparecem

CAUSAS POSSÍVEIS:
┌─────────────────────────────────────────────────────────────┐
│ 1. Nome do arquivo incorreto                                │
│ 2. Formato BMP incompatível                                 │
│ 3. Imagem muito grande                                      │
│ 4. Arquivo corrompido                                       │
└─────────────────────────────────────────────────────────────┘

VERIFICAÇÕES:

1. Nomes de arquivo (case-sensitive!):
   ✓ Visao2.bmp (correto)
   ✗ visao2.bmp (errado - minúsculo)
   ✗ Visao2.BMP (errado - extensão maiúscula)
   ✗ Visão2.bmp (errado - acento)

2. Formato BMP correto:
   - Resolução: 320x240 pixels
   - Profundidade: 24-bit (sem compressão)
   - Formato: Windows BMP

3. Como criar BMP correto (GIMP):
   a. Abrir imagem
   b. Imagem → Redimensionar → 320x240
   c. Arquivo → Exportar como
   d. Escolher "Windows BMP"
   e. Opções: 24 bits, sem compressão

4. Código de verificação:
   void listarArquivos() {
       File dir = SD.open("/");
       while (true) {
           File entry = dir.openNextFile();
           if (!entry) break;
           Serial.println(entry.name());
           entry.close();
       }
   }

CÓDIGOS DE ERRO showBMP():
┌────────┬────────────────────────────────────┐
│ Código │ Significado                        │
├────────┼────────────────────────────────────┤
│ 0      │ Sucesso                            │
│ 1      │ Coordenadas fora da tela           │
│ 2      │ Não é arquivo BMP válido           │
│ 3      │ Muitos planos de cor               │
│ 4      │ Formato/compressão não suportado   │
│ 5      │ Profundidade de paleta inválida    │
└────────┴────────────────────────────────────┘
```

---

## Problemas com Áudio

### DFPlayer Não Inicia

```
SINTOMA: Mensagem "Falha: 1.conexões! 2.cheque o cartão SD!"

CAUSAS POSSÍVEIS:
┌─────────────────────────────────────────────────────────────┐
│ 1. Conexões TX/RX invertidas                                │
│ 2. Falta resistor no RX                                     │
│ 3. Cartão SD não formatado corretamente                     │
│ 4. Arquivos MP3 não encontrados                             │
│ 5. DFPlayer defeituoso                                      │
└─────────────────────────────────────────────────────────────┘

VERIFICAÇÕES:

1. Conexões corretas:
   Arduino Pin 11 ──[1kΩ]──► DFPlayer RX
   Arduino Pin 10 ◄───────── DFPlayer TX
   Arduino 5V ──────────────► DFPlayer VCC
   Arduino GND ─────────────► DFPlayer GND

2. IMPORTANTE: Resistor de 1kΩ é OBRIGATÓRIO no RX!
   (DFPlayer usa 3.3V, Arduino usa 5V)

3. Formatação do SD do DFPlayer:
   - FAT16 ou FAT32
   - Máximo 32GB

4. Estrutura de pastas OBRIGATÓRIA:
   /01/001.mp3
   /01/010.mp3
   etc.

5. Código de teste simples:
   #include "SoftwareSerial.h"
   #include "DFRobotDFPlayerMini.h"

   SoftwareSerial mySerial(10, 11);
   DFRobotDFPlayerMini myDFPlayer;

   void setup() {
       Serial.begin(9600);
       mySerial.begin(9600);

       if (!myDFPlayer.begin(mySerial)) {
           Serial.println("ERRO!");
           while(true);
       }
       Serial.println("DFPlayer OK!");
       myDFPlayer.volume(20);
       myDFPlayer.play(1);  // Toca primeiro arquivo
   }
```

### Sem Som no Alto-falante

```
SINTOMA: DFPlayer inicia mas não há som

VERIFICAÇÕES:

1. Conexão do alto-falante:
   DFPlayer SPK_1 → Alto-falante (+)
   DFPlayer SPK_2 → Alto-falante (-)

2. Volume:
   playerMP3.volume(25);  // 0-30

3. Arquivo existe:
   - Nomes devem ser 001.mp3, 002.mp3, etc.
   - Dentro da pasta /01/

4. Testar alto-falante:
   - Conectar em fonte de áudio alternativa
   - Verificar impedância (deve ser 4-8Ω)

5. Capacitor de filtro:
   - Adicionar 100µF entre VCC e GND do DFPlayer
   - Reduz ruídos e melhora estabilidade
```

---

## Problemas com Comunicação I2C

### Slave Não Recebe Dados

```
SINTOMA: Master envia mas slave não executa

VERIFICAÇÕES:

1. Endereço I2C correto:
   Master: Wire.beginTransmission(8);
   Slave:  Wire.begin(8);  // Mesmo endereço!

2. Conexões:
   Mega SDA (20) ──────────► Uno A4 (SDA)
   Mega SCL (21) ──────────► Uno A5 (SCL)
   Mega GND ───────────────► Uno GND

3. Pull-up resistors (opcional mas recomendado):
   SDA ──[4.7kΩ]──► 5V
   SCL ──[4.7kΩ]──► 5V

4. Teste de comunicação:
   // No Master:
   Wire.beginTransmission(8);
   byte error = Wire.endTransmission();
   Serial.print("Erro I2C: ");
   Serial.println(error);
   // error = 0 significa sucesso

5. Scanner I2C (executar no Master):
   void scanI2C() {
       for (byte address = 1; address < 127; address++) {
           Wire.beginTransmission(address);
           byte error = Wire.endTransmission();
           if (error == 0) {
               Serial.print("Dispositivo em: 0x");
               Serial.println(address, HEX);
           }
       }
   }
```

---

## Problemas de Código

### Bug: Timer Não Desativa Poder

```
PROBLEMA: Código de desativação nunca é executado

CÓDIGO COM BUG:
for(NumT; NumT>0; NumT--) {
    if(NumT == 0) {  // ← Nunca é verdade dentro do loop!
        Wire.write(NumPoder);
    }
}

CÓDIGO CORRIGIDO:
for(; NumT > 0; NumT--) {
    delay(1000);
}
// Após o loop, NumT == 0
Wire.beginTransmission(8);
Wire.write(NumPoder);  // Código de desativação
Wire.endTransmission();
```

### Bug: Definição de Cor GREEN

```
PROBLEMA: GREEN definido com valor inválido

CÓDIGO COM BUG:
#define GREEN 0x07E01  // ← Valor de 5 dígitos hex (inválido)

CÓDIGO CORRIGIDO:
#define GREEN 0x07E0   // ← Valor correto de 4 dígitos hex
```

---

## LEDs de Status (Debug)

### Indicadores do Arduino

```
LED POWER (Verde):
• Aceso = Alimentação OK
• Apagado = Sem alimentação

LED L (Pin 13):
• Pisca durante upload
• Pode ser usado para debug:
  digitalWrite(13, HIGH);  // Debug: passou por aqui
  delay(100);
  digitalWrite(13, LOW);

LED TX/RX:
• Piscam durante comunicação serial
```

### Indicadores do DFPlayer

```
LED BUSY:
• Apagado = Reproduzindo áudio
• Aceso = Parado/Idle

LED no módulo:
• Pisca = Atividade no SD
• Estático = Erro ou idle
```

---

## Mensagens de Erro Comuns

| Mensagem | Causa | Solução |
|----------|-------|---------|
| "cannot start SD" | SD não detectado | Ver seção SD Card |
| "DFPlayer falha" | DFPlayer não iniciou | Ver seção Áudio |
| Tela branca | Driver errado | Verificar ID display |
| Touch não responde | Pinos não restaurados | Ver código Touch_getXY |
| Áudio cortado | Alimentação fraca | Usar fonte 9V 2A |

---

## Reset de Fábrica

Se nada funcionar, tente:

```
1. Desconectar tudo
2. Reformatar ambos os cartões SD
3. Recarregar os arquivos BMP e MP3
4. Fazer upload do código novamente
5. Verificar todas as conexões
6. Testar componentes individualmente
```

---

## Onde Buscar Ajuda

1. **Documentação oficial**:
   - Arduino: arduino.cc/reference
   - Adafruit GFX: learn.adafruit.com/adafruit-gfx-graphics-library

2. **Fóruns**:
   - Arduino Forum: forum.arduino.cc
   - Reddit: r/arduino

3. **GitHub Issues**:
   - MCUFRIEND_kbv: github.com/prenticedavid/MCUFRIEND_kbv/issues
   - DFRobotDFPlayerMini: wiki.dfrobot.com

4. **Vídeos tutoriais**:
   - YouTube: Buscar por "Arduino TFT touchscreen tutorial"
