# Identificação do Módulo WiFi UART

## Seu Módulo
- **28 pinos**
- **Interface:** UART
- **Padrão WiFi:** 802.11 b/g/n

## Módulos WiFi UART Comuns

### 1. ESP8266 (Mais Provável)
**Características:**
- Geralmente 8-22 pinos (dependendo do breakout)
- Tensão: **3.3V** (IMPORTANTE!)
- Interface UART
- WiFi 802.11 b/g/n
- Firmware AT commands
- Modelos comuns: ESP-01, ESP-12E, ESP-12F

**Como identificar:**
```
Procure por texto no chip:
- "ESP8266"
- "AI-THINKER"
- "ESP-12E" ou "ESP-12F"
```

### 2. ESP32 (Possível)
**Características:**
- 28-38 pinos (dependendo do módulo)
- Tensão: **3.3V** (IMPORTANTE!)
- WiFi 802.11 b/g/n + Bluetooth
- Mais potente que ESP8266
- Modelos: ESP32-WROOM-32, ESP32-DevKit

**Como identificar:**
```
Procure por texto no chip:
- "ESP32"
- "ESP32-WROOM"
- "ESPRESSIF"
```

### 3. Outros Módulos UART WiFi
- **HC-05/HC-06** (Bluetooth, não WiFi)
- **SIM800/SIM900** (GSM/GPRS, não WiFi)
- **Módulos genéricos WiFi serial**

## Como Descobrir Seu Módulo

### Método 1: Inspeção Visual
1. Procure texto impresso no chip metálico (blindagem)
2. Procure no PCB (placa) por modelo/fabricante
3. Tire uma foto e compartilhe para identificação

### Método 2: Teste de Comunicação
Conecte ao Arduino e teste com comandos AT:

**Baudrate comum:** 115200 ou 9600

**Teste básico:**
```
AT              → Resposta: OK
AT+GMR          → Versão do firmware
AT+CWMODE?      → Modo WiFi (ESP8266)
```

### Método 3: Pinagem
**ESP8266 típico:**
```
VCC    (3.3V!)
GND
TX     (conecta no RX do Arduino)
RX     (conecta no TX do Arduino via divisor de tensão!)
CH_PD  (enable - conectar em 3.3V)
GPIO0, GPIO2, RST (outros pinos)
```

**ESP32 típico:**
```
VCC    (3.3V!)
GND
TXD    (GPIO1)
RXD    (GPIO3)
EN     (enable)
Múltiplos GPIO, ADC, etc
```

## ⚠️ AVISO IMPORTANTE - TENSÃO

### Arduino Uno opera em 5V, mas ESP8266/ESP32 são 3.3V!

**Conexões SEGURAS:**

1. **Alimentação:**
   - Use pino 3.3V do Arduino OU
   - Regulador de tensão 5V → 3.3V (AMS1117-3.3 ou similar)

2. **TX do Arduino → RX do ESP:**
   - **OBRIGATÓRIO:** Use divisor de tensão (2 resistores)
   - Arduino TX (5V) não pode ir direto no ESP RX!
   ```
   Arduino TX ----[1kΩ]----+----[2kΩ]---- GND
                           |
                      ESP RX (≈3.3V)
   ```

3. **RX do Arduino ← TX do ESP:**
   - Conexão direta OK (3.3V é reconhecido como HIGH pelo Arduino)
   ```
   ESP TX → Arduino RX (direto)
   ```

## Próximos Passos

Depois de identificar seu módulo:
1. Confirme a tensão (3.3V!)
2. Identifique os pinos TX, RX, VCC, GND
3. Use o código de teste apropriado
4. Configure baudrate correto

## Onde Encontrar Mais Informações

- **Datasheet do módulo** (procure no Google: "ESP8266 datasheet")
- **Pinout diagram** (procure: "ESP8266 pinout" ou "ESP32 pinout")
- Marque/anote o modelo exato encontrado no chip

---

**Consegue identificar algum texto no seu módulo? Tire uma foto ou me diga o que está escrito!**
