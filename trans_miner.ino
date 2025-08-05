#include <RH_ASK.h>
#include <SPI.h>

const int buzzerPin = 9;
const int buttonPin = 8;            // Button pin
const int vibrationSensorPin = 7;    // Vibration sensor pin
const int gasSensorPin = A0;         // Gas sensor pin (analog)
const int gasThreshold = 300;        // Threshold value for gas detection

RH_ASK rf_driver;

void setup() {
  rf_driver.init();
  Serial.begin(9600);
  // Define sensor and button pins
  pinMode(buttonPin, INPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(gasSensorPin, INPUT);
  pinMode(buzzerPin,OUTPUT);
}

void loop() {
  // Check button state
  if (digitalRead(buttonPin) == HIGH) {
    const char *msg = "Button Pressed";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(1000);  // Debounce
  }
  
  // Check vibration sensor
  if (digitalRead(vibrationSensorPin) == HIGH) {
    const char *msg = "Vibration Detected";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(1000);  // Debounce
  }
  
  // Check gas sensor
  int gasLevel = analogRead(gasSensorPin);

  if (gasLevel > gasThreshold) {
    const char *msg = "Gas Detected";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(1000);  // Debounce
  }

  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    buf[buflen] = '\0';
    Serial.print("Received: ");
    Serial.println((char*)buf);
    if (strcmp((char*)buf, "Alert") == 0) {
      digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
      delay(1000);                    // Buzzer on for 2 seconds
      digitalWrite(buzzerPin, LOW);   // Turn off buzzer
    }
  }

  delay(100);  // Short delay before the next loop
}