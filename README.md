# Sensor Bring-Up — Arduino Nano 33 BLE Sense

**EE 446: Tiny Machine Learning for Ultra Low-Power Edge Computing | University of Washington, Spring 2026**

Hardware and sensor setup lab for the Arduino Nano 33 BLE Sense — verifying every onboard sensor works and can trigger simple threshold-based events, as a foundation for the TinyML labs that follow.

---

## What it does

Two sketches exercise the full onboard sensor suite:

**Task 10** — microphone (PDM), IMU (accelerometer/gyroscope/magnetometer), and proximity/gesture/light sensor:
- PDM microphone sampling with a sound-level threshold ("clap test")
- Motion detection via accelerometer magnitude threshold
- Ambient light level via the APDS9960 proximity sensor
- Proximity threshold trigger

**Task 11** — humidity/temperature, magnetometer, and RGB color sensing:
- HS300x humidity + temperature sensor, baseline-relative threshold detection
- Magnetometer field-strength threshold
- APDS9960 RGB color channel readings against a captured baseline

Both sketches establish a baseline reading, then flag "event detected" when live readings deviate from baseline by more than a fixed threshold — the basic sensor-fusion pattern used throughout the rest of the course.

---

## Repository contents

```
arduino/
  Task10_Lab2_EE446.ino    ← Microphone, IMU, proximity/light sensor test
  Task11_Lab2_EE446.ino    ← Humidity/temperature, magnetometer, RGB color sensor test
Lab2-Instructions.pdf       ← Lab handout (hardware setup, Arduino IDE setup, Blink example, sensor tests)
Lab2_EE446.pdf              ← Written report / task writeups
```

---

## Hardware

- **Arduino Nano 33 BLE Sense** (Nordic nRF52840) — onboard PDM microphone, BMI270 + BMM150 IMU, APDS9960 proximity/gesture/color/light sensor, HS300x humidity/temperature sensor

## Arduino libraries required

- `PDM` (bundled with the Nano 33 BLE Sense board package)
- `Arduino_BMI270_BMM150`
- `Arduino_APDS9960`
- `Arduino_HS300x`

## Quick start

Open either sketch in Arduino IDE, install the libraries above via Library Manager, select the Nano 33 BLE Sense board, upload, and open Serial Monitor at 115200 baud. Move the board / make noise / cover the light sensor to trigger threshold events.

---

## Authors

Sparsh Dadhich — University of Washington, ECE / Neuroscience

---

## License

MIT — see [LICENSE](LICENSE). This covers the author's own code, notebooks, and documentation in this repo.
