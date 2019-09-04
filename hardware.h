/*
 * hardware.h
 *
 * Created: August 30, 2019, 6:07:55 PM
 *  Author: K. C. Lee
 * Copyright (c) 2016 by K. C. Lee
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.

 If not, see http://www.gnu.org/licenses/gpl-3.0.en.html 
 */ 

#ifndef HARDWARE_H_
#define HARDWARE_H_

#ifndef __CSMC__
#define __CSMC__
#endif
#define STM8S003

#include "stm8s.h"
#include <stdio.h>

// STM8S003F3P6
enum _PA { PA1=0x02, PA2=0x04, PA3=0x08 };
enum _PB { PB4=0x10, PB5=0x20 };
enum _PC { PC3=0x08, PC4=0x10, PC5=0x20, PC6=0x40, PC7=0x80 };
enum _PD { PD1=0x02, PD2=0x04, PD3=0x08, PD4=0x10, PD5=0x20, PD6=0x40 };

enum _IO7SEG
{ 
	IOSEG_G=PA1,IOSEG_C=PA2,IOSEG_DP=PA3,
	IOSEG_D=PC3,IOSEG_E=PC4,IOSEG_A=PC5,IOSEG_F=PC6,IOSEG_B=PC7
};

enum _7SEG_ENCODE
{
	SEG_G=IOSEG_G>>1,SEG_C=IOSEG_C>>1,SEG_DP=IOSEG_DP>>1,
	SEG_D=IOSEG_D,SEG_E=IOSEG_E,SEG_A=IOSEG_A,SEG_F=IOSEG_F,SEG_B=IOSEG_B
};

enum _CC_DRV
{ 
	CC2=PB4,CC2_PORT=GPIOB_BaseAddress,
	CC3=PB5,CC3_PORT=GPIOB_BaseAddress,
	CC1=PD1,CC1_PORT=GPIOD_BaseAddress,
	CC4=PD2,CC4_PORT=GPIOD_BaseAddress,
	CC5=PD3,CC5_PORT=GPIOD_BaseAddress,
	CC6=PD4,CC6_PORT=GPIOD_BaseAddress
};


#define PA_SEGS	(IOSEG_C|IOSEG_G|IOSEG_DP)
#define PB_COLS	(CC2|CC3)
#define PC_SEGS	(IOSEG_D|IOSEG_E|IOSEG_A|IOSEG_F|IOSEG_B)
#define PD_COLS	(CC1|CC4|CC5|CC6)

#define PORTA_SEG(X)		(((X) & (PA_SEGS>>1))<<1)
#define PORTC_SEG(X)		((X) & PC_SEGS)

#define GPIO_ODR(X)			((GPIO_TypeDef *)(X))->ODR

#define COLUMN_MAX			6

#define VOLTAGE_COL			0
#define CURRENT_COL			3

typedef struct 
{
	uint8_t		Bit;
	uint16_t	Port;
} Column_Drv_t;

#include "irq.h"

#define CPU_CLK					2000000UL

// 16MHz/4096 -> 3906Hz
#define TIM2_PSCR				0x0c
// reload for 355Hz/6 columns -> 59Hz
#define TIM2_ARRH				0x00
#define TIM2_ARRL				0x0a
// update every 12 display refresh
#define TICK_MAX				(COLUMN_MAX*12-1)

// ADC clock div = 1/8
#define ADC_CLKDIV			0x20

#define ADC_OVERSAMPLE	16
#define ADC_DECIMATE		4

#define ADC_AVERAGES		4

#define VIN_CH					5
#define IIN_CH					6

#define CHAR_O					(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)
#define CHAR_L					(SEG_D|SEG_E|SEG_F)
#define CHAR_V					(SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)

#define ZERO_SUP				0x80
#define COL_MASK				0x0f

#define VIN_LOW					319
#define VIN_HIGH				1019
#define V_RANGE					1000
#define I_RANGE					1000

extern volatile uint8_t tick,update;
extern volatile uint8_t Display[];
extern const uint8_t CharMap[];

void Init_Hardware(void);
void Update(void);
uint16_t ADC_Conv(uint8_t Ch);

#endif
