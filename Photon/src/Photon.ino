/*
 * P2P - Peer Project
 * Description: Planticator 9000
 * Author: Trouble Shoes
 * Date: 2017.05.02
 */

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_DHT.h>

#define LIGHTPIN_IN A0
#define LIGHTPIN_POWER A5

// DHT parameters
#define DHTPIN D5
#define DHTTYPE AM2301

// Variables
double temperature;
double humidity;
int light;

// Pins
//int light_sensor_pin = A0;

// DHT sensor
//DHT dht(DHTPIN, DHTTYPE);
DHT dht(DHTPIN, DHTTYPE);
//AM2302 am2302(D4, D4);

Servo myServo;

int pos = 0;

int setServo(String command) {
   pos = command.toInt();
   myServo.write(pos);
   Particle.publish("Servo Position: %i *\n", pos);
   blinkLed();
   return pos;
}

void blinkLed() {
   digitalWrite(D7,HIGH);
   delay(150);
   digitalWrite(D7,LOW);
}

//// Uncomment for extra experiment with Distance sensor
//void setServoBySonar(const char *event, const char *data) {
//    if (data) {
//        setServo(data);
//    }
//}

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);
int lightValue();

/* This function is called once at start up ----------------------------------*/
void setup()
{
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  myServo.attach(A4);

  pinMode(LIGHTPIN_IN,INPUT);
  pinMode(LIGHTPIN_POWER,OUTPUT);
  // Next, write one pin of the photoresistor to be the maximum possible, so that we can use this for power.
  digitalWrite(LIGHTPIN_POWER,HIGH);

  Particle.variable("GetServo", &pos, INT);
  Particle.variable("GetTemp", &temperature, DOUBLE);
  Particle.variable("GetHumidity", &humidity, DOUBLE);
  Particle.variable("GetLight", &light, INT);
  Particle.function("SetServoWeb", setServo);
  setServo("40");
  //// Uncomment for extra experiment with Distance sensor
  //Particle.subscribe("SetServoSon", setServoBySonar);

  //Setup the Tinker application here

  //Register all the Tinker functions
  Particle.function("digitalread", tinkerDigitalRead);
  Particle.function("digitalwrite", tinkerDigitalWrite);
  Particle.function("analogread", tinkerAnalogRead);
  Particle.function("analogwrite", tinkerAnalogWrite);

  // Start DHT sensor
  dht.begin();
}

/* This function loops forever --------------------------------------------*/
void loop()
{
  // Temperature, humidity and light measurement
  temperature = dht.getTempCelcius();
  humidity = dht.getHumidity();
  light = lightValue();

  // Publish data
  Particle.publish("temperature", String(temperature) + " °C");
  Particle.publish("humidity", String(humidity) + "%");
  Particle.publish("light", String(light) + "%");

  delay(10000);
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

int lightValue() {
  return (int)(analogRead(LIGHTPIN_IN) / 40.96);
}
