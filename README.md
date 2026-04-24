# arucosNaprils — Edge Impulse Object Detection on ESP32-S3

ArUco / AprilTag detection using a trained Edge Impulse model running on the XIAO ESP32S3 Sense (or compatible ESP32 camera boards). Captures frames from the camera, runs inference via the Edge Impulse SDK, and prints bounding boxes and classification results over Serial.

---

## What it does

- Initialises the camera at QVGA (320×240) in JPEG format
- Captures a frame every loop iteration
- Converts JPEG to Grayscale and rescales to the model input size
- Runs the Edge Impulse classifier (`run_classifier`)
- Prints bounding boxes, classification scores, and anomaly results to Serial at 115200 baud

---

## Hardware

| Board | Status |
|---|---|
| XIAO ESP32S3 Sense | ✅ default — pins pre-configured |
| AI-Thinker ESP32-CAM | ✅ supported — uncomment in code |
| ESP-EYE | ✅ supported — uncomment in code |
| M5Stack Wide | ✅ supported — uncomment in code |

To switch board, change the `#define` at the top of the file:

```cpp
// default
#define CAMERA_MODEL_XIAO_ESP32S3

// alternatives — comment out the one above, uncomment one below
//#define CAMERA_MODEL_AI_THINKER
//#define CAMERA_MODEL_ESP_EYE
```

---

## Dependencies

### Arduino Core
Tested with **ESP32 Arduino Core 2.0.4**
https://github.com/espressif/arduino-esp32/releases/tag/2.0.4

### Edge Impulse Library
You need to export your trained model from Edge Impulse as an Arduino library and install it. The sketch imports:

```cpp
#include <detectionarucoaprilv2_inferencing.h>
```

This header name matches the exported library name. If you retrain or rename the project on Edge Impulse, update this include to match the new exported library name.

To export: Edge Impulse dashboard → Deployment → Arduino library → Build → download ZIP → install in Arduino IDE via Sketch → Include Library → Add .ZIP Library.

### esp_camera
Included in the ESP32 Arduino Core. No separate install needed.

---

## How to build and flash

1. Install Arduino IDE with ESP32 Arduino Core 2.0.4
2. Export and install your Edge Impulse model as an Arduino library (see above)
3. Open `arucosNaprils.ino`
4. Select your board under Tools → Board → ESP32 Arduino
5. Set the correct port
6. Flash

---

## Serial output

Open Serial Monitor at **115200 baud**. The sketch waits for the Serial connection before starting — if you want it to run immediately without a monitor connected, comment out this line in `setup()`:

```cpp
while (!Serial);
```

Example output when a tag is detected:

```
Predictions (DSP: 12 ms., Classification: 84 ms., Anomaly: 0 ms.):
Object detection bounding boxes:
  aruco (0.921875) [ x: 112, y: 88, width: 64, height: 64 ]
```

When nothing is detected, bounding boxes with `value == 0` are skipped silently.

---

## Tunable parameters

| Parameter | Location | Default | Notes |
|---|---|---|---|
| `debug_nn` | top of file | `false` | set `true` to print raw DSP features |
| `CAMERA_MODEL_*` | top of file | XIAO_ESP32S3 | select your board |
| `jpeg_quality` | `camera_config` | `12` | 0–63, lower = higher quality |
| `fb_count` | `camera_config` | `1` | frame buffer count |
| `xclk_freq_hz` | `camera_config` | `20000000` | try `10000000` if camera unstable |

---

## Known issues

- The sketch allocates the snapshot buffer on the heap every loop iteration via `malloc` and frees it after inference. If PSRAM is not available or fragmented, allocation can fail — check for the `ERR: Failed to allocate snapshot buffer` message on Serial.
- The model input size is defined by the Edge Impulse export (`EI_CLASSIFIER_INPUT_WIDTH` / `EI_CLASSIFIER_INPUT_HEIGHT`). If the captured frame size does not match, the `ei_camera_capture` function handles rescaling automatically via `crop_and_interpolate_rgb888`.
- BGR→RGB swap is applied manually in `ei_camera_get_data` due to a known issue in the esp32-camera driver (https://github.com/espressif/esp32-camera/issues/379).

---

## File structure

```
arucosNaprils.ino   — main sketch, camera init, inference loop
```

The Edge Impulse library (installed separately) provides:
```
detectionarucoaprilv2_inferencing.h   — model + SDK entry point
edge-impulse-sdk/dsp/image/image.hpp  — image processing utilities
```
