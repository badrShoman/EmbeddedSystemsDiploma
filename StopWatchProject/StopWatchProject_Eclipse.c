/*
 * StopWatchProject_Eclipse.c
 *
 *  Created on: Sep 20, 2022
 *      Author: Mohamed Badr
 */

#include <avr/io.h>				// Include avr input output library
#include <util/delay.h>			// Include time delay library
#include <avr/interrupt.h>		// Include avr interrupt library

unsigned char hours = 0;		// Hours counted global variable
unsigned char minutes = 0;		// Minutes counted global variable
unsigned char seconds = 0;		// Seconds counted global variable

/* Timer1 interrupt that handles stopwatch time increament */
ISR(TIMER1_COMPA_vect) {
	seconds++;					// Increase a second every second
	/* Increase a minute every 60 seconds and set seconds to 0 */
	if (seconds == 60) {
		seconds = 0;
		minutes++;
	}
	/* Increase an hourr every 60 minutes and set minutes to 0*/
	if (minutes == 60) {
		minutes = 0;
		hours++;
	}
	/* When reacing 24 hours start counting from 0 again */
	if (hours > 23) {
		seconds = 0;
		seconds = 0;
		hours = 0;
	}
}

/* Interrupt 0 activates upon resetting */
ISR(INT0_vect) {
	hours = 0;					// Set hours to 0
	minutes = 0;				// Set minutes to 0
	seconds = 0;				// Set seconds to 0
	/* Turn TIMER1 on and set pre-scaler to clock/64 */
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12);
	TCNT1 = 0;					// Reset counter value to 0
}

/* Interrupt 1 activates upon pausing */
ISR(INT1_vect) {
	/* Turn TIMER1 off without changing time values */
	TCCR1B &= ~(1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);
}

/* Interrupt 2 activates upon resuming */
ISR(INT2_vect) {
	/* Turn TIMER1 on with pre-scaler clock/64 without changing time values */
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS12);
}

/* Interrupt 0 Initialization */
void INT0_Init(void) {
	/* Trigger INT0 with the falling edge */
	MCUCR &= ~(1 << ISC00);
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);		// Enable external interrupt pin at INT0
}

/* Interrupt 1 Initialization */
void INT1_Init(void) {
	/* Trigger INT1 with the rising edge */
	MCUCR |= (1 << ISC10);
	MCUCR &= ~(1 << ISC11);
	GICR |= (1 << INT1);    	// Enable external interrupt pin at INT1
}

/* Interrupt 2 Initialization */
void INT2_Init(void) {
	MCUCSR &= ~(1 << ISC2);     // Trigger INT2 with the falling edge
	GICR |= (1 << INT2);		// Enable external interrupt pin at INT2
}

/* Timer1 Initialization */
void Timer1_CTC_Init(void) {
	TCNT1 = 0;					// Set timer initial value to 0
	/* Set compare value to 15625 representing 1 second
	 * for 1 Mhz clock with pre-scaler of 64*/
	OCR1A = 15625;
	TCCR1A |= (1 << FOC1A);		// Enable force compare unit A bit
	/* Set pre-scaler to clock/64 and enable compare mode */
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
	TIMSK = (1 << OCIE1A);		// Enable compare A match interrupt
}

int main(void) {
	sei();						// Enable global interrupt (I bit)
	DDRA |= 0x3F;				// Set PA0 to PA5 as output pins
	DDRB &= ~(1 << PB2);		// Set all PORTB as input pins
	DDRC |= 0x0F;				// Set PC0 to PC3 as output pins
	DDRD &= ~0x0C;				// Set all PORTD as input pins
	PORTA &= ~0x3F;				// Initialize all PORTA pins by 0
	PORTB |= (1 << PB2);		// Enable AVR internal pull-up at PB2
	PORTC &= ~0x0F;				// Initialize all PORTC pins by 0
	PORTD |= (1 << PD2);		// Enable AVR internal pull-up at PD2
	INT0_Init();				// Enable external interrupt 0 (INT0)
	INT1_Init();				// Enable external interrupt 1 (INT1)
	INT2_Init();				// Enable external interrupt 2 (INT2)
	Timer1_CTC_Init();			// Enable Timer1
	while (1) {
		PORTA = (1 << PA0);		// Enable PA0 anode
		PORTC = seconds % 10;	// Obtain seconds unit's value
		_delay_us(10);			// Wait 10 micro-seconds
		PORTA = (1 << PA1);		// Enable PA1 anode
		PORTC = seconds / 10;	// Obtain seconds ten's value
		_delay_us(10);			// Wait 10 micro-seconds
		PORTA = (1 << PA2);		// Enable PA2 anode
		PORTC = minutes % 10;	// Obtain minutes unit value
		_delay_us(10);			// Wait 10 micro-seconds
		PORTA = (1 << PA3);		// Enable PA3 anode
		PORTC = minutes / 10;	// Obtain minutes ten's value
		_delay_us(10);			// Wait 10 micro-seconds
		PORTA = (1 << PA4);		// Enable PA4 anode
		PORTC = hours % 10;		// Obtain hours unit value
		_delay_us(10);			// Wait 10 micro-seconds
		PORTA = (1 << PA5);		// Enable PA5 anode
		PORTC = hours / 10;		// Obtain hours ten's value
		_delay_us(10);			// Wait 10 micro-seconds
	}
}
