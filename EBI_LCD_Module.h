/**************************************************************************//**
 * @file     EBI_LCD_MODULE.h
 * @brief    EBI LCD module header file
 * @version  1.0.0
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __EBI_LCD_MODULE_H__
#define __EBI_LCD_MODULE_H__

/* LCD and application dimensions in pixels. */
#define LCD_W       240
#define LCD_H       320
#define APP_W       60
#define APP_H       60

/* Standard RGB565 colors. */
#define C_WHITE     0xFFFF
#define C_BLACK     0xAFB7
#define C_BLUE      0x001F
#define C_BLUE2     0x051F
#define C_RED       0xF800
#define C_MAGENTA   0xF81F
#define C_GREEN     0x07E0
#define C_CYAN      0x7FFF
#define C_YELLOW    0xFFE0

/* Shared font tables and gameplay timer state. */
extern uint8_t Font8x16[];
extern uint16_t Font16x32[];
extern uint8_t minutes;
extern uint8_t seconds;
extern uint8_t timer_running;

/* LCD, timer, and drawing helpers. */
void ILI9341_Initial(void);
void Timer3_Init(void);
void LCD_WR_REG(uint16_t cmd);
void LCD_WR_DATA(uint16_t dat);
uint16_t LCD_RD_DATA(void);
void LCD_SetWindow(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);
void LCD_PutString(uint16_t x, uint16_t y, uint8_t *s, uint32_t fColor, uint32_t bColor);
void LCD_PutChar16x32(uint16_t x, uint16_t y, uint16_t c, uint32_t fColor, uint32_t bColor);
void LCD_BlankArea(uint16_t X, uint16_t Y, uint16_t W, uint16_t H, uint16_t color);
uint16_t Get_TP_X(void);
uint16_t Get_TP_Y(void);
void    TimerDelay_Start(uint8_t ticks);
uint8_t TimerDelay_Done(void);
#endif  /* __EBI_LCD_MODULE_H__ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/



