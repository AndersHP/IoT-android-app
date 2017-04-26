#include "AM2302.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif


AM2302::AM2302(int pin) {
    this->pin = pin;
}


bool AM2302::begin() {
    pinSetFast(pin);
    // try to read data, as a test
    return readData();
}

bool AM2302::readData(void) {
  uint8_t reply[5];

  // Signal data request
  pinResetFast(pin);
  delay(2);
  pinSetFast(pin);
  
  // Wait for reply preamble
  waitForLow(200); // Response to low
  waitForHigh(85); // Response to high
  waitForLow(85); // Prepare to receive data
  
  // Receive data
  for (uint8_t i=0; i<5; i++) {
    reply[i] = readByte();
    Particle.publish("Read Byte: ", reply[i]);
  }
  
  waitForHigh(55); // End of transmission

  if (!parityCorrect(reply)) {
    Particle.publish("Bad parity!");
    return false;
  }

  humidity = reply[0];
  humidity *= 256;
  humidity += reply[1];
  humidity /= 10;
  Particle.publish("H", humidity);

  temp = reply[2];
  temp *= 256;
  temp += reply[3];
  temp /= 10;
  Particle.publish("T", temp);

  return true;
}


float AM2302::readTemperature(void) {
  if (! readData()) return 0;
  return temp;
}

float AM2302::readHumidity(void) {
  if (! readData()) return 0;
  return humidity;
}

bool AM2302::parityCorrect(uint8_t reply[5]) {
    int h = reply[0] * 256 + reply[1], t = reply[2] * 256 + reply[3], parity = reply[4];
    
    if(h + t == parity) {
        return true;
    }
    else {
        return false;
    }
}

int AM2302::waitForHigh(int limitMicroSec) {
    int totalDelay = 0;
    while(pinReadFast(pin) != HIGH && totalDelay <= limitMicroSec) {
        delayMicroseconds(1);
        totalDelay += 1;
    }
    return totalDelay;
}

int AM2302::waitForLow(int limitMicroSec) {
    int totalDelay = 0;
    while(pinReadFast(pin) != LOW && totalDelay <= limitMicroSec) {
        delayMicroseconds(1);
        totalDelay += 1;
    }
    return totalDelay;
}

uint8_t AM2302::readBit(void) {
    int discriminatingDelay;
    waitForHigh(55); 
    discriminatingDelay = waitForLow(75); 
    
    if(discriminatingDelay > 50) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t AM2302::readByte(void) {
    uint8_t result = 0;
  
    for (uint8_t i=0; i<8; i++) {
        result = result << 1;
        result += readBit();
    }
    
    return result;
}


/*
 * This method returns both temperature and humidity in a single call and using a single I2C request.
 *
 * If you want to obtain both temperature and humidity when you sample the sensor, be aware that calling
 * readTemperature() and readHumidity() in rapid succession may swamp the sensor and result in invalid
 * readings. 
 * Calling this method avoids the double request.
 */
bool AM2302::readTemperatureAndHumidity(float &t, float &h) {
    if (!readData()) return false;

    t = temp;
    h = humidity;

    return true;
}
