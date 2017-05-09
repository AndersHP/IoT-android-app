#include "application.h"
#include "NightModel.h"


NightModel::NightModel() {
}

int NightModel::calculateDelta(double current, double desired, double previous) {
  double pGain = 1.5;
  double error = current - desired;
  double previousError = previous - desired;
  double derivative = error - previousError;
  double delta = (error + derivative) / 2 * pGain;

  return (int)delta;
}

void NightModel::decideDesiredValues(void) {
  int temperatureDelta = this->calculateDelta(currentTemperature, desiredTemperature, lastTemperature);
  int airHumidityDelta = this->calculateDelta(currentAirHumidity, desiredAirHumidity, lastAirHumidity) / 2;

  desiredVentilation = currentVentilation + temperatureDelta + airHumidityDelta;

  desiredIrrigation = desiredSoilHumidity > currentSoilHumidity + 10;

  lastTemperature = currentTemperature;
  lastAirHumidity = currentAirHumidity;
  Particle.publish("NightModel::decideDesiredValues", String((double)currentVentilation, 2) + " -> " + String((double)desiredVentilation, 2) + "; T:" + String((double)temperatureDelta, 2) + " : H:" + String((double)airHumidityDelta, 2));
}
