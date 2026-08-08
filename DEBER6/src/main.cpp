#include <Arduino.h>

#define PIN_LED_R 25
#define PIN_LED_G 26
#define PIN_LED_B 27
#define PIN_BOTON_DESPERTAR GPIO_NUM_4

#define TIEMPO_SUENO_US 10000000ULL // 10 segundos

int conteoDespertares = 0;

void configurarPines() {
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    
    // Configuración del botón para el despertar externo
    pinMode(PIN_BOTON_DESPERTAR, INPUT_PULLUP);
}

void colorLED(bool r, bool g, bool b) {
    digitalWrite(PIN_LED_R, r ? HIGH : LOW);
    digitalWrite(PIN_LED_G, g ? HIGH : LOW);
    digitalWrite(PIN_LED_B, b ? HIGH : LOW);
}

void tareaActiva() {
    Serial.println("\n======================================");
    Serial.printf("Despertar número: %d\n", ++conteoDespertares);
    
    // En Wokwi, la simulación depende del temporizador para despertar del Light Sleep
    // En hardware real, ext0 también capturaría el botón instantáneamente.
    if (conteoDespertares > 1) {
        Serial.println("Causa de despertar: Evento de despertar (Temporizador/Botón)");
    } else {
        Serial.println("Causa de despertar: Encendido inicial");
    }
    
    Serial.println("Iniciando tarea activa...");
    
    // Indicador visual de encendido (Azul)
    colorLED(false, false, true);
    delay(1000);
    
    // Simular procesamiento de datos
    for (int i = 0; i < 3; i++) {
        colorLED(false, true, false); // Verde
        Serial.println("Procesando datos...");
        delay(500);
        colorLED(false, false, false);
        delay(500);
    }
    
    Serial.println("Tarea finalizada. Preparando para modo de bajo consumo.");
}

void setup() {
    Serial.begin(115200);
    delay(1000); 
    
    configurarPines();
    Serial.println("\n--- SISTEMA DE GESTIÓN DE ENERGÍA INICIADO ---");
}

void loop() {
    // 1. Ejecutar la tarea activa (simulación de carga de trabajo)
    tareaActiva();
    
    // 2. Transición al modo de ahorro (Indicador Rojo)
    colorLED(true, false, false);
    delay(1000);
    colorLED(false, false, false); // Apagar LED para máximo ahorro

    // ------------------------------------------------------------------
    // REQUISITOS DE LA TAREA: CONFIGURACIÓN EXPLÍCITA DE DESPERTARES
    // ------------------------------------------------------------------
    
    // Configuración 1: Despertar por Temporizador
    esp_sleep_enable_timer_wakeup(TIEMPO_SUENO_US);
    Serial.printf("Configurado para despertar por temporizador en %llu segundos.\n", TIEMPO_SUENO_US / 1000000);
    
    // Configuración 2: Despertar por Pin Externo (Botón)
    esp_sleep_enable_ext0_wakeup(PIN_BOTON_DESPERTAR, 0);
    Serial.println("Configurado para despertar por botón externo (GPIO4).");

    Serial.println("Entrando en Light Sleep (Ahorro de energía)...");
    Serial.println("ZZZzzz...");
    delay(100); 
    
    // 3. Iniciar modo de ahorro de energía (Light Sleep)
    // Se elige Light Sleep porque es estable en simulación y conserva el estado de ejecución
    esp_light_sleep_start();
    
    // Al despertar, la CPU reanuda la ejecución inmediatamente desde aquí
    Serial.println("\n¡Despertando del modo de ahorro!");
}