#include "ch.h"
#include "hal.h"
#include "lib_lcd.h"

#define LCD_PIN_RS 10
#define LCD_PIN_RW 11
#define LCD_PIN_EN 12
#define LCD_PORT_CRTL GPIOC

#define LCD_PIN_D4 4
#define LCD_PIN_D5 5
#define LCD_PIN_D6 6
#define LCD_PIN_D7 7
#define LCD_PORT_DATA GPIOB

#define LCD_PORT_MODE PAL_MODE_OUTPUT_PUSHPULL

#define LCD_2X16_CGRAM_ADDR 0x40
#define LCD_2X16_DDRAM_ADDR 0x80
#define LCD_2X16_NEXT_LINE  0x40

LcdStream myLCD;

static msg_t put(void *ip, uint8_t chr) {
    (void)ip;
    Lcd_Write_Data(chr);
    return RDY_OK;
}

static const struct LcdStreamVMT vmt = {NULL, NULL, put, NULL};
void lsObjectInit(LcdStream *msp) {
    msp->vmt = &vmt;
}

void Test_Lcd(void){
    Lcd_Cursor(0,0);
    chprintf((BaseSequentialStream *)&myLCD,"Salam Hore");
    Lcd_Cursor(0,1);
    chprintf((BaseSequentialStream *)&myLCD,"Pusing");
}

void Lcd_Pin_Dir(void){
    palSetPadMode(LCD_PORT_CRTL,LCD_PIN_RS,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_CRTL,LCD_PIN_RW,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_CRTL,LCD_PIN_EN,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D4,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D5,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D6,LCD_PORT_MODE);
    palSetPadMode(LCD_PORT_DATA,LCD_PIN_D7,LCD_PORT_MODE);
    palClearPad(LCD_PORT_CRTL,LCD_PIN_RW);
}

void Lcd_Write_Data(uint8_t chr){

    GPIOB->ODR=(chr & 0x00F0);
    GPIOC->ODR|=1<<LCD_PIN_RS;
    GPIOC->ODR|=1<<LCD_PIN_EN;
    chThdSleepMilliseconds(1);
    GPIOC->ODR&=~(1<<LCD_PIN_RS);
    GPIOC->ODR&=~(1<<LCD_PIN_EN);
    chThdSleepMilliseconds(1);

    GPIOB->ODR=((chr & 0x000F)<<4);
    GPIOC->ODR|=1<<LCD_PIN_RS;
    GPIOC->ODR|=1<<LCD_PIN_EN;
    chThdSleepMilliseconds(1);
    GPIOC->ODR&=~(1<<LCD_PIN_RS);
    GPIOC->ODR&=~(1<<LCD_PIN_EN);
    chThdSleepMilliseconds(1);
}

void Lcd_Write_Command(uint8_t cmd){

    GPIOB->ODR=(cmd & 0x00F0);
    GPIOC->ODR|=1<<LCD_PIN_EN;
    chThdSleepMilliseconds(1);
    GPIOC->ODR&=~(1<<LCD_PIN_EN);
    chThdSleepMilliseconds(1);

    GPIOB->ODR=((cmd & 0x000F)<<4);
    GPIOC->ODR|=1<<LCD_PIN_EN;
    chThdSleepMilliseconds(1);
    GPIOC->ODR&=~(1<<LCD_PIN_EN);
    chThdSleepMilliseconds(1);

}

void Lcd_Cursor(uint8_t column, uint8_t line){
    uint8_t position = LCD_2X16_DDRAM_ADDR;
    if (line > 0)
    {
    position |= LCD_2X16_NEXT_LINE;
    }
    Lcd_Write_Command(position | column);
}

void Lcd_Init(void){

    lsObjectInit(&myLCD);
    Lcd_Pin_Dir();

    chThdSleepMilliseconds(500);
    GPIOB->ODR=0x00;
    GPIOC->ODR=0x00;

    GPIOB->ODR=0x20;
    GPIOC->ODR|=1<<LCD_PIN_EN;
    chThdSleepMilliseconds(1);
    GPIOC->ODR&=~(1<<LCD_PIN_EN);
    chThdSleepMilliseconds(1);

    Lcd_Write_Command(0x28);
    chThdSleepMilliseconds(1);
    Lcd_Write_Command(0x0c);
    chThdSleepMilliseconds(1);

}

void Lcd_Clear (void){
    Lcd_Write_Command(0x01);
    chThdSleepMilliseconds(1);
}
