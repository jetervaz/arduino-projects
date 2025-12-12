/*
 * Controle de Mesa Motorizada com Arduino
 *
 * Funcionalidades:
 * - Controle manual: subir/descer com botões
 * - 2 posições de memória programáveis
 * - Sensor ultrassônico para medir altura
 * - Sistema de inversão de polaridade com 2 relés
 *
 * Componentes:
 * - Arduino Uno/Nano
 * - HC-SR04 (sensor ultrassônico)
 * - 2x Relés 5V
 * - 1x Motor DC
 * - 4x Botões
 */

#include <EEPROM.h>

// ===== DEFINIÇÃO DE PINOS =====
// Sensor Ultrassônico
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// Botões
const int BTN_UP = 4;      // Subir manual
const int BTN_DOWN = 5;    // Descer manual
const int BTN_POS1 = 6;    // Ir para posição salva 1
const int BTN_POS2 = 7;    // Ir para posição salva 2

// Relés (controle do motor)
const int RELAY1 = 8;      // Controle polaridade positiva
const int RELAY2 = 9;      // Controle polaridade negativa

// ===== CONSTANTES =====
const float TOLERANCE = 2.0;        // Tolerância em cm para considerar posição atingida
const unsigned long DEBOUNCE = 50;   // Tempo de debounce dos botões (ms)
const int MAX_DISTANCE = 150;        // Distância máxima do sensor (cm)
const int MIN_DISTANCE = 60;         // Distância mínima segura (cm)
const unsigned long SAVE_HOLD_TIME = 3000; // Tempo para salvar posição (3 segundos)

// Endereços EEPROM para salvar posições
const int EEPROM_POS1 = 0;
const int EEPROM_POS2 = 4;

// ===== VARIÁVEIS GLOBAIS =====
float currentHeight = 0;
float savedPosition1 = 0;
float savedPosition2 = 0;

// Estados dos botões
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool buttonPressed[4] = {false, false, false, false};

// Tempo de pressionamento para salvar posição
unsigned long buttonHoldTime[2] = {0, 0}; // Para BTN_POS1 e BTN_POS2

// ===== ENUMERAÇÃO DE ESTADOS DO MOTOR =====
enum MotorState {
  STOPPED,
  MOVING_UP,
  MOVING_DOWN
};

MotorState motorState = STOPPED;

// ===== SETUP =====
void setup() {
  Serial.begin(9600);

  // Configurar pinos do sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configurar pinos dos botões (pull-up interno)
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_POS1, INPUT_PULLUP);
  pinMode(BTN_POS2, INPUT_PULLUP);

  // Configurar pinos dos relés
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  // Garantir que motor inicie parado
  stopMotor();

  // Carregar posições salvas da EEPROM
  loadPositions();

  // Ler altura inicial
  currentHeight = getDistance();

  Serial.println("=== Mesa Motorizada Iniciada ===");
  Serial.print("Altura atual: ");
  Serial.print(currentHeight);
  Serial.println(" cm");
  Serial.print("Posição 1 salva: ");
  Serial.print(savedPosition1);
  Serial.println(" cm");
  Serial.print("Posição 2 salva: ");
  Serial.print(savedPosition2);
  Serial.println(" cm");
  Serial.println("\nComandos:");
  Serial.println("- Botão 1: Subir manual");
  Serial.println("- Botão 2: Descer manual");
  Serial.println("- Botão 3: Ir para posição 1 (segurar 3s para salvar)");
  Serial.println("- Botão 4: Ir para posição 2 (segurar 3s para salvar)");
}

// ===== LOOP PRINCIPAL =====
void loop() {
  // Atualizar altura atual
  currentHeight = getDistance();

  // Processar botões
  handleButtons();

  // Verificar se atingiu posição alvo (quando em movimento automático)
  checkTargetReached();

  // Pequeno delay para estabilidade
  delay(50);
}

// ===== FUNÇÕES DE CONTROLE DO MOTOR =====
void stopMotor() {
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  motorState = STOPPED;
  Serial.println("Motor PARADO");
}

void moveUp() {
  // Verificar limites de segurança
  if (currentHeight >= MAX_DISTANCE) {
    stopMotor();
    Serial.println("LIMITE SUPERIOR atingido!");
    return;
  }

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, LOW);
  motorState = MOVING_UP;
}

void moveDown() {
  // Verificar limites de segurança
  if (currentHeight <= MIN_DISTANCE) {
    stopMotor();
    Serial.println("LIMITE INFERIOR atingido!");
    return;
  }

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, HIGH);
  motorState = MOVING_DOWN;
}

