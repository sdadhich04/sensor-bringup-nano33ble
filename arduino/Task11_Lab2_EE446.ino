#include <Arduino_HS300x.h>
#include <Arduino_BMI270_BMM150.h>
#include <Arduino_APDS9960.h>

float rh = 0.0;
float temp = 0.0;
float mx = 0.0;
float my = 0.0;
float mz = 0.0;
int r = 0;
int g = 0;
int b = 0;
int clear = 0;

float baseRh = 0.0;
float baseTemp = 0.0;
float baseMag = 0.0;
int baseR = 0;
int baseG = 0;
int baseB = 0;
int baseClear = 0;

bool baselineSet = false;

const float RH_THR = 3.0;
const float TEMP_THR = 1.0;
const float MAG_THR = 20.0;
const int CLEAR_THR = 80;
const int COLOR_THR = 100;

unsigned long lastEventTime = 0;
const unsigned long COOLDOWN_MS = 2500;

void setup() {
  Serial.begin(115200);
  delay(2000);

  if (!HS300x.begin()) {
    Serial.println("Failed to initialize HS3003.");
    while (1);
  }

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU.");
    while (1);
  }

  if (!APDS.begin()) {
    Serial.println("Failed to initialize APDS9960.");
    while (1);
  }

  Serial.println("Task 11 system ready");
}

void loop() {
  // Read humidity + temperature
  temp = HS300x.readTemperature();
  rh = HS300x.readHumidity();

  // Read magnetometer
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(mx, my, mz);
  }

  // Read color/light
  if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b, clear);
  }

  float magMetric = sqrt(mx * mx + my * my + mz * mz);
  int colorDelta = abs(r - baseR) + abs(g - baseG) + abs(b - baseB);

  // Set baseline once after first stable read
  if (!baselineSet) {
    baseRh = rh;
    baseTemp = temp;
    baseMag = magMetric;
    baseR = r;
    baseG = g;
    baseB = b;
    baseClear = clear;
    baselineSet = true;
    delay(500);
    return;
  }

  int humid_jump = (abs(rh - baseRh) > RH_THR) ? 1 : 0;
  int temp_rise = ((temp - baseTemp) > TEMP_THR) ? 1 : 0;
  int mag_shift = (abs(magMetric - baseMag) > MAG_THR) ? 1 : 0;
  int light_or_color_change =
      (abs(clear - baseClear) > CLEAR_THR || colorDelta > COLOR_THR) ? 1 : 0;

  String label = "BASELINE_NORMAL";

  unsigned long now = millis();
  bool cooldownActive = (now - lastEventTime < COOLDOWN_MS);

  if (!cooldownActive) {
    if (mag_shift) {
      label = "MAGNETIC_DISTURBANCE_EVENT";
      lastEventTime = now;
    }
    else if (humid_jump || temp_rise) {
      label = "BREATH_OR_WARM_AIR_EVENT";
      lastEventTime = now;
    }
    else if (light_or_color_change) {
      label = "LIGHT_OR_COLOR_CHANGE_EVENT";
      lastEventTime = now;
    }
    else {
      label = "BASELINE_NORMAL";
    }
  } else {
    if (mag_shift) label = "MAGNETIC_DISTURBANCE_EVENT";
    else if (humid_jump || temp_rise) label = "BREATH_OR_WARM_AIR_EVENT";
    else if (light_or_color_change) label = "LIGHT_OR_COLOR_CHANGE_EVENT";
    else label = "BASELINE_NORMAL";
  }

  Serial.print("raw,rh=");
  Serial.print(rh, 2);
  Serial.print(",temp=");
  Serial.print(temp, 2);
  Serial.print(",mag=");
  Serial.print(magMetric, 2);
  Serial.print(",r=");
  Serial.print(r);
  Serial.print(",g=");
  Serial.print(g);
  Serial.print(",b=");
  Serial.print(b);
  Serial.print(",clear=");
  Serial.println(clear);

  Serial.print("flags,humid_jump=");
  Serial.print(humid_jump);
  Serial.print(",temp_rise=");
  Serial.print(temp_rise);
  Serial.print(",mag_shift=");
  Serial.print(mag_shift);
  Serial.print(",light_or_color_change=");
  Serial.println(light_or_color_change);

  Serial.print("FINAL_LABEL=");
  Serial.println(label);

  delay(500);
}