# Conexões - Módulo WiFi UART com Arduino Uno

## ⚠️ AVISO CRÍTICO - TENSÃO

**Arduino Uno = 5V | ESP8266/ESP32 = 3.3V**

**NUNCA** conecte 5V diretamente nos pinos do ESP! Isso pode **QUEIMAR** o módulo!

## Opções de Alimentação

### Opção 1: Usar 3.3V do Arduino (Corrente Limitada)
```
Arduino 3.3V → ESP VCC
Arduino GND  → ESP GND
```
⚠️ **Problema:** Pino 3.3V do Arduino fornece apenas ~50mA. ESP pode precisar de 200-300mA!
✓ **Funciona para:** Testes básicos sem WiFi ativo
✗ **Não funciona para:** WiFi ativo, transmissão de dados

### Opção 2: Regulador de Tensão 3.3V (RECOMENDADO)
```
Arduino 5V → [AMS1117-3.3] → ESP VCC
                    |
Arduino GND ←-------+-----→ ESP GND
```
✓ **Vantagem:** Fornece corrente suficiente (800mA+)
✓ **Componente:** Regulador AMS1117-3.3 ou LM1117-3.3 (~R$2-5)

### Opção 3: Fonte Externa 3.3V
```
Fonte 3.3V → ESP VCC
GND comum  → ESP GND + Arduino GND
```
✓ **Melhor opção** para uso contínuo/produção

## Conexões UART com Divisor de Tensão

### Esquema Completo

```
                    ARDUINO UNO                    MÓDULO WiFi (3.3V)
                    -----------                    ------------------

                    [  5V  ] ----+
                                 |
                            [Regulador]
                              3.3V |
                                   +-------------> VCC
                                   |
                    [ GND  ] ------+-------------> GND
                                   |
                    [Pin 3 ] ------+--[1kΩ]--+---> RX (com divisor!)
                    (TX)           |         |
                                   |      [2kΩ]
                                   |         |
                                   +---------+

                    [Pin 2 ] <-------------------- TX (conexão direta)
                    (RX)

                    [ 3.3V ] -----------------+---> CH_PD/EN (ESP8266)
                    (do reg.)                 |    ou EN (ESP32)
                                              |
                    [ 3.3V ] -----------------+---> GPIO0 (HIGH - modo normal)
                    (opcional)
```

### Divisor de Tensão para RX do ESP

**POR QUE PRECISA?**
- Arduino TX envia 5V
- ESP RX aceita máximo 3.3V
- Divisor reduz 5V → 3.3V

**CÁLCULO:**
```
Vout = Vin × (R2 / (R1 + R2))
3.3V = 5V × (2kΩ / (1kΩ + 2kΩ))
3.3V = 5V × 0.66 ✓
```

**MONTAGEM:**
```
Arduino Pin 3 (TX) ----[R1=1kΩ]----+----[R2=2kΩ]---- GND
                                   |
                              ESP RX (3.3V)
```

**Valores alternativos:**
- 1kΩ + 2kΩ (recomendado)
- 2.2kΩ + 3.3kΩ
- 4.7kΩ + 10kΩ

## Pinout Detalhado

### ESP8266 (ESP-01)
```
        +---------+
3.3V ---|VCC   TX |--- Arduino Pin 2 (RX)
        |         |
GND  ---|GND   RX |--- Divisor de tensão ← Arduino Pin 3 (TX)
        |         |
GND  ---|GPIO0 CH_|--- 3.3V (enable)
        |      PD |
GND  ---|GPIO2 RST|--- (não conectado ou 3.3V)
        +---------+
```

### ESP8266 (ESP-12E/F - Mais Pinos)
```
Pino     Função          Conexão Arduino
----     ------          ---------------
VCC      Alimentação     3.3V (via regulador)
GND      Ground          GND
TX       Transmite       Arduino Pin 2 (RX)
RX       Recebe          Divisor de tensão ← Arduino Pin 3 (TX)
CH_PD    Chip Enable     3.3V (sempre HIGH)
RST      Reset           3.3V ou botão para GND
GPIO0    Boot Mode       3.3V (HIGH = modo normal, LOW = flash)
GPIO2    Boot Mode       3.3V ou não conectado
GPIO15   Boot Mode       GND (sempre LOW)
```

