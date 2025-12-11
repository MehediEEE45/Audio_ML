# Audio Classification on STM32F746G-DISCO

Real-time speaker identification using MFCC + TensorFlow Lite Micro (TFLM).

**Author:** Mehedi
**Course / Group:** EEE 322 — Digital Signal Processing I Lab / ODD
**Date:** 9 December 2025

---

## Project overview

This repository implements a real-time speaker recognition system on the STM32F746G-Discovery board. Audio is captured using the on-board microphone/codec, converted to PCM, preprocessed to MFCC features, and classified on-device by a quantized neural network (TFLite/TFLM) integrated into the firmware.

## Key features

- Real-time audio capture (WM8994 codec / on-board microphone)
- PDM → PCM conversion with DMA
- MFCC extraction (40 coefficients)
- On-device inference using a quantized TFLite/TFLM model
- TFT LCD and serial output for predicted speaker
- Dataset: ~2595 one-second clips from multiple speakers (stored in `dataset/`)

## Repository layout

- `Core/` — MCU application code (headers in `Inc/`, sources in `Src/`).
- `X-CUBE-AI/` — AI model integration, generated C files and X-CUBE-AI config.
- `Drivers/`, `Utilities/` — BSP, HAL and helper libraries.
- `Debug/` — local build outputs and Makefile (excluded via `.gitignore`).
- `dataset/` — recorded `.wav` clips used during development (large files).
- `python/` — notebooks, preprocessing scripts and saved models (`python/saved_models`).

## Getting started

### Prerequisites

- STM32CubeIDE (recommended) or ARM GCC toolchain + `make`.
- `git` and `git-lfs` (recommended for dataset/model binaries).
- ST‑LINK or compatible programmer for flashing.

### Build & flash

1. Open `Audio_Classification.ioc` in STM32CubeMX/STM32CubeIDE and generate project files.
2. Build the project in STM32CubeIDE, or run the Makefile under `Debug/`.
3. Flash the binary using STM32CubeIDE, `st-flash`, or OpenOCD.

## Python tools and model workflow

- The `python/` folder contains scripts and notebooks for dataset preparation, MFCC extraction, training and conversion to TFLite.
- Example saved models are under `python/saved_models/` (e.g., quantized `.tflite`).

## Dataset & large files

This repository contains many large files (audio clips, trained model binaries). These are tracked with Git LFS (`*.wav`, `*.tflite`, `*.bin`) to keep Git history small. If you prefer not to keep large blobs in the repo, remove `dataset/` and publish a separate release archive.

If collaborators have older clones prior to the LFS migration, advise them to reclone or run:

```bash
git fetch origin
git reset --hard origin/main
```

## Usage notes

- Real-time latency and accuracy depend on microphone placement, background noise and model quantization. On-device accuracy observed around ~70% for this dataset; offline training accuracy is typically higher.
- For experimentation, modify `python/` scripts to retrain or convert models, then update `X-CUBE-AI/` integration.

## Contributing

- Open an issue or submit a PR for fixes, model updates, or CI additions.
- After history rewrite or LFS migration, notify collaborators to reclone.

## License

See `LICENSE_X-CUBE-AI.txt` for licensing details.

## Contact

Open issues in the GitHub repository for questions or follow-ups.
<<<<<<< HEAD
# Audio Classification for STM32F746G-Discovery

Overview
--------

This project demonstrates on-device audio classification for the STM32F746NGHx Discovery board. It captures audio, computes MFCC features, runs inference using an X-CUBE-AI model, and provides basic playback and serial/UI integration.

Highlights
----------

- Real-time audio capture and MFCC extraction (`Core/`).
- On-device inference via files in `X-CUBE-AI/`.
- Example playback, UI and serial-control code in `Core/Src` and `Drivers/BSP`.

Repository layout
---------------

- `Core/` — MCU application code (headers in `Inc/`, sources in `Src/`).
- `X-CUBE-AI/` — AI model, generated C code and X-CUBE-AI config.
- `Drivers/`, `Utilities/` — board support package, HAL and helper libraries.

Getting started
---------------

Prerequisites

