/*
 * commands_dynabox.h
 *
 *  Created on: 30 sty 2018
 *      Author: wjeszak
 */

#ifndef COMMANDS_DYNABOX_H_
#define COMMANDS_DYNABOX_H_

#include <inttypes.h>

extern void Dynabox_CheckElm();
extern void Dynabox_GetStatusBeforeMovement();
extern void Dynabox_ElmOff();
extern void Dynabox_ElmOffOn();
extern void Dynabox_GetStatus();
extern void Dynabox_GetSetStatus(uint8_t pos);

#endif /* COMMANDS_DYNABOX_H_ */
