# Módulo WiFi UART com Arduino Uno

Projeto para conectar e testar módulos WiFi UART (ESP8266, ESP32 ou similares) com Arduino Uno usando comunicação serial.

## Visão Geral

Este projeto permite:
- **Testar** módulos WiFi UART
- **Comunicar** via comandos AT
- **Conectar** Arduino à internet via WiFi
- **Aprender** comunicação serial UART

## ⚠️ Aviso Importante - Tensão

**Arduino Uno opera em 5V, mas ESP8266/ESP32 são 3.3V!**

- ✅ Use regulador de tensão 3.3V (AMS1117-3.3)
- ✅ Use divisor de tensão para RX do ESP
- ❌ NUNCA conecte 5V direto no ESP - vai queimar!

## Arquivos do Projeto

| Arquivo | Descrição |
|---------|-----------|
| `README.md` | Este arquivo - visão geral |
| `IDENTIFICACAO-MODULO.md` | Como identificar seu módulo WiFi |
| `CONEXOES.md` | Diagrama de conexões e pinout detalhado |
| `wifi-uart-test.ino` | Código Arduino para teste |

## Começando

### 1. Identifique Seu Módulo
Consulte `IDENTIFICACAO-MODULO.md` para:
- Descobrir se é ESP8266 ou ESP32
- Localizar os pinos corretos
- Entender a pinagem

### 2. Monte o Circuito
Consulte `CONEXOES.md` para:
- Esquema completo de conexões
- Montagem do regulador 3.3V
- Divisor de tensão
- Pinout detalhado

### 3. Upload do Código
```bash
# Compilar
arduino-cli compile --fqbn arduino:avr:uno wifi-uart-module/

# Upload
arduino-cli upload -p /dev/cu.usbmodem1301 --fqbn arduino:avr:uno wifi-uart-module/
```

### 4. Teste a Comunicação
1. Abra o Monitor Serial (9600 baud)
2. Configure "Both NL & CR" no Monitor Serial
3. Aguarde o teste automático
4. Digite comandos AT manualmente

## Componentes Necessários

| Item | Quantidade | Observações |
|------|------------|-------------|
| Arduino Uno | 1 | - |
| Módulo WiFi UART | 1 | ESP8266 ou ESP32 |
| Regulador AMS1117-3.3V | 1 | Essencial! |
| Resistor 1kΩ | 1 | Divisor de tensão |
| Resistor 2kΩ | 1 | Divisor de tensão |
| Capacitores 10µF e 100nF | 2 | Estabilização |
| Protoboard e jumpers | - | - |

## Conexões Básicas

### Resumo Rápido
```
Arduino 5V  → Regulador IN
Regulador OUT (3.3V) → ESP VCC
Arduino GND → ESP GND

Arduino Pin 3 (TX) → Divisor [1kΩ + 2kΩ] → ESP RX
Arduino Pin 2 (RX) ← ESP TX (direto)

3.3V → ESP CH_PD (ou EN)
```

**Veja `CONEXOES.md` para detalhes completos!**

## Comandos AT Básicos

### Testar Comunicação
```
AT              → Deve responder: OK
AT+GMR          → Versão do firmware
```

### WiFi (ESP8266)
```
AT+CWMODE=1              → Modo Station
AT+CWLAP                 → Listar redes WiFi
AT+CWJAP="SSID","SENHA"  → Conectar em rede
AT+CIFSR                 → Ver endereço IP
```

### Reset
```
AT+RST          → Reiniciar módulo
AT+RESTORE      → Configuração de fábrica
```

## Configuração de Baudrate

Módulos WiFi podem vir configurados em diferentes baudrates:
- **115200** (mais comum)
- **9600** (alguns módulos)
- **57600** (menos comum)

No código (`wifi-uart-test.ino`):
```cpp
const long WIFI_BAUDRATE = 115200;  // Ajuste aqui se necessário
```

## Exemplos de Uso

### 1. Conectar no WiFi
```
AT+CWMODE=1
AT+CWJAP="MinhaRede","minhaSenha123"
AT+CIFSR
```

### 2. Fazer Requisição HTTP
```
AT+CIPSTART="TCP","example.com",80
AT+CIPSEND=16
GET / HTTP/1.0

AT+CIPCLOSE
```

### 3. Criar Access Point
```
AT+CWMODE=2
AT+CWSAP="MeuArduino","senha123",5,3
```

## Solução de Problemas

### Sem Resposta do Módulo
**Causas:**
- Módulo não alimentado (verifique 3.3V)
- Baudrate incorreto (tente 9600 ou 115200)
- TX/RX invertidos
- CH_PD/EN não conectado em 3.3V

**Solução:**
1. Verifique tensão no VCC do ESP = 3.3V
2. Teste diferentes baudrates
3. Confirme TX → RX e RX → TX (cruzado)
4. Conecte CH_PD/EN em 3.3V

### Caracteres Estranhos/Lixo
**Causa:** Baudrate incorreto

**Solução:** Altere `WIFI_BAUDRATE` no código e teste:
```cpp
const long WIFI_BAUDRATE = 9600;    // Tente este
// ou
const long WIFI_BAUDRATE = 115200;  // Ou este
```

### Módulo Esquenta ou Cheira Queimado
**Causa:** Tensão incorreta (5V no ESP)

**Solução:**
1. Desligue IMEDIATAMENTE
2. Verifique todas as conexões
3. Confirme 3.3V no VCC
4. Módulo pode estar danificado

### Reset Constante
**Causa:** Corrente insuficiente

**Solução:**
- Use regulador externo (não pino 3.3V do Arduino)
- Verifique capacitores de estabilização
- Use fonte externa 3.3V se necessário

### "busy p..." ou "busy s..."
**Causa:** Módulo processando comando anterior

**Solução:**
- Aguarde alguns segundos
- Envie `AT+RST` para reiniciar
- Não envie comandos muito rápido

## Projetos Futuros

Com o módulo WiFi funcionando, você pode:
- Enviar dados de sensores para internet
- Controlar Arduino remotamente
- Criar servidor web
- Integrar com MQTT/APIs
- IoT (Internet of Things)

## Segurança

- Teste sempre com voltímetro antes de conectar ESP
- Use regulador de tensão adequado
- Não conecte 5V direto em pinos 3.3V
- Adicione fusível na alimentação para proteção
- Desconecte alimentação ao fazer alterações

## Recursos Adicionais

- **ESP8266 AT Commands:** [Documentação oficial](https://www.espressif.com/en/support/documents/technical-documents)
- **ESP32 AT Commands:** [GitHub Espressif](https://github.com/espressif/esp-at)
- **Tutorial Arduino + ESP8266:** Procure por "Arduino ESP8266 UART tutorial"

## Licença

Projeto open-source. Use e modifique livremente.

---

**Dúvidas? Consulte os arquivos detalhados:**
- `IDENTIFICACAO-MODULO.md` - Para identificar seu módulo
- `CONEXOES.md` - Para esquemas e pinout completos
