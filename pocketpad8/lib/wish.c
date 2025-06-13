#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

char l0_wish[22] = "                     ";
char l1_wish[22] = "                     ";
char l2_wish[22] = "                     ";
char l3_wish[22] = "                     ";
char scratch_wish[22] = "                     ";

char wish_3_star[22] = "--------***----------";
char wish_4_star[22] = "--------****---------";
char wish_5_star[22] = "--------*****--------";
char wish_result[22] = "--------***----------";
char wish_show_name[22] = "                     ";
char wish_show_star[16] = "*****          ";
char wish_10_pull[11] = "";

char loader_wish[5] = "/-\\|";
int loader_wish_idx = 0;

int wish_animation_idx = 0;

char wish_scr[5] = "MAIN";

int pity_4 = 0;
int pity_5 = 0;

bool wish_g_4 = false;
bool wish_g_5 = false;

int wish_max_pull = 3;

int v_idx_wish = 0;
int v_idx_max_wish = 1;

int h_idx_wish = 0;
int h_idx_max_wish = 1;

uint32_t timer_seed_wish = 0;
uint16_t timer_load_wish = 0;

bool is_wishing = false;

bool is_tenpull = false;

int wish_load_time_ms = 2000;

void matrix_scan_wish(void) {
    if(is_wishing) {
        if(timer_elapsed(timer_load_wish) < wish_load_time_ms) {
            wish_animation_idx = timer_elapsed(timer_load_wish) / (wish_load_time_ms / 20);
            strncpy(l2_wish, wish_result, wish_animation_idx);

            if(timer_elapsed(timer_load_wish) % 250 == 0){
                if(loader_wish_idx < 3) {
                    loader_wish_idx++;
                } else {
                    loader_wish_idx = 0;
                }
            }
        } else {
            is_wishing = false;
            wish_max_pull = 3;
            strcpy(wish_scr, "SHOW");
        }
    }
}

char* wish_get_name(char pull_char) {
    switch(pull_char) {
        case 'W':
        return "Big Bird             ";
        case 'L':
        return "Little Zombie        ";

        case 'A':
        return "Dragonboy            ";
        case 'B':
        return "Madam Sand           ";
        case 'C':
        return "Knight               ";

        case 'D':
        return "Captain's Sword      ";
        case 'E':
        return "Magic Book           ";
        case 'F':
        return "Greatsword           ";

        case 'I':
        return "Rock                 ";
    }
    return "Error                ";
}

char* wish_get_star(char pull_char) {
    switch(pull_char) {
        case 'W':
        case 'L':
        return "*****          ";

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        return "****           ";

        case 'I':
        return "***            ";
    }
    return "?????          ";
}

void start_wish_seed(void) {
    timer_seed_wish = timer_read32();
}

char wish_pity(int roll) {
    int chance_4 = 60; // 51 - 60
    int chance_5 = 6;

    // 5 star
    if(pity_5 > 73) {
        chance_5 = chance_5 + ((pity_5 - 73) * 60);
    }

    chance_4 = chance_4 + (pity_4 * 30);

    // 4 star
    if(pity_4 >= 7) {
        chance_4 = chance_4 + (pity_4 * 60);
    }

    if(pity_5 == 89 || roll <= chance_5) {
        pity_4 = 0;
        pity_5 = 0;
        start_wish_seed();

        // guaranteed / 50/50 5 star
        if(wish_g_5 || roll % 2 == 0) {
            // win
            wish_g_5 = false;
            wish_max_pull = 5;
            return 'W';
        } else {
            // lose
            wish_g_5 = true;
            wish_max_pull = 5;
            return 'L';
        }
    }

    if((pity_4 == 9) || (roll <= (chance_5 + chance_4))) {
        pity_4 = 0;
        pity_5++;
        chance_4 = 60;
        start_wish_seed();

        int rand_char = roll % 3;

        if(wish_g_4 || roll % 2 == 0) {
            // 4* win (character)
            wish_g_4 = false;
            if(wish_max_pull <= 4) {
                wish_max_pull = 4;
            }
            return 'A' + rand_char;
        } else {
            // 4* lose (weapon)
            wish_g_4 = true;
            if(wish_max_pull <= 4) {
                wish_max_pull = 4;
            }
            return 'D' + rand_char;
        }
    }

    if(roll > (chance_4 + chance_5)) {
        pity_4++;
        pity_5++;
        if(wish_max_pull <= 3) {
            wish_max_pull = 3;
        }
        return 'I';
    }
    return 'X';
}

void wishing(bool tenpull) {
    int pulls = tenpull ? 10 : 1;
    int roll = 0;
    is_tenpull = tenpull;
    v_idx_wish = 0;
    h_idx_wish = 0;

    for(int i = 0; i < pulls; i++) {
        roll = ((timer_elapsed32(timer_seed_wish) % 1000) + rand()) % 1000;
        wish_10_pull[i] = wish_pity(roll);
    }

    if(wish_max_pull == 5) {
        strcpy(wish_result, wish_5_star);
    } else if(wish_max_pull == 4) {
        strcpy(wish_result, wish_4_star);
    } else if(wish_max_pull == 3) {
        strcpy(wish_result, wish_3_star);
    }

    // start wish loading
    strcpy(l2_wish, "                     ");
    strcpy(wish_scr, "LOAD");
    timer_load_wish = timer_read();
    is_wishing = true;
}

