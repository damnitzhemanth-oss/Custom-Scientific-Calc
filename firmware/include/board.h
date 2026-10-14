#ifndef BOARD_H
#define BOARD_H

#include "stm32l4xx_hal.h"

#define ROW0_PORT   GPIOA
#define ROW0_PIN    GPIO_PIN_0
#define ROW1_PORT   GPIOA
#define ROW1_PIN    GPIO_PIN_1
#define ROW2_PORT   GPIOA
#define ROW2_PIN    GPIO_PIN_2
#define ROW3_PORT   GPIOA
#define ROW3_PIN    GPIO_PIN_3
#define ROW4_PORT   GPIOA
#define ROW4_PIN    GPIO_PIN_4
#define ROW5_PORT   GPIOA
#define ROW5_PIN    GPIO_PIN_5

#define COL0_PORT   GPIOB
#define COL0_PIN    GPIO_PIN_0
#define COL1_PORT   GPIOB
#define COL1_PIN    GPIO_PIN_1
#define COL2_PORT   GPIOB
#define COL2_PIN    GPIO_PIN_2
#define COL3_PORT   GPIOB
#define COL3_PIN    GPIO_PIN_10
#define COL4_PORT   GPIOB
#define COL4_PIN    GPIO_PIN_11
#define COL5_PORT   GPIOB
#define COL5_PIN    GPIO_PIN_12
#define COL6_PORT   GPIOB
#define COL6_PIN    GPIO_PIN_13
#define COL7_PORT   GPIOB
#define COL7_PIN    GPIO_PIN_14

#define NUM_ROWS    6
#define NUM_COLS    8

#define LCD_SCK_PORT    GPIOC
#define LCD_SCK_PIN     GPIO_PIN_10
#define LCD_MOSI_PORT   GPIOC
#define LCD_MOSI_PIN    GPIO_PIN_12
#define LCD_CS_PORT     GPIOB
#define LCD_CS_PIN      GPIO_PIN_15
#define LCD_DC_PORT     GPIOC
#define LCD_DC_PIN      GPIO_PIN_6
#define LCD_RST_PORT    GPIOC
#define LCD_RST_PIN     GPIO_PIN_7
#define LCD_ON_PORT     GPIOC
#define LCD_ON_PIN      GPIO_PIN_9
#define LCD_EXTCOMIN_PORT  GPIOC
#define LCD_EXTCOMIN_PIN   GPIO_PIN_8

#define LCD_WIDTH       400
#define LCD_HEIGHT      240
#define LCD_LINE_BYTES  ((LCD_WIDTH / 8) + 2)

extern SPI_HandleTypeDef hspi3;

#endif