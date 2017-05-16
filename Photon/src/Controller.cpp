#include "Controller.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif


Controller::Controller() {
  this->currentModel = &this->dayModel;
  this->maximumNightLightValue = 5;
  this->iterationsSinceLastIrrigation = 0;
}

void Controller::setVentilator(void(*ventilate)(int)) {
  this->ventilate = ventilate;
}
void Controller::setIrrigator(void(*irrigate)(void)) {
  this->irrigate = irrigate;
}

void Controller::setDesiredDayAirHumidity(double dayAirHumidity) {
  this->dayModel.setDesiredAirHumidity(dayAirHumidity);
}
void Controller::setDesiredDaySoilHumidity(double daySoilHumidity) {
  this->dayModel.setDesiredSoilHumidity(daySoilHumidity);
}
void Controller::setDesiredDayTemperature(double dayTemperature) {
  this->dayModel.setDesiredTemperature(dayTemperature);
}

void Controller::setDesiredNightAirHumidity(double nightAirHumidity) {
  this->nightModel.setDesiredAirHumidity(nightAirHumidity);
}
void Controller::setDesiredNightSoilHumidity(double nightSoilHumidity) {
  this->nightModel.setDesiredSoilHumidity(nightSoilHumidity);
}
void Controller::setDesiredNightTemperature(double nightTemperature) {
  this->nightModel.setDesiredTemperature(nightTemperature);
}

void Controller::setCurrentTemperature(double temperature) {
  this->dayModel.setCurrentTemperature(temperature);
  this->nightModel.setCurrentTemperature(temperature);
}
void Controller::setCurrentAirHumidity(double airHumidity) {
  this->dayModel.setCurrentAirHumidity(airHumidity);
  this->nightModel.setCurrentAirHumidity(airHumidity);
}
void Controller::setCurrentSoilHumidity(double soilHumidity) {
  this->dayModel.setCurrentSoilHumidity(soilHumidity);
  this->nightModel.setCurrentSoilHumidity(soilHumidity);
}
void Controller::setCurrentLight(int light) {
  this->dayModel.setCurrentLight(light);
  this->nightModel.setCurrentLight(light);

  if(light > this->maximumNightLightValue) {
    this->currentModel = &this->dayModel;
  }
  else {
    this->currentModel = &this->nightModel;
  }
}

void Controller::setCurrentVentilation(int ventilation) {
  this->dayModel.setCurrentVentilation(ventilation);
  this->nightModel.setCurrentVentilation(ventilation);
}

void Controller::control(void) {
  this->currentModel->decideDesiredValues();
  (*this->ventilate)(this->currentModel->getDesiredVentilation());
  if(this->currentModel->getDesiredIrrigation() && this->iterationsSinceLastIrrigation > 10) {
    (*this->irrigate)();
    this->iterationsSinceLastIrrigation = 0;
  }
  else {
    this->iterationsSinceLastIrrigation += 1;
  }
}
