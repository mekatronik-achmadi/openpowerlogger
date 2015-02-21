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

//  Lcd_Init();
//  Lcd_Clear();

  palSetPadMode(GPIOC,10,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,11,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOC,12,PAL_MODE_OUTPUT_PUSHPULL);

  palSetPadMode(GPIOB,4,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,5,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,6,PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB,7,PAL_MODE_OUTPUT_PUSHPULL);

  palClearPad(GPIOB,4);

  while (TRUE) {
//      Test_Lcd();
//      Lcd_Cursor(0,0);
//      Lcd_Write_Data('A');
  }
}
