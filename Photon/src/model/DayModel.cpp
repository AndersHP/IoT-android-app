#include "application.h"
#include "DayModel.h"


DayModel::DayModel() {
}

void DayModel::decideDesiredValues(void) {
  double pGain = 1.5;
  double airHumidityError = currentAirHumidity - desiredAirHumidity;
  double airHumidityDerivative = airHumidityError - lastAirHumidityError;
  double airHumidityDelta = (airHumidityError + airHumidityDerivative) / 2 * pGain;

  desiredVentilation = currentVentilation + (int)airHumidityDelta;

  // TODO desiredIrrigation

  lastAirHumidityError = airHumidityError;
  Particle.publish("DayModel::decideDesiredValues", String((double)currentVentilation, 2) + " -> " + String((double)desiredVentilation, 2) + "; " + String(airHumidityError, 2) + " -> " + String(airHumidityDelta, 2));
}
