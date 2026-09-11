# Arduino Nano 33 BLE Sensor Bring-Up

Two Arduino sketches for exercising onboard sensors on an Arduino Nano 33 BLE Sense during an EE 446 lab.

## What it does

`arduino/Task10_Lab2_EE446.ino` initializes the PDM microphone, BMI270/BMM150 IMU, and APDS9960 sensor. It samples microphone amplitude, accelerometer-derived motion, APDS clear-channel light, and proximity. The sketch prints raw values, four binary threshold flags, and a combined state label to the serial monitor once per second.

`arduino/Task11_Lab2_EE446.ino` initializes the HS300x, BMI270/BMM150 IMU, and APDS9960. Its first sensor read becomes a baseline. Later readings are compared with that baseline for humidity, temperature, magnetic-field magnitude, and light/color changes. It prints raw readings, flags, and one event label every 500 ms.

The thresholds and event labels are fixed in the sketches; this repository does not include logging, model training, or automated tests.

## Hardware and tools

- Arduino Nano 33 BLE Sense
- Arduino IDE
- `PDM`
- `Arduino_BMI270_BMM150`
- `Arduino_APDS9960`
- `Arduino_HS300x` (Task 11)

## Run

1. Install the listed libraries required by the sketch you plan to upload.
2. Connect the board, select the Arduino Nano 33 BLE Sense board and its port, then open one sketch from `arduino/`.
3. Upload the sketch and open Serial Monitor at 115200 baud.

Each sketch prints an initialization failure and stops if one of its required sensors cannot be started.

## Credits

Sparsh Dadhich. Developed for EE 446: Tiny Machine Learning for Ultra Low-Power Edge Computing at the University of Washington (Spring 2026).

## License

[MIT License](LICENSE).
