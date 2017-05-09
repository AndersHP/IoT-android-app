#include "application.h"

#ifndef SOIL_HUMIDITY_SENSOR_H
#define SOIL_HUMIDITY_SENSOR_H "SOIL_HUMIDITY_SENSOR_H"

class SoilHumiditySensor {
 public:
  SoilHumiditySensor(int voltagePin, int inputPin);

	double getCurrentSoilHumidity(void);

private:
	int voltagePin;
	int inputPin;
};

#endif /* SOIL_HUMIDITY_SENSOR_H */
