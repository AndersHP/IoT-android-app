#include "application.h"
	
class AM2302 {
 public:
  AM2302(int pinSend, int pinReceive);
  bool begin(void);
  float readTemperature(void);
  float readHumidity(void);
  bool readTemperatureAndHumidity(float&, float&);

 private:
  bool readData(void);
  bool parityCorrect(uint8_t reply[5]);
  int waitForHigh(int limitMicroSec);
  int waitForLow(int limitMicroSec);
  uint8_t readBit(void);
  uint8_t readByte(void);
  float humidity, temp;
  int pinSend;
  int pinReceive;
};
