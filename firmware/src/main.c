/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "lib_lcd.h"
#include "lib_mmc_spi.h"

extern bool_t filesystem_ready;
extern uint8_t mmc_spi_status_flag;
extern LcdStream myLCD;

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waRun, 128);
static msg_t Run(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palClearPad(GPIOA, 8);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOA, 8);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  /*
   * Creates the blinker thread.
   */
  palSetPadMode(GPIOA,8,PAL_MODE_OUTPUT_PUSHPULL);
  chThdCreateStatic(waRun, sizeof(waRun), NORMALPRIO, Run, NULL);

  Lcd_Init();
  Lcd_Clear();

  Lcd_Cursor(0,0);
  chprintf((BaseSequentialStream *)&myLCD,"SDC/MMC         ");
  Lcd_Cursor(0,1);
  chprintf((BaseSequentialStream *)&myLCD,"Preparing       ");
  Mmc_Init();
  chThdSleepMilliseconds(1000);

  uint8_t count=0;
  FATFS FatFs;
  FIL fil;

  while (TRUE){
      Mmc_Check();
      if((filesystem_ready==TRUE)&&(mmc_spi_status_flag==MMC_SPI_OK)){
          Lcd_Cursor(0,0);
          chprintf((BaseSequentialStream *)&myLCD,"data ke         ");
          Lcd_Cursor(0,1);
          chprintf((BaseSequentialStream *)&myLCD,"%3i            ",count);

          f_mount(0,&FatFs);
          f_append(&fil, "/data.txt");
          f_printf(&fil, "%s","data ");
          f_printf(&fil, "%d",count);
          f_printf(&fil, "%c",'\r');
          f_close(&fil);

          count++;
      }
      else{
          Lcd_Cursor(0,0);
          chprintf((BaseSequentialStream *)&myLCD,"SDC/MMC         ");
          Lcd_Cursor(0,1);
          chprintf((BaseSequentialStream *)&myLCD,"ERROR           ");
      }
      chThdSleepMilliseconds(1000);
  };
}
