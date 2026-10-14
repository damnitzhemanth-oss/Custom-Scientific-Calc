#include "keypad.h"
#include "board.h"

#define DEBOUNCE_TICKS  4

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

static uint8_t  stable[NUM_ROWS][NUM_COLS];
static uint8_t  counter[NUM_ROWS][NUM_COLS];
static uint8_t  event_row = 0xFF;
static uint8_t  event_col = 0xFF;

void keypad_init(void)
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

static uint8_t read_row(uint8_t r)
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

void keypad_scan(void)
{
    for (uint8_t r = 0; r < NUM_ROWS; r++) {
        uint8_t hit = read_row(r);
        for (uint8_t c = 0; c < NUM_COLS; c++) {
            uint8_t pressed = (hit >> c) & 1u;
            if (pressed) {
                if (counter[r][c] < DEBOUNCE_TICKS) counter[r][c]++;
            } else {
                counter[r][c] = 0;
            }
            if (counter[r][c] >= DEBOUNCE_TICKS && !stable[r][c]) {
                stable[r][c] = 1;
                event_row = r;
                event_col = c;
            } else if (counter[r][c] == 0) {
                stable[r][c] = 0;
            }
        }
    }
}

bool keypad_get_event(uint8_t *row, uint8_t *col)
{
    if (event_row == 0xFF) return false;
    *row = event_row;
    *col = event_col;
    event_row = 0xFF;
    event_col = 0xFF;
    return true;
}

const key_t *keypad_lookup(uint8_t row, uint8_t col)
{
    if (row >= NUM_ROWS || col >= NUM_COLS) return 0;
    return &keymap[row][col];
}