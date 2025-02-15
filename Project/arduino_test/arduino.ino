#include <eloquent_tinyml.h>
#include "model1.h"           // Incluir el modelo

// Crear una instancia de TensorFlow Lite
Eloquent::ML::Port::model_tflite tf;

// Función argmax manual
int argmax(float* array, int size) {
    int max_idx = 0;
    for (int i = 1; i < size; i++) {
        if (array[i] > array[max_idx]) {
            max_idx = i;
        }
    }
    return max_idx;
}

void setup() {
    Serial.begin(9600);
    tf.begin(model_tflite);  // Inicializar TensorFlow Lite con el modelo
}

void loop() {
    // Simular datos de entrada (ajusta con tus valores reales)
    float input[INPUT_SIZE] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};  // Normalizado a [0-1]
    float output[OUTPUT_SIZE];

    // Realizar la predicción
    tf.predict(input, output);

    // Obtener la clase predicha
    int label = argmax(output, OUTPUT_SIZE);

    // Mostrar el resultado
    Serial.print("Clase predicha: ");
    Serial.println(label);

    delay(1000);  // Esperar 1 segundo
}