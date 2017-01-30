#include <avr/delay.h>
#include <avr/io.h>

#include <usbdrv.h>
#include "main.h"
#include "keymap.h"

int   keyRegistered = 0;
uchar reportBuffer[7] = "";

uchar usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (usbRequest_t *)((void*)data);
  usbMsgPtr = reportBuffer;
  return 0;
}

void delayWithUsbPoll(long milli) {
  unsigned long last = millis();
  while (milli > 0) {
    unsigned long now = millis();
    milli -= now - last;
    last = now;
    usbPoll();
  }
}

void pinSetup() {
  // Set port B pin 0, 1, 2 as input, and enable pull up resistor
  PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

  // Set port A pin 0, 1 as output, and initially driven high
  PORTA |= (1 << PA0) | (1 << PA1);
  DDRA  |= (1 << DDA0) | (1 << DDA1);
}

void regKey(char key) {
  if (keyRegistered == KBOD_KRO) {
    return;
  }
  keyRegistered++;
  reportBuffer[keyRegistered] = key;
}

void selectRow(int pin) {
  PORTA &= ~(1 << pin);
}

void unselectRow(int pin) {
  PORTA |= 1 << pin;
}

void scanKey() {
  for (int i = 0; i < KBOD_MAT_RL; i++) {
    selectRow(KBOD_MAT_R[i]);
    delayWithUsbPoll(3);
    for (int j = 0; j < KBOD_MAT_CL; j++) {
      int pdata = PINB;
      delayWithUsbPoll(3);
      if (~(pdata & (1 << KBOD_MAT_C[j]))) {
        regKey(KEYMAP[i][j]);
      }
    }
    unselectRow(KBOD_MAT_R[i]);
    delayWithUsbPoll(3);
  }
}

void sendReport() {
  while (!usbInterruptIsReady()) {
    usbPoll();
    _delay_ms(5);
  }
  for (int k = keyRegistered + 1; k < KBOD_KRO; k++) {
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
  sei();
  pinSetup();
}

void loop() {
  usbPoll();
  scanKey();
  sendReport();
}
