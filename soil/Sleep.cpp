#include "Sleep.h"

#include <thread>

void Soil::sleep_for_ms(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
