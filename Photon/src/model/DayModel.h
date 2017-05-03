#include "application.h"
#include "Model.h"

#ifndef DAY_MODEL_H
#define DAY_MODEL_H "DAY_MODEL_H"

class DayModel : public Model {
 public:
  DayModel();
	void decideDesiredValues();

 private:
  double lastAirHumidityError;
};

#endif /* DAY_MODEL_H */
