#include "application.h"

#ifndef IRRIGATOR_H
#define IRRIGATOR_H "IRRIGATOR_H"

class Irrigator {
 public:
  Irrigator(int powerPin, int signalPin);

	void irrigate(void);

private:
	int powerPin;
	int signalPin;
};

#endif /* IRRIGATOR_H */
