#pragma once

/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 4

//                        0     1     2     3
#define MATRIX_ROW_PINS { GP26, GP10, GP12 }
#define MATRIX_COL_PINS { GP8,  GP9,  GP13, GP14 }

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 3

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed - default is 5 */
// higher debounce to compensate for encoder
// #define DEBOUNCE 5
#define DEBOUNCE 0

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE

/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define OLED_FONT_H "keyboards/grldtmk/pocketpad8/lib/font.c"
