#include "Cars.h"

class Bus : public Car {
public:
	Bus(string t, int m, int sp, int a):Car("�������", 10000, sp, a) {}
	Bus();
};