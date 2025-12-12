# Circuito - Mesa Motorizada com Arduino

## Componentes Necessários

- 1x Arduino Uno ou Nano
- 1x Motor DC (12V ou conforme sua mesa)
- 2x Relés 5V com contatos NA/NF (para inversão de polaridade)
- 1x Sensor Ultrassônico HC-SR04
- 4x Botões (push button)
- 4x Resistores 10kΩ (pull-down para os botões)
- 1x Fonte de alimentação para o motor (12V ou conforme motor)
- 1x Diodo 1N4007 (proteção contra pico de tensão do motor)
- Jumpers e protoboard

## Diagrama de Conexões

### Sensor Ultrassônico HC-SR04
```
HC-SR04          Arduino
--------         -------
VCC      --->    5V
TRIG     --->    Pino 2
ECHO     --->    Pino 3
GND      --->    GND
```

### Botões
```
Botão            Arduino      Função
-----            -------      ------
BTN1     --->    Pino 4       Subir Manual
BTN2     --->    Pino 5       Descer Manual
BTN3     --->    Pino 6       Ir para Posição 1 (Salva)
BTN4     --->    Pino 7       Ir para Posição 2 (Salva)

Cada botão conectado entre o pino e GND
Resistor pull-up interno ativado no código
```

### Relés (Ponte H para inversão de polaridade)
```
Relé 1           Arduino
------           -------
IN       --->    Pino 8
VCC      --->    5V
GND      --->    GND

Relé 2           Arduino
------           -------
IN       --->    Pino 9
VCC      --->    5V
GND      --->    GND
```

### Configuração dos Relés para Controle do Motor

```
Fonte +12V ----+
               |
            [Relé 1 COM]
               |
         +-----+-----+
         |           |
    [Relé 1 NA]  [Relé 1 NF]
         |           |
         |           +--------> Motor Terminal 1
         |
         +-------------------> Motor Terminal 2

Fonte GND -----+
               |
            [Relé 2 COM]
               |
         +-----+-----+
         |           |
    [Relé 2 NA]  [Relé 2 NF]
         |           |
         |           +--------> Motor Terminal 2
         |
         +-------------------> Motor Terminal 1
```

## Estados dos Relés

| Relé 1 | Relé 2 | Resultado        |
|--------|--------|------------------|
| LOW    | LOW    | Motor Parado     |
| HIGH   | LOW    | Motor Sobe       |
| LOW    | HIGH   | Motor Desce      |
| HIGH   | HIGH   | EVITAR (curto)   |

## Diagrama Completo

```
                    +5V
                     |
                 [Arduino]
                     |
    +----------------+----------------+
    |                |                |
[HC-SR04]      [4x Botões]      [2x Relés]
    |                                 |
    |                          [Motor DC] <--- Fonte 12V
    |                                 |
    +----------------+----------------+
                     |
                    GND
```

## Notas de Segurança

1. **Importante:** Nunca ativar ambos relés simultaneamente (evita curto-circuito)
2. Adicione diodo 1N4007 em paralelo com o motor (proteção contra EMF reversa)
3. Use fonte adequada para o motor (corrente suficiente)
4. Verifique a tensão do motor antes de conectar
5. Considere adicionar fusível na linha de alimentação do motor
