# Audio Classification with STM32F746G

Embedded audio classification example targeting the STM32F746G-Discovery board.

- Description: audio recording, MFCC feature extraction, model inference via X-CUBE-AI, playback and basic UI/serial support.
- MCU: STM32F746NGH6 (STM32F7 series)
- Build: open the included `.ioc` with STM32CubeMX/STM32CubeIDE or build via the provided Makefile in `Debug/`.
- Notes: build outputs and IDE files are excluded by `.gitignore` (folders like `Debug/`, generated X-CUBE-AI artifacts). Use `git lfs` if you plan to track large binary blobs.
- Files of interest: `Core/` (source), `X-CUBE-AI/` (AI model and config), `Drivers/`, `Utilities/`.

How to flash
- Build in STM32CubeIDE or generate project files, then flash with ST-LINK or your preferred programmer.

License
- See `LICENSE_X-CUBE-AI.txt` included in this repository.

Contact
- Repo pushed to your remote as requested. Open issues or request follow-ups here.
