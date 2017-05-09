#include "application.h"

#include "Model.h"
#include "DayModel.h"
#include "NightModel.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H "CONTROLLER_H"

class Controller {
 public:
  Controller();

	void setVentilator(void(*ventilate)(int));
	void setIrrigator(void(*irrigate)(void));

  void setDesiredDayAirHumidity(double dayAirHumidity);
  void setDesiredDaySoilHumidity(double daySoilHumidity);
  void setDesiredDayTemperature(double dayTemperature);

  void setDesiredNightAirHumidity(double nightAirHumidity);
  void setDesiredNightSoilHumidity(double nightSoilHumidity);
  void setDesiredNightTemperature(double nightTemperature);

  void setCurrentTemperature(double temperature);
  void setCurrentAirHumidity(double airHumidity);
  void setCurrentSoilHumidity(double soilHumidity);
  void setCurrentLight(int light);

  void setCurrentVentilation(int ventilation);

	void control(void);

private:
	void(*ventilate)(int);
	void(*irrigate)(void);
	DayModel dayModel;
	NightModel nightModel;
	Model* currentModel;
	int maximumNightLightValue;
  int iterationsSinceLastIrrigation;
};

#endif /* CONTROLLER_H */
