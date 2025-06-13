#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

void render_stats(void) {
    char blank[22] = "                     ";
    char barss[22] = "=====================";
    char capl[5] = "    ";
    char scrl[5] = "    ";
    char numl[5] = "    ";
    char wpmln[22] = "";
    char pad[22] = "";

    char line1[22] = "";
    char line2[22] = "";
    char line3[22] = "";
    char line4[22] = "";

    led_t led_state = host_keyboard_led_state();

    double barlen = 0;

    if(led_state.caps_lock) {
        strcpy(capl, "CAPS");
    } else {
        strcpy(capl, "    ");
    }

    if(led_state.num_lock) {
        strcpy(numl, "NMLK");
    } else {
        strcpy(numl, "    ");
    }

    if(led_state.scroll_lock) {
        strcpy(scrl, "SCLK");
    } else {
        strcpy(scrl, "    ");
    }

    sprintf(line1, "LAYER%d %s %s %s", get_highest_layer(layer_state), capl, scrl, numl);
    strcpy (line2, "---------------------");
    sprintf(line3, "WPM: %03d             ", get_current_wpm());

    if(get_current_wpm()/150 >= 1) {
        barlen = 1;
    } else {
        barlen = 22 * get_current_wpm() / 150;
    }
    strncpy(wpmln, barss, barlen);
    strncpy(pad, blank, 21 - barlen);

    if(barlen == 1) {
        sprintf(line4, "%s", wpmln);
    } else {
        sprintf(line4, "%s%s", wpmln, pad);
    }

    oled_set_cursor(0, 0);
    oled_write(line1, false);
    oled_set_cursor(0, 1);
    oled_write(line2, false);
    oled_set_cursor(0, 2);
    oled_write(line3, false);
    oled_set_cursor(0, 3);
    oled_write(line4, false);
}
