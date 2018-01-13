/*
 * timer.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <stdio.h>

#define NUMBER_OF_TIMERS 				4

#define TIMER_TEST_ELM					0
#define TIMER_DOOR_CLOSED 				1
#define TIMER_ELM_OFF_ON 				2
#define TIMER_OPEN_LOCKERBOX			3

enum T0Prescallers
{
	T0_PS_0 = 0,
	T0_PS_1 = (1 << CS00),
	T0_PS_8 = (1 << CS01),
	T0_PS_64 = (1 << CS00) | (1 << CS01),
	T0_PS_256 = (1 << CS02),
	T0_PS_1024 = (1 << CS00) | (1 << CS02)
};

class Timer
{
public:
	Timer(T0Prescallers Prescaller, uint8_t Tick);
	void Irq();
	void Assign(uint8_t handler_id, uint16_t interval, void(*fp)());
	void Enable (uint8_t handler_id);
	void Disable (uint8_t handler_id);
private:
	struct TimerHandler
	{
		void(*fp)();
		bool active;
		uint16_t interval;
		uint16_t counter;
	};
	TimerHandler timer_handlers[NUMBER_OF_TIMERS];
};

extern Timer timer;

extern void ElmTestDynabox();
extern void ElmTestLockerbox();
extern void DoorClosed();
extern void ElmOffOn();
extern void OpenLockerbox();

#endif /* TIMER_H_ */
