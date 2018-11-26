/*
 * AD5933.c
 *
 * Created: 9/19/2018 5:52:02 PM
 *  Author: milad
 */ 

#include "AD5933.h"

void impedanceInit()
{
	sendByte(highStartFreqReg, 0x0E);
	sendByte(midStartFreqReg, 0xA6);
	sendByte(lowStartFreqReg, 0x45);		//set start frequency 30KHz (clock frequency = external 16.776MHz)
	
	sendByte(highNoOfIncReg, 0x00);
	sendByte(lowNoOfIncReg, numOfIncrements);			//No. of Increments = 100
	
	sendByte(highFreqIncReg, 0x00);
	sendByte(midFreqIncReg, 0x01);
	sendByte(lowFreqIncReg, 0x40);			//set Incremental frequency 10Hz (clock frequency = external 16.776MHz)
		
	//Place the part into standby mode.
	sendByte(highControlReg, 0xB0);			//the control register should not be written to as part of a block write command.
	sendByte(lowControlReg, 0x00);			//internal system clock. Reset isn't issued. ****This register should not be altered****
		
	//Set Number of Settling Time Cycles
	sendByte(highNoOfSettCycleReg, 0x00);
	sendByte(lowNoOfSettCycleReg, 0x5F);	//15 cycles
		
	//Initialize with start frequency
	sendByte(highControlReg, 0x10);
		
	//Nominal delay (according to UG-364)
	_delay_ms(2);
	//Start frequency sweep
	sendByte(highControlReg, 0x20);
	
	//For changing the excitation voltage and PGA gain
	sendByte(highControlReg, 0x01);		// excitation voltage = 2V, PGA = 1
	//sendByte(highControlReg, 0x00);		// excitation voltage = 2V, PGA = 5
	//sendByte(highControlReg, 0x07);		// excitation voltage = 1V, PGA = 1
	//sendByte(highControlReg, 0x06);		// excitation voltage = 1V, PGA = 5
	//sendByte(highControlReg, 0x05);		// excitation voltage = 400mV, PGA = 1
	//sendByte(highControlReg, 0x04);		// excitation voltage = 400mV, PGA = 5
	//sendByte(highControlReg, 0x03);		// excitation voltage = 200mV, PGA = 1
	//sendByte(highControlReg, 0x02);		// excitation voltage = 200mV, PGA = 5
}
// returns the average of magnitudes
double impMeasuring()
{
	uint8_t realDataUpper, realDataLower, imaginaryDataUpper, imaginaryDataLower;
	int realPart, imaginePart;
	double magnitude = 0, impedance, Yx, Ycell, sum = 0, avgOfmagnitudes = 0;
	
	while(!(getByte(statusReg) & 0x04))
	{
		while(!(getByte(statusReg) & 0x02));
		realDataUpper = getByte(highRealDataReg);
		realDataLower = getByte(lowRealDataReg);
		realPart = (realDataUpper*256) + realDataLower;

		imaginaryDataUpper = getByte(highImgDataReg);
		imaginaryDataLower = getByte(lowImgDataReg);
		imaginePart = (imaginaryDataUpper*256) + imaginaryDataLower;

		if (realPart > 0x7FFF)
		{
			realPart = realPart - 0x10000;
		}
		if (imaginePart > 0x7FFF)
		{
			imaginePart = imaginePart - 0x10000;
		}			
		magnitude = pow(pow(realPart, 2) + pow(imaginePart, 2), 0.5);
		sum += magnitude;
		//Increment to next frequency point
		sendByte(highControlReg, 0x31);				//remember to calculate with the PGA of 1
	}
	avgOfmagnitudes = sum / numOfIncrements;
	//Place the part into power-down mode.
	sendByte(highControlReg, 0xA1);
	return avgOfmagnitudes;
}

double impCalibration()
{
	double range1000, range500, measureValue, Yx, Ycell, impedance;
	double gf, Nos;
	char dataImpedance[20];
	
	/////////////////////////////////// ADG715 variables ///////////////////////////////////
// 	char RCal1000 = 0x09;
// 	char RCal500 = 0x11;
	char RCal1000 = 0x21;
	char RCal500 = 0x11;
	char highYxMeasurement = 0x81;
	////////////////////////////////////////////////////////////////////////////////////////
	
	
	ADG715_write(RCal500);
	impedanceInit();
	range500 = impMeasuring();
// 	dtostrf(range500, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "RCAL500: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
		
	ADG715_write(RCal1000);
	impedanceInit();
	range1000 = impMeasuring();
// 	dtostrf(range1000, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "RCAL1000: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
	
	gf = (0.001 / (range500 - range1000));			// 2mS - 1mS = 1mS -> 0.001
	Nos = (range1000 - (0.001 / gf));				// 10mS -> 0.01
// 	dtostrf(gf, 16, 12, dataImpedance);
// 	usart_puts(USART_SERIAL1, "gf: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
// 	dtostrf(Nos, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "Nos: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");

	ADG715_write(highYxMeasurement);
	impedanceInit();
	measureValue = impMeasuring();
// 	dtostrf(measureValue, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "measureValue: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
	Yx = (measureValue - Nos) * gf;
	Ycell = Yx / (1 - 100 * Yx);
	impedance = 1 / Ycell;
// 	dtostrf(impedance, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "liquidImp: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
	return impedance;
}
