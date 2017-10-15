/*
 * elecromagnet.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include "electromagnet.h"

Elm::Elm()
{
	ELM_INIT;
}

bool Elm::ElmOn()
{
	if(!(ELM_CTRL_PPIN & (1 << ELM_CTRL_PIN)))
		return true;
	return false;
}
