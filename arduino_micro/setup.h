#ifndef _H_SETUP
#define _H_SETUP

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "descriptors.h"
#include "keymap.h"

#include <LUFA/Drivers/USB/USB.h>


#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t KBOD_MAT_C[KBOD_MAT_CL];
extern const uint8_t KBOD_MAT_R[KBOD_MAT_RL];
extern const uint8_t KBOD_PORTS_R[KBOD_MAT_RL];
extern const uint8_t KBOD_PIN_C[KBOD_MAT_CL];

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

void kbod_matrix_scan(void);
void kbod_setup(void);
void kbod_cycle(void);

void kbod_assign_modifier(char mod);
int kbod_assign_key(char key);

void idle_USBTask(void);


#ifdef __cplusplus
}
#endif

#endif