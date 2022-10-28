#include <stdio.h>
#include "..\\src\\headers\\Functions.h"
#include "..\\src\\headers\\Globals.h"

void InitialOperation() {
	while (true) {
#ifdef WITH_ENGINE_TEMP_CONTROLLER
		printf("%s", "\nPlease choose what you do want:\n\n"
				"\ta. Turn off vehicle engine.\n"
				"\tb. Set traffic light color.\n"
				"\tc. Sets cabinet temperature.\n"
				"\td. Set engine temperature.\n");
#else
		printf("%s", "\nPlease choose what you do want:\n\n"
						"\ta. Turn off vehicle engine.\n"
						"\tb. Set traffic light color.\n"
						"\tc. Sets cabinet temperature.\n");
#endif
		printf("%s", "\nYour choice: ");
		scanf(" %c", &userInput);
		switch (userInput) {
		case 'a':
		case 'A':
			engineTemperature = 30;
			engineState = false;
			printf("%s", "\nEngine turned off.\n\n");
			return;
		case 'b':
		case 'B':
			TrafficLight();
			break;
		case 'c':
		case 'C':
			CabinetTemperature();
			break;
#ifdef WITH_ENGINE_TEMP_CONTROLLER
		case 'd':
		case 'D':
			EngineTemperature();
			break;
#endif
		default:
			printf("%s", "\nPlease choose a valid option.\n");
		}
	}
}