- STM32CubeIDE (recommended) or ARM GCC toolchain + `make`.
- ST-LINK (or compatible) for flashing.

Build & flash

1. Open `Audio_Classification.ioc` in STM32CubeMX/STM32CubeIDE and generate the project files.
2. Build the project in STM32CubeIDE, or run the Makefile under `Debug/`.
3. Flash the binary using STM32CubeIDE, `st-flash`, or OpenOCD.

Working with the Python tools
-----------------------------

- Use the `python/` folder for dataset preparation, MFCC extraction and training. Notebooks and helper scripts are provided.
- Saved models are under `python/saved_models/` (example: quantized `.tflite`).

Dataset & model files
---------------------

The repository contains many audio clips and model binaries. These are tracked with Git LFS to keep the Git history manageable; LFS objects are stored on the remote. If you prefer not to store large files in the repo, remove `dataset/` and create a release package instead.

Contributing
------------

- Report issues or propose changes via PRs. If you modify history (e.g., LFS migration), tell collaborators to reclone or `git fetch && git reset --hard origin/main`.

License
-------

See `LICENSE_X-CUBE-AI.txt` for licensing terms.

Contact
-------

Repository is pushed to your GitHub remote. Open issues there for follow-ups.

# 🎤 Audio Classification on STM32F746G-DISCO  
### Real-Time Speaker Identification using MFCC + TensorFlow Lite Micro

**Author:** Mehedi  
**Group:** ODD  
**Course:** EEE 322 — Digital Signal Processing I Lab  
**Department:** Electrical & Electronic Engineering, SUST  
**Submission Date:** 9 December 2025  

---

## 📌 Project Overview
This project implements a **real-time speaker recognition system** on the **STM32F746G-DISCO** board.  
Speech is recorded using the on-board MEMS microphone, processed using DSP techniques (MFCC), and classified using a **TensorFlow Lite Micro (TFLM)** neural network deployed on the microcontroller.

### 🔥 Key Features
- Real-time **audio capture** using WM8994 codec  
- **PDM → PCM** conversion + DMA  
- **MFCC extraction** (40-coefficient version)  
- Neural network model trained in **Python (Keras)**  
- **Quantized TFLite** model for embedded inference  
- Speaker name displayed on the TFT LCD  
- Dataset: **2595 audio clips (1-second each)** from 5 speakers  

=======
# 🎤 Audio Classification on STM32F746G-DISCO  
### Real-Time Speaker Identification using MFCC + TensorFlow Lite Micro

**Author:** Mehedi  
**Group:** ODD  
**Course:** EEE 322 — Digital Signal Processing I Lab  
**Department:** Electrical & Electronic Engineering, SUST  
**Submission Date:** 9 December 2025  

---

## 📌 Project Overview
This project implements a **real-time speaker recognition system** on the **STM32F746G-DISCO** board.  
Speech is recorded using the on-board MEMS microphone, processed using DSP techniques (MFCC), and classified using a **TensorFlow Lite Micro (TFLM)** neural network deployed on the microcontroller.

### 🔥 Key Features
- Real-time **audio capture** using WM8994 codec  
- **PDM → PCM** conversion + DMA  
- **MFCC extraction** (40-coefficient version)  
- Neural network model trained in **Python (Keras)**  
- **Quantized TFLite** model for embedded inference  
- Speaker name displayed on the TFT LCD  
- Dataset: **2595 audio clips (1-second each)** from 5 speakers  

>>>>>>> origin/main
---

## 🛠️ Hardware Used
- **STM32F746G-DISCO** (Cortex-M7 @ 216 MHz, 1MB Flash, 320 KB SRAM)  
- **MP34DT01** MEMS microphone  
- **WM8994** Audio codec (ADC/DAC)  
- **4.3" TFT LCD**  
- SD card (optional for storage)  
- ST-Link for flashing & debugging  

---

## 🧰 Software & Libraries
| Component | Version |
|----------|---------|
| STM32CubeIDE | v1.19 |
| STM32CubeF7 HAL | v1.26.0 |
| Python | 3.8+ |
| Librosa | v0.10.0 |
| NumPy | v1.25.0 |
| TensorFlow | 2.15.0 |
| TensorFlow Lite Micro | Built manually |
| CMSIS-DSP | Optional |
| Jupyter Notebook | v6.5+ |

