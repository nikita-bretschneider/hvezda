/******************************************
 * Vánoční hvězda / ATTINY13
 * 2019 Nikita Lieselotte Bretschneider
 * ( nikita.bretschneider@gmx.de )
 ******************************************/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* #define F_CPU 1200000UL */

#define OUT1		PB0	// First LED chain, can be changed
#define OUT2		PB1	// Second LED chain, can be changed
#define OUT3		PB2	// Third LED chain, can be changed
#define OUTB		PB4	// Blue backlight, can be changed

#define PERIOD1		170
#define PERIOD2		230
#define PERIOD3		150

#define OUT_STATE_1	1
#define OUT_STATE_2	2
#define OUT_STATE_3	3


unsigned char PWM1 = 3;
unsigned char PWM2 = 3;
unsigned char PWM3 = 3;
unsigned char PWMB = 0x7F;
unsigned char pwm_cnt = 0;

unsigned char PERIOD1_cnt = 0;
unsigned char PERIOD2_cnt = 0;
unsigned char PERIOD3_cnt = 0;
unsigned char OUT_STATE = 0;


int main( void)
{
	DDRB |= _BV( OUT1)|_BV( OUT2)|_BV( OUT3)|_BV( OUTB);	// Set as output
	TCCR0B |= (1<<CS00);
	TCCR0B &= ~((1<<CS01) | (1<<CS02));
	TIMSK0 |= (1<<TOIE0);
	sei();

	while( 1) 
		{
			_delay_ms( 20);

			PERIOD1_cnt++; PERIOD2_cnt++; PERIOD3_cnt++;
			
			if ( PERIOD1 < PERIOD1_cnt) {
				PERIOD1_cnt = 0;
				OUT_STATE ^= _BV( OUT_STATE_1);
			}

			if ( PERIOD2 < PERIOD2_cnt) {
				PERIOD2_cnt = 0;
				OUT_STATE ^= _BV( OUT_STATE_2);
			}

			if ( PERIOD3 < PERIOD3_cnt) {
				PERIOD3_cnt = 0;
				OUT_STATE ^= _BV( OUT_STATE_3);
			}

			if (( OUT_STATE & _BV( OUT_STATE_1)) && ( 0x1A > PWM1)) PWM1++;
			if (( OUT_STATE & _BV( OUT_STATE_2)) && ( 0x1A > PWM2)) PWM2++;
			if (( OUT_STATE & _BV( OUT_STATE_3)) && ( 0x1A > PWM3)) PWM3++;

			if ((!( OUT_STATE & _BV( OUT_STATE_1))) && ( 0x00 < PWM1)) PWM1--;
			if ((!( OUT_STATE & _BV( OUT_STATE_2))) && ( 0x00 < PWM2)) PWM2--;
			if ((!( OUT_STATE & _BV( OUT_STATE_3))) && ( 0x00 < PWM3)) PWM3--;

			PWMB = 2 + (( PWM1)>>2) + (( PWM2)>>2) + (( PWM3)>>2);


		}

}

ISR( TIM0_OVF_vect)
{

	pwm_cnt++; 
	pwm_cnt &= 0x1F;
	if( pwm_cnt >= PWM1) { PORTB &= ~_BV( OUT1); } else { PORTB |= _BV( OUT1);}
	if( pwm_cnt >= PWM2) { PORTB &= ~_BV( OUT2); } else { PORTB |= _BV( OUT2);}
	if( pwm_cnt >= PWM3) { PORTB &= ~_BV( OUT3); } else { PORTB |= _BV( OUT3);}
	if( pwm_cnt >= PWMB) { PORTB &= ~_BV( OUTB); } else { PORTB |= _BV( OUTB);}
}
