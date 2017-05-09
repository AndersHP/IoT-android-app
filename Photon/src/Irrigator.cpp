#include "Irrigator.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif


Irrigator::Irrigator(int powerPin, int signalPin) {
  this->powerPin = powerPin;
  this->signalPin = signalPin;

  pinMode(this->signalPin, OUTPUT);
  digitalWrite(this->signalPin, HIGH);

  pinMode(this->powerPin, OUTPUT);
  digitalWrite(this->powerPin, HIGH);
}

void Irrigator::irrigate(void) {
  double currentSoilHumidity;
  int analogValue;

  digitalWrite(this->signalPin, LOW);
  delay(250);
  digitalWrite(this->signalPin, HIGH);
}
