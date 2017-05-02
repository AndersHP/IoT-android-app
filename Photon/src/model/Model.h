#include "application.h"

#ifndef MODEL_H
#define MODEL_H "MODEL_H"

class Model {
 public:
  Model();

	void setDesiredAirHumidity(double airHumidity);
	void setDesiredSoilHumidity(double soilHumidity);
	void setDesiredTemperature(double temperature);

	void setCurrentTemperature(double temperature);
  void setCurrentAirHumidity(double airHumidity);
  void setCurrentSoilHumidity(double soilHumidity);
  void setCurrentLight(int light);

	virtual void decideDesiredValues() = 0;

  int getDesiredVentilation(void);
  int getDesiredIrrigation(void);

protected:
	double desiredTemperature;
	double desiredAirHumidity;
	double desiredSoilHumidity;
	
	double currentTemperature;
	double currentAirHumidity;
	double currentSoilHumidity;
	int currentLight;

	int desiredVentilation;
	int desiredIrrigation;
};

#endif /* MODEL_H */
