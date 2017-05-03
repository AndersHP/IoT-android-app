#include "application.h"
#include "NightModel.h"


NightModel::NightModel() {
}

void NightModel::decideDesiredValues(void) {
  double pGain = 1.5;
  double airHumidityError = currentAirHumidity - desiredAirHumidity;
  double airHumidityDerivative = airHumidityError - lastAirHumidityError;
  double airHumidityDelta = (airHumidityError + airHumidityDerivative) / 2 * pGain;

  desiredVentilation = currentVentilation + (int)airHumidityDelta;

  // TODO desiredIrrigation

  lastAirHumidityError = airHumidityError;
  Particle.publish("NightModel::decideDesiredValues", String((double)currentVentilation, 2) + " -> " + String((double)desiredVentilation, 2) + "; " + String(airHumidityError, 2) + " -> " + String(airHumidityDelta, 2));
}
