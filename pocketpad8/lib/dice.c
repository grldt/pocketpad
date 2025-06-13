#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

static uint16_t rand_timer = 0;
static uint16_t roll_timer = 0;
int dice_roll = 0;
char a[15] =  "              ";
char b[15] =  "              ";
bool is_rolling = false;

void render_dice(void) {
    static const char image1 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x40,
        0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    static const char image2 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0c, 0xca, 0xd2, 0x11, 0x21, 0x20, 0x40, 0x40, 0x8e, 0x8e,
        0x8e, 0x40, 0x40, 0x20, 0x21, 0x11, 0x12, 0x0a, 0x0c, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    static const char image3 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x40, 0x40, 0x86, 0x86, 0x30, 0x30, 0x00, 0x00, 0xff,
        0x00, 0x00, 0x03, 0x03, 0x80, 0x98, 0x58, 0x40, 0x20, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    static const char image4 [] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03,
        0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    // Draw image to Oled
    oled_set_cursor(0, 0);
    oled_write_raw_P(image1, sizeof(image1));
    oled_set_cursor(0, 1);
    oled_write_raw_P(image2, sizeof(image2));
    oled_set_cursor(0, 2);
    oled_write_raw_P(image3, sizeof(image3));
    oled_set_cursor(0, 3);
    oled_write_raw_P(image4, sizeof(image4));

    // Set cursor position
    oled_set_cursor(oled_max_chars()/3, 0);
    oled_write_P(PSTR("Press any key "), false);
    oled_set_cursor(oled_max_chars()/3, 1);
    oled_write_P(PSTR("to roll!      "), false);
    oled_set_cursor(oled_max_chars()/3, 2);
    oled_write_P(PSTR("--------------"), false);
    oled_set_cursor(oled_max_chars()/3, 3);

    oled_write(a, false);

    rand_timer = timer_read();
}

bool is_pressed = false;

bool process_record_dice(uint16_t keycode, keyrecord_t *record) {
    if(record->event.pressed) {
        roll_timer = timer_read();

        dice_roll = dice_roll + ((record->event.key.col + 1) * record->event.key.row);

        is_rolling = true;
        is_pressed = true;
    } else {
        is_pressed = false;
    }

    return false;
}

void matrix_scan_dice(void) {
    if(is_rolling) {
        if(timer_elapsed(roll_timer) < 1000 || is_pressed) {
            dice_roll = dice_roll + (rand() % 1000) + (timer_elapsed(rand_timer) % 1000);
            dice_roll = dice_roll + (rand() % (timer_elapsed(rand_timer) % 1000));
            dice_roll = (dice_roll % 6) + 1;
            sprintf(b, "Rolled: %d     ", dice_roll);
            strcpy(a, b);
        } else if(!is_pressed) {
            is_rolling = false;
        }
    }
}