### ESP32 (DevKit - Simplificado)
```
Pino     Função          Conexão Arduino
----     ------          ---------------
3V3      Alimentação     3.3V (via regulador)
GND      Ground          GND
TXD      Transmite       Arduino Pin 2 (RX)
RXD      Recebe          Divisor de tensão ← Arduino Pin 3 (TX)
EN       Enable          3.3V (sempre HIGH)
GPIO0    Boot            3.3V (não usar para flash)
```

## Lista de Materiais

| Componente | Quantidade | Observações |
|------------|------------|-------------|
| Arduino Uno | 1 | - |
| Módulo WiFi (ESP8266/ESP32) | 1 | - |
| Regulador AMS1117-3.3V | 1 | Ou LM1117-3.3 |
| Resistor 1kΩ | 1 | Para divisor de tensão |
| Resistor 2kΩ | 1 | Para divisor de tensão |
| Capacitor 10µF | 1 | Estabilização do regulador |
| Capacitor 100nF | 1 | Filtro de ruído |
| Protoboard | 1 | Para montagem |
| Jumpers | - | Diversos |

## Montagem Passo a Passo

### 1. Alimentação (Regulador 3.3V)
```
1. Coloque regulador AMS1117-3.3 na protoboard
2. Conecte entrada (IN) ao 5V do Arduino
3. Conecte saída (OUT) ao barramento 3.3V da protoboard
4. Conecte GND do regulador ao GND
5. Adicione capacitor 10µF entre OUT e GND
6. Adicione capacitor 100nF entre OUT e GND
```

### 2. Divisor de Tensão
```
1. Conecte resistor 1kΩ entre Arduino Pin 3 e linha comum
2. Conecte resistor 2kΩ entre linha comum e GND
3. Ponto comum vai para RX do ESP
```

### 3. Conexões ESP
```
1. ESP VCC    → Barramento 3.3V
2. ESP GND    → GND
3. ESP TX     → Arduino Pin 2 (direto)
4. ESP RX     → Ponto comum do divisor
5. ESP CH_PD  → 3.3V (ESP8266)
   ou ESP EN  → 3.3V (ESP32)
```

## Verificação Antes de Ligar

- [ ] Regulador conectado corretamente (5V → 3.3V)
- [ ] GND comum entre Arduino e ESP
- [ ] Divisor de tensão montado (1kΩ + 2kΩ)
- [ ] RX do ESP no ponto do divisor (NÃO direto no Arduino!)
- [ ] TX do ESP direto no Arduino Pin 2
- [ ] CH_PD/EN do ESP em 3.3V
- [ ] Nenhuma conexão 5V direta no ESP

## Teste de Continuidade

Com multímetro:
1. Verifique tensão no VCC do ESP = 3.3V (não 5V!)
2. Verifique GND comum
3. Com Arduino enviando HIGH no Pin 3, meça divisor = ~3.3V

## Problemas Comuns

| Problema | Causa Provável | Solução |
|----------|----------------|---------|
| ESP não liga | Sem alimentação 3.3V | Verificar regulador |
| ESP esquenta | Conectou 5V direto! | Desligar IMEDIATAMENTE |
| Sem comunicação | TX/RX invertidos | Trocar conexões |
| Caracteres estranhos | Baudrate errado | Tentar 9600 ou 115200 |
| Reset constante | Corrente insuficiente | Usar regulador externo |
| Sem resposta | CH_PD/EN não conectado | Conectar em 3.3V |

## Próximo Passo

Depois de montar:
1. Upload do código `wifi-uart-test.ino`
2. Abrir Monitor Serial (9600 baud)
3. Configurar "Both NL & CR" no Monitor Serial
4. Testar com comando "AT"
