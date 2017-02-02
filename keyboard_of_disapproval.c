#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include <usbdrv.h>
#include "defs.h"
#include "keymap.h"
#include "bits.h"

int keyRegistered = 0;
uchar reportBuffer[7] = "";

void scanKey()
{
  for (int i = 0; i < KBOD_MAT_RL; i++)
  {
    select_row(KBOD_MAT_R[i]);
    for (int j = 0; j < KBOD_MAT_CL; j++)
    {
      int pdata = KBOD_C_PORT[j];
      if (bit_is_clear(pdata, KBOD_MAT_C[j]))
      {
        keyRegistered++;
        reportBuffer[keyRegistered] = KEYMAP[i][j];
        if (keyRegistered == KBOD_KRO)
        {
          unselect_row(KBOD_MAT_R[i]);
          return;
        }
      }
    }
    unselect_row(KBOD_MAT_R[i]);
  }
}

void sendReport()
{
  while (!usbInterruptIsReady())
  {
    usbPoll();
    _delay_ms(5);
  }
  for (int k = keyRegistered; k < KBOD_KRO; k++)
  {
    reportBuffer[k] = 0;
  }
  keyRegistered = 0;
  usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
}

void wdt_turnoff()
{
  __watchdog_reset();
  bitclr1(WDRF);
  bitset2(WDTCR, WDCE, WDE);
  WDTCR = 0;
}

int main()
{
  cli();

  wdt_turnoff();

  usbDeviceDisconnect();
  _delay_ms(250);
  usbDeviceConnect();
  usbInit();

  pinSetup();

  sei();

  for (;;)
  {
    usbPoll();
    scanKey();
    sendReport();
  }
}