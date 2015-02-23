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

/**
 * @brief buat Kalibrasi
 * @brief nilai disini adalah pengali untuk nilai ADC berupa bilangan pecahan antara 0 dan 1
 */
#define calib_v0 0.01
#define calib_i0 0.01
#define calib_v1 0.01
#define calib_i1 0.01

/**
 * @brief buat Testing
 * @brief jika ACCEL_TIME 1 maka selang waktu akan lebih cepat
 *        jika ACCEL_TIME 0 maka waktu berjalan normal
 * @brief jika VALUE_FLOAT 1 maka yang disimpan dan tampil adalah nilai float hasil kalibrasi,
 *        jika VALUE_FLOAT 0 maka nilai mentah dari ADC
 */
#define ACCEL_TIME 1
#define VALUE_FLOAT 0

/**
 * @brief   Float type support
 * @brief   take from chprintf.c and chprintf.h
 * @brief   include this header file in chprintf.h
 */
#if !defined(CHPRINTF_USE_FLOAT) || defined(__DOXYGEN__)
#define CHPRINTF_USE_FLOAT          TRUE
#endif
#define MAX_FILLER 16
#define FLOAT_PRECISION 10

#define ADC_GRP1_NUM_CHANNELS   4
#define ADC_GRP1_BUF_DEPTH      10

void Adc_Init(void);
void Adc_Calc(void);

#endif // LIB_ADC_H
