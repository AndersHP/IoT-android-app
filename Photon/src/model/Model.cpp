#include "Model.h"

Model::Model() {
  this->desiredVentilation = 0;
  this->desiredIrrigation = 0;
}

void Model::setDesiredTemperature(double temperature) {
  this->desiredTemperature = temperature;
}
void Model::setDesiredAirHumidity(double airHumidity) {
  this->desiredAirHumidity = airHumidity;
}
void Model::setDesiredSoilHumidity(double soilHumidity) {
  this->desiredSoilHumidity = soilHumidity;
}

void Model::setCurrentTemperature(double temperature) {
  this->currentTemperature = temperature;
}
void Model::setCurrentAirHumidity(double airHumidity) {
  this->currentAirHumidity = airHumidity;
}
void Model::setCurrentSoilHumidity(double soilHumidity) {
  this->currentSoilHumidity = soilHumidity;
}
void Model::setCurrentLight(int light) {
  this->currentLight = light;
}

int Model::getDesiredVentilation(void) {
  return this->desiredVentilation;
}
int Model::getDesiredIrrigation(void) {
  return this->desiredIrrigation;
}
