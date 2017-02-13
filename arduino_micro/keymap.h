#ifndef _H_KEYMAP
#define _H_KEYMAP

#include <bits.h>
#include <defs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KBOD_MAT_RL 8
#define KBOD_MAT_CL 8

extern const uint8_t KBOD_MAT_C[KBOD_MAT_CL];
extern const uint8_t KBOD_MAT_R[KBOD_MAT_RL];
extern const uint8_t KBOD_PORTS_R[KBOD_MAT_RL];
extern const uint8_t KBOD_PIN_C[KBOD_MAT_CL];

extern const char KEYMAP_BASE[KBOD_MAT_RL][KBOD_MAT_CL];
extern const char KEYMAP_FN[KBOD_MAT_RL][KBOD_MAT_CL];

#ifdef __cplusplus
}
#endif

#endif
