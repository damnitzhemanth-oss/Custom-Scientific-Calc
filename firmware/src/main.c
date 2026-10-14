#include "board.h"
#include "keypad.h"
#include "lcd.h"

SPI_HandleTypeDef hspi3;

static void SystemClock_Config(void);
static void GPIO_Init(void);
static void SPI3_Init(void);

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

void SysTick_Handler(void)
{
    HAL_IncTick();
}