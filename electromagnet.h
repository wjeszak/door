/*
 * electromagnet.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef ELECTROMAGNET_H_
#define ELECTROMAGNET_H_

#include <inttypes.h>
#include "machine.h"
#if defined (__AVR_ATmega88P__)
#define ELECTROMAGNET_CTRL_DDR 			DDRC
#define ELECTROMAGNET_CTRL_PORT 		PORTC
#define ELECTROMAGNET_CTRL_PIN 			2

#define ELECTROMAGNET_TEST_COIL_DDR		DDRB
#define ELECTROMAGNET_TEST_COIL_PPIN 	PINB
#define ELECTROMAGNET_TEST_COIL_PIN		1

#define ELECTROMAGNET_INIT 				ELECTROMAGNET_CTRL_DDR |= (1 << ELECTROMAGNET_CTRL_PIN); ELECTROMAGNET_OFF

#define ELECTROMAGNET_ON 				ELECTROMAGNET_CTRL_PORT &= ~(1 << ELECTROMAGNET_CTRL_PIN)
#define ELECTROMAGNET_OFF 				ELECTROMAGNET_CTRL_PORT |=  (1 << ELECTROMAGNET_CTRL_PIN)
#endif

#if defined (__AVR_ATmega8__)
#define ELECTROMAGNET_CTRL_DDR 			DDRD
#define ELECTROMAGNET_CTRL_PORT 		PORTD
#define ELECTROMAGNET_CTRL_PIN 			6

#define ELECTROMAGNET_TEST_COIL_DDR		DDRD
#define ELECTROMAGNET_TEST_COIL_PPIN 	PIND
#define ELECTROMAGNET_TEST_COIL_PIN		7

#define ELECTROMAGNET_INIT 				ELECTROMAGNET_CTRL_DDR |= (1 << ELECTROMAGNET_CTRL_PIN); ELECTROMAGNET_OFF

#define ELECTROMAGNET_ON 				ELECTROMAGNET_CTRL_PORT &= ~(1 << ELECTROMAGNET_CTRL_PIN)
#define ELECTROMAGNET_OFF 				ELECTROMAGNET_CTRL_PORT |=  (1 << ELECTROMAGNET_CTRL_PIN)

#endif
class ElectromagnetData : public EventData
{
public:

};

class Electromagnet : public Machine
{
public:
	Electromagnet();
	void On();
	void Off();
	void TestCoil(ElectromagnetData* pdata = NULL);
private:
	void ST_Off(ElectromagnetData* pdata);
	void ST_On(ElectromagnetData* pdata);
	void ST_Test(ElectromagnetData* pdata);

	enum States {ST_OFF = 0, ST_ON, ST_TEST_COIL, ST_MAX_STATES};
	const StateStruct* GetStateMap()
		{
			// to jest sprytne bo StateMap jest tworzone nie na stosie dzieki temu mozna zwrocic adres
			static const StateStruct StateMap[] =
			{
				{reinterpret_cast<StateFunc>(&Electromagnet::ST_Off)},
				{reinterpret_cast<StateFunc>(&Electromagnet::ST_On)},
				{reinterpret_cast<StateFunc>(&Electromagnet::ST_Test)}
			};
			return &StateMap[0];
		}
};

extern Electromagnet electromagnet;

#endif /* ELECTROMAGNET_H_ */
