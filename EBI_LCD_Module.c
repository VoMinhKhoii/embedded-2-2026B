/******************************************************************************
 * @file     EBI_LCD_Module.c
 *
 * @brief    Subfunctins to display contents on LCD module thru EBI interface
 * @version  1.0.0
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "NuMicro.h"

#include "EBI_LCD_Module.h"

/* LCD Module "RS" */
#define PH_CTRL_RS 1
#if PH_CTRL_RS
#define SET_RS PH3 = 1;
#define CLR_RS PH3 = 0;
#endif

/* LCD Module "RESET" */
#define SET_RST PB6 = 1;
#define CLR_RST PB6 = 0;

volatile uint8_t Timer3_flag = 0;
volatile uint8_t Timer3_cnt = 0;


volatile uint32_t g_u32AdcIntFlag_TP;
#define TIMEOUT_TICKS 10

static volatile uint8_t  _td_active  = 0;
static volatile uint8_t  _td_ticks   = 0;
static volatile uint8_t  _td_target  = 0;
static volatile uint16_t _td_timeout = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;
uint8_t timer_running = 0;
static uint8_t _game_ms_count = 0;

/**
 * @brief       Initial Timer3
 *
 * @param       None
 *
 * @return      None
 *
 * @details     To initialize Timer3 as periodic mode @100ms and enable interrupt
 */
void Timer3_Init(void)
{
    
    CLK_EnableModuleClock(TMR3_MODULE);
	CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_PCLK1, MODULE_NoMsk);

    /* Set the compare value for a 100 ms period. */
	TIMER3->CMP = 9599999; 

    /* Configure Timer3: Periodic mode, interrupt enabled, prescale = 0 */
    TIMER3->CTL = TIMER_CTL_INTEN_Msk | TIMER_PERIODIC_MODE;

    /* Enable Timer3 interrupt in NVIC */
    NVIC_EnableIRQ(TMR3_IRQn);

    /* Start Timer3 */
    TIMER3->CTL |= TIMER_CTL_CNTEN_Msk;

    /* Initialize flags */
    Timer3_flag = 0;
    Timer3_cnt = 0;

    /* Keep a UART trace during bring-up. */
    printf("Timer3 CMP: %d, CTL: 0x%08X\n", TIMER3->CMP, TIMER3->CTL);
}

/**
 * @brief       TMR3 IRQ handler
 *
 * @param       None
 *
 * @return      None
 *
 * @details     The Timer3 default IRQ, declared in startup_M480.s
 */
void TMR3_IRQHandler(void)
{
	PD->DOUT ^= (1 << 10);
    TIMER_ClearIntFlag(TIMER3);
    Timer3_flag = 1;
    Timer3_cnt++;

    /* Accumulate elapsed time only while gameplay is active. */
    if (timer_running)
    {
        if (++_game_ms_count >= 10)
        {
            _game_ms_count = 0;
            if (++seconds >= 60)
            {
                seconds = 0;
                if (minutes < 99) minutes++;
            }
        }
    }
}


/*---------------------------------------------------------------------------*/
/*  Implementation of the delay API                                          */
/*---------------------------------------------------------------------------*/
void TimerDelay_Start(uint8_t ticks)
{
    _td_active   = 1;
    _td_target   = ticks;
    _td_ticks    = 0;
    _td_timeout  = 0;
    printf("Delay start: %u ticks\n", (unsigned)ticks);
}

uint8_t TimerDelay_Done(void)
{
    if (!_td_active) {
        return 1;
    }

    /* Complete when the requested delay has elapsed. */
    if (_td_ticks >= _td_target) {
        _td_active = 0;
        printf("Delay complete: %u ticks\n", (unsigned)_td_ticks);
        return 1;
    }

    /* Fall back to a timeout so the main loop cannot stall forever. */
    _td_timeout++;
    if (_td_timeout >= TIMEOUT_TICKS) {
        _td_active = 0;
        printf("Delay timeout at %u ticks\n", (unsigned)_td_ticks);
        return 1;
    }

    return 0;
}
/**
 * @brief       Write command to LCD
 *
 * @param       cmd: the command will be written
 *
 * @return      None
 *
 * @details     To write command to LCD thru the EBI interface
 */
#if PH_CTRL_RS
void LCD_WR_REG(uint16_t cmd)
{
    CLR_RS
    EBI0_WRITE_DATA16(0x00000000, cmd);
    SET_RS
}
#else
void LCD_WR_REG(uint16_t cmd)
{
    EBI0_WRITE_DATA16(0x00000000, cmd);
}
#endif

/**
 * @brief       Write data to LCD
 *
 * @param       dat: the data will be written
 *
 * @return      None
 *
 * @details     To write data to LCD thru the EBI interface
 */
