bool runTruth = true;
const int ledPins[4] = {13, 12, 11, 10}; // Pins connected to the LEDs
int ledState[4] = {LOW, LOW, LOW, LOW};
unsigned long previousMillis = 0; // To manage timing
const long interval = 100;

void setup() {
  Serial.begin(9600); // Start serial communication
  pinMode(13, OUTPUT); // LED on pin 13
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void truthTable(){
  if (runTruth = true){ 
    for (int i = 0; i < 16; i++) {
    // Convert the number `i` to a binary representation and store it in ledState
    for (int j = 0; j < 4; j++) {
      ledState[j] = (i >> j) & 1;  // Extract each bit of `i` and store it in ledState
      digitalWrite(ledPins[j], ledState[j]);  // Set the LED state accordingly
    }   
    // Print the current state of the LEDs to the serial monitor as a truth table    
    delay(1000); // Wait 1 second before moving to the next combination
   }
     runTruth = false;
     digitalWrite(13, LOW);
     digitalWrite(12, LOW);
     digitalWrite(11, LOW);
     digitalWrite(10, LOW);
     loop();
  }
}
void loop() {
  unsigned long currentMillis = millis();
  
  // Check if it's time to read the voltage
  if (currentMillis - previousMillis >= interval) {
    // Save the last time you read the voltage
    previousMillis = currentMillis;
    
    // Read the analog voltage
    int analogValue1 = analogRead(A0);
    int analogValue2 = analogRead(A1);
    int analogValue3 = analogRead(A2);
    int analogValue4 = analogRead(A3);
    
    // Convert to voltage (5V max for Arduino Uno)
    float voltage1 = analogValue1 * (5.0 / 1023.0);
    float voltage2 = analogValue2 * (5.0 / 1023.0);
    float voltage3 = analogValue3 * (5.0 / 1023.0);
    float voltage4 = analogValue4 * (5.0 / 1023.0);

    Serial.print(voltage1, 3);  
    Serial.print(',');          
    Serial.print(voltage2, 3);  
    Serial.print(',');
    Serial.print(voltage3, 3);
    Serial.print(',');
    Serial.println(voltage4, 3);

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read until newline

    if (command == "Logic1_ON") {
      digitalWrite(13, HIGH);
    } 
    else if (command == "Logic1_OFF") {
      digitalWrite(13, LOW);
    } 
    else if (command == "Logic2_ON"){
      digitalWrite(12, HIGH);
    }
    else if (command == "Logic2_OFF"){
      digitalWrite(12, LOW);
    }
    else if (command == "Logic3_ON"){
      digitalWrite(11, HIGH);
    }
    else if (command == "Logic3_OFF"){
      digitalWrite(11, LOW);
    }
    else if (command == "Logic4_ON"){
      digitalWrite(10, HIGH);
    }
    else if (command == "Logic4_OFF"){
      digitalWrite(10, LOW);
    }
    else if (command == "Truth_ON"){
      truthTable();
    }
    else if (command == "Truth_OFF"){
    }
    else {
      Serial.println("Unknown command: " + command);
    }
  }
  }
}


