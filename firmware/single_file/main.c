#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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

#define LCD_SCK_PORT      GPIOC
#define LCD_SCK_PIN       GPIO_PIN_10
#define LCD_MOSI_PORT     GPIOC
#define LCD_MOSI_PIN      GPIO_PIN_12
#define LCD_CS_PORT       GPIOB
#define LCD_CS_PIN        GPIO_PIN_15
#define LCD_DC_PORT       GPIOC
#define LCD_DC_PIN        GPIO_PIN_6
#define LCD_RST_PORT      GPIOC
#define LCD_RST_PIN       GPIO_PIN_7
#define LCD_ON_PORT       GPIOC
#define LCD_ON_PIN        GPIO_PIN_9
#define LCD_EXTCOMIN_PORT GPIOC
#define LCD_EXTCOMIN_PIN  GPIO_PIN_8

#define LCD_WIDTH       400
#define LCD_HEIGHT      240
#define LCD_LINE_BYTES  ((LCD_WIDTH / 8) + 2)

#define DEBOUNCE_TICKS  4

SPI_HandleTypeDef hspi3;

typedef struct {
    uint8_t row;
    uint8_t col;
    char    label[6];
} key_t;

static const key_t keymap[NUM_ROWS][NUM_COLS] = {
    {{0,0,"SHFT"},{0,1,"2ND"},{0,2,"MENU"},{0,3,"<-"},{0,4,"->"},{0,5,"UP"},{0,6,"DN"},{0,7,"ENT"}},
    {{1,0,"sin"},{1,1,"cos"},{1,2,"tan"},{1,3,"7"},{1,4,"8"},{1,5,"9"},{1,6,"/"},{1,7,"DEL"}},
    {{2,0,"asin"},{2,1,"acos"},{2,2,"atan"},{2,3,"4"},{2,4,"5"},{2,5,"6"},{2,6,"*"},{2,7,"AC"}},
    {{3,0,"log"},{3,1,"ln"},{3,2,"sqrt"},{3,3,"1"},{3,4,"2"},{3,5,"3"},{3,6,"-"},{3,7,"ANS"}},
    {{4,0,"10^"},{4,1,"e^"},{4,2,"x^2"},{4,3,"0"},{4,4,"."},{4,5,"="},{4,6,"+"},{4,7,"pi"}},
    {{5,0,"x^y"},{5,1,"("},{5,2,")"},{5,3,"e"},{5,4,"1/x"},{5,5,"nCr"},{5,6,"nPr"},{5,7,"MODE"}},
};

typedef struct { GPIO_TypeDef *port; uint16_t pin; } pin_t;

static const pin_t rows[NUM_ROWS] = {
    {ROW0_PORT, ROW0_PIN}, {ROW1_PORT, ROW1_PIN}, {ROW2_PORT, ROW2_PIN},
    {ROW3_PORT, ROW3_PIN}, {ROW4_PORT, ROW4_PIN}, {ROW5_PORT, ROW5_PIN},
};

static const pin_t cols[NUM_COLS] = {
    {COL0_PORT, COL0_PIN}, {COL1_PORT, COL1_PIN}, {COL2_PORT, COL2_PIN}, {COL3_PORT, COL3_PIN},
    {COL4_PORT, COL4_PIN}, {COL5_PORT, COL5_PIN}, {COL6_PORT, COL6_PIN}, {COL7_PORT, COL7_PIN},
};

static uint8_t kp_stable[NUM_ROWS][NUM_COLS];
static uint8_t kp_counter[NUM_ROWS][NUM_COLS];
static uint8_t event_row = 0xFF;
static uint8_t event_col = 0xFF;

static void keypad_init(void)
{
    GPIO_InitTypeDef g = {0};
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Pull = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    for (uint8_t i = 0; i < NUM_ROWS; i++) {
        g.Pin = rows[i].pin;
        HAL_GPIO_Init(rows[i].port, &g);
        HAL_GPIO_WritePin(rows[i].port, rows[i].pin, GPIO_PIN_SET);
    }
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    for (uint8_t i = 0; i < NUM_COLS; i++) {
        g.Pin = cols[i].pin;
        HAL_GPIO_Init(cols[i].port, &g);
    }
}

static uint8_t keypad_read_row(uint8_t r)
{
    uint8_t mask = 0;
    HAL_GPIO_WritePin(rows[r].port, rows[r].pin, GPIO_PIN_RESET);
    for (volatile uint32_t d = 0; d < 50; d++) { }
    for (uint8_t c = 0; c < NUM_COLS; c++) {
        if (HAL_GPIO_ReadPin(cols[c].port, cols[c].pin) == GPIO_PIN_RESET)
            mask |= (1u << c);
    }
    HAL_GPIO_WritePin(rows[r].port, rows[r].pin, GPIO_PIN_SET);
    return mask;
}

static void keypad_scan(void)
{
    for (uint8_t r = 0; r < NUM_ROWS; r++) {
        uint8_t hit = keypad_read_row(r);
        for (uint8_t c = 0; c < NUM_COLS; c++) {
            uint8_t pressed = (hit >> c) & 1u;
            if (pressed) {
                if (kp_counter[r][c] < DEBOUNCE_TICKS) kp_counter[r][c]++;
            } else {
                kp_counter[r][c] = 0;
            }
            if (kp_counter[r][c] >= DEBOUNCE_TICKS && !kp_stable[r][c]) {
                kp_stable[r][c] = 1;
                event_row = r;
                event_col = c;
            } else if (kp_counter[r][c] == 0) {
                kp_stable[r][c] = 0;
            }
        }
    }
}

