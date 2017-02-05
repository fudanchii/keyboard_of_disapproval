#ifndef _H_DEFS
#define _H_DEFS

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_CONTROL_LEFT (1 << 0)
#define KEY_SHIFT_LEFT (1 << 1)
#define KEY_ALT_LEFT (1 << 2)
#define KEY_GUI_LEFT (1 << 3)
#define KEY_CONTROL_RIGHT (1 << 4)
#define KEY_SHIFT_RIGHT (1 << 5)
#define KEY_ALT_RIGHT (1 << 6)
#define KEY_GUI_RIGHT (1 << 7)

#define MOD_MASK(bit)     (bit | 0x80)
#define MOD_UNMASK(bit)   (bit ^ 0x80)
#define MOD_MASKED(bit)   ((bit & 0x80) == 0x80)
#define MOD_CONTROL_LEFT  MOD_MASK(KEY_CONTROL_LEFT)
#define MOD_SHIFT_LEFT    MOD_MASK(KEY_SHIFT_LEFT)
#define MOD_ALT_LEFT      MOD_MASK(KEY_ALT_LEFT)
#define MOD_GUI_LEFT      MOD_MASK(KEY_GUI_LEFT)
#define MOD_CONTROL_RIGHT MOD_MASK(KEY_CONTROL_RIGHT)
#define MOD_SHIFT_RIGHT   MOD_MASK(KEY_SHIFT_RIGHT)
#define MOD_ALT_RIGHT     MOD_MASK(KEY_ALT_RIGHT)
#define MOD_GUI_RIGHT     MOD_MASK(KEY_GUI_RIGHT)

#define KEY_NONE 0

#define KEY_A 4
#define KEY_B 5
#define KEY_C 6
#define KEY_D 7
#define KEY_E 8
#define KEY_F 9
#define KEY_G 10
#define KEY_H 11
#define KEY_I 12
#define KEY_J 13
#define KEY_K 14
#define KEY_L 15
#define KEY_M 16
#define KEY_N 17
#define KEY_O 18
#define KEY_P 19
#define KEY_Q 20
#define KEY_R 21
#define KEY_S 22
#define KEY_T 23
#define KEY_U 24
#define KEY_V 25
#define KEY_W 26
#define KEY_X 27
#define KEY_Y 28
#define KEY_Z 29
#define KEY_1 30
#define KEY_2 31
#define KEY_3 32
#define KEY_4 33
#define KEY_5 34
#define KEY_6 35
#define KEY_7 36
#define KEY_8 37
#define KEY_9 38
#define KEY_0 39

#define KEY_ENTER 40
#define KEY_ESC 41
#define KEY_BACKSP 42
#define KEY_TAB 43
#define KEY_SPACE 44

#define KEY_DASH 45   // - _
#define KEY_EQUAL 46  // = +
#define KEY_LSQBRK 47 // [ {
#define KEY_RSQBRK 48 // ] }
#define KEY_BKSLSH 49 // \ |

#define KEY_SMCOLO 51 // ; :
#define KEY_QUOTE 52  // ' "
#define KEY_GRACCT 53 // ` ~
#define KEY_COMMA 54  // , <
#define KEY_DPUNCT 55 // . >
#define KEY_SLASH 56  // / ?

#define KEY_CAPSLOCK 57

#define KEY_F1 58
#define KEY_F2 59
#define KEY_F3 60
#define KEY_F4 61
#define KEY_F5 62
#define KEY_F6 63
#define KEY_F7 64
#define KEY_F8 65
#define KEY_F9 66
#define KEY_F10 67
#define KEY_F11 68
#define KEY_F12 69

#define KEY_INSERT 73
#define KEY_HOME 74
#define KEY_PGUP 75
#define KEY_DEL 76
#define KEY_END 77
#define KEY_PGDOWN 78

#define KEY_ARIGHT 79
#define KEY_ALEFT 80
#define KEY_ADOWN 81
#define KEY_AUP 82

#define KBOD_HWMOD_FN 0xFF

#define KBOD_NKRO 10

#define is_modifier(r, c) MOD_MASKED(KEYMAP[r][c])
#define is_hwmod_fn(r, c) (KEYMAP[r][c] == KBOD_HWMOD_FN)
#define is_active(c)      bit_is_clear(KBOD_PIN_C[c], KBOD_MAT_C[c])

#define select_row(idx)   bitclr1(KBOD_PORTS_R[idx], KBOD_MAT_R[idx])
#define unselect_row(idx) bitset1(KBOD_PORTS_R[idx], KBOD_MAT_R[idx])

typedef struct {
    uint8_t modifier;
    uint8_t reserved;
    uint8_t keys[KBOD_NKRO];
} ATTR_PACKED KBOD_Report_t;

#ifdef __cplusplus
}
#endif

#endif
