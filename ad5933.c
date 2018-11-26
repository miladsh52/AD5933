/*
 * AD5933.c
 *
 * Created: 9/19/2018 5:52:02 PM
 *  Author: milad
 */ 

#include "AD5933.h"

void usart_puts(usart_if usart, char *str)
{
	while (*str)
	usart_serial_putchar(usart, *str++);
}

/*
void impedanceInit()
{
	sendByte(highStartFreqReg, 0x18);
	sendByte(midStartFreqReg, 0x6A);
	sendByte(lowStartFreqReg, 0x73);		//set start frequency 50KHz (clock frequency = external 16.776MHz)
	
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
	
	//For changing the excitation voltage and PGA gain
	sendByte(highControlReg, 0x07);		// excitation voltage = 1V, PGA = 1
}

double impMeasuring()
{
	uint8_t realDataUpper, realDataLower, imaginaryDataUpper, imaginaryDataLower;
	int realPart, imaginePart;
	double magnitude = 0, impedance, Yx, Ycell;
	char data[20];
	int counter = 0;
	while(!(getByte(statusReg) & 0x02));
	realDataUpper = getByte(highRealDataReg);
	realDataLower = getByte(lowRealDataReg);
	realPart = (realDataUpper*256) + realDataLower;
// 	usart_serial_putchar(USART_SERIAL, realPart>>8);
// 	usart_serial_putchar(USART_SERIAL, realPart);
			
	imaginaryDataUpper = getByte(highImgDataReg);
	imaginaryDataLower = getByte(lowImgDataReg);
	imaginePart = (imaginaryDataUpper*256) + imaginaryDataLower;
// 	usart_serial_putchar(USART_SERIAL, imaginePart>>8);
// 	usart_serial_putchar(USART_SERIAL, imaginePart);
// 			
// 	usart_serial_putchar(USART_SERIAL, 0xAA);
// 	usart_serial_putchar(USART_SERIAL, 0xBB);
	
	if (realPart > 0x7FFF)
	{
		realPart = realPart - 0x10000;
	}
	if (imaginePart > 0x7FFF)
	{
		imaginePart = imaginePart - 0x10000;
	}
	magnitude = pow(pow(realPart, 2) + pow(imaginePart, 2), 0.5);
			
	//Place the part into power-down mode.
	//sendByte(highControlReg, 0xA1);
	
	return magnitude;
}

void impCalibration()
{
	double range1, range2, measureValue, Yx, Ycell, impedance;
	double gf, Nos;
	char dataImpedance[20];
	char RCal100 = 0x91;			//s1, s5, s8 are closed -- other switches are opened
	char RCal500 = 0x92;			//s2, s5, s8 are closed -- other switches are opened
	char bodyMeasurement = 0x6C;	//s1, s2, s5, s8 are opened -- s3, s4, s6, s7 are closed
	
	
	ADG715_write(RCal100);
	impedanceInit();
	range1 = impMeasuring();
// 	dtostrf(range1, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "range1: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
	
	ADG715_write(RCal500);
	impedanceInit();
	range2 = impMeasuring();
// 	dtostrf(range2, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "range2: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
	
	gf = (0.008 / (range1 - range2));		// 10mS - 2mS = 8mS -> 0.008
	Nos = (range1 - (0.01 / gf));			// 10mS -> 0.01
// 	dtostrf(gf, 20, 10, dataImpedance);
// 	usart_puts(USART_SERIAL1, "gf: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");
// 	dtostrf(Nos, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "Nos: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r");

	ADG715_write(bodyMeasurement);
	impedanceInit();
	measureValue = impMeasuring();
	Yx = (measureValue - Nos) * gf;
	Ycell = Yx / (1 - 100 * Yx);
	impedance = 1 / Ycell;
// 	dtostrf(impedance, 6, 2, dataImpedance);
// 	usart_puts(USART_SERIAL1, "your body impedance is: ");
// 	usart_puts(USART_SERIAL1, dataImpedance);
// 	usart_puts(USART_SERIAL1, "\r\n");
}
*/

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

double impMeasuring()
{
	uint8_t realDataUpper, realDataLower, imaginaryDataUpper, imaginaryDataLower;
	int realPart, imaginePart;
	uint8_t increment = numOfIncrements;				//No. of Increments = 40
	double magnitude = 0, impedance, Yx, Ycell, sum = 0, avgImpedance = 0, avgOfmagnitudes = 0;
	char data[20];
	while(!(getByte(statusReg) & 0x04) && increment != 0)
	{
		while(!(getByte(statusReg) & 0x02));
		if (increment < numOfIncrements)
		{
			realDataUpper = getByte(highRealDataReg);
			realDataLower = getByte(lowRealDataReg);
			realPart = (realDataUpper*256) + realDataLower;
//  			usart_serial_putchar(USART_SERIAL1, realPart>>8);
//  			usart_serial_putchar(USART_SERIAL1, realPart);
			
			imaginaryDataUpper = getByte(highImgDataReg);
			imaginaryDataLower = getByte(lowImgDataReg);
			imaginePart = (imaginaryDataUpper*256) + imaginaryDataLower;
//  			usart_serial_putchar(USART_SERIAL1, imaginePart>>8);
//  			usart_serial_putchar(USART_SERIAL1, imaginePart);
//  			
//  			usart_serial_putchar(USART_SERIAL1, 0xAA);
//  			usart_serial_putchar(USART_SERIAL1, 0xBB);
			
			
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
		}
		//Increment to next frequency point
		sendByte(highControlReg, 0x31);				//remember to calculate with the PGA of 1
		increment = increment - 1;
	}
	avgOfmagnitudes = sum / (numOfIncrements - 1);
	// 	dtostrf(avgImpedance, 9, 2, data);
	// 	usart_puts(USART_SERIAL1, data);
	
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
