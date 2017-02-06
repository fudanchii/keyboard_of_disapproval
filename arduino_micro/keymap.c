#include "keymap.h"

const char KEYMAP_BASE[KBOD_MAT_RL][KBOD_MAT_CL] = {
    { KEY_GRACCT,       KEY_1,         KEY_2,        KEY_3,     KEY_4,         KEY_5,         KEY_6,    KEY_7 }, { KEY_8, KEY_9, KEY_0,      KEY_DASH,   KEY_EQUAL,  KEY_BACKSP, KEY_NONE,   KEY_NONE },
    { KEY_TAB,          KEY_Q,         KEY_W,        KEY_E,     KEY_R,         KEY_T,         KEY_Y,    KEY_U }, { KEY_I, KEY_O, KEY_P,      KEY_LSQBRK, KEY_RSQBRK, KEY_BKSLSH, KEY_NONE,   MOD_SHIFT_RIGHT },
    { KEY_CAPSLOCK,     KEY_A,         KEY_S,        KEY_D,     KEY_F,         KEY_G,         KEY_H,    KEY_J }, { KEY_K, KEY_L, KEY_SMCOLO, KEY_QUOTE,  KEY_ENTER,  KEY_COMMA,  KEY_DPUNCT, KEY_SLASH },
    { MOD_SHIFT_LEFT,   KEY_Z,         KEY_X,        KEY_C,     KEY_V,         KEY_B,         KEY_N,    KEY_M },
    { MOD_CONTROL_LEFT, KBOD_HWMOD_FN, MOD_ALT_LEFT, KEY_SPACE, MOD_ALT_RIGHT, MOD_GUI_RIGHT, KEY_MENU, MOD_CONTROL_RIGHT },
};