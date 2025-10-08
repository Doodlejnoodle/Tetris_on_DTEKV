// This file was written by someone else
// For now

#include <stdint.h>
#include "grid.h"
#include "vga.h"

#define SCREENBUFFER 0x08000000
#define SCREEN_W 320
#define SCREEN_H 240
#define OFFSET 100
#define BLOCK_PIX 12

#define RED 0xE0
#define CYAN 0x1F
#define BLUE 0x3
#define ORANGE 0xF0
#define YELLOW 0xFC
#define GREEN 0x1C
#define PURPLE 0xE3
#define WHITE 0xFF

int colorList[] = {
    0,
    RED,
    CYAN,
    BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    PURPLE,
    WHITE
};

volatile uint8_t *screenbuffer = (uint8_t *)SCREENBUFFER;

// draws one square
void draw_square(int x, int y, int colorIndex)
{
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            screenbuffer[(y + i) * SCREEN_W + (OFFSET + x + j)] = colorList[colorIndex];
        }
    }
}

// updates the screen according to the grid
void to_screen()
{
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            draw_square(x * 12, y * 12, grid[y][x]);
        }
    }
}

// resets all squares to black
void reset_screen()
{
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++)
    {
        screenbuffer[i] = 0;
    }
}

// draws an outline around the grid
void draw_outline()
{
    for (int y = 0; y < SCREEN_H; y++)
    {
        screenbuffer[y * SCREEN_W + 99] = WHITE;
        screenbuffer[y * SCREEN_W + 98] = WHITE;
        screenbuffer[y * SCREEN_W + 220] = WHITE;
        screenbuffer[y * SCREEN_W + 221] = WHITE;
    }
}

static inline void fb_put_pixel(int x, int y, uint16_t color) {
    if ((unsigned)x >= SCREEN_W || (unsigned)y >= SCREEN_H) return;
    screenbuffer[y * SCREEN_W + x] = color;
}

/* fill rectangle with color (x,y in pixels, w,h in pixels) */
static void fb_fill_rect(int x0, int y0, int w, int h, uint16_t color) {
    if (w <= 0 || h <= 0) return;
    if (x0 < 0) { w += x0; x0 = 0; }
    if (y0 < 0) { h += y0; y0 = 0; }
    if (x0 >= SCREEN_W || y0 >= SCREEN_H) return;
    if (x0 + w > SCREEN_W) w = SCREEN_W - x0;
    if (y0 + h > SCREEN_H) h = SCREEN_H - y0;

    for (int y = 0; y < h; ++y) {
        uint8_t *row = (uint8_t*)&screenbuffer[(y0 + y) * SCREEN_W + x0];
        draw_square(*row, color, w);
    }
}

/* Public: draw a grid cell (grid coords) */
void draw_cell(int gx, int gy, uint8_t value) {
    int px = gx * BLOCK_PIX;
    int py = gy * BLOCK_PIX;
    uint16_t c = (value < 8) ? colorList[value] : 0xFFFF;
    /* draw a small border by drawing slightly smaller rect */
    fb_fill_rect(px, py, BLOCK_PIX - 1, BLOCK_PIX - 1, c);
}

/* Default platform hooks (weak-like)*/

// resets screen and draws the outline
void setup_screen()
{
    reset_screen();
    draw_outline();
}