#include "lcd.h"
#include "board.h"

static void cs(uint8_t v)  { HAL_GPIO_WritePin(LCD_CS_PORT,  LCD_CS_PIN,  v ? GPIO_PIN_SET : GPIO_PIN_RESET); }
static void rst(uint8_t v) { HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, v ? GPIO_PIN_SET : GPIO_PIN_RESET); }
static void on(uint8_t v)  { HAL_GPIO_WritePin(LCD_ON_PORT,  LCD_ON_PIN,  v ? GPIO_PIN_SET : GPIO_PIN_RESET); }
static void dc(uint8_t v)  { HAL_GPIO_WritePin(LCD_DC_PORT,  LCD_DC_PIN,  v ? GPIO_PIN_SET : GPIO_PIN_RESET); }

static void spi_tx(uint8_t b)
{
    HAL_SPI_Transmit(&hspi3, &b, 1, 10);
}

void lcd_send_command(uint8_t cmd)
{
    cs(1);
    spi_tx(cmd);
    spi_tx(0x00);
    cs(0);
}

void lcd_send_line(uint8_t line, const uint8_t *data)
{
    cs(1);
    spi_tx(line + 1);
    for (uint16_t i = 0; i < (LCD_WIDTH / 8); i++)
        spi_tx(data[i]);
    spi_tx(0x00);
    cs(0);
}

void lcd_init(void)
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

    cs(0);
    rst(0);
    HAL_Delay(10);
    rst(1);
    HAL_Delay(10);
    on(1);
    HAL_Delay(10);

    lcd_send_command(0x00);
    HAL_Delay(5);
    lcd_send_command(0x04);
    HAL_Delay(5);
}

void lcd_clear(void)
{
    static uint8_t zero[LCD_WIDTH / 8];
    for (uint8_t l = 0; l < (LCD_HEIGHT / 8); l++)
        lcd_send_line(l, zero);
}

void lcd_extcommin_toggle(void)
{
    HAL_GPIO_TogglePin(LCD_EXTCOMIN_PORT, LCD_EXTCOMIN_PIN);
}