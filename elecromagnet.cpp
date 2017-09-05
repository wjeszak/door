/*
 * elecromagnet.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>

#include "comm_prot.h"
#include "electromagnet.h"
#include "timer.h"
#include "usart.h"

Electromagnet::Electromagnet()
{
	ELECTROMAGNET_INIT;
}
