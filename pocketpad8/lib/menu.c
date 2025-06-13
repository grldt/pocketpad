#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

typedef union {
    uint32_t raw;
    struct {
        uint8_t screen_mode;
        uint8_t app_mode;
    };
} user_config_t;

user_config_t user_config;

// int setting_layer = 9;
int v_sel = 0;
int v_idx = 0;
int h_sel = 0;
int h_idx = 0;

bool app_on = false;

int t_line = 0;
int prev_layer = 0;

char c_page[4] = "set";
char blank[23] = "                     ";
char line0[23] = "                     ";
char line1[23] = "                     ";
char line2[23] = "                     ";
char line3[23] = "                     ";
char up[23] = "                     ";
char dn[23] = "                     ";

uint16_t reset_timer = 0;
bool is_reset = false;

bool menu_on = false;

// current page = lyr (choose layer)
char menu_lyr [4][22] = {"Layer 0              ",
                         "Layer 1              ",
                         "Layer 2              ",
                         "Layer 3              "};

// current page = app (choose app)
char menu_app [3][22] = {"Timer                ",
                         "Dice Roll            ",
                         "Wishing Simulator    "};
                        //  "Secret Code          "};

// current page = set (settings)
char menu_set [5][22] = {"Change layer         ",  // lyr
                         "Apps                 ",  // app
                         "Change screen        ",  // chs
                         "About board          ",  // abt
                         "Bootloader mode      "}; // qkb

// current page = chs (choose screen)
char menu_chs [2][22] = {"Stats                ",
                         "Bongo                "};
                        //  "Music Bars           "};
                        //  "Debug                "};
//add menu here
void render_stats(void);

void render_bongo(void);

// void render_mbars(void);

void render_timer(void);

void render_dice(void);

void render_wish(void);
void start_wish_seed(void);

// void render_secret(void);

// void render_debug(int layer_screen);

// add menu here
void getLayerScreen(int layer_screen) {

    if(app_on) {
        switch(layer_screen) {
            // apps
            case 0:
                render_timer();
                break;
            case 1:
                render_dice();
                break;
            case 2:
                render_wish();
                start_wish_seed();
                break;
            case 3:
                // render_secret();
                break;
            default:
                render_timer();
                break;
        }
        return;
    }

    switch(layer_screen) {
        // screen
        case 0:
            render_stats();
            break;
        case 1:
            render_bongo();
            break;
        // case 2:
        //     render_mbars();
        //     break;
        // case 3:
        //     render_debug(layer_screen);
        //     break;

        default:
            render_stats();
            break;
    }
}

void render_blank(void) {
    oled_clear();
    oled_set_cursor(0, 0);
    oled_write(blank, false);
    oled_set_cursor(0, 1);
    oled_write(blank, false);
    oled_set_cursor(0, 2);
    oled_write(blank, false);
    oled_set_cursor(0, 3);
    oled_write(blank, false);
}

void init_menu_sel(void) {
    v_sel = 0;
    v_idx = 0;
    h_sel = 0;
    h_idx = 0;
}

void open_menu(bool menuu) {
    menu_on = menuu;
}

bool is_menu_open(void) {
    return menu_on;
}

bool is_app_open(void) {
    return app_on;
}

void menu_setting(void) {
    t_line = sizeof(menu_set) / sizeof(menu_set[0]);

    strcpy(c_page, "set");
    //             123456789012345678901
    // strcpy(line0, "Settings:            ");
    strcpy(line0, "Menu:                ");

    if(v_idx == 0) {
        //           ---------------------
        sprintf(up, "                     ");
    } else {
        //            ---------------------
        sprintf(up, "                    %c", 30);
    }
    strcpy(line1, up);

    strcpy(line2, menu_set[v_idx]);

    if(v_idx+1 == t_line) {
        //           ---------------------
        sprintf(dn, "                     ");
    } else {
        //            ---------------------
        sprintf(dn, "                    %c", 31);
    }
    strcpy(line3, dn);
}

