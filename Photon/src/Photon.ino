/*
 * P2P - Peer Project
 * Description: Planticator 9000
 * Author: Trouble Shoes
 * Date: 2017.05.02
 */

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT.h>

#include "Controller.h"
#include "SoilHumiditySensor.h"
#include "Irrigator.h"

// LED parameters
#define LEDPIN D7

// Irrigation parameters
#define WATERPIN_OUT D2
#define WATERPIN_POWER D3

// Actuator parameters
#define VENTILATORPIN A4

// Light parameters
#define LIGHTPIN_IN A0
#define LIGHTPIN_POWER A5

// DHT parameters
#define DHTPIN D5
#define DHTTYPE AM2301

// DHT parameters
#define SOIL_IN A1
#define SOIL_POWER D1

// Sensor Input Variables
double temperature;
double airHumidity;
double soilHumidity;
int light;

// Overrides
int disabled = 0;

// Outputs
int ventilation = 0;
int irrigation = 0;

// DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Soil humidity sensor
SoilHumiditySensor soilHumiditySensor(SOIL_POWER, SOIL_IN);

// Irrigation motor
Irrigator irrigator(WATERPIN_POWER, WATERPIN_OUT);

// Ventilation Actuator
Servo ventilator;

// App Logic Controller
Controller controller;


void setVentilationPercentage(int percentage) {
   ventilation = percentage;
   if(ventilation > 100) {
     ventilation = 100;
   }
   else if(ventilation < 0) {
     ventilation = 0;
   }
   Particle.publish("Ventilating!", String(ventilation));
   ventilator.write(ventilation + 40);
}

int setOpening(String command) {
   setVentilationPercentage(command.toInt());
   blinkLed();
   return ventilation;
}

int setDisabled(String command) {
  disabled = command.toInt();
  blinkLed();
  return disabled;
}

void doIrrigation(void) {
   Particle.publish("Irrigating!", "now");
   irrigator.irrigate();
   irrigation = 10;
}

int irrigate(String command) {
   doIrrigation();
   blinkLed();
   return 0;
}

void blinkLed() {
   digitalWrite(LEDPIN,HIGH);
   delay(150);
   digitalWrite(LEDPIN,LOW);
}

int lightValue() {
  return (int)(analogRead(LIGHTPIN_IN) / 40.96);
}

double next(String configuration, String separator, int& lastPos) {
  double value;
  int pos = configuration.indexOf(separator, lastPos);
  value = (double)configuration.substring(lastPos, pos).toFloat();
  lastPos = pos + separator.length();
  return value;
}

int configure(String configuration) {
  String separator = ":";
  int lastPos = 0;
  double dayAirHumidity;
  double daySoilHumidity;
  double dayTemperature;
  double nightAirHumidity;
  double nightSoilHumidity;
  double nightTemperature;

  dayAirHumidity = next(configuration, separator, lastPos);
  daySoilHumidity = next(configuration, separator, lastPos);
  dayTemperature = next(configuration, separator, lastPos);
  nightAirHumidity = next(configuration, separator, lastPos);
  nightSoilHumidity = next(configuration, separator, lastPos);
  nightTemperature = next(configuration, separator, lastPos);

  controller.setDesiredDayAirHumidity(dayAirHumidity);
  controller.setDesiredDaySoilHumidity(daySoilHumidity);
  controller.setDesiredDayTemperature(dayTemperature);
  controller.setDesiredNightAirHumidity(nightAirHumidity);
  controller.setDesiredNightSoilHumidity(nightSoilHumidity);
  controller.setDesiredNightTemperature(nightTemperature);

  blinkLed();

  return 0;
}

int logKnownWiFi(String command) {
    WiFiAccessPoint ap[5];
    int found = WiFi.getCredentials(ap, 5);
    String ssids = "";
    for (int i = 0; i < found; i++) {
        ssids += String(ap[i].ssid) + " ";
    }
    Particle.publish("all-wifi-ssids", ssids);

    blinkLed();

    return found;
}

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);

