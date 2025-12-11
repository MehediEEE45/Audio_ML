# Audio Classification on STM32F746G-DISCO

Real-time speaker identification using MFCC + TensorFlow Lite Micro (TFLM).

Author: Mehedi  
Course / Group: EEE 322 — Digital Signal Processing I Lab / ODD  
Date: 9 December 2025

---

## Summary

This project runs a real-time speaker recognition system on the STM32F746G‑DISCOVERY board. The pipeline captures audio from the on-board microphone, converts it to PCM, extracts MFCC features, and performs on‑device inference with a quantized TensorFlow Lite Micro model. Results are shown on the TFT and output over serial.

---

## Quick start

Prerequisites:
- STM32CubeIDE or ARM GCC toolchain + make
- git and git-lfs (recommended for dataset/model binaries)
- ST‑Link or compatible programmer

Build and flash:
1. Open `Audio_Classification.ioc` in STM32CubeMX/STM32CubeIDE and generate project files.  
2. Build in STM32CubeIDE or run the Makefile under `Debug/`.  
3. Flash with STM32CubeIDE, st-flash, or OpenOCD.

If you encounter LFS-related problems after a migration, reclone or run:
```bash
git fetch origin
git reset --hard origin/main
```

---

## Project highlights

- Real-time audio capture using WM8994 + on-board MEMS microphone (MP34DT01)  
- PDM → PCM conversion with DMA  
- MFCC extraction (40 coefficients) on the MCU  
- Quantized TFLite (int8) model for TFLM inference  
- TFT LCD + serial output for predicted speaker  
- Dataset: ~2,595 one‑second clips across 5 speakers

---

## Hardware

- STM32F746G‑DISCOVERY (Cortex‑M7 @ 216 MHz, 1 MB Flash, 320 KB SRAM)  
- MP34DT01 MEMS microphone (on-board)  
- WM8994 audio codec  
- 4.3" TFT LCD  
- SD card (optional)  
- ST‑Link for flashing & debugging

---

## Software & dependencies

Recommended versions used during development:
- STM32CubeIDE v1.19, STM32CubeF7 HAL v1.26.0  
- Python 3.8+, librosa v0.10.0, NumPy v1.25.0  
- TensorFlow 2.15.0, TensorFlow Lite Micro (manually integrated)  
- Jupyter Notebook v6.5+, optional CMSIS‑DSP

---

## Repository layout

.
├── README.md  
├── dataset/                # recorded WAV clips (per speaker)  
├── python/                 # preprocessing, training, conversion scripts  
│   ├── split_audio.py  
│   ├── extract_mfcc.py  
│   ├── train_model.py  
│   ├── convert_to_tflite.py  
│   └── ai_test.py  
├── stm32/                  # STM32 project and integration files  
│   ├── Core/  
│   ├── Drivers/  
│   ├── Middlewares/  
│   └── xx_model_data.cc    # model C array (generated)  
└── report.tex

---

## Dataset & preprocessing

- Recordings were split into 1‑second WAV clips using `pydub`. Total ≈ 2,595 clips across 5 speakers.  
- Example split script (python/split_audio.py):

```python
from pydub import AudioSegment
import math, os

audio = AudioSegment.from_file("fa him.wav")
chunk_length_ms = 1000
os.makedirs("fa_him", exist_ok=True)

for i in range(math.ceil(len(audio) / chunk_length_ms)):
    start = i * chunk_length_ms
    end = start + chunk_length_ms
    audio[start:end].export(f"fa_him/clip_{i+1:03}.wav", format="wav")
```

### MFCC extraction
- Extracted 40 MFCC coefficients per clip using `librosa.feature.mfcc`.  
- Standard pipeline: pre‑emphasis → framing → Hamming window → FFT → Mel filters → log energy → DCT → MFCC. See `python/extract_mfcc.py`.

---

## Model training & conversion

Example architecture (reference):
- Input: 40 MFCC features  
- Dense(100) + ReLU + Dropout(0.2)  
- Dense(200) + ReLU + Dropout(0.2)  
- Dense(100) + ReLU + Dropout(0.2)  
- Output: Softmax (5 classes)

Training snippet:
```python
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit(X_train, y_train, epochs=100, batch_size=32)
model.save("audio_classification_100.keras")
```

Convert to quantized TFLite (int8):
```python
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_dataset_gen
converter.inference_input_type = tf.int8
converter.inference_output_type = tf.int8
tflite_model = converter.convert()
open("audio_class_quant.tflite", "wb").write(tflite_model)
```

Typical quantized model size: ~57 KB.

---

## Firmware integration

Key firmware components:
- record.c — read audio from WM8994 via DMA  
- mfcc.c — compute MFCC on MCU  
- ai_on() — perform inference using TFLM/X‑CUBE‑AI integration  
- display.c — update TFT with the predicted speaker  
- role.c — application state machine

Workflow:
Microphone → WM8994 → PCM → MFCC → NN Model → LCD / Serial

---

## Results & limitations

- Real-time latency: ≈ 300 ms  
- On-device accuracy: ≈ 70% (depends on noise, placement, quantization)  
- Offline training accuracy: up to ≈ 99.8% (likely overfit; validate on held-out test set)  
- Max tested distance: ≈ 1 meter

Known challenges:
- Background noise and variable speech volume reduce performance.  
- RAM constraints when holding MFCC buffers and TFLM interpreter simultaneously.  
- Quantization can degrade accuracy — use a representative dataset for calibration.

---

## Troubleshooting

- If model fails to load or inference crashes, check available RAM and stack sizes in the linker and FreeRTOS task configuration.  
- If captured audio is noisy or clipped, verify WM8994 init and DMA buffer sizes.  
- For quantization issues, ensure representative data covers expected audio variance.

---

## Project assets


(docs![2](https://github.com/user-attachments/assets/b446922b-3859-4732-8137-2a64ac9ac3d6)
/output.jpg)![1](https://github.com/user-attachments/assets/b27e0875-7b4b-425f-a8b6-5ab1a744f9aa)
![3](https://github.com/user-attachments/assets/e15122fe-0750-46ab-bdbe-f2a23d7097ee)
![2](https://github.com/user-attachments/assets/21743132-decf-4e7f-9edb-187a44cc1a62)
![IMG_20251210_122422](https://github.com/user-attachments/assets/f2b390e1-6d50-49f3-a33d-7ee847022705)
![IMG_20251209_213312](https://github.com/user-attachments/assets/e667e180-fc3e-4009-9fdf-d8151ef5a0bd)


---

## Contributing

- Open an issue for bugs, feature requests, or questions.  
- Submit PRs for fixes, model improvements, or CI additions.  
- After history rewrite or LFS migration, ask collaborators to reclone.

---

## License

See `LICENSE_X-CUBE-AI.txt` for licensing details applicable to generated AI code and model artifacts.

---

## Contact

For questions or support, open an issue in this repository.