static bool keypad_get_event(uint8_t *row, uint8_t *col)
{
    if (event_row == 0xFF) return false;
    *row = event_row;
    *col = event_col;
    event_row = 0xFF;
    event_col = 0xFF;
    return true;
}

static const key_t *keypad_lookup(uint8_t row, uint8_t col)
{
    if (row >= NUM_ROWS || col >= NUM_COLS) return 0;
    return &keymap[row][col];
}

static void lcd_cs(uint8_t v)  { HAL_GPIO_WritePin(LCD_CS_PORT,  LCD_CS_PIN,  v ? GPIO_PIN_SET : GPIO_PIN_RESET); }
static void lcd_rst(uint8_t v) { HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, v ? GPIO_PIN_SET : GPIO_PIN_RESET); }
static void lcd_on(uint8_t v)  { HAL_GPIO_WritePin(LCD_ON_PORT,  LCD_ON_PIN,  v ? GPIO_PIN_SET : GPIO_PIN_RESET); }

static void lcd_spi_tx(uint8_t b)
{
    HAL_SPI_Transmit(&hspi3, &b, 1, 10);
}

static void lcd_send_command(uint8_t cmd)
{
    lcd_cs(1);
    lcd_spi_tx(cmd);
    lcd_spi_tx(0x00);
    lcd_cs(0);
}

static void lcd_send_line(uint8_t line, const uint8_t *data)
{
    lcd_cs(1);
    lcd_spi_tx(line + 1);
    for (uint16_t i = 0; i < (LCD_WIDTH / 8); i++)
        lcd_spi_tx(data[i]);
    lcd_spi_tx(0x00);
    lcd_cs(0);
}

static void lcd_init(void)
{
    GPIO_InitTypeDef g = {0};
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Pull = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;

    g.Pin = LCD_CS_PIN;       HAL_GPIO_Init(LCD_CS_PORT, &g);
    g.Pin = LCD_RST_PIN;      HAL_GPIO_Init(LCD_RST_PORT, &g);
    g.Pin = LCD_ON_PIN;       HAL_GPIO_Init(LCD_ON_PORT, &g);
    g.Pin = LCD_DC_PIN;       HAL_GPIO_Init(LCD_DC_PORT, &g);
    g.Pin = LCD_EXTCOMIN_PIN; HAL_GPIO_Init(LCD_EXTCOMIN_PORT, &g);

    lcd_cs(0);
    lcd_rst(0);
    HAL_Delay(10);
    lcd_rst(1);
    HAL_Delay(10);
    lcd_on(1);
    HAL_Delay(10);

    lcd_send_command(0x00);
    HAL_Delay(5);
    lcd_send_command(0x04);
    HAL_Delay(5);
}

static void lcd_clear(void)
{
    static uint8_t zero[LCD_WIDTH / 8];
    for (uint8_t l = 0; l < (LCD_HEIGHT / 8); l++)
        lcd_send_line(l, zero);
}

static void lcd_extcommin_toggle(void)
{
    HAL_GPIO_TogglePin(LCD_EXTCOMIN_PORT, LCD_EXTCOMIN_PIN);
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState = RCC_HSI_ON;
    osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    osc.PLL.PLLState = RCC_PLL_ON;
    osc.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    osc.PLL.PLLM = 1;
    osc.PLL.PLLN = 10;
    osc.PLL.PLLR = 2;
    osc.PLL.PLLQ = 2;
    HAL_RCC_OscConfig(&osc);

    clk.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV1;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_4);
}

static void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
}

static void SPI3_Init(void)
{
    __HAL_RCC_SPI3_CLK_ENABLE();

    GPIO_InitTypeDef g = {0};
    g.Mode = GPIO_MODE_AF_PP;
    g.Pull = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    g.Alternate = GPIO_AF6_SPI3;

    g.Pin = LCD_SCK_PIN;  HAL_GPIO_Init(LCD_SCK_PORT,  &g);
    g.Pin = LCD_MOSI_PIN; HAL_GPIO_Init(LCD_MOSI_PORT, &g);

    hspi3.Instance = SPI3;
    hspi3.Init.Mode = SPI_MODE_MASTER;
    hspi3.Init.Direction = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi3.Init.NSS = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
    HAL_SPI_Init(&hspi3);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    SPI3_Init();

    keypad_init();
    lcd_init();
    lcd_clear();

    uint32_t last_extcommin = HAL_GetTick();
    uint32_t last_scan = HAL_GetTick();

    while (1) {
        uint32_t now = HAL_GetTick();

        if (now - last_scan >= 5) {
            last_scan = now;
            keypad_scan();

            uint8_t r, c;
            while (keypad_get_event(&r, &c)) {
                const key_t *k = keypad_lookup(r, c);
                (void)k;
            }
        }

        if (now - last_extcommin >= 1000) {
            last_extcommin = now;
            lcd_extcommin_toggle();
        }
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}