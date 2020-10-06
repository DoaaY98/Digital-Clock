/*
 * main.c
 *
 *  Created on: Aug 26, 2020
 *      Author: doaay
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
unsigned char f_seconds=0;
unsigned char s_seconds=0;
unsigned char f_minutes=0;
unsigned char s_minutes=0;
unsigned char f_hours=0;
unsigned char s_hours=0;

void Timer1_Init(){
	TCCR1A =(1<<FOC1A)|(1<<FOC1B);
	TCCR1B =(1<<CS12)|(1<<CS10)|(1<<WGM12);
	TCNT1=0;
	OCR1A=1000;
	TIMSK|= (1<<OCIE1A);

}
void INT1_INIT(){
	DDRD &= ~(1<<PD3); //INT1 OUTPUT
	MCUCR |=(1<<ISC11) |(1<<ISC10);
	GICR |=(1<<INT1);
	SREG |=(1<<7);

}
void INT0_INIT(){
	DDRD &= ~(1<<PD2);
	PORTD |=(1<<PD2);

	MCUCR |=(1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR |=(1<<INT0);
	SREG|=(1<<7);
}
void INT2_INIT(){
	DDRB &= ~(1<<PB2);
	PORTB |=(1<<PB2);
	MCUCSR &= ~(1<<ISC2);
	GICR |=(1<<INT2);
	SREG|=(1<<7);
}

ISR(INT0_vect){
	SREG|=(1<<7);
	f_seconds=0;
	s_seconds=0;
	f_minutes=0;
	s_minutes=0;
	f_hours=0;
	s_hours=0;

}

ISR(INT1_vect){
	SREG |=(1<<7);
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS10);

}
ISR(INT2_vect){
	SREG |=(1<<7);
	TCCR1B =(1<<CS12)|(1<<CS10);
}
ISR(TIMER1_COMPA_vect){
	SREG |=(1<<7);
	f_seconds++;
	TCNT1=0;
	unsigned char value[6]={s_hours,f_hours,s_minutes,f_minutes,s_seconds,f_seconds};
	for(unsigned char i=0;i<6;i++){
			PORTA =(1<<i);
			PORTC=value[i];
			_delay_ms(2);


		}
}
int main(){

	SREG |= (1<<7);
	DDRC |=(0x0F);
	DDRA |=(0x3F);

	//PORTA|=(1<<PA5);


	Timer1_Init();
	INT1_INIT();
	INT0_INIT();
	INT2_INIT();
	while(1){
		unsigned char value[6]={s_hours,f_hours,s_minutes,f_minutes,s_seconds,f_seconds};

		if(f_seconds==10 && s_seconds!=5){
					f_seconds=0;
					s_seconds++;
				}
				else if(f_seconds ==10 && s_seconds==5){
					f_seconds=0;
					s_seconds=0;
					f_minutes++;
					if(f_minutes ==10 && s_minutes!=5 ){
						f_minutes=0;
						s_minutes++;
					}
					else if(f_minutes==10 && s_minutes==5){
						f_minutes=0;
						s_minutes=0;
						f_hours++;
						if(f_hours==10){
							f_hours=0;
							s_hours++;
						}
						if(f_hours==4 && s_hours==2 ){
							f_seconds=0;
							s_seconds=0;
							f_minutes=0;
							s_minutes=0;
							f_hours=0;
							s_hours=0;

						}
					}

				}
		//PORTC=f_seconds;
		for(unsigned char i=0;i<6;i++){
			PORTA =(1<<i);
			PORTC=value[i];
			_delay_ms(2);


		}



	}
}

