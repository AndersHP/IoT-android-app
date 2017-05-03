#include "application.h"
#include "Model.h"

#ifndef NIGHT_MODEL_H
#define NIGHT_MODEL_H "NIGHT_MODEL_H"

class NightModel : public Model {
 public:
  NightModel();
	void decideDesiredValues();

 private:
  int calculateDelta(double current, double desired, double previous);
  double lastTemperature;
  double lastAirHumidity;
};

#endif /* NIGHT_MODEL_H */