---

## 📂 Project Structure
.
├── README.md
├── dataset/
│ ├── fa_him/
│ ├── imran/
│ ├── nayeem/
│ ├── shahed/
│ └── talukder/
├── python/
│ ├── split_audio.py
│ ├── make_metadata.py
│ ├── extract_mfcc.py
│ ├── train_model.py
│ ├── convert_to_tflite.py
│ └── ai_test.py
├── stm32/
│ ├── Core/
│ ├── Drivers/
│ ├── Middlewares/
│ └── xx_model_data.cc
└── report.tex


---

## 🎙️ Dataset Collection
- 10 minutes of speech collected per speaker  
- Converted to WAV format  
- Split into **1-second clips** using Python (`pydub`)  
- Total clips: **2595**  
- Structured as:
dataset/
├── fa_him/
├── imran/
├── nayeem/
├── shahed/
├── talukder/


### 🔧 Split Audio into 1-Second Clips
```python
from pydub import AudioSegment
import math, os

audio = AudioSegment.from_file("fa him.wav")
chunk_length_ms = 1000
os.makedirs("fa_him", exist_ok=True)

for i in range(math.ceil(len(audio)/chunk_length_ms)):
    start = i * chunk_length_ms
    end = start + chunk_length_ms
    audio[start:end].export(f"fa_him/clip_{i+1:03}.wav", format="wav")
🎵 MFCC Feature Extraction
Extracted 40 MFCC coefficients per clip

Used librosa.feature.mfcc()

MFCC extraction pipeline:

Pre-emphasis

Framing

Hamming window

FFT

Mel filter banks

Log energy

DCT → MFCC

🤖 Model Training (Python)
Neural Network Architecture
text

Input: 40 MFCC features
Layer 1: Dense(100) + ReLU + Dropout(0.20)
Layer 2: Dense(200) + ReLU + Dropout(0.20)
Layer 3: Dense(100) + ReLU + Dropout(0.20)
Output: Softmax (5 classes)
Training Script (simplified)
python

model.compile(
    loss='categorical_crossentropy',
    optimizer='adam',
    metrics=['accuracy']
)

model.fit(X_train, y_train, epochs=100, batch_size=32)
model.save("audio_classification_100.keras")
Achieved Accuracy
Type	Accuracy
Offline training	99.8%
On-device	~70%

🧪 Converting Keras Model → TFLite Micro
Int8 Quantization
python

converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = rep_dataset_gen
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8

tflite_model = converter.convert()
open("audio_class_quant.tflite", "wb").write(tflite_model)
TFLite Model Size
Copy code
audio_class_quant.tflite → 57.3 KB
Converted to C array:


xx_model_data.cc
🧵 FreeRTOS Integration
Major Tasks
record.c → audio recording from WM8994

mfcc.c → MFCC extraction on MCU

ai_on() → run inference

display.c → show predicted speaker on LCD

role.c → application state machine

System Workflow
css
Copy code
Microphone → WM8994 → PCM → MFCC → NN Model → LCD Display
📊 Results
Metric	Value
Real-time latency	~300 ms
Embedded accuracy	~70%
Max distance tested	~1 meter

Challenges
Background noise

Inconsistent speech loudness

RAM limitations for MFCC buffer + TFLite model

📸 Project Output
(Add images to docs/ and link here)

scss
Copy code
![Output 1](docs/output.jpg)
![Output 2](docs/1.jpg)
![Output 3](docs/2.jpg)
![Output 4](docs/3.jpg)
📎 References
STM32F746G-DISCO Reference Manual

TensorFlow Lite Micro Documentation

CMSIS-DSP

STM32CubeIDE, STM32CubeMX

📬 Contact
<<<<<<< HEAD
For contributions, issues, or discussions, feel free to open a GitHub issu
=======
For contributions, issues, or discussions, feel free to open a GitHub issu
>>>>>>> origin/main
