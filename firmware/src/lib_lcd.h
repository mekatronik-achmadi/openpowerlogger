#ifndef LIB_LCD_H
#define LIB_LCD_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "chprintf.h"
#include "memstreams.h"
#include "evtimer.h"
#include "chstreams.h"
#include "pal_lld.h"

#define _lcd_stream_data _base_sequential_stream_data

struct LcdStreamVMT {
    _base_sequential_stream_methods
};

typedef struct {
    const struct LcdStreamVMT *vmt;
    _base_sequential_stream_data
} LcdStream;

#ifdef __cplusplus
extern "C" {
#endif
void lsObjectInit(LcdStream *msp);
#ifdef __cplusplus
}
#endif

void Lcd_Pin_Dir(void);
void Lcd_Write_Command(uint8_t cmd);
void Lcd_Write_Data(uint8_t chr);
void Lcd_Init(void);
void Lcd_Cursor(uint8_t column, uint8_t line);
void Lcd_Clear (void);
void Test_Lcd(void);

#endif // LIB_LCD_H
