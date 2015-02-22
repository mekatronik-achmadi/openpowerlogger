#ifndef LIB_ADC_H
#define LIB_ADC_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "memstreams.h"

#define calib_v0 0.01
#define calib_i0 0.01
#define calib_v1 0.01
#define calib_i1 0.01

#define offset_v0 0
#define offset_i0 0
#define offset_v1 0
#define offset_i1 0

#define VALUE_FLOAT

#define ADC_GRP1_NUM_CHANNELS   4
#define ADC_GRP1_BUF_DEPTH      10

void Adc_Init(void);
void Adc_Calc(void);

#endif // LIB_ADC_H