void main_wish(void) {
    v_idx_max_wish = 1;
    strcpy(l0_wish, "Wishing Simulator    ");

    sprintf(scratch_wish, "Pity 4*: %02d | 5*: %02d ", pity_4, pity_5);
    strcpy(l1_wish, scratch_wish);

    sprintf(scratch_wish, "%c 1 pull             ", (v_idx_wish == 0) ? 16 : 32);
    strcpy(l2_wish, scratch_wish);

    sprintf(scratch_wish, "%c 10 pull            ", (v_idx_wish == 1) ? 16 : 32);
    strcpy(l3_wish, scratch_wish);
}

void load_wish(void) {
    sprintf(scratch_wish, "Wishing %c            ", loader_wish[loader_wish_idx]);
    strcpy(l0_wish, scratch_wish);

    strcpy(l1_wish, "                     ");
    strcpy(l2_wish, "                     ");
    strcpy(l3_wish, "                     ");
}

void show_wish(void) {
    sprintf(scratch_wish, "                %02d/%02d",h_idx_wish + 1, is_tenpull ? 10 : 01);
    strcpy(l0_wish, scratch_wish);

    strcpy(l1_wish, wish_get_name(wish_10_pull[h_idx_wish]));

    sprintf(scratch_wish, "%s%c Next", wish_get_star(wish_10_pull[h_idx_wish]), (v_idx_wish == 0) ? 16 : 32);
    strcpy(l2_wish, scratch_wish);

    if(is_tenpull) {
        sprintf(scratch_wish, "               %c Skip", (v_idx_wish == 1) ? 16 : 32);
        strcpy(l3_wish, scratch_wish);
    }
}

void summ_wish(void) {
    strcpy(l0_wish, "                     ");
    sprintf(scratch_wish, " %c %c %c %c %c %c %c %c %c %c ",
                            wish_10_pull[0],
                            wish_10_pull[1],
                            wish_10_pull[2],
                            wish_10_pull[3],
                            wish_10_pull[4],
                            wish_10_pull[5],
                            wish_10_pull[6],
                            wish_10_pull[7],
                            wish_10_pull[8],
                            wish_10_pull[9]);

    strcpy(l1_wish, scratch_wish);
    strcpy(l2_wish, "                     ");

    sprintf(scratch_wish, "              %c Close", (v_idx_wish == 0) ? 16 : 32);

    strcpy(l3_wish, scratch_wish);
}

void init_render_wish(void) {
    oled_set_cursor(0, 0);
    oled_write(l0_wish, false);
    oled_set_cursor(0, 1);
    oled_write(l1_wish, false);
    oled_set_cursor(0, 2);
    oled_write(l2_wish, false);
    oled_set_cursor(0, 3);
    oled_write(l3_wish, false);
}

void render_wish(void) {
    init_render_wish();

    if(strcmp(wish_scr, "MAIN") == 0) {
        main_wish(); //wish main screen
    } else if(strcmp(wish_scr, "LOAD") == 0) {
        load_wish(); //wish loading screen
    } else if(strcmp(wish_scr, "SHOW") == 0) {
        show_wish(); //wish showing screen
    } else if(strcmp(wish_scr, "SUMM") == 0) {
        summ_wish(); //wish summary screen
    }
}

void v_nav_wish(bool up) {
    if(up) {
        if(v_idx_wish > 0) {
            v_idx_wish--;
        } else {
            v_idx_wish = 0;
        }
    } else {
        if(v_idx_wish < v_idx_max_wish) {
            v_idx_wish++;
        } else {
            v_idx_wish = v_idx_max_wish;
        }
    }
}

void h_nav_wish(bool right) {
    if((strcmp(wish_scr, "MAIN") == 0) && right) {
        switch(v_idx_wish) {
            case 0:
                wishing(false); // 1 pull
                break;
            case 1:
                wishing(true); // 10 pull
                break;
            default:
                break;
        }
        return;
    }

    if((strcmp(wish_scr, "SHOW") == 0) && right) {
        switch(v_idx_wish) {
            case 0:
                if(is_tenpull) {
                    if(h_idx_wish < 9) {
                        h_idx_wish++;
                    } else {
                        strcpy(wish_scr, "SUMM");
                        h_idx_wish = 0;
                    }
                } else {
                    strcpy(wish_scr, "MAIN");
                    h_idx_wish = 0;
                }
                break;
            case 1:
                if(is_tenpull) {
                    strcpy(wish_scr, "SUMM");
                } else {
                    strcpy(wish_scr, "MAIN");
                }
                v_idx_wish = 0;
                h_idx_wish = 0;
                break;
            default:
                break;
        }
        return;
    }

    if((strcmp(wish_scr, "SUMM") == 0) && right) {
        strcpy(wish_scr, "MAIN");
        h_idx_wish = 0;
        return;
    }
}

bool process_record_wish(uint16_t keycode, keyrecord_t *record) {

    uint8_t krow = record->event.key.row;
    uint8_t kcol = record->event.key.col;

    if(record->event.pressed) {

        if(keycode == KC_UP || (krow == 1 && kcol == 1)) { // up
            v_nav_wish(true);
            return false;
        }

        if(keycode == KC_DOWN || (krow == 2 && kcol == 1)) { // down
            v_nav_wish(false);
            return false;
        }

        if(keycode == KC_ENTER || keycode == KC_RIGHT || (krow == 2 && kcol == 2)) { // right/ok
            h_nav_wish(true);
            return false;
        }

        if(keycode == KC_LEFT || keycode == KC_ESC || (krow == 2 && kcol == 0) ) { // left/back
            h_nav_wish(false);
            return false;
        }
    }

    return false;
}
