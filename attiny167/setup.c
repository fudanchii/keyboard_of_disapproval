void pinSetup()
{
    // Set port B pin 0, 2 and port A pin 7 as input,
    // and enable pull up resistor
    bitset2(PORTB, PB0, PB2);
    bitclr2(DDRB, DDB0, DDB2);

    // Set port A pin 0, 1 as output, and initially driven high
    bitset3(PORTA, PA0, PA1, PA7);
    bitclr1(DDRA, DDA7);
    bitset2(DDRA, DDA0, DDA1);
}

uchar usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (usbRequest_t *)((void *)data);
    usbMsgPtr = reportBuffer;
    return 0;
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