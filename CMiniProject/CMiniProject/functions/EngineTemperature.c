#include <stdio.h>
#include "..\\src\\headers\\Functions.h"
#include "..\\src\\headers\\Globals.h"

void EngineTemperature() {
	while (true) {
		printf("%s", "Enter engine temperature: ");
		scanf(" %hu", &engineTemperature);
		if (engineTemperature < 70 || engineTemperature > 175) {
			printf("%s",
					"\nEngine temperature is invalid, initiate engine temperature stabilization protocol.\n\n");
		} else {
			if (engineTemperature < 100 || engineTemperature > 150) {
				engineTemperatureControl = '1';
				engineTemperature = 125;
			} else {
				engineTemperatureControl = '0';
			}
			VehicleStatus();
			return;
		}
	}
}
