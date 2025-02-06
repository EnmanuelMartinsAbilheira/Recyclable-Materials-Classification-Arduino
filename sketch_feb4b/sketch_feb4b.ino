#include <TensorFlowLite.h>
#include "color_model.h"  // Your model array
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Class labels (match your training)
const char* CLASS_NAMES[] = {"can", "glass", "paper", "plastic", "plastic_bag", "styrofoam"};

// TensorFlow Lite setup
tflite::MicroErrorReporter error_reporter;
tflite::AllOpsResolver resolver;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Memory allocation for TensorFlow
constexpr int kTensorArenaSize = 8 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

// Color sensor pins (adjust to your setup)
const int S0 = 2, S1 = 3, S2 = 4, S3 = 5, OUT = 6;

void setup() {
  Serial.begin(9600);
  
  // Initialize color sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Sensor frequency scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);  // 20% scaling

  // Load TFLite model
  model = tflite::GetModel(color_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while(1);
  }

  // Initialize interpreter
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, &error_reporter
  );
  interpreter = &static_interpreter;

  // Allocate tensors
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("Allocation failed!");
    while(1);
  }

  // Get input/output pointers
  input = interpreter->input(0);
  output = interpreter->output(0);
}

void loop() {
  // 1. Read RGB values from sensor
  float r, g, b = readRGB();  // Implement your sensor reading function
  
  // 2. Preprocess input (normalize 0-255 -> 0.0-1.0)
  input->data.f[0] = r / 255.0;
  input->data.f[1] = g / 255.0;
  input->data.f[2] = b / 255.0;

  // 3. Run inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Inference failed!");
    return;
  }

  // 4. Get predictions
  int prediction = argmax(output->data.f, 6);
  Serial.print("Detected: ");
  Serial.println(CLASS_NAMES[prediction]);

  float confidence = output->data.f[prediction];
    if (confidence < 0.7) Serial.println("Unknown material");

  delay(1000);
}

// Helper function to find max probability
int argmax(float* data, int len) {
  int maxIndex = 0;
  for (int i = 1; i < len; i++) {
    if (data[i] > data[maxIndex]) {
      maxIndex = i;
    }
  }
  return maxIndex;
}

float readRGB() {
  // TCS3200 example implementation
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  float r = pulseIn(OUT, LOW);
  
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  float g = pulseIn(OUT, LOW);
  
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  float b = pulseIn(OUT, LOW);

  // Normalize based on your sensor calibration
  r = map(r, 0, 255, 0, 255);  // Adjust these ranges
  g = map(g, 0, 255, 0, 255);
  b = map(b, 0, 255, 0, 255);
  
  return {r, g, b};
}