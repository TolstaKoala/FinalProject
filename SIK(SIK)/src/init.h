/*
 * init.h
 *
 *  Created on: 11 апр. 2017 г.
 *      Author: aod
 */

#ifndef INIT_H_
#define INIT_H_

#include <stdint.h>

typedef struct{
	uint16_t data 	:8;
	uint16_t FE		:1;
	uint16_t PE		:1;
	uint16_t BE		:1;
	uint16_t OE		:1;
	uint16_t dummy	:4;
}UartData;

typedef union{
    UartData bit;
    uint16_t array;
}UartDataReg;

void initCPUFreq();
void portUART1Init(uint32_t baudrate);
void portUART2Init(uint32_t baudrate);
void timerInit();

#endif /* INIT_H_ */
