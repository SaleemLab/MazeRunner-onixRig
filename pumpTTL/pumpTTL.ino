// Define the pins for both pumps
const int pumpLPin = 7; // Pump L
const int pumpRPin = 9; // Pump R

void setup() {
  // Start both pins as INPUT (high impedance) to keep the pumps off initially
  pinMode(pumpLPin, INPUT);
  pinMode(pumpRPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Send commands like 'l100' or 'r500' to control pumps.");
}

void loop() {
  if (Serial.available() > 0) {
    // Read the first character to determine which pump to trigger
    char pumpSelect = Serial.read();
    
    // Check if the character corresponds to a valid pump command
    if (pumpSelect == 'l' || pumpSelect == 'L' || pumpSelect == 'r' || pumpSelect == 'R') {
      
      // Look for the next valid integer in the serial buffer (the duration)
      int duration = Serial.parseInt();
      
      // Only trigger if a valid duration greater than 0 was sent
      if (duration > 0) {
        if (pumpSelect == 'l' || pumpSelect == 'L') {
          triggerPump(pumpLPin, duration);
        } else if (pumpSelect == 'r' || pumpSelect == 'R') {
          triggerPump(pumpRPin, duration);
        }
      }
    }
  }
}

// Modified trigger function that accepts the specific pin and duration
void triggerPump(int pin, int ms) {
  // Pull the pin to GND to trigger the pump
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  
  // Keep it open for the specified duration
  delay(ms);

  // Return to INPUT (high impedance) so that the pump turns off/resets
  pinMode(pin, INPUT);
  
  Serial.print("Triggered pin ");
  Serial.print(pin);
  Serial.print(" for ");
  Serial.print(ms);
  Serial.println(" ms");
}