void LCD_WR_DATA(uint16_t dat)
{
    EBI0_WRITE_DATA16(0x00030000, dat);

}

/**
 * @brief       Set LCD window
 *
 * @param       x_s: the start position in the x-direction of window
 *              x_e: the end position in the x-direction of window
 *              y_s: the start position in the y-direction of window
 *              y_e: the end position in the y-direction of window
 * @return      None
 *
 * @details     To set the window area of LCD module
 */
void LCD_SetWindow(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e)
{
    /* X range */
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(x_s>>8);
    LCD_WR_DATA(x_s);
    LCD_WR_DATA(x_e>>8);
    LCD_WR_DATA(x_e);

    /* Y range */
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(y_s>>8);
    LCD_WR_DATA(y_s);
    LCD_WR_DATA(y_e>>8);
    LCD_WR_DATA(y_e);

    /* Memory write */
    LCD_WR_REG(0x2C);

}

/**
 * @brief       Initial ILI9341 LCD driver
 *
 * @param       None
 *
 * @return      None
 *
 * @details     To initialize the ILI9341 LCD driver
 */
void ILI9341_Initial(void)
{
    /* Reset the LCD controller before sending its init sequence. */
    SET_RST;
    CLK_SysTickDelay(5000);

    CLR_RST;
    CLK_SysTickDelay(20000);

    SET_RST;
    CLK_SysTickDelay(40000);

    /* Program the LCD controller registers. */
    LCD_WR_REG(0xCB);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x02);

    LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xC1);
    LCD_WR_DATA(0x30);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x85);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x78);

    LCD_WR_REG(0xEA);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x81);

    LCD_WR_REG(0xF7);
    LCD_WR_DATA(0x20);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x23);

    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x10);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x3e);
    LCD_WR_DATA(0x28);

    LCD_WR_REG(0xC7);
    LCD_WR_DATA(0x86);

    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x48);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x18);

    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x82);
    LCD_WR_DATA(0x27);

    LCD_WR_REG(0xF2);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x26);
    LCD_WR_DATA(0x01);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x4E);
    LCD_WR_DATA(0xF1);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0xC1);
    LCD_WR_DATA(0x48);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x0F);

    LCD_WR_REG(0x11);
    CLK_SysTickDelay(200000);   // Delay 200ms

    LCD_WR_REG(0x29);           //Display on

    printf("Initial ILI9341 LCD Module done.\n\n");

}

/**
 * @brief       Put an 8x16 character to LCD screen
 *
 * @param       x: the start position in the x-direction
 *              y: the start position in the y-direction
 *              c: the character of ASCII
 *              fcolor: the front color
 *              bcolor: the background color
 * @return      None
 *
 * @details     To show an 8x16 character on the LCD screen
 */
void LCD_PutChar8x16(uint16_t x, uint16_t y, uint8_t c, uint32_t fColor, uint32_t bColor)
{
    uint32_t i, j;
    uint8_t m;

    for(i=0; i<16; i++) {
        m = Font8x16[c*16+i];
        LCD_SetWindow(x, x+7, y+i, y+i);
        for(j=0; j<8; j++) {
            if((m & 0x80) == 0x80) {
                LCD_WR_DATA(fColor);
            } else {
                LCD_WR_DATA(bColor);
            }
            m <<= 1;
        }
    }

}

/**
 * @brief       Put a string to LCD screen
 *
 * @param       x: the start position in the x-direction
 *              y: the start position in the y-direction
 *              *s: the point of string
 *              fcolor: the front color
 *              bcolor: the background color
 * @return      None
 *
 * @details     To show a string on the LCD screen
 */
void LCD_PutString(uint16_t x, uint16_t y, uint8_t *s, uint32_t fColor, uint32_t bColor)
{
    uint8_t     l = 0;

    while(*s) {
        if(*s < 0x80) {
            LCD_PutChar8x16(x+l*8, y, *s, fColor, bColor);
            s++;
            l++;
        }
    }

}


/**
 * @brief       Blank an area of LCD screen
 *
 * @param       X: the X start position of this area, 0~239
 *              Y: the Y start position of this area, 0~319
 *              W: the width of this area, X+W < 240
 *              H: the height of this area, Y+H < 320
 *              color: the color will be filled into this area
 * @return      None
 *
 * @details     To fill a fixed color into an area on the LCD screen
 */
void LCD_BlankArea(uint16_t X, uint16_t Y, uint16_t W, uint16_t H, uint16_t color)
{
    uint16_t    i, j;

    LCD_SetWindow(X, X+W-1, Y, Y+H-1);
    for(j=0; j<H; j++) {
        for(i=0; i<W; i++)
            LCD_WR_DATA(color);
    }

}
