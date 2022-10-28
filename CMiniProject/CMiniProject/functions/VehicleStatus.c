#include <stdio.h>
#include "..\\src\\headers\\Functions.h"
#include "..\\src\\headers\\Globals.h"

void VehicleStatus() {

	if (engineState == '0') {
		printf("%s", "\nEngine state is: OFF\n");
	} else {
		printf("%s", "\nEngine state is: ON\n");
	}
	if (cabinetTemperatureControl == '0') {
		printf("%s", "AC is: OFF\n");
	} else {
		printf("%s", "AC is: ON\n");
	}
	printf("Vehicle speed is: %d\n", vehicleSpeed);
	printf("Cabinet temperature is: %d\n", cabinetTemperature);
#ifdef WITH_ENGINE_TEMP_CONTROLLER
	if (engineTemperatureControl == '0') {
			printf("%s", "Engine temperature control is: OFF\n");
		} else {
			printf("%s", "Engine temperature control is: ON\n");
		}
		printf("Engine temperature is: %d\n", engineTemperature);
#endif
}
