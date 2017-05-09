#include "SoilHumiditySensor.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif


SoilHumiditySensor::SoilHumiditySensor(int voltagePin, int inputPin) {
  this->voltagePin = voltagePin;
  this->inputPin = inputPin;

  pinMode(this->voltagePin, OUTPUT);
  digitalWrite(this->voltagePin, LOW);
}

double SoilHumiditySensor::getCurrentSoilHumidity(void) {
  double currentSoilHumidity;
  int analogValue;

  digitalWrite(this->voltagePin, HIGH);
  delay(500);
  analogValue = analogRead(this->inputPin);
  digitalWrite(this->voltagePin, LOW);

  currentSoilHumidity = (4096 - analogValue) / 40.96;

  return currentSoilHumidity;
}
