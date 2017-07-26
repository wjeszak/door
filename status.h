/*
 * status.h
 *
 *  Created on: 26 lip 2017
 *      Author: tomek
 */

#ifndef STATUS_H_
#define STATUS_H_

#define ILOSC_PROBEK 		10

void AdcInit();
uint16_t AdcRead(uint8_t kanal);

#define ADC_BITY_1_2 		5
#define ADC_BITY_3_4		4
#define ADC_BITY_5_6		3

#define V_BITY_R_R 			580 			// w rzeczywistoœci 3.4 V, ale dla bezpieczeñstwa, R - rozwarty, Z - zwarty
#define V_BITY_Z_R 			500 			// - '' -			2.5 V
#define V_BITY_R_Z 			320 			// - '' - 			1.6 V

uint8_t GetStatus();


#endif /* STATUS_H_ */
