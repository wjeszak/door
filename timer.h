/*
 * timer.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

enum TimerId
{
	TTest_Elm, TDoorClosed, TElmOffOn, TWaitingForOpen, TEmergencyOff, TEmergencyOn, TLockerboxOpenedReply, TLockerboxD0Timer, TNumberOfTimers
};
/*#define NUMBER_OF_TIMERS 				6

#define TIMER_TEST_ELM					0
#define TIMER_DOOR_CLOSED 				1
#define TIMER_ELM_OFF_ON 				2
#define TIMER_WAITING_FOR_OPEN			3
#define TIMER_EMERGENCY_OFF 			4 		// default 300 ms
#define TIMER_EMERGENCY_ON 				5
*/

#define LOCKERBOX_EMERG_ON1				350
#define LOCKERBOX_EMERG_OFF				500
#define LOCKERBOX_EMERG_ON2 			450
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
	void Assign(TimerId handler_id, uint16_t interval, void(*fp)());
	void Enable (TimerId handler_id);
	void Disable (TimerId handler_id);
private:
	struct TimerHandler
	{
		void(*fp)();
		bool active;
		uint16_t interval;
		uint16_t counter;
	};
	TimerHandler timer_handlers[TNumberOfTimers];
};

extern Timer timer;

extern void ElmTestDynabox();
extern void ElmTestLockerbox();
extern void DoorClosed();
extern void ElmOffOn();
extern void WaitingForOpen();
extern void EmergencyOff();
extern void EmergencyOn();
extern void EmergencyOff2();
extern void LockerboxOpenedReply();
extern void LockerboxD0();

#endif /* TIMER_H_ */
