#include "application.h"
#include "DayModel.h"


DayModel::DayModel() {
}

int DayModel::calculateDelta(double current, double desired, double previous) {
  double pGain = 1.5;
  double error = current - desired;
  double previousError = previous - desired;
  double derivative = error - previousError;
  double delta = (error + derivative) / 2 * pGain;

  return (int)delta;
}

void DayModel::decideDesiredValues(void) {
  int temperatureDelta = this->calculateDelta(currentTemperature, desiredTemperature, lastTemperature);
  int airHumidityDelta = this->calculateDelta(currentAirHumidity, desiredAirHumidity, lastAirHumidity) / 2;

  desiredVentilation = currentVentilation + temperatureDelta + airHumidityDelta;

  desiredIrrigation = desiredSoilHumidity > currentSoilHumidity + 10;

  lastTemperature = currentTemperature;
  lastAirHumidity = currentAirHumidity;
  
  Particle.publish("model-used", "DayModel");
}
