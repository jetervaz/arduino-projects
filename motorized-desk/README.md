# Mesa Motorizada com Arduino

Sistema de controle inteligente para mesa motorizada com altura ajustável, usando Arduino, sensor ultrassônico e controle por relés.

## Funcionalidades

- **Controle Manual**: Botões para subir e descer a mesa manualmente
- **Memória de Posições**: 2 posições programáveis que podem ser salvas e acessadas rapidamente
- **Sensor de Altura**: Sensor ultrassônico HC-SR04 para medir a distância até o chão
- **Segurança**: Limites de altura configuráveis (mínimo e máximo)
- **Inversão de Polaridade**: Sistema com 2 relés para controlar direção do motor DC

## Componentes Necessários

| Componente | Quantidade | Observações |
|------------|------------|-------------|
| Arduino Uno ou Nano | 1 | - |
| Motor DC | 1 | Conforme especificação da mesa (geralmente 12V) |
| Módulo Relé 5V | 2 | Com contatos NA/NF |
| Sensor HC-SR04 | 1 | Sensor ultrassônico |
| Botões Push Button | 4 | Normalmente aberto |
| Resistores 10kΩ | 4 | Para pull-down (ou usar pull-up interno) |
| Diodo 1N4007 | 1 | Proteção do motor |
| Fonte 12V | 1 | Para alimentar o motor (corrente adequada) |
| Protoboard | 1 | Para montagem |
| Jumpers | - | Diversos |

## Pinagem

### Sensor Ultrassônico HC-SR04
- **VCC** → 5V
- **TRIG** → Pino Digital 2
- **ECHO** → Pino Digital 3
- **GND** → GND

### Botões
- **Subir Manual** → Pino Digital 4
- **Descer Manual** → Pino Digital 5
- **Posição 1** → Pino Digital 6
- **Posição 2** → Pino Digital 7

Todos os botões conectados entre o pino e GND (pull-up interno ativado)

### Relés
- **Relé 1 (IN)** → Pino Digital 8
- **Relé 2 (IN)** → Pino Digital 9
- **VCC** → 5V
- **GND** → GND

## Montagem do Circuito

Consulte o arquivo `CIRCUITO.md` para diagrama detalhado e `FRITZING-CONNECTIONS.txt` para montagem no Fritzing.

### Configuração dos Relés (Inversão de Polaridade)

Os relés formam uma ponte H simplificada que inverte a polaridade do motor:

**Para Subir (Relé1=HIGH, Relé2=LOW)**
```
+12V → Relé1 NA → Motor+
GND  → Relé2 COM → Motor-
```

**Para Descer (Relé1=LOW, Relé2=HIGH)**
```
+12V → Relé1 NF → Motor-
GND  → Relé2 NA → Motor+
```

**Parado (Ambos=LOW)**
```
Motor desconectado da alimentação
```

## Instalação do Código

1. Abra o arquivo `motorized-desk.ino` no Arduino IDE ou use arduino-cli
2. Selecione sua placa (Arduino Uno ou Nano)
3. Selecione a porta COM correta
4. Faça o upload

### Usando arduino-cli:

```bash
# Compilar
arduino-cli compile --fqbn arduino:avr:uno motorized-desk/

# Upload (substitua a porta)
arduino-cli upload -p /dev/cu.usbmodem1301 --fqbn arduino:avr:uno motorized-desk/
```

## Como Usar

### Controle Manual
- Pressione **Botão 1** para subir a mesa
- Pressione **Botão 2** para descer a mesa
- Solte o botão para parar

### Salvar Posições
1. Ajuste a mesa manualmente para a altura desejada
2. **Segure** o botão de posição (3 ou 4) por **3 segundos**
3. A posição será salva na memória permanente (EEPROM)

### Ir para Posição Salva
- **Pressione rapidamente** o botão de posição (3 ou 4)
- A mesa irá automaticamente para a altura salva

## Ajustes no Código

Você pode modificar os seguintes parâmetros no código conforme necessário:

```cpp
const float TOLERANCE = 2.0;        // Tolerância em cm (quanto pode variar da posição alvo)
const int MAX_DISTANCE = 150;       // Altura máxima permitida (cm do chão)
const int MIN_DISTANCE = 60;        // Altura mínima permitida (cm do chão)
const unsigned long SAVE_HOLD_TIME = 3000; // Tempo para salvar posição (ms)
```

## Monitor Serial

Abra o Monitor Serial (115200 baud) para ver:
- Altura atual da mesa
- Posições salvas
- Status do motor
- Avisos de segurança

## Segurança

- O código implementa limites de altura mínima e máxima
- **NUNCA** os dois relés são ativados simultaneamente (previne curto-circuito)
- Adicione fusível na linha de alimentação do motor
- Use diodo de proteção em paralelo com o motor
- Teste o circuito sem carga antes de conectar o motor real
- Verifique todas as conexões antes de ligar

## Solução de Problemas

### Motor não se move
- Verifique conexões dos relés
- Verifique alimentação do motor (12V presente?)
- Teste relés individualmente (ouça o "click")

### Sensor não funciona
- Verifique conexões TRIG e ECHO
- Verifique se sensor está voltado para o chão
- Distância deve estar entre 2cm e 400cm

### Posições não salvam
- Verifique se está segurando botão por 3 segundos completos
- Monitor serial mostrará mensagem de confirmação

### Motor gira na direção errada
- Inverta os fios do motor OU
- Troque os valores HIGH/LOW nos relés no código

## Melhorias Futuras

- Adicionar display LCD para mostrar altura
- Implementar controle via Bluetooth/WiFi
- Adicionar mais posições de memória
- Sistema de detecção de obstáculos
- Controle de velocidade (PWM)

## Licença

Projeto open-source. Use e modifique livremente.
