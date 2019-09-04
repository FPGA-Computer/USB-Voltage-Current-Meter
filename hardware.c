/*
 * hardware.c
 *
 * Created: August 30, 2019, 6:35:08 PM
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

#include "hardware.h"

volatile uint8_t Display_Col,tick,update;
volatile uint8_t Display[COLUMN_MAX];

const uint8_t CharMap[] =
{
/* 7 segment display

     --- A ---
    |         |
    F         B
    |         |
     --- G ---
    |         |
    E         C
    |         |
     --- D ---  DP
*/

	/* 0 */	SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,
	/* 1 */	      SEG_B|SEG_C,
	/* 2 */	SEG_A|SEG_B|      SEG_D|SEG_E|      SEG_G,
	/* 3 */	SEG_A|SEG_B|SEG_C|SEG_D|            SEG_G,
	/* 4 */	      SEG_B|SEG_C|            SEG_F|SEG_G,
	/* 5 */	SEG_A|      SEG_C|SEG_D|      SEG_F|SEG_G,
	/* 6 */	SEG_A|      SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
	/* 7 */	SEG_A|SEG_B|SEG_C,
	/* 8 */ SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
	/* 9 */	SEG_A|SEG_B|SEG_C|SEG_D|      SEG_F|SEG_G,
};

const Column_Drv_t ColumnDrv[] = 
{	
	{	~CC1,CC1_PORT }, 
	{	~CC2,CC2_PORT },
	{ ~CC3,CC3_PORT },
	{	~CC4,CC4_PORT },
	{	~CC5,CC5_PORT },
	{	~CC6,CC6_PORT },
};

void Init_Hardware(void)
{
	// Clk = 16MHz
	CLK->CKDIVR = 0;
	
	// turn on ADC, clock = /8
	ADC1->CR1 = ADC_CLKDIV|ADC1_CR1_ADON;
	// ADC data is right align
	ADC1->CR2 = ADC1_CR2_ALIGN;
	
	Display_Col =0;
	
	// 7 seg drivers = push/pull
	GPIOA->DDR = GPIOA->CR1 = PA_SEGS;
	GPIOC->DDR = GPIOC->CR1 = PC_SEGS;
	
	// column drivers = open drain, set to '1'
	GPIOB->DDR = GPIOB->ODR = PB_COLS;	
	GPIOD->DDR = GPIOD->ODR = PD_COLS;
	
	TIM2->PSCR = TIM2_PSCR;
	TIM2->ARRH = TIM2_ARRH;
	TIM2->ARRL = TIM2_ARRL;
	
		// TIM2 enable
	TIM2->CR1 = TIM2_CR1_CEN;
	// Update interrupt
	TIM2->IER = TIM2_IER_UIE;
	
	update=0;
	
		// Turn on interrupts
	rim();
}

uint16_t ADC_Conv(uint8_t Ch)
{
	uint8_t i;
	uint16_t ADC_Sum;
	
	// select ADC channel
	ADC1->CSR = Ch;
	ADC_Sum=0;
	
	for(i=ADC_OVERSAMPLE;i;i--)
	{
		// clear EOC flag
		ADC1->CSR &= ~ADC1_CSR_EOC;
		// start ADC, clock = /8
		ADC1->CR1 = ADC_CLKDIV|ADC1_CR1_ADON;

		while(!(ADC1->CSR & ADC1_CSR_EOC))
		  /* wait */;
		
		// read 16-bit value L->H
		ADC_Sum += *((uint16_t*)&ADC1->DRH);
	}
	return(ADC_Sum/ADC_DECIMATE);
}

@far @interrupt void TIM2_Update_IRQ(void)
{
	TIM2->SR1 &= ~TIM2_SR1_UIF;
	// clear display
	GPIOB->ODR = PB_COLS;	
	GPIOD->ODR = PD_COLS;
	
	if(Display_Col>=COLUMN_MAX)
	  Display_Col=0;
	else
		Display_Col++;	

	// Turn on column
	GPIO_ODR(ColumnDrv[Display_Col].Port) = ColumnDrv[Display_Col].Bit;
	
	// Turn on segments
	GPIOA->ODR = PORTA_SEG(Display[Display_Col]);
	GPIOC->ODR = PORTC_SEG(Display[Display_Col]);
	
	if(tick)
		tick--;
	else
	{
		tick = TICK_MAX;
		update = 1;
	}
}
