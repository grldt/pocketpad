#include "pocketpad8.h"

#include <stdio.h>
#include <string.h>

bool left_on  = false;
bool right_on = false;

bool cw  = false;
bool ccw = false;

bool isInScreenMode(int mode);
bool isInAppMode(int mode);
bool is_app_open(void);

void open_menu(bool menuu);
bool is_menu_open(void);

void matrix_scan_dice(void);
void matrix_scan_menu(void);
void matrix_scan_timer(void);
void matrix_scan_wish(void);

void matrix_scan_user(void) {
    matrix_scan_menu();
    matrix_scan_dice();
    matrix_scan_timer();
    matrix_scan_wish();
}

bool process_record_menu(uint16_t keycode, keyrecord_t *record);
bool process_record_dice(uint16_t keycode, keyrecord_t *record);
bool process_record_timer(uint16_t keycode, keyrecord_t *record);
bool process_record_wish(uint16_t keycode, keyrecord_t *record);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_menu_open()) {
        return process_record_menu(keycode, record);
    }

    uint8_t kcol = record->event.key.col;
    uint8_t krow = record->event.key.row;

    if (isInAppMode(0)) { // Timer: app 0
        if (!(krow == 0 && kcol == 0)) {
            return process_record_timer(keycode, record);
        }
    }

    if (isInAppMode(1)) { // Dice Roll: app 1
        if (!(krow == 0 && kcol == 0)) {
            return process_record_dice(keycode, record);
        }
    }

    if (isInAppMode(2)) { // Wishing simulator: app 2
        if (!(krow == 0 && kcol == 0)) {
            return process_record_wish(keycode, record);
        }
    }

    if (krow == 0 && kcol == 3) { // top right switch
        if (keycode == KC_SPC && record->event.pressed) {
            open_menu(true);
            oled_clear();
        }
        return false;
    } else {
        return true;
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    oled_clear();
    oled_render();

    return state;
}

// Draw to OLED
bool oled_task_menu(void);

bool oled_task_user(void) {
    return oled_task_menu();

    return false;
}

void keyboard_post_init_menu(void);

void keyboard_post_init_user(void) {

    keyboard_post_init_menu();
}

void eeconfig_init_menu(void);

void eeconfig_init_user(void) { // EEPROM is getting reset!

    eeconfig_init_menu();
}

// void shutdown_menu(void);

// void shutdown_user (void) {
//     shutdown_menu();
// }
