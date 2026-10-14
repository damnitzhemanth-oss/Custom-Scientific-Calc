#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void lcd_init(void);
void lcd_clear(void);
void lcd_send_line(uint8_t line, const uint8_t *data);
void lcd_send_command(uint8_t cmd);
void lcd_extcommin_toggle(void);

#endif