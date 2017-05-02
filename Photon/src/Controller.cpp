#include "Controller.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif


Controller::Controller(int pinSend, int pinReceive) {
    this->pinSend = pinSend;
    this->pinReceive = pinReceive;
}


bool Controller::begin() {
    pinSetFast(pinSend);
    // try to read data, as a test
    return readData();
}

bool Controller::readData(void) {
    uint8_t reply[5];

    // Signal data request
    pinResetFast(pinSend);
    delay(2);
    pinSetFast(pinSend);

    // Wait for reply preamble
    waitForLow(200); // Response to low
    waitForHigh(85); // Response to high
    waitForLow(85); // Prepare to receive data

    // Receive data
    for (uint8_t i=0; i<5; i++) {
        reply[i] = readByte();
    }

    waitForHigh(55); // End of transmission

    if (!parityCorrect(reply)) {
        Particle.publish("Bad parity!");
        return false;
    }
    //Particle.publish("Parity OK");

    humidity = reply[0];
    humidity *= 256;
    humidity += reply[1];
    humidity /= 10;
    //Particle.publish("H", humidity);

    temp = reply[2];
    temp *= 256;
    temp += reply[3];
    temp /= 10;
    //Particle.publish("T", temp);

    return true;
}


float Controller::readTemperature(void) {
    if (! readData()) return 0;
    return temp;
}

float Controller::readHumidity(void) {
    if (! readData()) return 0;
    return humidity;
}

bool Controller::parityCorrect(uint8_t reply[5]) {
    int parity = reply[4];

    //Particle.publish("Parity: ", " (" + String(reply[0]) + ", " + String(reply[1]) + ", " + String(reply[2]) + ", " + String(reply[3]) + ", (" + String(reply[0] + reply[1] + reply[2] + reply[3]) + "), " + String(reply[4]) + ")");
    if(reply[0] + reply[1] + reply[2] + reply[3] == parity) {
        return true;
    }
    else {
        return false;
    }
}

int Controller::waitForHigh(int limitMicroSec) {
    int totalDelay = 0;
    unsigned long start = micros();
    while(pinReadFast(pinReceive) != HIGH && totalDelay <= limitMicroSec) {
        delayMicroseconds(1);
        unsigned long end = micros();
        if(start > end) {
            totalDelay = (int)(end + (35791394 - start));
        }
        else {
            totalDelay = (int)(end - start);
        }
    }
    //if(pinReadFast(pin) == HIGH) {
    //    Particle.publish("Went HIGH: " + String(totalDelay) + " / " + String(limitMicroSec));
    //}
    //else {
    //    Particle.publish("NOT HIGH! " + String(totalDelay) + " / " + String(limitMicroSec));
    //}
    return totalDelay;
}

int Controller::waitForLow(int limitMicroSec) {
    int totalDelay = 0;
    unsigned long start = micros();
    while(pinReadFast(pinReceive) != LOW && totalDelay <= limitMicroSec) {
        delayMicroseconds(1);
        unsigned long end = micros();
        if(start > end) {
            totalDelay = (int)(end + (35791394 - start));
        }
        else {
            totalDelay = (int)(end - start);
        }
    }
    //if(pinReadFast(pin) == LOW) {
    //    Particle.publish("Went LOW: " + String(totalDelay) + " / " + String(limitMicroSec));
    //}
    //else {
    //    Particle.publish("NOT LOW! " + String(totalDelay) + " / " + String(limitMicroSec));
    //}
    return totalDelay;
}

uint8_t Controller::readBit(void) {
    int discriminatingDelay;
    waitForHigh(55);
    discriminatingDelay = waitForLow(75);
    if(discriminatingDelay > 60) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t Controller::readByte(void) {
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
bool Controller::readTemperatureAndHumidity(float &t, float &h) {
    if (!readData()) return false;

    t = temp;
    h = humidity;

    return true;
}
