/*
 * AD5933.h
 *
 * Created: 9/19/2018 5:52:11 PM
 *  Author: milad
 */ 


#ifndef AD5933_H_
#define AD5933_H_

#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>
#include "twiFunctions.h"

void impedanceInit();
double impMeasuring();
double impCalibration();



#endif /* AD5933_H_ */
