/*
 * mini_project2.c
 *
 *  Created on: May 1, 2022
 *      Author: Ahmed Nasr
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/* global variables for each 7 segments*/
unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hour1=0;
unsigned char hour2=0;
void Timer1(){/*Timer1 enable and configuration function*/
	TCNT1 = 0;
	OCR1A = 1000;
	TIMSK |= (1<<OCIE1A);
	TCCR1A =(1<<FOC1A) | (1<<FOC1B);
	TCCR1B |=(1<<CS10) | (1<<CS12) | (1<<WGM12);
}
void intiat(){/*GPIO configurations and initial values , external interrupts enable and configurations ,global interrupts enable*/
	DDRD &=(~(1<<PD3)) & (~(1<<PD2));
	DDRB &=(~(1<<PB2));
	DDRC=0x0f;
	DDRA=0x3F;
	SREG |= (1<<7);
	GICR   |= (1<<INT2) |(1<<INT1) | (1<<INT0) ;
	MCUCR |=(1<<ISC01) | (1<<ISC11) | (1<<ISC10);
	MCUCSR |=(1<<ISC2);
	PORTD |= (1<<PD2);
	PORTB |= (1<<PB2);
	PORTA &=0xC0;
	PORTC &=0xf0;
}
ISR(INT1_vect){/* External INT1 Interrupt Service Routine */
	TIMSK &=(~(1<<OCIE1A)); /*disable timer1*/
}
ISR(INT0_vect){/* External INT0 Interrupt Service Routine */
	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hour1=0;
	hour2=0;
}
ISR(INT2_vect){/* External INT2 Interrupt Service Routine */
	TIMSK |= (1<<OCIE1A); /*enable timer1*/
}
ISR(TIMER1_COMPA_vect){/* Timer1 Interrupt Service Routine */
		TCNT1 = 0;
		sec1++;
		if(sec1==10){
		sec1=0;
		sec2++;
		if(sec2==6){
			sec2=0;
			min1++;
				}
		if(min1==10){
			min1=0;
			min2++;
		}
		if(min2==6){
			min2=0;
			hour1++;
		}
		if(hour1==10){
			hour1=0;
			hour2++;
		}
		}
}
int main(void){
	intiat();
	Timer1();
	while(1){/*infinitely outputs the global variables on the 6 (7 segments)*/
		PORTA =(1<<0);
		PORTC=sec1;
		_delay_ms(2);
		PORTA =(1<<1);
		PORTC=sec2;
		_delay_ms(2);
		PORTA =(1<<2);
		PORTC=min1;
		_delay_ms(2);
		PORTA =(1<<3);
		PORTC=min2;
		_delay_ms(2);
		PORTA =(1<<4);
		PORTC=hour1;
		_delay_ms(2);
		PORTA =(1<<5);
		PORTC=hour2;
		_delay_ms(2);
}}



