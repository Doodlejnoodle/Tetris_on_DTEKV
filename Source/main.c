/* labmain.c 
// This file was originally written 2024 by Artur Podobas and Pedro Antunes
// This file was modified mostly by Laiba
*/

#include <stdint.h>
#include "game.h"

//Lab specific
extern void print(const char *s);
extern void print_dec(unsigned int n);
extern int nextprime(int);
extern void enable_interrupt(void);
extern void delay(int);

/* Device-registers (memory mapped) */
#define LEDS           ((volatile int*)0x04000000)
#define SWITCHES       ((volatile int*)0x04000010)
#define TMR1_STAT      ((volatile unsigned short*)0x04000020)
#define TMR1_CTRL      ((volatile unsigned short*)0x04000024)
#define TMR1_PERIODL   ((volatile unsigned short*)0x04000028)
#define TMR1_PERIODH   ((volatile unsigned short*)0x0400002C)
#define DISPLAY_BASE   0x04000050
#define DISPLAY_OFFSET 0x10
#define BTN2           ((volatile int*)0x040000d0)

//Project specific
extern void draw_square();
extern void draw_cell();
extern void to_screen();


/* --- Seven-seg encoding (0–9). 0 = segment on for this HW mapping) --- */
static const int segm_encoding[10] = {
    0b11000000, 0b11111001, 0b10100100, 0b10110000,
    0b10011001, 0b10010010, 0b10000010, 0b11111000,
    0b10000000, 0b10010000
};

/* --- Globals shared between IRQ and main --- */
volatile int seconds = 0;
volatile int minutes = 0;
volatile int hours   = 0;

/* startup LED sequence */
volatile int led_value = 0;       // value shown on first 4 LEDs during start
volatile int led_sequence_done = 0;

/* timer tick counting (0.1s ticks -> count 10 -> ~1s) */
volatile int timeoutcount = 0;

/* Prime state */
int prime = 1234567;

/* ===== I/O helper functions (assignment parts c–g) ===== */

void set_leds(int led_mask) {
    /* Only 10 LSB are used on board */
    *LEDS = led_mask & 0x3FF;
}

void set_displays(int display_number, int value) {
    if (display_number < 0 || display_number > 5) return;
    volatile int *addr = (volatile int *)(unsigned long)(DISPLAY_BASE + display_number * DISPLAY_OFFSET);
    *addr = value;
}

int get_sw(void) {
    return *SWITCHES & 0x3FF; /* 10 LSB from switches */
}

int get_btn(void) {
    return *BTN2 & 0x1; /* least significant bit = second button */
}

/* Convenience: put a decimal digit (0-9) onto a 7-seg display_number */
void display_digit_seg(int display_number, int digit) {
    if (digit < 0 || digit > 9) digit = 0;
    set_displays(display_number, segm_encoding[digit]);
}

/* Show current time on all 6 displays: leftmost = hours (tens, ones),
   middle = minutes, rightmost = seconds (two digits) */
void update_all_displays_from_time(void) {
    display_digit_seg(5, (hours / 10) % 10);
    display_digit_seg(4, hours % 10);
    display_digit_seg(3, (minutes / 10) % 10);
    display_digit_seg(2, minutes % 10);
    display_digit_seg(1, (seconds / 10) % 10);
    display_digit_seg(0, seconds % 10);
}

/* ===== Timer init (labinit) ===== */
void labinit(void) {
    /* Setup period for ~0.1s ticks assuming 30 MHz clock:
       period = (30_000_000 / 10) - 1 */
    unsigned int period = (30000000 / 10) - 1;
    *TMR1_PERIODL = (unsigned short)(period & 0xFFFF);
    *TMR1_PERIODH = (unsigned short)(period >> 16);

    /* start timer + enable interrupt (bits as before) */
    *TMR1_CTRL = 0x7;

    /* enable global interrupts in CPU */
    enable_interrupt();
}

/* ===== Interrupt handler (timer-driven) =====
   Expected to be called from interrupt vector with 'cause' argument. */
void handle_interrupt(unsigned cause) {
    /* Clear timer flag if set */
    if (*TMR1_STAT & 0x1) {
        *TMR1_STAT = 0;
    }

    timeoutcount++;
    if (timeoutcount >= 10) { /* ~1 second elapsed */
        timeoutcount = 0;

        /* If startup LED sequence not finished, increment the 4-LED binary counter */
        if (!led_sequence_done) {
            /* show 4 LSBs of led_value on the first 4 LEDs */
            set_leds(led_value & 0xF);

            if ((led_value & 0xF) == 0xF) {
                /* all 4 LEDs are now ON -> startup sequence finished */
                led_sequence_done = 1;
                /* Option: leave first 4 LEDs on briefly, then clear so LEDs used later to show seconds */
                /* We'll clear them so they can be reused to show seconds afterwards */
                set_leds(0);
            } else {
                led_value = (led_value + 1) & 0xF; /* increment 0..15 */
            }
            /* During startup sequence we don't advance the clock counters. */
            return;
        }

        /* Normal clock update: seconds -> minutes -> hours */
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 100) { /* keep hours to two digits */
                    hours = 0;
                }
            }
        }

        /* Update 7-seg displays to show HH:MM:SS */
        update_all_displays_from_time();

        /* Use LEDs to display seconds in binary (or any other meaningful info).
           Show only 10 LSBs (board has 10 LEDs). */
        //set_leds(seconds & 0x3FF);
    }
}


/* ===== Main: init, prime loop + button-handling to set time ===== */
int main(void) {
    /* Initialize timer + interrupts and clear outputs */
    labinit();
    game_init();
    
/*    
    set_leds(0);
    update_all_displays_from_time();
*/
    /* Main loop: print primes and handle button/switch input */
    while (1) {
// read buttons
//        if (LEFT_BTN)  game_move_left();
//        if (RIGHT_BTN) game_move_right();
//        if (ROTATE_BTN) game_rotate();
//        if (DOWN_BTN)  game_soft_drop();

        game_step_gravity();
        to_screen();
        delay(1600);
    }

    return 0;
}