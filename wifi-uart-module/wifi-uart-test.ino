/*
 * Teste de Módulo WiFi UART com Arduino Uno
 *
 * Este código permite testar comunicação com módulos WiFi UART
 * como ESP8266, ESP32 ou similares usando comandos AT
 *
 * Funciona como passthrough serial:
 * - O que você digita no Monitor Serial vai para o módulo WiFi
 * - O que o módulo WiFi responde aparece no Monitor Serial
 *
 * Hardware:
 * - Arduino Uno
 * - Módulo WiFi UART (ESP8266/ESP32)
 * - Divisor de tensão para RX do módulo
 * - Regulador 3.3V (se necessário)
 */

#include <SoftwareSerial.h>

// ===== CONFIGURAÇÃO DOS PINOS =====
// Ajuste conforme sua conexão
const int ESP_RX = 2;  // Pino conectado ao TX do módulo WiFi
const int ESP_TX = 3;  // Pino conectado ao RX do módulo WiFi (via divisor de tensão!)

// ===== CONFIGURAÇÃO DE BAUDRATE =====
// Tente diferentes valores se não funcionar:
// ESP8266: geralmente 115200 ou 9600
// ESP32: geralmente 115200
const long WIFI_BAUDRATE = 115200;

// Criar objeto SoftwareSerial para comunicação com módulo WiFi
SoftwareSerial wifiSerial(ESP_RX, ESP_TX);

// ===== SETUP =====
void setup() {
  // Iniciar comunicação serial com PC (Monitor Serial)
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguardar porta serial conectar (necessário para USB nativo)
  }

  Serial.println("=== Teste de Módulo WiFi UART ===");
  Serial.println();
  Serial.println("Iniciando comunicação com módulo WiFi...");
  Serial.print("Baudrate: ");
  Serial.println(WIFI_BAUDRATE);
  Serial.println();

  // Iniciar comunicação serial com módulo WiFi
  wifiSerial.begin(WIFI_BAUDRATE);

  delay(1000);

  Serial.println("===== COMANDOS AT BÁSICOS =====");
  Serial.println("AT              - Teste de comunicação");
  Serial.println("AT+GMR          - Versão do firmware");
  Serial.println("AT+CWMODE?      - Ver modo WiFi (ESP8266)");
  Serial.println("AT+CWMODE=1     - Modo Station (ESP8266)");
  Serial.println("AT+CWLAP        - Listar redes WiFi");
  Serial.println("AT+CWJAP=\"SSID\",\"SENHA\" - Conectar WiFi");
  Serial.println("AT+CIFSR        - Ver IP obtido");
  Serial.println("AT+RST          - Reset do módulo");
  Serial.println();
  Serial.println("===== TESTE AUTOMÁTICO =====");

  // Teste automático inicial
  delay(500);
  testWiFiModule();

  Serial.println();
  Serial.println("===== MODO PASSTHROUGH =====");
  Serial.println("Digite comandos AT no Monitor Serial");
  Serial.println("Certifique-se de configurar 'Both NL & CR' no Monitor Serial");
  Serial.println();
}

// ===== LOOP PRINCIPAL =====
void loop() {
  // Passthrough bidirecional:

  // Se receber dados do Monitor Serial, enviar para módulo WiFi
  if (Serial.available()) {
    char c = Serial.read();
    wifiSerial.write(c);
  }

  // Se receber dados do módulo WiFi, mostrar no Monitor Serial
  if (wifiSerial.available()) {
    char c = wifiSerial.read();
    Serial.write(c);
  }
}

// ===== FUNÇÕES AUXILIARES =====

void testWiFiModule() {
  Serial.println("Enviando comando AT...");

  // Limpar buffer
  while (wifiSerial.available()) {
    wifiSerial.read();
  }

  // Enviar comando AT básico
  wifiSerial.println("AT");

  // Aguardar resposta (timeout 2 segundos)
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < 2000) {
    if (wifiSerial.available()) {
      char c = wifiSerial.read();
      response += c;
      Serial.write(c);  // Mostrar em tempo real
    }
  }

  Serial.println();

  // Verificar resposta
  if (response.indexOf("OK") >= 0) {
    Serial.println("✓ Módulo WiFi respondeu!");
    Serial.println();

    // Tentar obter versão
    Serial.println("Obtendo versão do firmware...");
    wifiSerial.println("AT+GMR");

    startTime = millis();
    while (millis() - startTime < 2000) {
      if (wifiSerial.available()) {
        Serial.write(wifiSerial.read());
      }
    }
    Serial.println();

  } else if (response.length() == 0) {
    Serial.println("✗ Sem resposta do módulo WiFi!");
    Serial.println();
    Serial.println("Possíveis problemas:");
    Serial.println("1. Módulo não está alimentado (verifique 3.3V)");
    Serial.println("2. Baudrate incorreto (tente 9600 ou 115200)");
    Serial.println("3. Conexões TX/RX invertidas");
    Serial.println("4. Pino CH_PD/EN não conectado em 3.3V (ESP8266)");
    Serial.println("5. Divisor de tensão incorreto");
    Serial.println();

  } else {
    Serial.println("? Resposta inesperada:");
    Serial.println(response);
    Serial.println();
    Serial.println("Tente enviar comandos AT manualmente");
    Serial.println();
  }
}

/*
 * ===== EXEMPLOS DE USO =====
 *
 * 1. CONECTAR A UMA REDE WIFI (ESP8266):
 *    AT+CWMODE=1
 *    AT+CWJAP="NomeDaRede","SenhaDaRede"
 *    AT+CIFSR
 *
 * 2. CRIAR ACCESS POINT (ESP8266):
 *    AT+CWMODE=2
 *    AT+CWSAP="MeuAP","senha123",5,3
 *
 * 3. FAZER REQUISIÇÃO HTTP:
 *    AT+CIPSTART="TCP","example.com",80
 *    AT+CIPSEND=16
 *    GET / HTTP/1.0\r\n\r\n
 *    AT+CIPCLOSE
 *
 * 4. VERIFICAR STATUS:
 *    AT+CWMODE?     - Modo atual
 *    AT+CWJAP?      - WiFi conectado
 *    AT+CIFSR       - Endereço IP
 *    AT+CIPSTATUS   - Status de conexão
 *
 * 5. RESTAURAR CONFIGURAÇÕES:
 *    AT+RESTORE     - Reset para configuração de fábrica
 *
 * ===== TROUBLESHOOTING =====
 *
 * Problema: Caracteres estranhos/lixo no Serial
 * Solução: Baudrate incorreto. Tente 9600, 57600 ou 115200
 *
 * Problema: Nenhuma resposta
 * Solução:
 *   - Verifique alimentação 3.3V
 *   - Verifique TX/RX não estão invertidos
 *   - ESP8266: Conecte CH_PD em 3.3V
 *   - ESP32: Conecte EN em 3.3V
 *
 * Problema: "busy p..." ou "busy s..."
 * Solução: Módulo processando. Aguarde ou envie AT+RST
 *
 * Problema: "ERROR"
 * Solução: Comando inválido ou sintaxe incorreta
 *
 * Problema: Não conecta no WiFi
 * Solução:
 *   - Verifique SSID e senha corretos
 *   - Rede deve ser 2.4GHz (não 5GHz)
 *   - Tente AT+CWLAP para ver redes disponíveis
 */
