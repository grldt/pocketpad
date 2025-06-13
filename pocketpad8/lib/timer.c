#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

char blanks[22] = "                     ";
char asdfasdf[22] = "     00h 00m 00s     ";
char progress_bar[22] = "---------------------";

uint32_t timer_s = 0;

char l0_timer[22] = "                     ";
char l1_timer[22] = "                     ";
char l2_timer[22] = "                     ";
char l3_timer[22] = "                     ";

char scratch[22] = "                     ";

char loader_timer = '-';

bool timer_on = false;
bool timer_paused = false;
bool timer_invert = false;
bool timer_done = false;

int timerH = 0;
int timerM = 0;
int timerS = 0;

int timer_sel = 0;

void render_timer(void);

void nav_v_timer(bool up) {
    if(up) {
        switch(timer_sel) {
            case 0:
                if(timerH < 23) {
                    timerH++;
                }
                break;
            case 1:
                if(timerM < 59) {
                    timerM++;
                }
                break;
            case 2:
                if(timerS < 59) {
                    timerS++;
                }
                break;
            default:
                break;
        }
    } else {
        switch(timer_sel) {
            case 0:
                if(timerH > 0) {
                    timerH--;
                }
                break;
            case 1:
                if(timerM > 0) {
                    timerM--;
                }
                break;
            case 2:
                if(timerS > 0) {
                    timerS--;
                }
                break;
            default:
                break;
        }
    }
}

void nav_h(bool right) {
    if(right) {
        if(timer_sel < 3) {
            timer_sel++;
        } else if(timer_sel == 3) {
            timer_on = true;
            timer_s = timer_read32();
        }
    } else {
        if(timer_sel > 0) {
            timer_sel--;
        }
    }
}

bool process_record_timer(uint16_t keycode, keyrecord_t *record) {

    uint8_t krow = record->event.key.row;
    uint8_t kcol = record->event.key.col;

    if(record->event.pressed) {
        if(timer_on) {
            timer_paused = true;
            timer_on = false;
            timer_done = false;
        } else if(!timer_on) {
            if(timer_done) {
                timer_done = false;
                render_timer();
                return false;
            }

            if(timer_paused) {
                timer_paused = false;
                timer_on = true;
                timer_s = timer_read32();
                return false;
            }

            if(keycode == KC_ENTER || keycode == KC_RIGHT || (krow == 2 && kcol == 2)) { // ok
                nav_h(true); // right
                return false;
            }

            if(keycode == KC_LEFT || keycode == KC_ESC || (krow == 2 && kcol == 0) ) { // back
                nav_h(false); // left
                return false;
            }

            if(keycode == KC_UP || (krow == 1 && kcol == 1)) { // up
                nav_v_timer(true); // up
                return false;
            }

            if(keycode == KC_DOWN || (krow == 2 && kcol == 1)) { // down
                nav_v_timer(false); // down
                return false;
            }
        }
    }

    return false;
}

void matrix_scan_timer(void) {
    if(timer_on) {
        if(timer_elapsed32(timer_s) > 999) {
            if(timerS > 0) {
                timerS--;
            } else if(timerS == 0) {
                if(timerM > 0) {
                    timerS = 59;
                    timerM--;
                } else if(timerM == 0) {
                    if(timerH > 0) {
                        timerS = 59;
                        timerM = 59;
                        timerH--;
                    } else if(timerH == 0){
                        timer_on = false;
                        timer_paused = false;
                        timer_done = true;
                        timer_s = timer_read32();
                    }
                }
            }

            timer_s = timer_read32();
        }

        if(timer_elapsed32(timer_s) < 250) {
            loader_timer = '-';
        } else if(timer_elapsed32(timer_s) < 500) {
            loader_timer = '\\';
        } else if(timer_elapsed32(timer_s) < 750) {
            loader_timer = '|';
        } else if(timer_elapsed32(timer_s) < 1000) {
            loader_timer = '/';
        }
    }

    if(timer_done) {
        if(timer_elapsed32(timer_s) < 250) {
            timer_invert = true;
        } else if(timer_elapsed32(timer_s) < 500) {
            timer_invert = false;
        } else if(timer_elapsed32(timer_s) < 750) {
            timer_invert = false;
        } else if(timer_elapsed32(timer_s) < 1000) {
            timer_invert = false;
        } else {
            timer_s = timer_read32();
        }
    }
}


