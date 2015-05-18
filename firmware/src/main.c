#include "ch.h"
#include "hal.h"
#include "lib_lcd.h"
#include "lib_mmc_spi.h"
#include "lib_adc.h"

extern adcsample_t adc_v0,adc_i0,adc_v1,adc_i1;
extern float val_v0,val_i0,val_v1,val_i1;

extern bool_t filesystem_ready;
extern uint8_t mmc_spi_status_flag;
extern LcdStream myLCD;

uint16_t val_day=0,val_mid=0;
uint8_t detik,menit;
bool_t saving_flag=FALSE;

#define buffer_size 16

bool_t hasHeader=FALSE;

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

void Welcome_Message(void){
    Lcd_Cursor(0,0);
    chprintf((BaseSequentialStream *)&myLCD,"Developed by:   ");
    Lcd_Cursor(0,1);
    chprintf((BaseSequentialStream *)&myLCD,"Jordy A W       ");
    chThdSleepMilliseconds(500);

    Lcd_Cursor(0,0);
    chprintf((BaseSequentialStream *)&myLCD,"Developed by:   ");
    Lcd_Cursor(0,1);
    chprintf((BaseSequentialStream *)&myLCD,"Achmadi S.T.    ");
    chThdSleepMilliseconds(500);

    Lcd_Cursor(0,0);
    chprintf((BaseSequentialStream *)&myLCD,"Developed by:   ");
    Lcd_Cursor(0,1);
    chprintf((BaseSequentialStream *)&myLCD,"TF ITS          ");
    chThdSleepMilliseconds(500);
}

int main(void) {

  halInit();
  chSysInit();

  palSetPadMode(GPIOA,8,PAL_MODE_OUTPUT_PUSHPULL);
  chThdCreateStatic(waRun, sizeof(waRun), NORMALPRIO, Run, NULL);

  Adc_Init();

  Lcd_Init();
  Lcd_Clear();

  Welcome_Message();

  Lcd_Cursor(0,0);
  chprintf((BaseSequentialStream *)&myLCD,"SDC/MMC         ");
  Lcd_Cursor(0,1);
  chprintf((BaseSequentialStream *)&myLCD,"Preparing       ");
  Mmc_Init();
  chThdSleepMilliseconds(500);

  detik=0;

  FATFS FatFs;
  UINT bw;
  FIL *fil;
  fil =(FIL *)malloc(sizeof (FIL));
  char buffer[buffer_size];

  while (TRUE){

      Adc_Calc();

#if OVERRIDE_MMC
      filesystem_ready=TRUE;
      mmc_spi_status_flag=MMC_SPI_OK;
#else
      Mmc_Check();
#endif

      if((filesystem_ready==TRUE)&&(mmc_spi_status_flag==MMC_SPI_OK)){

          if(hasHeader==FALSE){

              Lcd_Cursor(0,0);
              chprintf((BaseSequentialStream *)&myLCD,"data logging    ");
              Lcd_Cursor(0,1);
              chprintf((BaseSequentialStream *)&myLCD,"started         ");

              f_mount(0,&FatFs);
              f_open(fil, "/TF_ITS_power_monitor.log", FA_WRITE | FA_OPEN_ALWAYS);
              f_lseek(fil, f_size(fil));

              chsnprintf(buffer,buffer_size,"Volt0;Amp0;");
              f_write(fil, buffer, strlen(buffer), &bw);

              chsnprintf(buffer,buffer_size,"Volt1;Amp1;");
              f_write(fil, buffer, strlen(buffer), &bw);

              chsnprintf(buffer,buffer_size,"Day;Mid");
              f_write(fil, buffer, strlen(buffer), &bw);

              chsnprintf(buffer,buffer_size,"\n");
              f_write(fil, buffer, strlen(buffer), &bw);

              f_close(fil);
              f_mount(0,NULL);

              chThdSleepMilliseconds(500);
              hasHeader=TRUE;
          }

#if VALUE_FLOAT
          Lcd_Cursor(0,0);
          chprintf((BaseSequentialStream *)&myLCD,"%4.1f %4.1f %3d   ",val_v0,val_i0,val_day);
          Lcd_Cursor(0,1);
          chprintf((BaseSequentialStream *)&myLCD,"%4.1f %4.1f %3d   ",val_v1,val_i1,val_mid);
#else
          Lcd_Cursor(0,0);
          chprintf((BaseSequentialStream *)&myLCD,"%4d %4d %3d   ",adc_v0,adc_i0,val_day);
          Lcd_Cursor(0,1);
          chprintf((BaseSequentialStream *)&myLCD,"%4d %4d %3d   ",adc_v1,adc_i1,val_mid);
#endif
          if(saving_flag){

              f_mount(0,&FatFs);
              f_open(fil, "/TF_ITS_power_monitor.log", FA_WRITE | FA_OPEN_ALWAYS);
              f_lseek(fil, f_size(fil));

#if VALUE_FLOAT
              chsnprintf(buffer,buffer_size,"%4.1f;%4.1f;",val_v0,val_i0);
              f_write(fil, buffer, strlen(buffer), &bw);

              chsnprintf(buffer,buffer_size,"%4.1f;%4.1f;",val_v1,val_i1);
              f_write(fil, buffer, strlen(buffer), &bw);

#else
              chsnprintf(buffer,buffer_size,"%4d;%4d;",adc_v0,adc_i0);
              f_write(fil, buffer, strlen(buffer), &bw);

              chsnprintf(buffer,buffer_size,"%4d;%4d;",adc_v1,adc_i1);
              f_write(fil, buffer, strlen(buffer), &bw);
#endif
              chsnprintf(buffer,buffer_size,"%3d;%3d;",val_day,val_mid);
              f_write(fil, buffer, strlen(buffer), &bw);

              chsnprintf(buffer,buffer_size,"\n");
              f_write(fil, buffer, strlen(buffer), &bw);

              f_close(fil);
              f_mount(0,NULL);


              saving_flag=FALSE;
          }

      }
      else{
          Lcd_Cursor(0,0);
          chprintf((BaseSequentialStream *)&myLCD,"SDC/MMC         ");
          Lcd_Cursor(0,1);
          chprintf((BaseSequentialStream *)&myLCD,"ERROR           ");
      }

      detik++;

#if ACCEL_TIME
      if(detik>5){
          detik=0;
          menit++;
      }
      if(menit>5){
          detik=0;
          menit=0;
          val_mid++;

          saving_flag=TRUE;
      }
      if(val_mid>5){
          detik=0;
          menit=0;
          val_mid=0;
          val_day++;
      }
      chThdSleepMicroseconds(100);
#else
      if(detik>60){
          detik=0;
          menit++;
      }
      if(menit>30){
          detik=0;
          menit=0;
          val_mid++;

          saving_flag=TRUE;
      }
      if(val_mid>48){
          detik=0;
          menit=0;
          val_mid=0;
          val_day++;
      }
      chThdSleepMilliseconds(1000);
#endif

  };

  return 0;
}
