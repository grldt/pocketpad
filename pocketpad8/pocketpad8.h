#pragma once

#include "quantum.h"

#define xxx KC_NO

#define LAYOUT( \
                   K03, \
    K10, K11, K12, K13, \
    K20, K21, K22, K23 \
) { \
    { xxx, xxx, xxx, K03 }, \
    { K10, K11, K12, K13 }, \
    { K20, K21, K22, K23 }  \
}
