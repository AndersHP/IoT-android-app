#include "application.h"
#include "Model.h"

#ifndef DAY_MODEL_H
#define DAY_MODEL_H "DAY_MODEL_H"

class DayModel : public Model {
 public:
  DayModel();
	void decideDesiredValues();

 private:
  int calculateDelta(double current, double desired, double previous);
  double lastTemperature;
  double lastAirHumidity;
};

#endif /* DAY_MODEL_H */
