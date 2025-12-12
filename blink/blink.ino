// Exemplo básico Blink
// Pisca o LED integrado do Arduino

void setup() {
  // Inicializa o pino do LED integrado como saída
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // Liga o LED
  delay(1000);                      // Aguarda 1 segundo
  digitalWrite(LED_BUILTIN, LOW);   // Desliga o LED
  delay(1000);                      // Aguarda 1 segundo
}
