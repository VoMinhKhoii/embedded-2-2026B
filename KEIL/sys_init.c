#include "NuMicro.h"

void SYS_Init(void)
{
    /* unlock protected registers */
    SYS_UnlockReg();

    /* 1) Enable HXT and wait for it */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* 2) HCLK = HXT */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));

    /* 3) PLL to 192 MHz, PCLK0/1 = HCLK/2 */
    CLK_SetCoreClock(FREQ_192MHZ);
    CLK->PCLKDIV = CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2;

    /* 4) Enable modules */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(EBI_MODULE);

    /* 5) Set multi-function pins for EBI AD0..AD15, nWR, nRD, nCS0 */
    /* AD0–AD5: PG9..PG14 */
    SYS->GPG_MFPH = (SYS->GPG_MFPH & ~(
        SYS_GPG_MFPH_PG9MFP_Msk  | SYS_GPG_MFPH_PG10MFP_Msk |
        SYS_GPG_MFPH_PG11MFP_Msk | SYS_GPG_MFPH_PG12MFP_Msk |
        SYS_GPG_MFPH_PG13MFP_Msk | SYS_GPG_MFPH_PG14MFP_Msk))
      | (SYS_GPG_MFPH_PG9MFP_EBI_AD0  | SYS_GPG_MFPH_PG10MFP_EBI_AD1 |
         SYS_GPG_MFPH_PG11MFP_EBI_AD2 | SYS_GPG_MFPH_PG12MFP_EBI_AD3 |
         SYS_GPG_MFPH_PG13MFP_EBI_AD4 | SYS_GPG_MFPH_PG14MFP_EBI_AD5);

    /* AD6–AD7: PD8..PD9 */
    SYS->GPD_MFPH = (SYS->GPD_MFPH & ~(SYS_GPD_MFPH_PD8MFP_Msk|SYS_GPD_MFPH_PD9MFP_Msk))
                  | (SYS_GPD_MFPH_PD8MFP_EBI_AD6|SYS_GPD_MFPH_PD9MFP_EBI_AD7);

    /* AD8–AD9: PE14..PE15 */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & ~(SYS_GPE_MFPH_PE14MFP_Msk|SYS_GPE_MFPH_PE15MFP_Msk))
                  | (SYS_GPE_MFPH_PE14MFP_EBI_AD8|SYS_GPE_MFPH_PE15MFP_EBI_AD9);

    /* AD10–AD11: PE0..PE1 */
    SYS->GPE_MFPL = (SYS->GPE_MFPL & ~(SYS_GPE_MFPL_PE0MFP_Msk|SYS_GPE_MFPL_PE1MFP_Msk))
                  | (SYS_GPE_MFPL_PE0MFP_EBI_AD11|SYS_GPE_MFPL_PE1MFP_EBI_AD10);

    /* AD12–AD15: PH8..PH11 */
    SYS->GPH_MFPH = (SYS->GPH_MFPH & ~(
        SYS_GPH_MFPH_PH8MFP_Msk | SYS_GPH_MFPH_PH9MFP_Msk |
        SYS_GPH_MFPH_PH10MFP_Msk| SYS_GPH_MFPH_PH11MFP_Msk))
      | (SYS_GPH_MFPH_PH8MFP_EBI_AD12 | SYS_GPH_MFPH_PH9MFP_EBI_AD13 |
         SYS_GPH_MFPH_PH10MFP_EBI_AD14| SYS_GPH_MFPH_PH11MFP_EBI_AD15);

    /* nWR, nRD: PE4, PE5 */
    SYS->GPE_MFPL = (SYS->GPE_MFPL & ~(SYS_GPE_MFPL_PE4MFP_Msk|SYS_GPE_MFPL_PE5MFP_Msk))
                  | (SYS_GPE_MFPL_PE4MFP_EBI_nWR|SYS_GPE_MFPL_PE5MFP_EBI_nRD);

    /* nCS0: PD14 */
    SYS->GPD_MFPH = (SYS->GPD_MFPH & ~SYS_GPD_MFPH_PD14MFP_Msk)
                  | SYS_GPD_MFPH_PD14MFP_EBI_nCS0;

    /* RS line on PH.3 */
    GPIO_SetMode(PH, BIT3, GPIO_MODE_OUTPUT);
    PH3 = 1;

    /* RESET on PB.6, backlight on PB.7 */
    GPIO_SetMode(PB, BIT6|BIT7, GPIO_MODE_OUTPUT);
    PB6 = 1;  /* de-assert LCD reset */
    PB7 = 0;  /* LCD backlight off until main */

    /* UART0 PD2/PD3 just in case you want prints */
    SYS->GPD_MFPL = (SYS->GPD_MFPL & ~(SYS_GPD_MFPL_PD2MFP_Msk|SYS_GPD_MFPL_PD3MFP_Msk))
                  | (SYS_GPD_MFPL_PD2MFP_UART0_RXD|SYS_GPD_MFPL_PD3MFP_UART0_TXD);

    /* lock back */
    SYS_LockReg();
}
