#include "lib_mmc_spi.h"
#include "lib_lcd.h"

extern LcdStream myLCD;

bool_t filesystem_ready=TRUE;
uint8_t mmc_spi_status_flag=MMC_SPI_OK;

FATFS MMC_FS;

MMCDriver MMCD1;

FRESULT err;
uint32_t clusters;
FATFS *fsp;

static SPIConfig hs_spicfg = {NULL, GPIOB, 12, 0};

static SPIConfig ls_spicfg = {NULL, GPIOB, 12, SPI_CR1_BR_2 | SPI_CR1_BR_1};

static MMCConfig mmccfg = {&SPID2, &ls_spicfg, &hs_spicfg};

void Mmc_Check(){

    mmc_spi_status_flag=MMC_SPI_OK;

    filesystem_ready=FALSE;

    if (mmcConnect(&MMCD1)) {
        filesystem_ready = TRUE;
//          Lcd_Cursor(0,0);
//          chprintf((BaseSequentialStream *)&myLCD,"%i",filesystem_ready);
    }
    else{
        err = f_mount(0, &MMC_FS);
        if (err == FR_OK){
            filesystem_ready = TRUE;
//              Lcd_Cursor(0,0);
//              chprintf((BaseSequentialStream *)&myLCD,"%i",filesystem_ready);
        }
        else{
//              Lcd_Cursor(0,0);
//              chprintf((BaseSequentialStream *)&myLCD,"%i",filesystem_ready);
        }
    }

    if (!filesystem_ready) {
        mmc_spi_status_flag=MMC_SPI_FAIL;
//        Lcd_Cursor(0,1);
//        chprintf((BaseSequentialStream *)&myLCD,"%i",mmc_spi_status_flag);
        return;
    }

    err = f_getfree("/", &clusters, &fsp);
    if (err != FR_OK) {
        mmc_spi_status_flag=MMC_SPI_ERROR;
//        Lcd_Cursor(0,1);
//        chprintf((BaseSequentialStream *)&myLCD,"%i",mmc_spi_status_flag);
        return;
    }

//    Lcd_Cursor(0,1);
//    chprintf((BaseSequentialStream *)&myLCD,"%i",mmc_spi_status_flag);

    chThdSleepMilliseconds(10);
}

FRESULT f_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS | FA_READ);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}

void Mmc_Init(){
    palSetPadMode(GPIOB, 15, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
    palSetPadMode(GPIOB, 14, PAL_MODE_INPUT);
    palSetPadMode(GPIOB, 13, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
    palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOB, 12);
    mmcObjectInit(&MMCD1);
    mmcStart(&MMCD1, &mmccfg);
    chThdSleepMilliseconds(50);
}

void Mmc_Example(){
    Mmc_Check();

#if _USE_STRFUNC
    FATFS FatFs;
    FIL fil;
    if((filesystem_ready==TRUE)&&(mmc_spi_status_flag==MMC_SPI_OK)){
        f_mount(0,&FatFs);
        f_append(&fil, "/text.txt");
        f_printf(&fil, "%5d", 1234);
        f_printf(&fil,"String\n");
        f_close(&fil);
        f_mount(0,NULL);
    }
#else
    char buffer[36];
    FATFS FatFs;
    UINT bw;
    FIL *fil;
    fil =(FIL *)malloc(sizeof (FIL));
    if((filesystem_ready==TRUE)&&(mmc_spi_status_flag==MMC_SPI_OK)){
        chsnprintf(buffer,36,"String\n\r");
        f_mount(0,&FatFs);
        f_open(fil, "/text.txt" , FA_WRITE | FA_OPEN_ALWAYS);
        f_lseek(fil, f_size(fil));
        f_write(fil, buffer, strlen(buffer), &bw);
        f_close(fil);
        f_mount(0,NULL);
    }
#endif
}
