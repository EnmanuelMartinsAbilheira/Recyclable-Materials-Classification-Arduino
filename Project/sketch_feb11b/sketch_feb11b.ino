#include <tcs3200.h>

// Color sensor setup
int red = 0, green = 0, blue = 0, white = 0;
tcs3200 tcs(50, 51, 52, 53, 49);  // (S0, S1, S2, S3, output pin)

// Potentiometer and additional analog inputs setup
const int analogInPin1 = A0;  // First potentiometer
const int analogInPin2 = A1;  // Second analog input
const int analogInPin3 = A2;  // Third analog input
const int analogOutPin = 9;   // PWM output pin
int sensorValue1 = 0;         // Value from A0
int sensorValue2 = 0;         // Value from A1
int sensorValue3 = 0;         // Value from A2
int outputValue = 0;          // Value output to PWM

// Color sensor timing
unsigned long previousMillis = 0;
const long interval = 200;  // Update color every 200ms

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read and process analog inputs
  sensorValue1 = analogRead(analogInPin1);  // Read A0
  sensorValue2 = analogRead(analogInPin2);  // Read A1
  sensorValue3 = analogRead(analogInPin3);  // Read A2

  // Map A0 to PWM output (optional, if you still want to control the LED)
  outputValue = map(sensorValue1, 0, 1023, 0, 255);
  analogWrite(analogOutPin, outputValue);

  // Update color sensor values periodically
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    red = tcs.colorRead('r');
    green = tcs.colorRead('g');
    blue = tcs.colorRead('b');
    white = tcs.colorRead('c');
  }

  // Print all values to serial monitor
  //Serial.print("A0: ");
  Serial.print(sensorValue1);
  Serial.print(",");
  //Serial.print(" | A1: ");
  Serial.print(sensorValue2);
  Serial.print(",");
  //Serial.print(" | A2: ");
  Serial.print(sensorValue3);
  Serial.print(",");
  //Serial.print(" | R: ");
  Serial.print(red);
  Serial.print(",");
  //Serial.print(" G: ");
  Serial.print(green);
  Serial.print(",");
  //Serial.print(" B: ");
  Serial.print(blue);
  Serial.print(",");
  //Serial.print(" W: ");
  Serial.print(white);
  Serial.print(",");
  //Serial.print(" Label ");
  Serial.print("'Garrafa Agua 1.5L palstico Cheira'");
  //Serial.print(",");
  Serial.println();

  delay(20);  // Maintain responsiveness
}