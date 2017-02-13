#include "setup.h"

void send_report(void);
void receive_report(void);

KBOD_Report_t cReport;

int keyPressed = 0;
bool UsingReportProtocol = true;

static uint16_t idleMSRemaining = 0;
static uint16_t idleCount = 500;

#define do_scan() {                             \
    keyPressed = 0;                             \
    memset(&cReport, 0, sizeof(KBOD_Report_t)); \
    kbod_matrix_scan();                         \
} while(0)

#define blink_led(ms) for (int i = 0; i < 2; i++) { \
    PORTC ^= _BV(PC7);                              \
    _delay_ms(ms);                                  \
    PORTC ^= _BV(PC7);                              \
    _delay_ms(ms);                                  \
}

const uint8_t KBOD_MAT_R[KBOD_MAT_RL] = { PC6, PD7, PE6, PB4, PB5, PB6, PB7, PD6 };
const uint8_t KBOD_MAT_C[KBOD_MAT_CL] = { PD0, PD1, PF0, PF1, PF4, PF5, PF6, PF7 };

                                         // PORTC, PORTD, PORTE, PORTB, PORTB, PORTB, PORTB, PORTD
const uint8_t KBOD_PORTS_R[KBOD_MAT_RL] = { 0x08,  0x0B,  0x0E,  0x05,  0x05,  0x05,  0x05,  0x0B };

                                       // PIND, PIND, PINF, PINF, PINF, PINF, PINF, PINF 
const uint8_t KBOD_PIN_C[KBOD_MAT_CL] = { 0x09, 0x09, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F };


void kbod_setup()
{
    USB_Init();

    // Drive high | enable pull up resistor
    bitset4(PORTB, PB4, PB5, PB6, PB7);
    bitset2(PORTC, PC6, PC7);
    bitset4(PORTD, PD0, PD1, PD6, PD7);
    bitset1(PORTE, PE6);
    bitset6(PORTF, PF0, PF1, PF4, PF5, PF6, PF7);

    // set as input
    bitclr2(DDRD, DDD0, DDD1);
    bitclr6(DDRF, DDF0, DDF1, DDF4, DDF5, DDF6, DDF7);

    // set as output
    bitset4(DDRB, DDB4, DDB5, DDB6, DDB7);
    bitset2(DDRC, DDC6, DDC7);
    bitset2(DDRD, DDD6, DDD7);
    bitset1(DDRE, DDE6);
}

void kbod_cycle()
{
    USB_USBTask();
    idle_USBTask();
}

void kbod_assign_modifier(char mod)
{
    cReport.modifier |= mod ? mod : KEY_GUI_RIGHT;
}

int kbod_assign_key(char key)
{
    if (keyPressed == KBOD_NKRO) return keyPressed;
    cReport.keys[keyPressed++] = key;
    return keyPressed;
}

void EVENT_USB_Device_Connect()
{
    UsingReportProtocol = true;
    blink_led(100);
}

void EVENT_USB_Device_Disconnect()
{

}

void EVENT_USB_Device_ConfigurationChanged()
{
    bool ConfigSuccess = true;

    /* Setup HID Report Endpoints */
    ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);

    /* Turn on Start-of-Frame events for tracking HID report period expiry */
    USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_StartOfFrame(void)
{
    if (idleMSRemaining)
        idleMSRemaining--;
}

void EVENT_USB_Device_ControlRequest(void)
{
    /* Handle HID Class specific requests */
    switch (USB_ControlRequest.bRequest)
    {
        case HID_REQ_GetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                do_scan();

                /* Write the report data to the control endpoint */
                Endpoint_Write_Control_Stream_LE(&cReport, sizeof(cReport));
                Endpoint_ClearIN();
                Endpoint_ClearStatusStage();
            }

            break;
        case HID_REQ_SetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                /* Wait until the LED report has been sent by the host */
                while (!(Endpoint_IsOUTReceived()))
                {
                    if (USB_DeviceState == DEVICE_STATE_Unattached)
                      return;
                }

                /* Read in the LED report from the host */
                uint8_t LEDStatus = Endpoint_Read_8();

                Endpoint_ClearOUT();
                Endpoint_ClearStatusStage();
                // TODO: process report here
            }

            break;
        case HID_REQ_GetProtocol:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                /* Write the current protocol flag to the host */
                Endpoint_Write_8(UsingReportProtocol);

                Endpoint_ClearIN();
                Endpoint_ClearStatusStage();
            }

            break;
        case HID_REQ_SetProtocol:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                Endpoint_ClearStatusStage();

                /* Set or clear the flag depending on what the host indicates that the current Protocol should be */
                UsingReportProtocol = (USB_ControlRequest.wValue != 0);
            }

            break;
        case HID_REQ_SetIdle:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                Endpoint_ClearStatusStage();

                /* Get idle period in MSB, idleCount must be multiplied by 4 to get number of milliseconds */
                idleCount = ((USB_ControlRequest.wValue & 0xFF00) >> 6);
            }

            break;
        case HID_REQ_GetIdle:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();

                /* Write the current idle duration to the host, must be divided by 4 before sent to host */
                Endpoint_Write_8(idleCount >> 2);

                Endpoint_ClearIN();
                Endpoint_ClearStatusStage();
            }

            break;
    }
}


void idle_USBTask()
{
    if (USB_DeviceState != DEVICE_STATE_Configured) {
        blink_led(100);
        return;
    }

    do_scan();
    send_report();
    receive_report();
}

/**
 * send_report responsible in sending key event report on idle,
 * this function will also send report if different keys pressed
 * while idle. */
void send_report()
{
    static KBOD_Report_t prevReport;
    bool                 sendReport = false;

    if (idleCount && (!(idleMSRemaining))) {
        idleMSRemaining = idleCount;
        sendReport = true;
    }
    else {
        sendReport = (memcmp(&prevReport, &cReport, sizeof(KBOD_Report_t)) != 0);
    }

    Endpoint_SelectEndpoint(KEYBOARD_IN_EPADDR);

    if (Endpoint_IsReadWriteAllowed() && sendReport) {
        prevReport = cReport;
        Endpoint_Write_Stream_LE(&cReport, sizeof(cReport), NULL);
        Endpoint_ClearIN();
    }
}

void receive_report()
{
    // TODO: handle OUT report
}