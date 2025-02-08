// int sensorValue = analogRead(34); // GPIO34
// Serial.println(sensorValue);

#define S0 16
#define S1 17
#define S2 18
#define S3 19
#define OUT 34

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  Serial.begin(115200);
}

void loop() {
  // Select Red filter
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  int redFrequency = pulseIn(OUT, LOW);

  // Select Green filter
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  int greenFrequency = pulseIn(OUT, LOW);

  // Select Blue filter
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  int blueFrequency = pulseIn(OUT, LOW);

  // Print the results
  Serial.print("Red: ");
  Serial.print(redFrequency);
  Serial.print(" Green: ");
  Serial.print(greenFrequency);
  Serial.print(" Blue: ");
  Serial.println(blueFrequency);

  delay(500);
}