/* This function is called once at start up ----------------------------------*/
void setup()
{
  Serial.begin(9600);

  // Setup LED
  pinMode(LEDPIN, OUTPUT);

  // Setup Actuator
  ventilator.attach(VENTILATORPIN);

  // Setup Light Sensor
  pinMode(LIGHTPIN_IN,INPUT);
  pinMode(LIGHTPIN_POWER,OUTPUT);
  // Next, write one pin of the photoresistor to be the maximum possible,
  // so that we can use this for power.
  digitalWrite(LIGHTPIN_POWER,HIGH);

  Particle.variable("GetOpening", &ventilation, INT);
  Particle.variable("GetWatered", &irrigation, INT);
  Particle.variable("GetTemp", &temperature, DOUBLE);
  Particle.variable("GetHumidity", &airHumidity, DOUBLE);
  Particle.variable("GetSoil", &soilHumidity, DOUBLE);
  Particle.variable("GetLight", &light, INT);
  Particle.variable("GetDisabled", &disabled, INT);
  Particle.function("SetDisabled", setDisabled);
  Particle.function("SetOpening", setOpening);
  Particle.function("Irrigate", irrigate);
  Particle.function("Configure", configure);
  Particle.function("LogWifi", logKnownWiFi);

  // Reset actuator
  setOpening("0");

  //Setup the Tinker application here
  //Register all the Tinker functions
  Particle.function("digitalread", tinkerDigitalRead);
  Particle.function("digitalwrite", tinkerDigitalWrite);
  Particle.function("analogread", tinkerAnalogRead);
  Particle.function("analogwrite", tinkerAnalogWrite);

  // Start DHT sensor
  dht.begin();

  // Attach Controller functions
  controller.setVentilator(&setVentilationPercentage);
  controller.setIrrigator(&doIrrigation);

  // Default desired values
  controller.setDesiredDayAirHumidity(70);
  controller.setDesiredDaySoilHumidity(50);
  controller.setDesiredDayTemperature(25);
  controller.setDesiredNightAirHumidity(70);
  controller.setDesiredNightSoilHumidity(50);
  controller.setDesiredNightTemperature(20);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
  // Keep cloud connection alive
  Particle.process();

  // We haven't irrigated yet on this iteration - incremental cooldown
  if(irrigation > 0) {
    irrigation -= 1;
  }
  else if(irrigation < 0) {
    irrigation = 0;
  }

  // Temperature, humidity and light measurement
  temperature = dht.getTempCelcius();
  airHumidity = dht.getHumidity();
  soilHumidity = soilHumiditySensor.getCurrentSoilHumidity();
  light = lightValue();

  // Keep cloud connection alive
  Particle.process();

  // Publish data
  Particle.publish("temperature", String(temperature));
  Particle.publish("airHumidity", String(airHumidity));
  Particle.publish("soilHumidity", String(soilHumidity));
  Particle.publish("light", String(light));

  controller.setCurrentTemperature(temperature);
  controller.setCurrentAirHumidity(airHumidity);
  controller.setCurrentSoilHumidity(soilHumidity);
  controller.setCurrentLight(light);
  controller.setCurrentVentilation(ventilation);

  // Keep cloud connection alive
  Particle.process();

  if(disabled == 0) {
    controller.control();
  }

  delay(60000);
}

/*******************************************************************************
* Function Name  : tinkerDigitalRead
* Description    : Reads the digital value of a given pin
* Input          : Pin
* Output         : None.
* Return         : Value of the pin (0 or 1) in INT type
                   Returns a negative number on failure
*******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
* Function Name  : tinkerDigitalWrite
* Description    : Sets the specified pin HIGH or LOW
* Input          : Pin and value
* Output         : None.
* Return         : 1 on success and a negative number on failure
*******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
* Function Name  : tinkerAnalogRead
* Description    : Reads the analog value of a pin
* Input          : Pin
* Output         : None.
* Return         : Returns the analog value in INT type (0 to 4095)
                   Returns a negative number on failure
*******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		return -3;
	}
	else if (pin.startsWith("A"))
	{
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
* Function Name  : tinkerAnalogWrite
* Description    : Writes an analog value (PWM) to the specified pin
* Input          : Pin and Value (0 to 255)
* Output         : None.
* Return         : 1 on success and a negative number on failure
*******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}
