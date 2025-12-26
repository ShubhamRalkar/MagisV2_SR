// Do not remove the include below
#include "PlutoPilot.h"

#include "API/Oled.h"
#include "API/RC-Interface.h"
#include <string.h>

static uint8_t fb[1024];

static void fb_clear(void)
{
    memset(fb, 0, sizeof(fb));
}

static void fb_commit(void)
{
    Oled_Update(fb);
}

/**
 * Configures Pluto's receiver to use PPM or default ESP mode; activate the line matching your setup.
 * AUX channel configurations is only for PPM recievers if no custom configureMode function is called this are the default setup
 * ARM mode : Rx_AUX2, range 1300 to 2100
 * ANGLE mode : Rx_AUX2, range 900 to 2100
 * BARO mode : Rx_AUX3, range 1300 to 2100
 * MAG mode : Rx_AUX1, range 900 to 1300
 * HEADFREE mode : Rx_AUX1, range 1300 to 1700
 * DEV mode : Rx_AUX4, range 1500 to 2100
 */


void plutoRxConfig ( void ) {
  // Receiver mode: Uncomment one line for ESP or CAM or PPM setup.
   Receiver_Mode ( Rx_ESP );    // Onboard ESP
  //Receiver_Mode ( Rx_CAM );    // WiFi CAMERA
  // Receiver_Mode ( Rx_PPM );    // PPM based
}

static void Test_Lines(void)
{
    fb_clear();

    Oled_DrawLine(fb, 0, 0, 127, 63, true);
    Oled_DrawLine(fb, 0, 63, 127, 0, true);
    Oled_DrawHLine(fb, 0, 32, 128, true);
    Oled_DrawVLine(fb, 64, 0, 64, true);

    fb_commit();
}

static void Run_Oled_Tests(void)
{
    static int test = 0;

    fb_clear();

    switch (test)
    {
        case 0:
            Oled_DrawRect(fb, 1, 11, 50, 30, true);
            break;

        case 1:
            Oled_DrawCircle(fb, 64, 32, 15, true);
            break;

        case 2:
            Oled_DrawEye(fb, 30, 32, 14, 4, 5, 0);
            break;

        case 3:
            Oled_DrawArrow(fb, 64, 32, 24, OLED_ARROW_UP, true);
            break;

        case 4:
            Oled_DrawRCJoysticks(fb, 1500, 1500, 1500, 1500);
            break;

        case 5:
            Oled_FillRect(fb, 15, 11, 50, 30, true);
            break;

        case 6:
            Oled_DrawRoundedRect(fb, 20, 20, 50, 30, 5, true);
            break;

        case 7:
            Oled_FillRoundedRect(fb, 10, 10, 50, 30, 15, true);
            break;

        case 8:
            Oled_FillCircle(fb, 64, 32, 15, true);
            break;

        case 9:
            Oled_DrawEye(fb, 64, 32, 14, 4, 0, true);
            break;
        
        case 10:
            Oled_DrawEyeOutlineWithPupil(fb, 64, 32, 14, 4, 0);
            break;
        
        case 11:
            Oled_DrawXEye(fb, 64, 32, 4);
            break;

        case 12:
            Test_Lines();
            break;
    }

    fb_commit();
    test = (test + 1) % 13;
}



// The setup function is called once at Pluto's hardware startup
void plutoInit ( void ) {
  // Add your hardware initialization code here
  Oled_Init();
}

// The function is called once before plutoLoop when you activate Developer Mode
void onLoopStart ( void )
{
    Oled_Init();
    Oled_SetMode_User();

    memset(fb, 0, sizeof(fb));
    Oled_Update(fb);   // IMPORTANT: sync shadow buffer
}


// The loop function is called in an endless loop
void plutoLoop ( void )
{
    static uint32_t lastUpdate = 0;

    if (millis() - lastUpdate > 5000)   // update once in 5 second
    {
        lastUpdate = millis();
        Run_Oled_Tests();
    }
}


// The function is called once after plutoLoop when you deactivate Developer Mode
void onLoopFinish ( void )
{
    Oled_SetMode_System();   // return OLED to firmware UI
    Oled_Clear();
}