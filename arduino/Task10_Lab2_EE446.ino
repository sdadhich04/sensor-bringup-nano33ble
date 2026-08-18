#include <PDM.h>
#include <Arduino_BMI270_BMM150.h>
#include <Arduino_APDS9960.h>

short sampleBuffer[256];
volatile int samplesRead = 0;

int micValue = 0;
int lightValue = 0;
float motionValue = 0.0;
int proximityValue = 0;

const int SOUND_THR = 50;
const int DARK_THR = 50;
const float MOTION_THR = 0.12;
const int PROXIMITY_THR = 200;

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  PDM.onReceive(onPDMdata);

  // Mic
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM microphone.");
    while (1);
  }

  // IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU.");
    while (1);
  }

  // Light + proximity
  if (!APDS.begin()) {
    Serial.println("Failed to initialize APDS9960 sensor.");
    while (1);
  }

  Serial.println("System is ready to go");
}

void loop() {
  // Microphone
  if (samplesRead > 0) {
    long sum = 0;
    for (int i = 0; i < samplesRead; i++) {
      sum += abs(sampleBuffer[i]);
    }
    micValue = sum / samplesRead;
    samplesRead = 0;
  }

  // IMU motion
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    float magnitude = sqrt(x * x + y * y + z * z);
    motionValue = fabs(magnitude - 1.0);
  }

  // Proximity
  if (APDS.proximityAvailable()) {
    proximityValue = APDS.readProximity();
  }

  // Light
  int r, g, b, c;
  if (APDS.colorAvailable()) {
    APDS.readColor(r, g, b, c);
    lightValue = c;
  }

  // Binary flags
  int sound = (micValue > SOUND_THR) ? 1 : 0;
  int dark = (lightValue > DARK_THR) ? 1 : 0;
  int moving = (motionValue > MOTION_THR) ? 1 : 0;
  int near = (proximityValue > PROXIMITY_THR) ? 1 : 0;

  String label;

  if (sound == 0 && dark == 1 && moving == 0 && near == 1) {
    label = "QUIET_BRIGHT_STEADY_FAR";
  }
  else if (sound == 1 && dark == 1 && moving == 0 && near == 1) {
    label = "NOISY_BRIGHT_STEADY_FAR";
  }
  else if (sound == 0 && dark == 0 && moving == 0 && near == 0) {
    label = "QUIET_DARK_STEADY_NEAR";
  } 
  else if (sound == 1 && dark == 1 && moving == 1 && near == 0) {
    label = "NOISY_BRIGHT_MOVING_NEAR";
  }


  Serial.print("raw,mic=");
  Serial.print(micValue);
  Serial.print(",clear=");
  Serial.print(lightValue);
  Serial.print(",motion=");
  Serial.print(motionValue, 3);
  Serial.print(",prox=");
  Serial.println(proximityValue);

  Serial.print("flags,sound=");
  Serial.print(sound);
  Serial.print(",dark=");
  Serial.print(dark);
  Serial.print(",moving=");
  Serial.print(moving);
  Serial.print(",near=");
  Serial.println(near);

  Serial.print("state,");
  Serial.println(label);

  delay(1000);
}