void menu_layers(void) {
    t_line = sizeof(menu_lyr) / sizeof(menu_lyr[0]);

    strcpy(c_page, "lyr");
    //             123456789012345678901
    //            ---------------------
    sprintf(up, "%c Change layer to:   ", 17);
    strcpy(line0, up);

    if(v_idx == 0) {
        //           ---------------------
        sprintf(up, "                     ");
    } else {
        //            ---------------------
        sprintf(up, "                    %c", 30);
    }
    strcpy(line1, up);

    strcpy(line2, menu_lyr[v_idx]);

    if(v_idx+1 == t_line) {
        //           ---------------------
        sprintf(dn, "                     ");
    } else {
        //            ---------------------
        sprintf(dn, "                    %c", 31);
    }
    strcpy(line3, dn);
}

void menu_apps(void) {
    t_line = sizeof(menu_app) / sizeof(menu_app[0]);

    strcpy(c_page, "app");
    strcpy(line0, "Choose app:          ");
    //             123456789012345678901
    sprintf(up, "%c Choose app:        ", 17);
    strcpy(line0, up);

    if(v_idx == 0) {
        //           ---------------------
        sprintf(up, "                     ");
    } else {
        //           ---------------------
        sprintf(up, "                    %c", 30);
    }
    strcpy(line1, up);

    strcpy(line2, menu_app[v_idx]);

    if(v_idx+1 == t_line) {
        //           ---------------------
        sprintf(dn, "                     ");
    } else {
        //           ---------------------
        sprintf(dn, "                    %c", 31);
    }
    strcpy(line3, dn);
}

int about_content(int idx) {

    switch(idx) {
        case 0:          //123456789012345678901
            //             ---------------------
            strcpy(line1, "Board Name:          ");
            strcpy(line2, "PocketPad by grldtmk ");
            break;
        case 1:
            strcpy(line1, "FW Version: v0.19.12 ");
            strcpy(line2, "2025.06.06           "); // UPDATE WHEN FINAL
            break;
        case 2:
            strcpy(line1, "Download updates:    ");
            strcpy(line2, "bit.ly/grldtmkfw     ");
            break;
        default:
            break;
    }

    return 3;
}

void about_screen(void) {
    t_line = about_content(v_idx);
    strcpy(c_page, "abt");

    if(v_idx == 0) {
        //            123456789012345678901
        sprintf(up, "%c About              ", 17);
    } else {
        //            123456789012345678901
        //            ---------------------
        sprintf(up, "%c About             %c", 17, 30);
    }
    strcpy(line0, up);

    if(v_idx+1 == t_line) {
        //           123456789012345678901
        //           ---------------------
        sprintf(dn, "                     ");
    } else {
        //           123456789012345678901
        //           ---------------------
        sprintf(dn, "                    %c", 31);
    }
    strcpy(line3, dn);
}

void menu_choose_screen(void) {
    t_line = sizeof(menu_chs) / sizeof(menu_chs[0]);
    strcpy(c_page, "chs");
    //             123456789012345678901
    //            ---------------------
    sprintf(up, "%c Choose screen:     ", 17);
    strcpy(line0, up);

    if(v_idx == 0) {

        //           ---------------------
        sprintf(up, "                     ");
    } else {
        //           123456789012345678901
        //           ---------------------
        sprintf(up, "                    %c", 30);
    }
    strcpy(line1, up);

    strcpy(line2, menu_chs[v_idx]);

    if(v_idx+1 == t_line) {
        //           ---------------------
        sprintf(dn, "                     ");
    } else {
        //           ---------------------
        sprintf(dn, "                    %c", 31);
    }
    strcpy(line3, dn);
}

