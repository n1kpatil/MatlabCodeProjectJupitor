#include <Arduino_FreeRTOS.h>

// LED Pins
const int ledPins[4] = {13, 12, 11, 10};
int ledState[4] = {LOW, LOW, LOW, LOW};

// Analog input pins
const int analogPins[5] = {A0, A1, A2, A3, A4};

// Control flags
bool runTruth = false;

// Task handles
TaskHandle_t voltageTaskHandle;
TaskHandle_t truthTaskHandle;
TaskHandle_t serialTaskHandle;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(analogPins[i], INPUT);
  }

  // Create tasks
  xTaskCreate(voltageTask, "Voltage", 128, NULL, 1, &voltageTaskHandle);
  xTaskCreate(serialTask, "SerialCmd", 256, NULL, 2, &serialTaskHandle);
  xTaskCreate(truthTableTask, "TruthTable", 256, NULL, 1, &truthTaskHandle);
}

void loop() {
  // Not used in FreeRTOS
}

void sendZeroVoltages() {
  Serial.println("0.000,0.000,0.000,0.000");
}

// --- Task: Read and print analog voltages ---
void voltageTask(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    bool anyLedOn = false;
    for (int i = 0; i < 5; i++) {
      if (digitalRead(ledPins[i]) == HIGH) {
        anyLedOn = true;
        break;
      }
    }

    float voltages[5];

    if (anyLedOn) {
      for (int i = 0; i < 5; i++) {
        int analogValue = analogRead(analogPins[i]);
        voltages[i] = analogValue * (5.0 / 1023.0);
      }

      Serial.write((uint8_t *)voltages, sizeof(voltages));  // Send 16 bytes
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// --- Task: Handle serial commands ---
void serialTask(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');
      command.trim();

      if (command == "Logic1_ON") digitalWrite(13, HIGH);
      else if (command == "Logic1_OFF") {
        digitalWrite(13, LOW);
        sendZeroVoltages();
      }
      else if (command == "Logic2_ON") digitalWrite(12, HIGH);
      else if (command == "Logic2_OFF") {
        digitalWrite(12, LOW);
        sendZeroVoltages();
      }
      else if (command == "Logic3_ON") digitalWrite(11, HIGH);
      else if (command == "Logic3_OFF") {
        digitalWrite(11, LOW);
        sendZeroVoltages();
      }
      else if (command == "Logic4_ON") digitalWrite(10, HIGH);
      else if (command == "Logic4_OFF") {
        digitalWrite(10, LOW);
        sendZeroVoltages();
      }
      else if (command == "Truth_ON") runTruth = true;
      else if (command == "Truth_OFF") runTruth = false;
      else Serial.println("Unknown command: " + command);
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// --- Task: Display truth table ---
void truthTableTask(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    if (runTruth) {
      for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
          ledState[j] = (i >> j) & 1;
          digitalWrite(ledPins[j], ledState[j]);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait 1 second
      }

      // Reset LEDs after truth table completes
      for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      sendZeroVoltages();
      runTruth = false;
    }

    vTaskDelay(50 / portTICK_PERIOD_MS); // Check every 100ms
  }
}