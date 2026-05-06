/**
 * @file clockpower.c
 * @brief Clock and Power Control Driver implementation for LPC1769.
 *
 * This file implements the API declared in clockpower.h.
 */

#include "clockpower.h"

#include "LPC17xx.h"
#include "system_LPC17xx.h"

uint32_t CLKPWR_GetCClk(void) {
    /*
     * SystemCoreClock is the CMSIS global variable with the current CPU clock.
     * SystemCoreClockUpdate() refreshes it from the LPC17xx clock registers.
     */
    SystemCoreClockUpdate();

    return SystemCoreClock;
}

uint32_t CLKPWR_GetPClk(CLKPWR_PeripheralClock periph) {
    uint32_t pclk_sel;
    uint32_t div_code;
    uint32_t cclk;

    cclk = CLKPWR_GetCClk();

    /*
     * The enum CLKPWR_PeripheralClock stores the bit position of the
     * 2-bit divider field inside PCLKSEL0 or PCLKSEL1.
     *
     * Positions 0..31  -> PCLKSEL0
     * Positions 32..63 -> PCLKSEL1
     */
    if ((uint32_t)periph < 32U) {
        pclk_sel = LPC_SC->PCLKSEL0;
        div_code = (pclk_sel >> (uint32_t)periph) & 0x03U;
    } else if ((uint32_t)periph < 64U) {
        pclk_sel = LPC_SC->PCLKSEL1;
        div_code = (pclk_sel >> ((uint32_t)periph - 32U)) & 0x03U;
    } else {
        return 0U;
    }

    /*
     * LPC1769 peripheral clock divider encoding:
     *
     * 00b -> CCLK / 4
     * 01b -> CCLK / 1
     * 10b -> CCLK / 2
     * 11b -> CCLK / 8
     */
    switch ((CLKPWR_ClockDiv)div_code) {
        case CLKPWR_DIV_4:
            return cclk / 4U;

        case CLKPWR_DIV_1:
            return cclk;

        case CLKPWR_DIV_2:
            return cclk / 2U;

        case CLKPWR_DIV_8:
            return cclk / 8U;

        default:
            return 0U;
    }
}

void CLKPWR_SetPClk(CLKPWR_PeripheralClock periph, CLKPWR_ClockDiv div) {
    /*
     * Each peripheral clock divider uses 2 bits.
     * We clear those 2 bits first, then write the selected divider.
     */
    if ((uint32_t)periph < 32U) {
        LPC_SC->PCLKSEL0 &= ~(0x03UL << (uint32_t)periph);
        LPC_SC->PCLKSEL0 |=  (((uint32_t)div & 0x03UL) << (uint32_t)periph);
    } else if ((uint32_t)periph < 64U) {
        uint32_t shift = (uint32_t)periph - 32U;

        LPC_SC->PCLKSEL1 &= ~(0x03UL << shift);
        LPC_SC->PCLKSEL1 |=  (((uint32_t)div & 0x03UL) << shift);
    }
}

void CLKPWR_Enable(CLKPWR_PeripheralPower periph) {
    /*
     * PCONP controls power/clock enable for peripherals.
     * Writing 1 to the peripheral bit enables it.
     */
    if ((uint32_t)periph < 32U) {
        LPC_SC->PCONP |= (1UL << (uint32_t)periph);
    }
}

void CLKPWR_Disable(CLKPWR_PeripheralPower periph) {
    /*
     * Writing 0 to the peripheral bit disables it.
     */
    if ((uint32_t)periph < 32U) {
        LPC_SC->PCONP &= ~(1UL << (uint32_t)periph);
    }
}
