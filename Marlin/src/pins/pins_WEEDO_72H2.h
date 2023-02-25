/**
* WEEDO 72H Motherboad pin configuration
* Copyright (C) 2022 WEEDO3D Perron
*
*
*/


#ifndef BOARD_NAME
#define BOARD_NAME "WEEDO 72H2"
#endif

// X Endstop
#define X_MIN_PIN           -1
#define X_MAX_PIN           PA7

// Y Endstop
#define Y_MIN_PIN			PC4
#define Y_MAX_PIN			-1

// Z Endstop
#define Z_MIN_PIN			PC13
#define Z_MAX_PIN			PC5

// X Motor
#define X_STEP_PIN         PB8
#define X_DIR_PIN          PB7
#define X_ENABLE_PIN       PB9

// Y Motor
#define Y_STEP_PIN          PA12
#define Y_DIR_PIN           PA11
#define Y_ENABLE_PIN        PA15

// Z Motor
#define Z_STEP_PIN          PC7
#define Z_DIR_PIN           PC6
#define Z_ENABLE_PIN        PA8

// E0 Motor
#define E0_STEP_PIN          PB4
#define E0_DIR_PIN           PB3
#define E0_ENABLE_PIN        PB6

// Extruder0
#define HEATER_0_PIN            PC14
#define TEMP_0_PIN              PC0
#define AUTO_FAN_PIN            PC15


// Heater Bed
#define HEATER_BED_PIN		PA5
#define TEMP_BED_PIN        PC1

// BUZZ
#define BEEPER_PIN          PA0
#define TONE_TIMER          2
#define TONE_CHANNEL        1

//#define POWEROFFPIN 28

#define LED_PIN             PA6

#define SD_DETECT_PIN       PB5

#define STM_SD_CS           PB10
#define STM_SD_BUSY         PB1


//
// Onboard SD support
//
#define SDIO_D0_PIN        PC8
#define SDIO_D1_PIN        PC9
#define SDIO_D2_PIN        PC10
#define SDIO_D3_PIN        PC11
#define SDIO_CK_PIN        PC12
#define SDIO_CMD_PIN       PD2

#define SDIO_SUPPORT 

// #define SPI_SPEED           SPI_QUARTER_SPEED

#define SD_DETECT_STATE     LOW

#define DOGLCD_CS           PB12
#define DOGLCD_A0           PC2

#define LCD_RESET_PIN       PB11
#define DOGLCD_SCK          PB13
#define DOGLCD_MOSI         PB15
#define LCD_CONTRAST 200

// Encoder
#define BTN_EN1             PC3
#define BTN_EN2             PA1
#define BTN_ENC             PA4

// Extend IO
#define EXTEND_IO0          PB0 


#define FLASH_EEPROM_EMULATION
#define EEPROM_PAGE_SIZE     uint16(0x800) // 2KB
#define EEPROM_START_ADDRESS uint32(0x8000000 + 512 * 1024 - 2 * EEPROM_PAGE_SIZE)
#undef E2END
#define E2END                (EEPROM_PAGE_SIZE - 1) // 2KB

#define SPI_SPEED           SPI_EIGHTH_SPEED

// #define RX_BUFFER_SIZE 512

// fake define for notice, you should modify the define in usart.h
// #define USART_RX_BUF_SIZE 512
// #define USART_TX_BUF_SIZE 512