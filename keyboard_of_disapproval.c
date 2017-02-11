#include <stdint.h>

#include "defs.h"
#include "bits.h"

#ifdef BOARD_ARDUINO_MICRO
  #include "arduino_micro/setup.h"
#elif BOARD_DIGISPARK_PRO
  #include "digispark_pro/setup.h"
#endif

void kbod_matrix_scan()
{
    int kro_full = 0;
    for (int row = 0; row < KBOD_MAT_RL; row++)
    {
        select_row(row);
        _delay_ms(3);
        for (int col = 0; col < KBOD_MAT_CL; col++)
        {
            if (not_active(col)) continue;

            // TODO: handle hwmod FN key
            if (is_hwmod_fn(row, col)) continue;

            if (is_modifier(row, col)) {
                kbod_assign_modifier(MOD_UNMASK(KEYMAP_BASE[row][col]));
                continue;
            }

            kro_full = kbod_assign_key(KEYMAP_BASE[row][col]);
            if (kro_full) break;
        }
        unselect_row(row);
        if (kro_full) { return; }
    }
}

int main()
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
    for (;;) { kbod_cycle(); }
}