// ===== FUNÇÃO DO SENSOR ULTRASSÔNICO =====
float getDistance() {
  // Enviar pulso
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Ler eco
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30ms

  // Calcular distância em cm
  float distance = duration * 0.034 / 2;

  // Validar leitura
  if (distance == 0 || distance > 400) {
    Serial.println("AVISO: Leitura inválida do sensor");
    return currentHeight; // Retornar última leitura válida
  }

  return distance;
}

// ===== FUNÇÕES DE GERENCIAMENTO DE BOTÕES =====
void handleButtons() {
  // Ler estado dos botões
  bool btnUpState = digitalRead(BTN_UP);
  bool btnDownState = digitalRead(BTN_DOWN);
  bool btnPos1State = digitalRead(BTN_POS1);
  bool btnPos2State = digitalRead(BTN_POS2);

  unsigned long currentTime = millis();

  // Botão SUBIR manual
  if (btnUpState == LOW) {
    moveUp();
    Serial.print("Subindo manual - Altura: ");
    Serial.print(currentHeight);
    Serial.println(" cm");
  }
  // Botão DESCER manual
  else if (btnDownState == LOW) {
    moveDown();
    Serial.print("Descendo manual - Altura: ");
    Serial.print(currentHeight);
    Serial.println(" cm");
  }
  // Botão POSIÇÃO 1
  else if (btnPos1State == LOW) {
    handlePositionButton(0, savedPosition1, currentTime);
  }
  // Botão POSIÇÃO 2
  else if (btnPos2State == LOW) {
    handlePositionButton(1, savedPosition2, currentTime);
  }
  // Nenhum botão pressionado
  else {
    // Parar motor se estava em modo manual
    if (motorState != STOPPED && btnUpState == HIGH && btnDownState == HIGH) {
      // Só para se não estiver indo para posição automática
      stopMotor();
    }

    // Resetar contadores de hold time
    buttonHoldTime[0] = 0;
    buttonHoldTime[1] = 0;
  }
}

void handlePositionButton(int buttonIndex, float savedPos, unsigned long currentTime) {
  // Iniciar contador se primeira detecção
  if (buttonHoldTime[buttonIndex] == 0) {
    buttonHoldTime[buttonIndex] = currentTime;
  }

  unsigned long holdDuration = currentTime - buttonHoldTime[buttonIndex];

  // Se segurou por 3 segundos, salvar posição
  if (holdDuration >= SAVE_HOLD_TIME) {
    savePosition(buttonIndex);
    buttonHoldTime[buttonIndex] = 0; // Resetar para evitar múltiplos salvamentos
    stopMotor();
  }
  // Se soltou antes de 3 segundos, ir para posição salva
  else {
    goToPosition(savedPos);
  }
}

void goToPosition(float targetHeight) {
  if (targetHeight == 0) {
    Serial.println("Posição não configurada!");
    stopMotor();
    return;
  }

  float difference = targetHeight - currentHeight;

  // Verificar se já está na posição
  if (abs(difference) <= TOLERANCE) {
    stopMotor();
    Serial.println("Já está na posição alvo!");
    return;
  }

  // Mover na direção correta
  if (difference > 0) {
    moveUp();
    Serial.print("Indo para posição: ");
    Serial.print(targetHeight);
    Serial.print(" cm (Atual: ");
    Serial.print(currentHeight);
    Serial.println(" cm)");
  } else {
    moveDown();
    Serial.print("Indo para posição: ");
    Serial.print(targetHeight);
    Serial.print(" cm (Atual: ");
    Serial.print(currentHeight);
    Serial.println(" cm)");
  }
}

void checkTargetReached() {
  // Esta função pode ser expandida para verificar se atingiu a posição alvo
  // em modo automático e parar o motor
}

// ===== FUNÇÕES DE MEMÓRIA (EEPROM) =====
void savePosition(int positionIndex) {
  if (positionIndex == 0) {
    savedPosition1 = currentHeight;
    EEPROM.put(EEPROM_POS1, savedPosition1);
    Serial.print("Posição 1 SALVA: ");
    Serial.print(savedPosition1);
    Serial.println(" cm");
  } else {
    savedPosition2 = currentHeight;
    EEPROM.put(EEPROM_POS2, savedPosition2);
    Serial.print("Posição 2 SALVA: ");
    Serial.print(savedPosition2);
    Serial.println(" cm");
  }
}

void loadPositions() {
  EEPROM.get(EEPROM_POS1, savedPosition1);
  EEPROM.get(EEPROM_POS2, savedPosition2);

  // Validar valores lidos (evitar valores absurdos de EEPROM virgem)
  if (savedPosition1 < 0 || savedPosition1 > 400 || isnan(savedPosition1)) {
    savedPosition1 = 0;
  }
  if (savedPosition2 < 0 || savedPosition2 > 400 || isnan(savedPosition2)) {
    savedPosition2 = 0;
  }
}
