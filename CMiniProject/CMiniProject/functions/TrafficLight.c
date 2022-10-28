#include <stdio.h>
#include "..\\src\\headers\\Functions.h"
#include "..\\src\\headers\\Globals.h"

void TrafficLight() {
	while (true) {
		printf("%s", "\n\tEnter 'G' for green light.\n"
				"\tEnter 'O' for orange light.\n"
				"\tEnter 'R' for red light.\n");
		printf("%s", "\nYour choice: ");
		scanf(" %c", &userInput);
		switch (userInput) {
		case 'g':
		case 'G':
			vehicleSpeed = 100;
			VehicleStatus();
			return;
		case 'o':
		case 'O':
			vehicleSpeed = 30;
			if (cabinetTemperatureControl == false) {
				cabinetTemperatureControl = true;
				cabinetTemperature *= (5 / 4) + 1;
			}
			if (engineTemperatureControl == false) {
				engineTemperatureControl = true;
				engineTemperature *= (5 / 4) + 1;
			}
			VehicleStatus();
			return;
		case 'r':
		case 'R':
			vehicleSpeed = 0;
			VehicleStatus();
			return;
		default:
			printf("%s", "Please choose a valid option.\n");
		}
	}
}
