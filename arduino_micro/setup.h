#ifndef _H_SETUP
#define _H_SETUP

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "descriptors.h"

#include <LUFA/Drivers/USB/USB.h>

#include <kbod.h>

#define KBOD_MAT_RL 8
#define KBOD_MAT_CL 8

#ifdef __cplusplus
extern "C" {
#endif

const int PROGMEM KBOD_MAT_C[KBOD_MAT_CL] = { PD0, PD1, PF0, PF1, PF4, PF5, PF6, PF7 };
const int PROGMEM KBOD_MAT_R[KBOD_MAT_RL] = { PC6, PD7, PE6, PB4, PB5, PB6, PB7, PD6 };

const int PROGMEM KBOD_PORTS_R[KBOD_MAT_RL] = { PORTC, PORTD, PORTE, PORTB, PORTB, PORTB, PORTB, PORTD };
const int PROGMEM KBOD_PIN_C[KBOD_MAT_CL] = { PIND, PIND, PINF, PINF, PINF, PINF, PINF, PINF };

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

void kbod_setup(void);
void kbod_cycle(void);

void kbod_assign_modifier(char mod);
int kbod_assign_key(char key);

void idle_USBTask(void);


#ifdef __cplusplus
}
#endif

#endif