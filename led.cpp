/*
 * led.cpp
 *
 *  Created on: 3 lut 2018
 *      Author: wjeszak
 */

#include "led.h"
#include "timer.h"

Led::Led()
{
	LED_GREEN_DDR  |= (1 << LED_GREEN1_PIN) | (1 << LED_GREEN2_PIN);
	LED_RED_DDR  |= (1 << LED_RED1_PIN) | (1 << LED_RED2_PIN);

	LED_GREEN_PORT |= (1 << LED_GREEN1_PIN) | (1 << LED_GREEN2_PIN);
	LED_RED_PORT |= (1 << LED_RED1_PIN) | (1 << LED_RED2_PIN);
}

void Led::SetParams(uint8_t led_color1, uint8_t led_color2, uint8_t led_pulses)
{
	color1 = led_color1;
	color2 = led_color2;
	pulses = led_pulses;
	timer.Assign(TLedPulse, LED_PULSE_PERIOD, Pulse);
	timer.Assign(TLedDelay, LED_PULSE_DELAY_PERIOD, Delay);
}
