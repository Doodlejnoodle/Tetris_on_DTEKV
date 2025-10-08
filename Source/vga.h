#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

/* value: 0 empty, 1..7 piece colors */
void draw_cell(int grid_x, int grid_y, uint8_t value);

/* hardware hooks - weak defaults provided in render.c */
uint32_t platform_read_buttons(void);
uint32_t platform_get_ms(void);

/* button masks for platform_read_buttons() */
#define BTN_LEFT   (1u<<0)  //code the buttons
#define BTN_RIGHT  (1u<<1)  //same
#define BTN_ROTATE (1u<<2)  //same
#define BTN_DOWN   (1u<<3)  //same

#endif // RENDER_H