void bootloader_screen(void) {
    t_line = 2;
    strcpy(c_page, "qkb");

    if(!is_reset) {
        sprintf(up, "%c Bootloader mode?   ", 17);
        strcpy(line0, up);

        sprintf(up, "                     ");
        strcpy(line1, up);


        if(h_idx == 0) {
            sprintf(up, "%c No           Yes   ", 16);
        } else if (h_idx == 1) {
            sprintf(up, "  No         %c Yes   ", 16);
        }
        strcpy(line2, up);

        sprintf(dn, "                     ");
        strcpy(line3, dn);
    }
}

void render_menu(void) {
    // oled_clear();
    oled_set_cursor(0, 0);
    oled_write(line0, false);
    oled_set_cursor(0, 1);
    oled_write(line1, false);
    oled_set_cursor(0, 2);
    oled_write(line2, false);
    oled_set_cursor(0, 3);
    oled_write(line3, false);

    if(strcmp(c_page, "set") == 0) {
        menu_setting();
        return;
    }

    if(strcmp(c_page, "lyr") == 0) {
        menu_layers();
        return;
    }

    if(strcmp(c_page, "app") == 0) {
        menu_apps();
        return;
    }

    if(strcmp(c_page, "chs") == 0) {
        menu_choose_screen();
        return;
    }

    if(strcmp(c_page, "abt") == 0) {
        about_screen();
        return;
    }

    if(strcmp(c_page, "qkb") == 0) {
        bootloader_screen();
        return;
    }
}

void nav_v(bool down) {
    if(down) {
        if(v_idx+1 < t_line) {
            v_idx += 1;
        }
    } else {
        if(v_idx > 0) {
            v_idx -= 1;
        }
    }
}

void nav_back(void) {
    if(strcmp(c_page, "set") == 0) {
        init_menu_sel();
        // layer_move(prev_layer);
        // prev_layer = 0;
        render_blank();
        oled_clear();
        menu_on = false;
        getLayerScreen(user_config.screen_mode);
    }

    if(strcmp(c_page, "lyr") == 0) {
        menu_setting();
        v_idx = 0;
    }

    if(strcmp(c_page, "app") == 0) {
        menu_setting();
        v_idx = 1;
    }

    if(strcmp(c_page, "chs") == 0) {
        menu_setting();
        v_idx = 2;
    }

    if(strcmp(c_page, "abt") == 0) {
        menu_setting();
        v_idx = 3;
    }

    if(strcmp(c_page, "qkb") == 0) {
        if(h_idx == 0) {
            menu_setting();
            v_idx = 4;
        }
        h_idx = 0;
    }

    // init_menu_sel();
}

void nav_next(void) {
    v_sel = v_idx;
    h_sel = h_idx;

    if(strcmp(c_page, "set") == 0) {

        switch (v_sel) {
            case 0:
                // init_menu_sel();
                strcpy(c_page, "lyr");
                // render_menu();
                break;
            case 1:
                // init_menu_sel();
                strcpy(c_page, "app");
                // render_menu();
                break;
            case 2:
                // init_menu_sel();
                strcpy(c_page, "chs");
                // render_menu();
                break;
            case 3:
                // init_menu_sel();
                strcpy(c_page, "abt");
                // render_menu();
                break;
            case 4:
                // init_menu_sel();
                strcpy(c_page, "qkb");
                // render_menu();
                break;
            default:
                break;
        }

        init_menu_sel();
        render_menu();
        return;
    }

    if(strcmp(c_page, "lyr") == 0) {
        app_on = false;
        strcpy(c_page, "set");
        menu_on = false;
        render_blank();
        getLayerScreen(user_config.screen_mode);
        layer_move(v_sel);
        init_menu_sel();
        return;
    }

    if((strcmp(c_page, "app") == 0) || (strcmp(c_page, "chs") == 0)) {

        if(strcmp(c_page, "app") == 0) {
            app_on = true;
            user_config.app_mode = v_sel;
        } else {
            app_on = false;
            user_config.screen_mode = v_sel;
        }
        eeconfig_update_user(user_config.raw);
        strcpy(c_page, "set");
        menu_on = false;
        render_blank();
        // getLayerScreen(user_config.screen_mode);
        init_menu_sel();
        return;
    }

    if(strcmp(c_page, "abt") == 0) {
        switch (v_sel) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                SEND_STRING("bit.ly/grldtmkfw");
                break;
            default:
                break;
        }
    }

    if(strcmp(c_page, "qkb") == 0) {
        // if(v_sel == 0) {
        //     is_reset = true;
        //     reset_timer = timer_read32();
        //     return;
        // } else {
        //     strcpy(c_page, "set");
        //     init_menu_sel();
        //     menu_on = false;
        //     render_blank();
        //     getLayerScreen(user_config.screen_mode);
        //     return;
        // }

        if(h_sel == 0) {
            h_idx = 1;
        } else if(h_sel == 1) {
            //             123456789012345678901
            strcpy(line0, "   Bootloader Mode   ");
            strcpy(line1, "---------------------");
            strcpy(line2, "Waiting for new firm-");
            strcpy(line3, "ware to be flashed...");

            is_reset = true;
            reset_timer = timer_read32();
        }
    }
}

