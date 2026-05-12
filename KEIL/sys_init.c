#include "NuMicro.h"

void SYS_Init(void)
{
    /* Unlock protected registers before clock and pin configuration. */
    SYS_UnlockReg();

    /* Enable the external high-speed crystal and wait for it to stabilize. */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Start with HCLK sourced from HXT. */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));

    /* Raise the core clock and divide peripheral clocks from it. */
    CLK_SetCoreClock(FREQ_192MHZ);
    CLK->PCLKDIV = CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2;

    /* Enable the modules required by the game. */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(EBI_MODULE);

    /* Configure EBI address/data and control pins for the LCD. */
    /* AD0-AD5: PG9-PG14 */
    SYS->GPG_MFPH = (SYS->GPG_MFPH & ~(
        SYS_GPG_MFPH_PG9MFP_Msk  | SYS_GPG_MFPH_PG10MFP_Msk |
        SYS_GPG_MFPH_PG11MFP_Msk | SYS_GPG_MFPH_PG12MFP_Msk |
        SYS_GPG_MFPH_PG13MFP_Msk | SYS_GPG_MFPH_PG14MFP_Msk))
      | (SYS_GPG_MFPH_PG9MFP_EBI_AD0  | SYS_GPG_MFPH_PG10MFP_EBI_AD1 |
         SYS_GPG_MFPH_PG11MFP_EBI_AD2 | SYS_GPG_MFPH_PG12MFP_EBI_AD3 |
         SYS_GPG_MFPH_PG13MFP_EBI_AD4 | SYS_GPG_MFPH_PG14MFP_EBI_AD5);

    /* AD6-AD7: PD8-PD9 */
    SYS->GPD_MFPH = (SYS->GPD_MFPH & ~(SYS_GPD_MFPH_PD8MFP_Msk|SYS_GPD_MFPH_PD9MFP_Msk))
                  | (SYS_GPD_MFPH_PD8MFP_EBI_AD6|SYS_GPD_MFPH_PD9MFP_EBI_AD7);

    /* AD8-AD9: PE14-PE15 */
    SYS->GPE_MFPH = (SYS->GPE_MFPH & ~(SYS_GPE_MFPH_PE14MFP_Msk|SYS_GPE_MFPH_PE15MFP_Msk))
                  | (SYS_GPE_MFPH_PE14MFP_EBI_AD8|SYS_GPE_MFPH_PE15MFP_EBI_AD9);

    /* AD10-AD11: PE0-PE1 */
    SYS->GPE_MFPL = (SYS->GPE_MFPL & ~(SYS_GPE_MFPL_PE0MFP_Msk|SYS_GPE_MFPL_PE1MFP_Msk))
                  | (SYS_GPE_MFPL_PE0MFP_EBI_AD11|SYS_GPE_MFPL_PE1MFP_EBI_AD10);

    /* AD12-AD15: PH8-PH11 */
    SYS->GPH_MFPH = (SYS->GPH_MFPH & ~(
        SYS_GPH_MFPH_PH8MFP_Msk | SYS_GPH_MFPH_PH9MFP_Msk |
        SYS_GPH_MFPH_PH10MFP_Msk| SYS_GPH_MFPH_PH11MFP_Msk))
      | (SYS_GPH_MFPH_PH8MFP_EBI_AD12 | SYS_GPH_MFPH_PH9MFP_EBI_AD13 |
         SYS_GPH_MFPH_PH10MFP_EBI_AD14| SYS_GPH_MFPH_PH11MFP_EBI_AD15);

    /* nWR and nRD: PE4 and PE5 */
    SYS->GPE_MFPL = (SYS->GPE_MFPL & ~(SYS_GPE_MFPL_PE4MFP_Msk|SYS_GPE_MFPL_PE5MFP_Msk))
                  | (SYS_GPE_MFPL_PE4MFP_EBI_nWR|SYS_GPE_MFPL_PE5MFP_EBI_nRD);

    /* nCS0: PD14 */
    SYS->GPD_MFPH = (SYS->GPD_MFPH & ~SYS_GPD_MFPH_PD14MFP_Msk)
                  | SYS_GPD_MFPH_PD14MFP_EBI_nCS0;

    /* Use PH3 as the LCD register-select line. */
    GPIO_SetMode(PH, BIT3, GPIO_MODE_OUTPUT);
    PH3 = 1;

    /* Keep the display in reset-release state and leave backlight off for now. */
    GPIO_SetMode(PB, BIT6|BIT7, GPIO_MODE_OUTPUT);
    PB6 = 1;
    PB7 = 0;

    /* Route UART0 to PD2/PD3 for debug output. */
    SYS->GPD_MFPL = (SYS->GPD_MFPL & ~(SYS_GPD_MFPL_PD2MFP_Msk|SYS_GPD_MFPL_PD3MFP_Msk))
                  | (SYS_GPD_MFPL_PD2MFP_UART0_RXD|SYS_GPD_MFPL_PD3MFP_UART0_TXD);

    /* Relock the protected registers after setup completes. */
    SYS_LockReg();
}
