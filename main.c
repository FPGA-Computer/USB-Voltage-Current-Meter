/*
 * main.c
 *
 * Created: August 30, 2019, 6:07:55 PM
 *  Author: K. C. Lee
 * Copyright (c) 2017 by K. C. Lee
 
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

void main(void)
{
	Init_Hardware();
	
	while(1)
	{
		wfi();		
		if(update)
		{
			update=0;
			Update();
		}
	}
}

void BinTo7Seg(int16_t Number,uint8_t Pos)
{
	uint8_t ZeroSup = Pos & ZERO_SUP;
	uint16_t Digit;
	
	Pos &= COL_MASK;
	
	if(Number<0)
	  Number = 0;
	
	Digit = Number/100;
	Number = Number%100;
	
	if(!Digit && ZeroSup)			// Supress 0
		Display[Pos++] = 0;
	else
	{	
		Display[Pos++] = CharMap[Digit];
		ZeroSup = 0;
	}
	
	Digit = Number/10;
	
	if(!Digit && ZeroSup)			// Supress 0
		Display[Pos++] = 0;
	else
		Display[Pos++] = CharMap[Digit];

	Digit = Number%10;
	Display[Pos] = CharMap[Digit];	
}

// For debug - display the raw value
void BinDisplay(int16_t Number)
{
	int16_t Digit;
	
	Digit = Number/10000;
	Number = Number%10000;
	Display[0] = 0;
	Display[1] = CharMap[Digit];
	Digit = Number/1000;
	Number = Number%1000;	
	Display[2] = CharMap[Digit];	
	BinTo7Seg(Number,3);
}

uint16_t Voltage[ADC_AVERAGES],Current[ADC_AVERAGES];
uint8_t SumIndex, flag;

void Update(void)
{	uint8_t i;
	int16_t V_Avg,I_Avg;
	
	V_Avg =ADC_Conv(VIN_CH);
	I_Avg = ADC_Conv(IIN_CH);

	// moving average
	Voltage[SumIndex] = ADC_Conv(VIN_CH);
	Current[SumIndex] = ADC_Conv(IIN_CH);

	if (++SumIndex >=ADC_AVERAGES)
	{
		SumIndex=0;
		flag = 1;
	}
	
	V_Avg = I_Avg = 0;
	
	for(i=0;i<ADC_AVERAGES;i++)
	{
		V_Avg += Voltage[i];
		I_Avg += Current[i];
	}

	V_Avg /= ADC_AVERAGES;
	I_Avg /= ADC_AVERAGES;

//#define CALIBRATE_I
//#define CALIBRATE_V
#ifdef CALIBRATE_I
	BinDisplay(I_Avg);
#else
#ifdef CALIBRATE_V
	BinDisplay(V_Avg);
#else

	// calibrated values - depends on parts
	V_Avg = (V_Avg*12200L)/48977L;
	
	// current requires piecewise fit.  Can divider it further to improve accuracy
	if(I_Avg<1014)	// 0.6A
		I_Avg = ((I_Avg-83)*1634000L)/2518955L;	
	else
		I_Avg = ((I_Avg-62)*86000L)/135423L;

	if(flag)
	{	
		Display[VOLTAGE_COL] = 0;
		
		if(V_Avg < VIN_LOW)
		{
			Display[VOLTAGE_COL+1] = CHAR_L;
			Display[VOLTAGE_COL+2] = CHAR_O;
		}
		else if(V_Avg >= VIN_HIGH)
		{ 
			Display[VOLTAGE_COL+1] = CHAR_O;
			Display[VOLTAGE_COL+2] = CHAR_V;
		}
		else if(V_Avg >= V_RANGE)
			BinTo7Seg(V_Avg/10,VOLTAGE_COL);
		else
		{
			BinTo7Seg(V_Avg,VOLTAGE_COL);
			Display[VOLTAGE_COL] |= SEG_DP;			// turn on decimal point
		}
		
		if(I_Avg < I_RANGE)
			BinTo7Seg(I_Avg,CURRENT_COL|ZERO_SUP);
		else
		{
			BinTo7Seg(I_Avg/10,CURRENT_COL);
			// turn on decimal point
			Display[CURRENT_COL] |= SEG_DP;		
		}
	}
#endif
#endif
}

