#include "NuMicro.h"
#include "EBI_LCD_Module.h"
#include "game_screens.h"
#include "game_session.h"
#include "sys_init.h"

int32_t main(void)
{
    GameSession session = {0};

    /* Initialize the MCU, display interface, and debug UART. */
    SYS_UnlockReg();
    SYS_Init();
    UART_Open(UART0, 115200);

    /* Bring up the LCD over the external bus interface. */
    EBI_Open(EBI_BANK0, EBI_BUSWIDTH_16BIT,
             EBI_TIMING_NORMAL, 0, EBI_CS_ACTIVE_LOW);
    ILI9341_Initial();

    /* Start the 100 ms system heartbeat used by game timing. */
    Timer3_Init();
    __enable_irq();

    /* Enable the LCD backlight after initialization completes. */
    GPIO_SetMode(PB, BIT7, GPIO_MODE_OUTPUT);
    PB7 = 1;

    /* Show the splash screen, then hand control to the game session. */
    DrawStartScreen();
    GameSession_WaitForStart();
    GameSession_ConfigureControls();
    GameSession_Begin(&session);
    GameSession_Run(&session);

    return 0;
}
