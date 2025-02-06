# color_recognition_train.py
import os
import cv2
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
import tensorflow as tf
from tensorflow.keras import layers, models

# Load dataset from Kaggle directory
DATASET_PATH = "C:/Users/lmars/.cache/kagglehub/datasets/minhoheo/recyable-materials/versions/2/recyclable_materials"
IMG_SIZE = 48
# Filter to include only directories in CLASSES
CLASSES = [d for d in sorted(os.listdir(DATASET_PATH)) 
           if os.path.isdir(os.path.join(DATASET_PATH, d))]

# Load images and labels
images = []
labels = []

for class_idx, class_name in enumerate(CLASSES):
    class_path = os.path.join(DATASET_PATH, class_name)
    # Check if class_path is a directory (redundant safety check)
    if os.path.isdir(class_path):
        for img_file in os.listdir(class_path):
            img_path = os.path.join(class_path, img_file)
            img = cv2.imread(img_path)
            if img is not None:
                img = cv2.resize(img, (IMG_SIZE, IMG_SIZE))
                img = img / 255.0  # Normalize
                images.append(img)
                labels.append(class_idx)
            else:
                print(f"Warning: Failed to load image {img_path}")

# Convert to numpy arrays
X = np.array(images)
y = tf.keras.utils.to_categorical(labels, num_classes=len(CLASSES))

# Split dataset
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

# Build TinyML model
model = models.Sequential([
    layers.Input(shape=(IMG_SIZE, IMG_SIZE, 3)),
    layers.Conv2D(8, (3, 3), activation='relu'),
    layers.MaxPooling2D((2, 2)),
    layers.Flatten(),
    layers.Dense(16, activation='relu'),
    layers.Dense(len(CLASSES), activation='softmax')
])

model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])

# Train
model.fit(X_train, y_train, epochs=150, validation_data=(X_test, y_test))

# Convert to TensorFlow Lite
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()

# Save model and labels
with open('color_model.tflite', 'wb') as f:
    f.write(tflite_model)

with open('labels.txt', 'w') as f:
    f.write('\n'.join(CLASSES))