void timer_pause(void) {
    //         "                     "
    strcpy(l0_timer, "Timer Paused         ");
    strcpy(l1_timer, blanks);
    sprintf(scratch, "%02dh %02dm %02ds ||       ", timerH, timerM, timerS);
    strcpy(l2_timer, scratch);
    strcpy(l3_timer, blanks);
}

void timer_finish(void) {
    if(timer_invert) {
        strcpy(l0_timer, "     Time is up!     ");
        strcpy(l1_timer, "                     ");
        strcpy(l2_timer, "Press any key to stop");
        strcpy(l3_timer, "                     ");
    } else {
        strcpy(l0_timer, "=====Time is up!=====");
        strcpy(l1_timer, "=====================");
        strcpy(l2_timer, "Press any key to stop");
        strcpy(l3_timer, "=====================");
    }
}

void timer_setting(void) {
    strcpy(l0_timer, "Set Timer            ");

    switch(timer_sel) {
        case 0:
            if(timerH < 23) {
                sprintf(scratch, " %c                   ", 30);
                strcpy(l1_timer, scratch);
            } else {
                strcpy(l1_timer, blanks);
            }

            if(timerH > 0) {
                sprintf(scratch, " %c                   ", 31);
                strcpy(l3_timer, scratch);
            } else {
                strcpy(l3_timer, blanks);
            }

            break;

        case 1:
            if(timerM < 59) {
                sprintf(scratch, "     %c               ", 30);
                strcpy(l1_timer, scratch);
            } else {
                strcpy(l1_timer, blanks);
            }

            if(timerM > 0) {
                sprintf(scratch, "     %c               ", 31);
                strcpy(l3_timer, scratch);
            } else {
                strcpy(l3_timer, blanks);
            }

            break;

        case 2:
            if(timerS < 59) {
                sprintf(scratch, "         %c           ", 30);
                strcpy(l1_timer, scratch);
            } else {
                strcpy(l1_timer, blanks);
            }

            if(timerS > 0) {
                sprintf(scratch, "         %c           ", 31);
                strcpy(l3_timer, scratch);
            } else {
                strcpy(l3_timer, blanks);
            }

            break;

        default:
            strcpy(l1_timer, blanks);
            strcpy(l3_timer, blanks);
            break;
    }

    sprintf(scratch, "%02dh %02dm %02ds  %c Start ", timerH, timerM, timerS, (timer_sel == 3) ? 16 : 32);
    strcpy(l2_timer, scratch);
}

void timer_running(void) {
    strcpy(l0_timer, "Timer running        ");

    strcpy(l1_timer, blanks);
    strcpy(l3_timer, blanks);

    sprintf(scratch, "%02dh %02dm %02ds %c        ", timerH, timerM, timerS, loader_timer);
    strcpy(l2_timer, scratch);
}

void init_render_timer(void) {
    oled_set_cursor(0, 0);
    oled_write(l0_timer, false);
    oled_set_cursor(0, 1);
    oled_write(l1_timer, false);
    oled_set_cursor(0, 2);
    oled_write(l2_timer, false);
    oled_set_cursor(0, 3);
    oled_write(l3_timer, false);
}

void render_timer(void) {
    init_render_timer();

    if(!timer_on) {
        if(!timer_done) {
            if(!timer_paused) {
                timer_setting();
            } else {
                timer_pause();
            }
        } else {
            timer_finish();
        }
    } else if(timer_on && !timer_done) {
        timer_running();
    }
}
