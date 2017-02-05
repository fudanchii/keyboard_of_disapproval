#include "setup.h"

void send_report(void);
void receive_report(void);

KBOD_Report_t cReport = {
    .modifier = 0,
    .reserved = 0,
    .keys =     { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

int keyPressed = 0;
bool UsingReportProtocol = true;

static uint16_t idleMSRemaining = 0;
static uint16_t idleCount = 500;

#define init_prescan() {                \
    keyPressed = 0;                     \
    cReport.modifier = 0;               \
    memset(cReport.keys, 0, KBOD_NKRO); \
} while(0);


void kbod_setup()
{
    // Drive high | enable pull up resistor
    bitset4(PORTB, PB4, PB5, PB6, PB7);
    bitset1(PORTC, PC6);
    bitset4(PORTD, PD0, PD1, PD6, PD7);
    bitset1(PORTE, PE6);
    bitset6(PORTF, PF0, PF1, PF4, PF5, PF6, PF7);

    // set as input
    bitclr2(DDRD, DDD0, DDD1);
    bitclr6(DDRF, DDF0, DDF1, DDF4, DDF5, DDF6, DDF7);

    // set as output
    bitset4(DDRB, DDB4, DDB5, DDB6, DDB7);
    bitset2(DDRC, DDC6);
    bitset2(DDRD, DDD6, DDD7);
    bitset1(DDRE, DDE6);

    USB_Init();
}

void kbod_cycle()
{
    idle_USBTask();
    USB_USBTask();
}

void kbod_assign_modifier(char mod)
{
    cReport.modifier |= mod;
}

int kbod_assign_key(char key)
{
    if (keyPressed == KBOD_NKRO) return 1;
    cReport.keys[keyPressed++] = key;
    return 0;
}

void EVENT_USB_Device_Connect()
{
    UsingReportProtocol = true;
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

                init_prescan();
                kbod_matrix_scan();

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
    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    init_prescan();
    kbod_matrix_scan();
    send_report();
    receive_report();
}

void send_report()
{
    static KBOD_Report_t prevReport;
    bool                 sendReport = false;

    /* Check if the idle period is set and has elapsed */
    if (IdleCount && (!(IdleMSRemaining)))
    {
        /* Reset the idle time remaining counter */
        IdleMSRemaining = IdleCount;

        /* Idle period is set and has elapsed, must send a report to the host */
        SendReport = true;
    }
    else
    {
        /* Check to see if the report data has changed - if so a report MUST be sent */
        sendReport = (memcmp(&prevReport, &cReport, sizeof(KBOD_Report_t)) != 0);
    }

    /* Select the Keyboard Report Endpoint */
    Endpoint_SelectEndpoint(KEYBOARD_IN_EPADDR);

    /* Check if Keyboard Endpoint Ready for Read/Write and if we should send a new report */
    if (Endpoint_IsReadWriteAllowed() && sendReport)
    {
        /* Save the current report data for later comparison to check for changes */
        prevReport = cReport;

        /* Write Keyboard Report Data */
        Endpoint_Write_Stream_LE(&cReport, sizeof(cReport), NULL);

        /* Finalize the stream transfer to send the last packet */
        Endpoint_ClearIN();
    }
}

void receive_report()
{
    // TODO: handle OUT report
}