void render_bootloader(void) {
    oled_set_cursor(0, 0);
    oled_write("   Bootloader Mode   ", false);
    oled_set_cursor(0, 1);
    oled_write("---------------------", false);
    oled_set_cursor(0, 2);
    oled_write("Waiting for new firm-", false);
    oled_set_cursor(0, 3);
    oled_write("ware to be flashed...", false);
}

bool process_record_menu(uint16_t keycode, keyrecord_t *record) {

    if(is_reset) {
        return false;
    }

    uint8_t krow = record->event.key.row;
    uint8_t kcol = record->event.key.col;

    // check if the layer is on settings
    if(record->event.pressed && menu_on) {

        if(keycode == KC_LEFT || keycode == KC_ESC || (krow == 2 && kcol == 0) ) { // back
            // back
            nav_back();
            return false;
        }

        if(keycode == KC_ENTER || keycode == KC_RIGHT || (krow == 2 && kcol == 2)) { // ok
            // ok
            nav_next();
            return false;
        }

        if(keycode == KC_UP || (krow == 1 && kcol == 1)) { // up
            nav_v(false); // up
            return false;
        }

        if(keycode == KC_DOWN || (krow == 2 && kcol == 1)) { // down
            nav_v(true); // down
            return false;
        }
        return false;
    }

    return true;
}

void matrix_scan_menu(void) {
    if(is_reset && timer_elapsed32(reset_timer) > 1500) {
        reset_keyboard();
    }
}

bool oled_task_menu(void) {

    if(is_reset) {
        render_bootloader();
    } else if(menu_on == true) { // if settings is active
        render_menu();
    } else {
        if(app_on){
            getLayerScreen(user_config.app_mode);
        } else {
            getLayerScreen(user_config.screen_mode);
        }
    }

    return false;
}

void keyboard_post_init_menu(void) {
    // Call the keymap level matrix init.

    // Read the user config from EEPROM
    user_config.raw = eeconfig_read_user();
    //            ---------------------
    // sprintf(lf, "%c                    ", 17);
    // sprintf(ul, "%c%c                   ", 17, 30);
    // sprintf(up, "%c                    ", 30);
    // sprintf(dn, "%c                    ", 31);
    init_menu_sel();
    strcpy(c_page, "set");
}

void eeconfig_init_menu(void) {  // EEPROM is getting reset!

    user_config.raw = 0;
    user_config.screen_mode = 0;
    user_config.app_mode = 0;

    eeconfig_update_user(user_config.raw);
}

bool isInScreenMode (int mode) {
    if(user_config.screen_mode == mode) {
        return true;
    }

    return false;
}

bool isInAppMode (int mode) {
    if(app_on && user_config.app_mode == mode) {
        return true;
    }

    return false;
}
