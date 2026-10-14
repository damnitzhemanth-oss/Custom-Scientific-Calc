#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t row;
    uint8_t col;
    char    label[6];
} key_t;

void        keypad_init(void);
void        keypad_scan(void);
bool        keypad_get_event(uint8_t *row, uint8_t *col);
const key_t *keypad_lookup(uint8_t row, uint8_t col);

#endif