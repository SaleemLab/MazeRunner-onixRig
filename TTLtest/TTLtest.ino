const int ledPin = 8; // LED pin
const int ledPin2 = 9; // LED pin

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(9600);
  Serial.println("Starting LED sequence...");
  Serial.println("Type 'X' to stop, 'Y' to restart.");
}

void loop() {
  // Initial sequence: 3s ON, 2s OFF (twice), then 3s ON
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    delay(3000);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    delay(2000);
  }

  // Third 3s ON
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  delay(3000);

  // Continuous loop: 1s ON, 2s OFF with keyboard interrupt
  while (true) {
    // Check for serial input
    if (Serial.available() > 0) {
      char command = Serial.read();
      if (command == 'x' || command == 'X') {
        Serial.println("Interrupt received. Stopping program...");
        digitalWrite(ledPin, LOW); // Turn off LED
        digitalWrite(ledPin2, LOW); // Turn off LED
        // Wait here until 'Y' is pressed
        while (true) {
          if (Serial.available() > 0) {
            char restartCmd = Serial.read();
            if (restartCmd == 'y' || restartCmd == 'Y') {
              Serial.println("Restart command received. Restarting sequence...");
              return; // Exit loop() and restart from the top
            }
          }
        }
      }
    }

    // LED pattern
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    delay(2000); // 2 seconds OFF
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    delay(750); // 750 ms ON
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
    delay(250); // 250 mseconds OFF
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin2, HIGH);
    delay(500); // 500 ms ON
    
  }
}
