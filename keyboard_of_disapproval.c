#include <stdint.h>

#include "defs.h"
#include "bits.h"

#include "arduino_micro/setup.h"

void kbod_matrix_scan()
{
    int kdetected = 0;
    int kro = 0;
    uint8_t kbuff[KBOD_NKRO+7];
    const char (*keymap)[KBOD_MAT_RL][KBOD_MAT_RL] = &KEYMAP_BASE;
    memset(kbuff, sizeof(kbuff), 0);
    for (int row = 0; row < KBOD_MAT_RL; row++)
    {
        select_row(row);
        _delay_us(3);
        for (int col = 0; col < KBOD_MAT_CL; col++)
        {
            if (not_active(col)) continue;

            if (is_hwmod_fn(row, col)) {
                keymap = &KEYMAP_FN;
                continue;
            }

            kbuff[kdetected] = (row << 4 ) | (col & 0x0F);
            kdetected++;
        }
        unselect_row(row);
    }

    for (int k = 0; k < kdetected; k++) {
        uint8_t r = kbuff[k] >> 4;
        uint8_t c = kbuff[k] & 0x0F;
        if (is_modifier(r, c)) {
            kbod_assign_modifier(MOD_UNMASK((*keymap)[r][c]));
            continue;
        }
        kro = kbod_assign_key((*keymap)[r][c]);
        if (kro == KBOD_NKRO) break;
    }
}

int main(void)
{
    cli();

    /** Disable watchdog **/
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    // Disable clock division
    clock_prescale_set(clock_div_1);

    // Board specific setup
    kbod_setup();

    sei();

    // Keyboard event cycle
    for (;;) kbod_cycle();
}