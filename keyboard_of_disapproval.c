#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include <usbdrv.h>
#include "main.h"
#include "keymap.h"
#include "bits.h"

int   keyRegistered = 0;
uchar reportBuffer[7] = "";

uchar usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (usbRequest_t *)((void*)data);
  usbMsgPtr = reportBuffer;
  return 0;
}

void pinSetup() {
  // Set port B pin 0, 2 and port A pin 7 as input,
  // and enable pull up resistor
  bitset2(PORTB, PB0, PB2);
  bitclr2(DDRB, DDB0, DDB2);

  // Set port A pin 0, 1 as output, and initially driven high
  bitset3(PORTA, PA0, PA1, PA7);
  bitclr1(DDRA, DDA7);
  bitset2(DDRA, DDA0, DDA1);
}

void regKey(char key) {
  keyRegistered++;
  reportBuffer[keyRegistered] = key;
}


void scanKey() {
  for (int i = 0; i < KBOD_MAT_RL; i++) {
    select_row(KBOD_MAT_R[i]);
    for (int j = 0; j < KBOD_MAT_CL; j++) {
      int pdata = KBOD_C_PORT[j];
      if (bit_is_clear(pdata, KBOD_MAT_C[j])) {
        regKey(KEYMAP[i][j]);
        if (keyRegistered == KBOD_KRO) {
          unselect_row(KBOD_MAT_R[i]);
          return;
        }
      }
    }
    unselect_row(KBOD_MAT_R[i]);
  }
}

void sendReport() {
  while (!usbInterruptIsReady()) {
    usbPoll();
    _delay_ms(5);
  }
  for (int k = keyRegistered + 1; k <= KBOD_KRO; k++) {
    reportBuffer[k] = 0;
  }
  keyRegistered = 0;
  usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
}

void setup() {
  cli();
  usbDeviceDisconnect();
  _delay_ms(250);
  usbDeviceConnect();
  usbInit();
  pinSetup();
  sei();
}

void loop() {
  usbPoll();
  scanKey();
  sendReport();
}
