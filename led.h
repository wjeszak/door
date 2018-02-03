/*
 * led.h
 *
 *  Created on: 3 lut 2018
 *      Author: wjeszak
 */

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define LED_GREEN_DDR 				DDRD
#define LED_GREEN_PORT 				PORTD
#define LED_GREEN1_PIN 				4
#define LED_GREEN2_PIN				5

#define LED_RED_DDR 				DDRC
#define LED_RED_PORT 				PORTC
#define LED_RED1_PIN 				1
#define LED_RED2_PIN 				0

#define LED_GREEN1_ON 			LED_GREEN_PORT &= ~(1 << LED_GREEN1_PIN)
#define LED_GREEN2_ON 			LED_GREEN_PORT &= ~(1 << LED_GREEN2_PIN)
#define LED_GREEN1_OFF 			LED_GREEN_PORT |= (1 << LED_GREEN1_PIN)
#define LED_GREEN2_OFF 			LED_GREEN_PORT |= (1 << LED_GREEN2_PIN)

#define LED_RED1_ON 			LED_RED_PORT &= ~(1 << LED_RED1_PIN)
#define LED_RED2_ON 			LED_RED_PORT &= ~(1 << LED_RED2_PIN)
#define LED_RED1_OFF 			LED_RED_PORT |= (1 << LED_RED1_PIN)
#define LED_RED2_OFF 			LED_RED_PORT |= (1 << LED_RED2_PIN)

#define LED_PULSE_PERIOD		100			// [ms]
#define LED_PULSE_DELAY_PERIOD	1000		// [ms]

class Led
{
public:
	Led();
	void SetParams(uint8_t led_color1, uint8_t led_color2, uint8_t led_pulses);
	uint8_t color1, color2, pulses_cnt, pulses;
	uint8_t current_led;
};

extern Led led;

#endif /* LED_H_ */
