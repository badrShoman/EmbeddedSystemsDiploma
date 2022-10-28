/*
 ============================================================================
 Name        : CMiniProject.c
 Author      : Mohamed Badr
 Version     : v1.0
 Copyright   : No copyrights
 Description : A mini C project just to put my knowledge to test.
 ============================================================================
 */

#include <stdio.h>
#include "..\\src\\headers\\Functions.h"
#include "..\\src\\headers\\Globals.h"

int main() {
	setbuf(stdout, NULL); // Disable buffering to display output without flushing at the cost of performance
	while (true) {
		printf("%s", "Please choose what you do want:\n\n"
				"\ta. Turn on vehicle engine.\n"
				"\tb. Turn off vehicle engine.\n"
				"\tc. Quit the system.\n");
		printf("%s", "\nYour choice: ");
		scanf(" %c", &userInput);
		switch (userInput) {
		case 'a':
		case 'A':
			engineTemperature = 125;
			engineState = true;
			printf("%s", "\nEngine turned on.\n");
			InitialOperation();
			break;
		case 'b':
		case 'B':
			engineTemperature = 30;
			engineState = false;
			printf("%s", "\nEngine turned off.\n\n");
			break;
		case 'c':
		case 'C':
			printf("%s", "\nQuiting the system.\n\n");
			return 0;
			break;
		default:
			printf("%s", "\nPlease choose a valid option.\n\n");
		}
	}
}
