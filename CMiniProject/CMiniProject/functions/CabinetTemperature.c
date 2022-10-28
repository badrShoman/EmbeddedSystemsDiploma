#include <stdio.h>
#include "..\\src\\headers\\Functions.h"
#include "..\\src\\headers\\Globals.h"

void CabinetTemperature() {
	while (true) {
		printf("%s", "Enter cabinet temperature: ");
		scanf(" %hu", &cabinetTemperature);
		if (cabinetTemperature < 0 || cabinetTemperature > 35) {
			printf("%s",
					"\nCabinet temperature is invalid, initiate cabinet temperature stabilization protocol.\n\n");
		} else {
			if (cabinetTemperature < 10 || cabinetTemperature > 30) {
				cabinetTemperatureControl = '1';
				cabinetTemperature = 20;
			} else {
				cabinetTemperatureControl = '0';
			}
			VehicleStatus();
			return;
		}
	}
}
