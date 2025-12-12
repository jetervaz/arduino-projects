# Arduino Projects

Repositório para projetos Arduino usando arduino-cli.

## Hardware Suportado

- Arduino Uno
- Arduino Nano

## Estrutura

Cada projeto terá sua própria pasta com o sketch Arduino.

## Uso

Para compilar um sketch:
```bash
arduino-cli compile --fqbn arduino:avr:uno nome-do-projeto/
```

Para fazer upload (substitua PORT pela porta correta):
```bash
arduino-cli upload -p PORT --fqbn arduino:avr:uno nome-do-projeto/
```

## Listar placas conectadas

```bash
arduino-cli board list
```
