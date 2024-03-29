/***************************************************************************//**
 * @file
 * @brief External Bus Interface (EBI) Peripheral API
 * @version 5.8.1
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "em_ebi.h"
#if defined(EBI_COUNT) && (EBI_COUNT > 0)
#include "em_assert.h"
#include "em_bus.h"

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EBI
 * @brief EBI External Bus Interface (EBI) Peripheral API
 * @details
 *  This module contains functions to control the EBI peripheral of Silicon
 *  Labs 32-bit MCUs and SoCs. The EBI is used for accessing external parallel
 *  devices. The devices appear as part of the internal memory map of the MCU.
 * @{
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* The ROUTE register has been renamed in the newest platform so these register
 * field names have been created to make it easier to work with both
 * the new and the old names in a generic way. */
#if defined(_EBI_ROUTE_MASK)
#define _EBI_GENERIC_ALEPEN_SHIFT  _EBI_ROUTE_ALEPEN_SHIFT
#define _EBI_GENERIC_BLPEN_SHIFT   _EBI_ROUTE_BLPEN_SHIFT
#define _EBI_GENERIC_EBIPEN_SHIFT  _EBI_ROUTE_EBIPEN_SHIFT
#define _EBI_GENERIC_CS0PEN_SHIFT  _EBI_ROUTE_CS0PEN_SHIFT
#define _EBI_GENERIC_CS1PEN_SHIFT  _EBI_ROUTE_CS1PEN_SHIFT
#define _EBI_GENERIC_CS2PEN_SHIFT  _EBI_ROUTE_CS2PEN_SHIFT
#define _EBI_GENERIC_CS3PEN_SHIFT  _EBI_ROUTE_CS3PEN_SHIFT
#define _EBI_GENERIC_RESETVALUE    _EBI_ROUTE_RESETVALUE
#define EBI_GENERIC_ROUTE_REG     EBI->ROUTE
#define _EBI_GENERIC_ALB_MASK      _EBI_ROUTE_ALB_MASK
#define _EBI_GENERIC_APEN_MASK     _EBI_ROUTE_APEN_MASK
#define EBI_GENERIC_TFTPEN        EBI_ROUTE_TFTPEN
#else
#define _EBI_GENERIC_ALEPEN_SHIFT  _EBI_ROUTEPEN_ALEPEN_SHIFT
#define _EBI_GENERIC_BLPEN_SHIFT   _EBI_ROUTEPEN_BLPEN_SHIFT
#define _EBI_GENERIC_EBIPEN_SHIFT  _EBI_ROUTEPEN_EBIPEN_SHIFT
#define _EBI_GENERIC_CS0PEN_SHIFT  _EBI_ROUTEPEN_CS0PEN_SHIFT
#define _EBI_GENERIC_CS1PEN_SHIFT  _EBI_ROUTEPEN_CS1PEN_SHIFT
#define _EBI_GENERIC_CS2PEN_SHIFT  _EBI_ROUTEPEN_CS2PEN_SHIFT
#define _EBI_GENERIC_CS3PEN_SHIFT  _EBI_ROUTEPEN_CS3PEN_SHIFT
#define _EBI_GENERIC_RESETVALUE    _EBI_ROUTEPEN_RESETVALUE
#define EBI_GENERIC_ROUTE_REG     EBI->ROUTEPEN
#define _EBI_GENERIC_ALB_MASK      _EBI_ROUTEPEN_ALB_MASK
#define _EBI_GENERIC_NANDPEN_MASK  _EBI_ROUTEPEN_NANDPEN_MASK
#define _EBI_GENERIC_APEN_MASK     _EBI_ROUTEPEN_APEN_MASK
#define EBI_GENERIC_TFTPEN        EBI_ROUTEPEN_TFTPEN
#endif

/***************************************************************************//**
 * @brief
 *   Perform a single-bit write operation on an EBI route register.
 *
 * @param[in] bit
 *   bit Bit position to write, 0-31.
 *
 * @param[in] val
 *   0 to clear bit and 1 to set bit.
 ******************************************************************************/
__STATIC_INLINE void EBI_RouteBitWrite(uint32_t bit, uint32_t val)
{
  BUS_RegBitWrite(&(EBI_GENERIC_ROUTE_REG), bit, val);
}
/** @endcond */

/***************************************************************************//**
 * @brief
 *   Configure and enable the External Bus Interface.
 *
 * @param[in] ebiInit
 *   The EBI configuration structure.
 *
 * @note
 *   GPIO lines must be configured as a PUSH_PULL for the correct operation.
 *   GPIO and EBI clocks must be enabled in the CMU.
 ******************************************************************************/
void EBI_Init(const EBI_Init_TypeDef *ebiInit)
{
  uint32_t ctrl = EBI->CTRL;

#if defined(_EFM32_GECKO_FAMILY)
  /* Set polarity of address ready */
  EBI_PolaritySet(ebiLineARDY, ebiInit->ardyPolarity);
  /* Set polarity of address latch enable */
  EBI_PolaritySet(ebiLineALE, ebiInit->alePolarity);
  /* Set polarity of write enable */
  EBI_PolaritySet(ebiLineWE, ebiInit->wePolarity);
  /* Set polarity of read enable */
  EBI_PolaritySet(ebiLineRE, ebiInit->rePolarity);
  /* Set polarity of chip select lines */
  EBI_PolaritySet(ebiLineCS, ebiInit->csPolarity);
#else
  /* Enable Independent Timing for devices that support it. */
  ctrl |= EBI_CTRL_ITS;

  /* Set the polarity of address ready. */
  EBI_BankPolaritySet(ebiInit->banks, ebiLineARDY, ebiInit->ardyPolarity);
  /* Set the polarity of address latch enable. */
  EBI_BankPolaritySet(ebiInit->banks, ebiLineALE, ebiInit->alePolarity);
  /* Set polarity of write enable */
  EBI_BankPolaritySet(ebiInit->banks, ebiLineWE, ebiInit->wePolarity);
  /* Set polarity of read enable */
  EBI_BankPolaritySet(ebiInit->banks, ebiLineRE, ebiInit->rePolarity);
  /* Set polarity of chip select lines */
  EBI_BankPolaritySet(ebiInit->banks, ebiLineCS, ebiInit->csPolarity);
  /* Set polarity of byte lane line */
  EBI_BankPolaritySet(ebiInit->banks, ebiLineBL, ebiInit->blPolarity);
#endif

  /* Configure EBI mode and control settings.  */
#if defined(_EFM32_GECKO_FAMILY)
  ctrl &= ~(_EBI_CTRL_MODE_MASK
            | _EBI_CTRL_ARDYEN_MASK
            | _EBI_CTRL_ARDYTODIS_MASK
            | _EBI_CTRL_BANK0EN_MASK
            | _EBI_CTRL_BANK1EN_MASK
            | _EBI_CTRL_BANK2EN_MASK
            | _EBI_CTRL_BANK3EN_MASK);
  if ( ebiInit->enable) {
    if ( ebiInit->banks & EBI_BANK0 ) {
      ctrl |= EBI_CTRL_BANK0EN;
    }
    if ( ebiInit->banks & EBI_BANK1 ) {
      ctrl |= EBI_CTRL_BANK1EN;
    }
    if ( ebiInit->banks & EBI_BANK2 ) {
      ctrl |= EBI_CTRL_BANK2EN;
    }
    if ( ebiInit->banks & EBI_BANK3 ) {
      ctrl |= EBI_CTRL_BANK3EN;
    }
  }
  ctrl |= ebiInit->mode;
  ctrl |= (ebiInit->ardyEnable << _EBI_CTRL_ARDYEN_SHIFT);
  ctrl |= (ebiInit->ardyDisableTimeout << _EBI_CTRL_ARDYTODIS_SHIFT);
#else
  if (ebiInit->banks & EBI_BANK0) {
    ctrl &= ~(_EBI_CTRL_MODE_MASK
              | _EBI_CTRL_ARDYEN_MASK
              | _EBI_CTRL_ARDYTODIS_MASK
              | _EBI_CTRL_BL_MASK
              | _EBI_CTRL_NOIDLE_MASK
              | _EBI_CTRL_BANK0EN_MASK);
    ctrl |= (ebiInit->mode << _EBI_CTRL_MODE_SHIFT);
    ctrl |= (ebiInit->ardyEnable << _EBI_CTRL_ARDYEN_SHIFT);
    ctrl |= (ebiInit->ardyDisableTimeout << _EBI_CTRL_ARDYTODIS_SHIFT);
    ctrl |= (ebiInit->blEnable << _EBI_CTRL_BL_SHIFT);
    ctrl |= (ebiInit->noIdle << _EBI_CTRL_NOIDLE_SHIFT);
    if ( ebiInit->enable) {
      ctrl |= EBI_CTRL_BANK0EN;
    }
  }
  if (ebiInit->banks & EBI_BANK1) {
    ctrl &= ~(_EBI_CTRL_BL1_MASK
              | _EBI_CTRL_MODE1_MASK
              | _EBI_CTRL_ARDY1EN_MASK
              | _EBI_CTRL_ARDYTO1DIS_MASK
              | _EBI_CTRL_NOIDLE1_MASK
              | _EBI_CTRL_BANK1EN_MASK);
    ctrl |= (ebiInit->mode << _EBI_CTRL_MODE1_SHIFT);
    ctrl |= (ebiInit->ardyEnable << _EBI_CTRL_ARDY1EN_SHIFT);
    ctrl |= (ebiInit->ardyDisableTimeout << _EBI_CTRL_ARDYTO1DIS_SHIFT);
    ctrl |= (ebiInit->blEnable << _EBI_CTRL_BL1_SHIFT);
    ctrl |= (ebiInit->noIdle << _EBI_CTRL_NOIDLE1_SHIFT);
    if ( ebiInit->enable) {
      ctrl |= EBI_CTRL_BANK1EN;
    }
  }
  if (ebiInit->banks & EBI_BANK2) {
    ctrl &= ~(_EBI_CTRL_BL2_MASK
              | _EBI_CTRL_MODE2_MASK
              | _EBI_CTRL_ARDY2EN_MASK
              | _EBI_CTRL_ARDYTO2DIS_MASK
              | _EBI_CTRL_NOIDLE2_MASK
              | _EBI_CTRL_BANK2EN_MASK);
    ctrl |= (ebiInit->mode << _EBI_CTRL_MODE2_SHIFT);
    ctrl |= (ebiInit->ardyEnable << _EBI_CTRL_ARDY2EN_SHIFT);
    ctrl |= (ebiInit->ardyDisableTimeout << _EBI_CTRL_ARDYTO2DIS_SHIFT);
    ctrl |= (ebiInit->blEnable << _EBI_CTRL_BL2_SHIFT);
    ctrl |= (ebiInit->noIdle << _EBI_CTRL_NOIDLE2_SHIFT);
    if ( ebiInit->enable) {
      ctrl |= EBI_CTRL_BANK2EN;
    }
  }
  if (ebiInit->banks & EBI_BANK3) {
    ctrl &= ~(_EBI_CTRL_BL3_MASK
              | _EBI_CTRL_MODE3_MASK
              | _EBI_CTRL_ARDY3EN_MASK
              | _EBI_CTRL_ARDYTO3DIS_MASK
              | _EBI_CTRL_NOIDLE3_MASK
              | _EBI_CTRL_BANK3EN_MASK);
    ctrl |= (ebiInit->mode << _EBI_CTRL_MODE3_SHIFT);
    ctrl |= (ebiInit->ardyEnable << _EBI_CTRL_ARDY3EN_SHIFT);
    ctrl |= (ebiInit->ardyDisableTimeout << _EBI_CTRL_ARDYTO3DIS_SHIFT);
    ctrl |= (ebiInit->blEnable << _EBI_CTRL_BL3_SHIFT);
    ctrl |= (ebiInit->noIdle << _EBI_CTRL_NOIDLE3_SHIFT);
    if ( ebiInit->enable) {
      ctrl |= EBI_CTRL_BANK3EN;
    }
  }
#endif

  /* Configure timing. */
#if defined(_EFM32_GECKO_FAMILY)
  EBI_ReadTimingSet(ebiInit->readSetupCycles,
                    ebiInit->readStrobeCycles,
                    ebiInit->readHoldCycles);
  EBI_WriteTimingSet(ebiInit->writeSetupCycles,
                     ebiInit->writeStrobeCycles,
                     ebiInit->writeHoldCycles);
  EBI_AddressTimingSet(ebiInit->addrSetupCycles,
                       ebiInit->addrHoldCycles);
#else
  EBI_BankReadTimingSet(ebiInit->banks,
                        ebiInit->readSetupCycles,
                        ebiInit->readStrobeCycles,
                        ebiInit->readHoldCycles);
  EBI_BankReadTimingConfig(ebiInit->banks,
                           ebiInit->readPageMode,
                           ebiInit->readPrefetch,
                           ebiInit->readHalfRE);
  EBI_BankWriteTimingSet(ebiInit->banks,
                         ebiInit->writeSetupCycles,
                         ebiInit->writeStrobeCycles,
                         ebiInit->writeHoldCycles);
  EBI_BankWriteTimingConfig(ebiInit->banks,
                            ebiInit->writeBufferDisable,
                            ebiInit->writeHalfWE);
  EBI_BankAddressTimingSet(ebiInit->banks,
                           ebiInit->addrSetupCycles,
                           ebiInit->addrHoldCycles);
  EBI_BankAddressTimingConfig(ebiInit->banks,
                              ebiInit->addrHalfALE);
#endif

  /* Activate the new configuration. */
  EBI->CTRL = ctrl;

  /* Configure Address Latch Enable. */
  switch (ebiInit->mode) {
    case ebiModeD16A16ALE:
    case ebiModeD8A24ALE:
      /* Address Latch Enable. */
      EBI_RouteBitWrite(_EBI_GENERIC_ALEPEN_SHIFT, 1);
      break;
#if defined(EBI_CTRL_MODE_D16)
    case ebiModeD16:
#endif
    case ebiModeD8A8:
      /* Make sure that Address Latch is disabled. */
      EBI_RouteBitWrite(_EBI_GENERIC_ALEPEN_SHIFT, 0);
      break;
  }

#if !defined(_EFM32_GECKO_FAMILY)
  /* Limit the pin enable. */
  EBI_GENERIC_ROUTE_REG = (EBI_GENERIC_ROUTE_REG & ~_EBI_GENERIC_ALB_MASK) | ebiInit->aLow;
  EBI_GENERIC_ROUTE_REG = (EBI_GENERIC_ROUTE_REG & ~_EBI_GENERIC_APEN_MASK) | ebiInit->aHigh;
#if defined(_EBI_ROUTE_LOCATION_MASK)
  /* Location. */
  EBI->ROUTE = (EBI->ROUTE & ~_EBI_ROUTE_LOCATION_MASK) | ebiInit->location;
#endif

  /* Enable the EBI BL pin if necessary. */
  if (ctrl & (_EBI_CTRL_BL_MASK | _EBI_CTRL_BL1_MASK | _EBI_CTRL_BL2_MASK | _EBI_CTRL_BL3_MASK)) {
    EBI_RouteBitWrite(_EBI_GENERIC_BLPEN_SHIFT, ebiInit->blEnable);
  }
#endif

  /* Enable EBI pins EBI_WEn and EBI_REn. */
  EBI_RouteBitWrite(_EBI_GENERIC_EBIPEN_SHIFT, 1);

  /* Enable chip select lines. */
  EBI_ChipSelectEnable(ebiInit->csLines, true);
}

/***************************************************************************//**
 * @brief
 *   Disable External Bus Interface
 ******************************************************************************/
void EBI_Disable(void)
{
  /* Disable pins */
  EBI_GENERIC_ROUTE_REG = _EBI_GENERIC_RESETVALUE;
  /* Disable banks */
  EBI->CTRL = _EBI_CTRL_RESETVALUE;
}

/***************************************************************************//**
 * @brief
 *   Enable or disable EBI Bank.
 *
 * @param[in] banks
 *   Banks to reconfigure, mask of EBI_BANK<n> flags.
 *
 * @param[in] enable
 *   True to enable, false to disable.
 ******************************************************************************/
void EBI_BankEnable(uint32_t banks, bool enable)
{
  if (banks & EBI_BANK0) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BANK0EN_SHIFT, enable);
  }
  if (banks & EBI_BANK1) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BANK1EN_SHIFT, enable);
  }
  if (banks & EBI_BANK2) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BANK2EN_SHIFT, enable);
  }
  if (banks & EBI_BANK3) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BANK3EN_SHIFT, enable);
  }
}

/***************************************************************************//**
 * @brief
 *   Return the base address of the EBI bank.
 *
 * @param[in] bank
 *   A bank to return the start address for.
 *
 * @return
 *   an absolute address of the bank.
 ******************************************************************************/
uint32_t EBI_BankAddress(uint32_t bank)
{
#if defined (EBI_CTRL_ALTMAP)
  if (EBI->CTRL & EBI_CTRL_ALTMAP) {
    switch (bank) {
      case EBI_BANK0:
        return EBI_MEM_BASE;

      case EBI_BANK1:
        return EBI_MEM_BASE + 0x10000000UL;

      case EBI_BANK2:
        return EBI_MEM_BASE + 0x20000000UL;

      case EBI_BANK3:
        return EBI_MEM_BASE + 0x30000000UL;

      default:
        EFM_ASSERT(0);
        break;
    }
  }
#endif
  switch (bank) {
    case EBI_BANK0:
      return EBI_MEM_BASE;

    case EBI_BANK1:
      return EBI_MEM_BASE + 0x04000000UL;

    case EBI_BANK2:
      return EBI_MEM_BASE + 0x08000000UL;

    case EBI_BANK3:
      return EBI_MEM_BASE + 0x0C000000UL;

    default:
      EFM_ASSERT(0);
      break;
  }
  return 0;
}

#if defined(_EBI_NANDCTRL_MASK)
/***************************************************************************//**
 * @brief
 *   Enable or disable EBI NAND Flash Support
 *
 * @param[in] banks
 *   Banks to reconfigure, mask of EBI_BANK<n> flags.
 *
 * @param[in] enable
 *   True to enable, false to disable.
 ******************************************************************************/
void EBI_NANDFlashEnable(uint32_t banks, bool enable)
{
  if (banks & EBI_BANK0) {
    BUS_RegBitWrite(&(EBI->NANDCTRL), _EBI_NANDCTRL_BANKSEL_SHIFT, _EBI_NANDCTRL_BANKSEL_BANK0);
  }
  if (banks & EBI_BANK1) {
    BUS_RegBitWrite(&(EBI->NANDCTRL), _EBI_NANDCTRL_BANKSEL_SHIFT, _EBI_NANDCTRL_BANKSEL_BANK1);
  }
  if (banks & EBI_BANK2) {
    BUS_RegBitWrite(&(EBI->NANDCTRL), _EBI_NANDCTRL_BANKSEL_SHIFT, _EBI_NANDCTRL_BANKSEL_BANK2);
  }
  if (banks & EBI_BANK3) {
    BUS_RegBitWrite(&(EBI->NANDCTRL), _EBI_NANDCTRL_BANKSEL_SHIFT, _EBI_NANDCTRL_BANKSEL_BANK3);
  }
  BUS_RegBitWrite(&(EBI->NANDCTRL), _EBI_NANDCTRL_EN_SHIFT, enable);
}
#endif

/***************************************************************************//**
 * @brief
 *   Enable or disable EBI Chip Select.
 *
 * @param[in] cs
 *   ChipSelect lines to reconfigure, mask of EBI_CS<n> flags.
 *
 * @param[in] enable
 *   True to enable, false to disable.
 ******************************************************************************/
void EBI_ChipSelectEnable(uint32_t cs, bool enable)
{
  if (cs & EBI_CS0) {
    EBI_RouteBitWrite(_EBI_GENERIC_CS0PEN_SHIFT, enable);
  }
  if (cs & EBI_CS1) {
    EBI_RouteBitWrite(_EBI_GENERIC_CS1PEN_SHIFT, enable);
  }
  if (cs & EBI_CS2) {
    EBI_RouteBitWrite(_EBI_GENERIC_CS2PEN_SHIFT, enable);
  }
  if (cs & EBI_CS3) {
    EBI_RouteBitWrite(_EBI_GENERIC_CS3PEN_SHIFT, enable);
  }
}

/***************************************************************************//**
 * @brief
 *   Configure EBI pin polarity.
 *
 * @param[in] line
 *   Which pin/line to configure.
 *
 * @param[in] polarity
 *   Active high or active low.
 ******************************************************************************/
void EBI_PolaritySet(EBI_Line_TypeDef line, EBI_Polarity_TypeDef polarity)
{
  switch (line) {
    case ebiLineARDY:
      BUS_RegBitWrite(&EBI->POLARITY, _EBI_POLARITY_ARDYPOL_SHIFT, polarity);
      break;
    case ebiLineALE:
      BUS_RegBitWrite(&EBI->POLARITY, _EBI_POLARITY_ALEPOL_SHIFT, polarity);
      break;
    case ebiLineWE:
      BUS_RegBitWrite(&EBI->POLARITY, _EBI_POLARITY_WEPOL_SHIFT, polarity);
      break;
    case ebiLineRE:
      BUS_RegBitWrite(&EBI->POLARITY, _EBI_POLARITY_REPOL_SHIFT, polarity);
      break;
    case ebiLineCS:
      BUS_RegBitWrite(&EBI->POLARITY, _EBI_POLARITY_CSPOL_SHIFT, polarity);
      break;
#if defined(_EBI_POLARITY_BLPOL_MASK)
    case ebiLineBL:
      BUS_RegBitWrite(&EBI->POLARITY, _EBI_POLARITY_BLPOL_SHIFT, polarity);
      break;
#endif
#if defined (_EBI_TFTPOLARITY_MASK)
    case ebiLineTFTVSync:
      BUS_RegBitWrite(&EBI->TFTPOLARITY, _EBI_TFTPOLARITY_VSYNCPOL_SHIFT, polarity);
      break;
    case ebiLineTFTHSync:
      BUS_RegBitWrite(&EBI->TFTPOLARITY, _EBI_TFTPOLARITY_HSYNCPOL_SHIFT, polarity);
      break;
    case ebiLineTFTDataEn:
      BUS_RegBitWrite(&EBI->TFTPOLARITY, _EBI_TFTPOLARITY_DATAENPOL_SHIFT, polarity);
      break;
    case ebiLineTFTDClk:
      BUS_RegBitWrite(&EBI->TFTPOLARITY, _EBI_TFTPOLARITY_DCLKPOL_SHIFT, polarity);
      break;
    case ebiLineTFTCS:
      BUS_RegBitWrite(&EBI->TFTPOLARITY, _EBI_TFTPOLARITY_CSPOL_SHIFT, polarity);
      break;
#endif
    default:
      EFM_ASSERT(0);
      break;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure timing values of read bus accesses.
 *
 * @param[in] setupCycles
 *   A number of clock cycles for the address setup before REn is asserted.
 *
 * @param[in] strobeCycles
 *   The number of cycles the REn is held active. After the specified number of
 *   cycles, data is read. If set to 0, 1 cycle is inserted by hardware.
 *
 * @param[in] holdCycles
 *   The number of cycles CSn is held active after the REn is deasserted.
 ******************************************************************************/
void EBI_ReadTimingSet(uint32_t setupCycles, uint32_t strobeCycles, uint32_t holdCycles)
{
  uint32_t readTiming;

  /* Check that timings are within limits */
  EFM_ASSERT(setupCycles <= _EBI_RDTIMING_RDSETUP_MASK >> _EBI_RDTIMING_RDSETUP_SHIFT);
  EFM_ASSERT(strobeCycles <= _EBI_RDTIMING_RDSTRB_MASK >> _EBI_RDTIMING_RDSTRB_SHIFT);
  EFM_ASSERT(holdCycles <= _EBI_RDTIMING_RDHOLD_MASK >> _EBI_RDTIMING_RDHOLD_SHIFT);

  /* Configure timing values */
  readTiming = (setupCycles << _EBI_RDTIMING_RDSETUP_SHIFT)
               | (strobeCycles << _EBI_RDTIMING_RDSTRB_SHIFT)
               | (holdCycles << _EBI_RDTIMING_RDHOLD_SHIFT);

  EBI->RDTIMING = (EBI->RDTIMING
                   & ~(_EBI_RDTIMING_RDSETUP_MASK
                       | _EBI_RDTIMING_RDSTRB_MASK
                       | _EBI_RDTIMING_RDHOLD_MASK))
                  | readTiming;
}

/***************************************************************************//**
 * @brief
 *   Configure timing values of write bus accesses.
 *
 * @param[in] setupCycles
 *   A number of clock cycles for the address setup before WEn is asserted.
 *
 * @param[in] strobeCycles
 *   A number of cycles WEn is held active. If set to 0, 1 cycle is inserted by hardware.
 *
 * @param[in] holdCycles
 *   A number of cycles CSn is held active after the WEn is deasserted.
 ******************************************************************************/
void EBI_WriteTimingSet(uint32_t setupCycles, uint32_t strobeCycles, uint32_t holdCycles)
{
  uint32_t writeTiming;

  /* Check that timings are within limits */
  EFM_ASSERT(setupCycles <= _EBI_WRTIMING_WRSETUP_MASK >> _EBI_WRTIMING_WRSETUP_SHIFT);
  EFM_ASSERT(strobeCycles <= _EBI_WRTIMING_WRSTRB_MASK >> _EBI_WRTIMING_WRSTRB_SHIFT);
  EFM_ASSERT(holdCycles <= _EBI_WRTIMING_WRHOLD_MASK >> _EBI_WRTIMING_WRHOLD_SHIFT);

  /* Configure timing values. */
  writeTiming = (setupCycles << _EBI_WRTIMING_WRSETUP_SHIFT)
                | (strobeCycles << _EBI_WRTIMING_WRSTRB_SHIFT)
                | (holdCycles << _EBI_WRTIMING_WRHOLD_SHIFT);

  EBI->WRTIMING = (EBI->WRTIMING
                   & ~(_EBI_WRTIMING_WRSETUP_MASK
                       | _EBI_WRTIMING_WRSTRB_MASK
                       | _EBI_WRTIMING_WRHOLD_MASK))
                  | writeTiming;
}

/***************************************************************************//**
 * @brief
 *   Configure timing values of address latch bus accesses.
 *
 * @param[in] setupCycles
 *   Sets the number of cycles the address is held after ALE is asserted.
 *
 * @param[in] holdCycles
 *   Sets the number of cycles the address is driven onto the ADDRDAT bus before
 *   ALE is asserted. If set 0, 1 cycle is inserted by hardware.
 ******************************************************************************/
void EBI_AddressTimingSet(uint32_t setupCycles, uint32_t holdCycles)
{
  uint32_t addressLatchTiming;

  /* Check that timings are within limits */
  EFM_ASSERT(setupCycles <= _EBI_ADDRTIMING_ADDRSETUP_MASK >> _EBI_ADDRTIMING_ADDRSETUP_SHIFT);
  EFM_ASSERT(holdCycles <= _EBI_ADDRTIMING_ADDRHOLD_MASK >> _EBI_ADDRTIMING_ADDRHOLD_SHIFT);

  /* Configure address latch timing values. */
  addressLatchTiming = (setupCycles << _EBI_ADDRTIMING_ADDRSETUP_SHIFT)
                       | (holdCycles << _EBI_ADDRTIMING_ADDRHOLD_SHIFT);

  EBI->ADDRTIMING = (EBI->ADDRTIMING
                     & ~(_EBI_ADDRTIMING_ADDRSETUP_MASK
                         | _EBI_ADDRTIMING_ADDRHOLD_MASK))
                    | addressLatchTiming;
}

#if defined(_EBI_TFTCTRL_MASK)
/***************************************************************************//**
 * @brief
 *   Configure and initialize TFT Direct Drive.
 *
 * @param[in] ebiTFTInit
 *   The TFT Initialization structure.
 ******************************************************************************/
void EBI_TFTInit(const EBI_TFTInit_TypeDef *ebiTFTInit)
{
  uint32_t ctrl;

  /* Configure the base address for the frame buffer offset to the EBI bank. */
  EBI_TFTFrameBaseSet(ebiTFTInit->addressOffset);

  /* Configure the display size and porch areas. */
  EBI_TFTSizeSet(ebiTFTInit->hsize,
                 ebiTFTInit->vsize);
  EBI_TFTHPorchSet(ebiTFTInit->hPorchFront,
                   ebiTFTInit->hPorchBack,
                   ebiTFTInit->hPulseWidth);
  EBI_TFTVPorchSet(ebiTFTInit->vPorchFront,
                   ebiTFTInit->vPorchBack,
                   ebiTFTInit->vPulseWidth);

  /* Configure timing settings. */
  EBI_TFTTimingSet(ebiTFTInit->dclkPeriod,
                   ebiTFTInit->startPosition,
                   ebiTFTInit->setupCycles,
                   ebiTFTInit->holdCycles);

  /* Configure line polarity settings. */
  EBI_PolaritySet(ebiLineTFTCS, ebiTFTInit->csPolarity);
  EBI_PolaritySet(ebiLineTFTDClk, ebiTFTInit->dclkPolarity);
  EBI_PolaritySet(ebiLineTFTDataEn, ebiTFTInit->dataenPolarity);
  EBI_PolaritySet(ebiLineTFTVSync, ebiTFTInit->vsyncPolarity);
  EBI_PolaritySet(ebiLineTFTHSync, ebiTFTInit->hsyncPolarity);

  /* Main control, EBI bank select, mask and blending configuration. */
  ctrl = (uint32_t)ebiTFTInit->bank
         | (uint32_t)ebiTFTInit->width
         | (uint32_t)ebiTFTInit->colSrc
         | (uint32_t)ebiTFTInit->interleave
         | (uint32_t)ebiTFTInit->fbTrigger
         | (uint32_t)(ebiTFTInit->shiftDClk == true
                      ? (1 << _EBI_TFTCTRL_SHIFTDCLKEN_SHIFT) : 0)
         | (uint32_t)ebiTFTInit->maskBlend
         | (uint32_t)ebiTFTInit->driveMode;

  EBI->TFTCTRL = ctrl;

  /* Enable TFT pins. */
  if (ebiTFTInit->driveMode != ebiTFTDDModeDisabled) {
    EBI_GENERIC_ROUTE_REG |= EBI_GENERIC_TFTPEN;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure and initialize TFT size settings.
 *
 * @param[in] horizontal
 *   TFT display horizontal size in pixels.
 * @param[in] vertical
 *   TFT display vertical size in pixels.
 ******************************************************************************/
void EBI_TFTSizeSet(uint32_t horizontal, uint32_t vertical)
{
  EFM_ASSERT((horizontal - 1) <= _EBI_TFTSIZE_HSZ_MASK >> _EBI_TFTSIZE_HSZ_SHIFT);
  EFM_ASSERT((vertical - 1) <= _EBI_TFTSIZE_VSZ_MASK >> _EBI_TFTSIZE_VSZ_SHIFT);

  EBI->TFTSIZE = ((horizontal - 1) << _EBI_TFTSIZE_HSZ_SHIFT)
                 | ((vertical - 1) << _EBI_TFTSIZE_VSZ_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Configure and initialize Horizontal Porch Settings.
 *
 * @param[in] front
 *   Horizontal front porch size in pixels.
 * @param[in] back
 *   Horizontal back porch size in pixels.
 * @param[in] pulseWidth
 *   Horizontal synchronization pulse width. Set to required -1.
 ******************************************************************************/
void EBI_TFTHPorchSet(uint32_t front, uint32_t back, uint32_t pulseWidth)
{
  EFM_ASSERT(front <= _EBI_TFTHPORCH_HFPORCH_MASK >> _EBI_TFTHPORCH_HFPORCH_SHIFT);
  EFM_ASSERT(back <= _EBI_TFTHPORCH_HBPORCH_MASK >> _EBI_TFTHPORCH_HBPORCH_SHIFT);
  EFM_ASSERT((pulseWidth - 1) <= _EBI_TFTHPORCH_HSYNC_MASK >> _EBI_TFTHPORCH_HSYNC_SHIFT);

  EBI->TFTHPORCH = (front << _EBI_TFTHPORCH_HFPORCH_SHIFT)
                   | (back << _EBI_TFTHPORCH_HBPORCH_SHIFT)
                   | ((pulseWidth - 1) << _EBI_TFTHPORCH_HSYNC_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Configure Vertical Porch Settings.
 *
 * @param[in] front
 *   Vertical front porch size in pixels.
 * @param[in] back
 *   Vertical back porch size in pixels.
 * @param[in] pulseWidth
 *   Vertical synchronization pulse width. Set to required -1.
 ******************************************************************************/
void EBI_TFTVPorchSet(uint32_t front, uint32_t back, uint32_t pulseWidth)
{
  EFM_ASSERT(front <= _EBI_TFTVPORCH_VFPORCH_MASK >> _EBI_TFTVPORCH_VFPORCH_SHIFT);
  EFM_ASSERT(back <= _EBI_TFTVPORCH_VBPORCH_MASK >> _EBI_TFTVPORCH_VBPORCH_SHIFT);
  EFM_ASSERT((pulseWidth - 1) <= _EBI_TFTVPORCH_VSYNC_MASK >> _EBI_TFTVPORCH_VSYNC_SHIFT);

  EBI->TFTVPORCH = (front << _EBI_TFTVPORCH_VFPORCH_SHIFT)
                   | (back << _EBI_TFTVPORCH_VBPORCH_SHIFT)
                   | ((pulseWidth - 1) << _EBI_TFTVPORCH_VSYNC_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Configure TFT Direct Drive Timing Settings.
 *
 * @param[in] dclkPeriod
 *   DCLK period in internal cycles.
 *
 * @param[in] start
 *   A starting position of the external direct drive, relative to the DCLK inactive edge.
 *
 * @param[in] setup
 *   A number of cycles that RGB data is driven before the active edge of DCLK.
 *
 * @param[in] hold
 *   A number of cycles that RGB data is held after the active edge of DCLK.
 ******************************************************************************/
void EBI_TFTTimingSet(uint32_t dclkPeriod, uint32_t start, uint32_t setup, uint32_t hold)
{
  /* Check that timings are within limits */
  EFM_ASSERT(dclkPeriod <= _EBI_TFTTIMING_DCLKPERIOD_MASK >> _EBI_TFTTIMING_DCLKPERIOD_SHIFT);
  EFM_ASSERT(start <= _EBI_TFTTIMING_TFTSTART_MASK >> _EBI_TFTTIMING_TFTSTART_SHIFT);
  EFM_ASSERT(setup <= _EBI_TFTTIMING_TFTSETUP_MASK >> _EBI_TFTTIMING_TFTSETUP_SHIFT);
  EFM_ASSERT(hold <= _EBI_TFTTIMING_TFTHOLD_MASK >> _EBI_TFTTIMING_TFTHOLD_SHIFT);

  EBI->TFTTIMING = (dclkPeriod << _EBI_TFTTIMING_DCLKPERIOD_SHIFT)
                   | (start << _EBI_TFTTIMING_TFTSTART_SHIFT)
                   | (setup << _EBI_TFTTIMING_TFTSETUP_SHIFT)
                   | (hold << _EBI_TFTTIMING_TFTHOLD_SHIFT);
}
#endif

#if !defined(_EFM32_GECKO_FAMILY)
/***************************************************************************//**
 * @brief
 *   Configure read operation parameters for a selected bank.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure write timing for.
 *
 * @param[in] pageMode
 *   Enables or disables the half cycle WE strobe in the last strobe cycle.
 *
 * @param[in] prefetch
 *   Enables or disables the half cycle WE strobe in the last strobe cycle.
 *
 * @param[in] halfRE
 *   Enables or disables the half cycle WE strobe in the last strobe cycle.
 ******************************************************************************/
void EBI_BankReadTimingConfig(uint32_t banks, bool pageMode, bool prefetch, bool halfRE)
{
  /* Verify only valid banks are used */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  /* Configure read operation parameters. */
  if ( banks & EBI_BANK0 ) {
    BUS_RegBitWrite(&EBI->RDTIMING, _EBI_RDTIMING_PAGEMODE_SHIFT, pageMode);
    BUS_RegBitWrite(&EBI->RDTIMING, _EBI_RDTIMING_PREFETCH_SHIFT, prefetch);
    BUS_RegBitWrite(&EBI->RDTIMING, _EBI_RDTIMING_HALFRE_SHIFT, halfRE);
  }
  if ( banks & EBI_BANK1 ) {
    BUS_RegBitWrite(&EBI->RDTIMING1, _EBI_RDTIMING_PAGEMODE_SHIFT, pageMode);
    BUS_RegBitWrite(&EBI->RDTIMING1, _EBI_RDTIMING_PREFETCH_SHIFT, prefetch);
    BUS_RegBitWrite(&EBI->RDTIMING1, _EBI_RDTIMING_HALFRE_SHIFT, halfRE);
  }
  if ( banks & EBI_BANK2 ) {
    BUS_RegBitWrite(&EBI->RDTIMING2, _EBI_RDTIMING_PAGEMODE_SHIFT, pageMode);
    BUS_RegBitWrite(&EBI->RDTIMING2, _EBI_RDTIMING_PREFETCH_SHIFT, prefetch);
    BUS_RegBitWrite(&EBI->RDTIMING2, _EBI_RDTIMING_HALFRE_SHIFT, halfRE);
  }
  if ( banks & EBI_BANK3 ) {
    BUS_RegBitWrite(&EBI->RDTIMING3, _EBI_RDTIMING_PAGEMODE_SHIFT, pageMode);
    BUS_RegBitWrite(&EBI->RDTIMING3, _EBI_RDTIMING_PREFETCH_SHIFT, prefetch);
    BUS_RegBitWrite(&EBI->RDTIMING3, _EBI_RDTIMING_HALFRE_SHIFT, halfRE);
  }
}

/***************************************************************************//**
 * @brief
 *   Configure timing values of read bus accesses.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure timing for.
 *
 * @param[in] setupCycles
 *   A number of clock cycles for address setup before REn is asserted.
 *
 * @param[in] strobeCycles
 *   The number of cycles the REn is held active. After the specified number of
 *   cycles, data is read. If set to 0, 1 cycle is inserted by hardware.
 *
 * @param[in] holdCycles
 *   The number of cycles CSn is held active after the REn is deasserted.
 ******************************************************************************/
void EBI_BankReadTimingSet(uint32_t banks, uint32_t setupCycles, uint32_t strobeCycles, uint32_t holdCycles)
{
  uint32_t readTiming;

  /* Verify only valid banks are used. */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  /* Check that timings are within limits */
  EFM_ASSERT(setupCycles <= _EBI_RDTIMING_RDSETUP_MASK >> _EBI_RDTIMING_RDSETUP_SHIFT);
  EFM_ASSERT(strobeCycles <= _EBI_RDTIMING_RDSTRB_MASK >> _EBI_RDTIMING_RDSTRB_SHIFT);
  EFM_ASSERT(holdCycles <= _EBI_RDTIMING_RDHOLD_MASK >> _EBI_RDTIMING_RDHOLD_SHIFT);

  /* Configure timing values. */
  readTiming = (setupCycles << _EBI_RDTIMING_RDSETUP_SHIFT)
               | (strobeCycles << _EBI_RDTIMING_RDSTRB_SHIFT)
               | (holdCycles << _EBI_RDTIMING_RDHOLD_SHIFT);

  if (banks & EBI_BANK0) {
    EBI->RDTIMING = (EBI->RDTIMING
                     & ~(_EBI_RDTIMING_RDSETUP_MASK
                         | _EBI_RDTIMING_RDSTRB_MASK
                         | _EBI_RDTIMING_RDHOLD_MASK))
                    | readTiming;
  }
  if (banks & EBI_BANK1) {
    EBI->RDTIMING1 = (EBI->RDTIMING1
                      & ~(_EBI_RDTIMING1_RDSETUP_MASK
                          | _EBI_RDTIMING1_RDSTRB_MASK
                          | _EBI_RDTIMING1_RDHOLD_MASK))
                     | readTiming;
  }
  if (banks & EBI_BANK2) {
    EBI->RDTIMING2 = (EBI->RDTIMING2
                      & ~(_EBI_RDTIMING2_RDSETUP_MASK
                          | _EBI_RDTIMING2_RDSTRB_MASK
                          | _EBI_RDTIMING2_RDHOLD_MASK))
                     | readTiming;
  }
  if (banks & EBI_BANK3) {
    EBI->RDTIMING3 = (EBI->RDTIMING3
                      & ~(_EBI_RDTIMING3_RDSETUP_MASK
                          | _EBI_RDTIMING3_RDSTRB_MASK
                          | _EBI_RDTIMING3_RDHOLD_MASK))
                     | readTiming;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure write operation parameters for a selected bank.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure write timing for.
 *
 * @param[in] writeBufDisable
 *   If true, disable the write buffer.
 *
 * @param[in] halfWE
 *   Enables or disables half cycle WE strobe in the last strobe cycle.
 ******************************************************************************/
void EBI_BankWriteTimingConfig(uint32_t banks, bool writeBufDisable, bool halfWE)
{
  /* Verify only valid banks are used. */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  /* Configure write operation parameters. */
  if ( banks & EBI_BANK0 ) {
    BUS_RegBitWrite(&EBI->WRTIMING, _EBI_WRTIMING_WBUFDIS_SHIFT, writeBufDisable);
    BUS_RegBitWrite(&EBI->WRTIMING, _EBI_WRTIMING_HALFWE_SHIFT, halfWE);
  }
  if ( banks & EBI_BANK1 ) {
    BUS_RegBitWrite(&EBI->WRTIMING1, _EBI_WRTIMING_WBUFDIS_SHIFT, writeBufDisable);
    BUS_RegBitWrite(&EBI->WRTIMING1, _EBI_WRTIMING_HALFWE_SHIFT, halfWE);
  }
  if ( banks & EBI_BANK2 ) {
    BUS_RegBitWrite(&EBI->WRTIMING2, _EBI_WRTIMING_WBUFDIS_SHIFT, writeBufDisable);
    BUS_RegBitWrite(&EBI->WRTIMING2, _EBI_WRTIMING_HALFWE_SHIFT, halfWE);
  }
  if ( banks & EBI_BANK3 ) {
    BUS_RegBitWrite(&EBI->WRTIMING3, _EBI_WRTIMING_WBUFDIS_SHIFT, writeBufDisable);
    BUS_RegBitWrite(&EBI->WRTIMING3, _EBI_WRTIMING_HALFWE_SHIFT, halfWE);
  }
}

/***************************************************************************//**
 * @brief
 *   Configure timing values of write bus accesses.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure write timing for.
 *
 * @param[in] setupCycles
 *   A number of clock cycles for address setup before WEn is asserted.
 *
 * @param[in] strobeCycles
 *   A number of cycles WEn is held active. If set to 0, 1 cycle is inserted by hardware.
 *
 * @param[in] holdCycles
 *   Number of cycles CSn is held active after  WEn is deasserted.
 ******************************************************************************/
void EBI_BankWriteTimingSet(uint32_t banks, uint32_t setupCycles, uint32_t strobeCycles, uint32_t holdCycles)
{
  uint32_t writeTiming;

  /* Verify only valid banks are used.*/
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  /* Check that timings are within limits */
  EFM_ASSERT(setupCycles <= _EBI_WRTIMING_WRSETUP_MASK >> _EBI_WRTIMING_WRSETUP_SHIFT);
  EFM_ASSERT(strobeCycles <= _EBI_WRTIMING_WRSTRB_MASK >> _EBI_WRTIMING_WRSTRB_SHIFT);
  EFM_ASSERT(holdCycles <= _EBI_WRTIMING_WRHOLD_MASK >> _EBI_WRTIMING_WRHOLD_SHIFT);

  /* Configure timing values. */
  writeTiming = (setupCycles << _EBI_WRTIMING_WRSETUP_SHIFT)
                | (strobeCycles << _EBI_WRTIMING_WRSTRB_SHIFT)
                | (holdCycles << _EBI_WRTIMING_WRHOLD_SHIFT);

  if (banks & EBI_BANK0) {
    EBI->WRTIMING = (EBI->WRTIMING
                     & ~(_EBI_WRTIMING_WRSETUP_MASK
                         | _EBI_WRTIMING_WRSTRB_MASK
                         | _EBI_WRTIMING_WRHOLD_MASK))
                    | writeTiming;
  }
  if (banks & EBI_BANK1) {
    EBI->WRTIMING1 = (EBI->WRTIMING1
                      & ~(_EBI_WRTIMING1_WRSETUP_MASK
                          | _EBI_WRTIMING1_WRSTRB_MASK
                          | _EBI_WRTIMING1_WRHOLD_MASK))
                     | writeTiming;
  }
  if (banks & EBI_BANK2) {
    EBI->WRTIMING2 = (EBI->WRTIMING2
                      & ~(_EBI_WRTIMING2_WRSETUP_MASK
                          | _EBI_WRTIMING2_WRSTRB_MASK
                          | _EBI_WRTIMING2_WRHOLD_MASK))
                     | writeTiming;
  }
  if (banks & EBI_BANK3) {
    EBI->WRTIMING3 = (EBI->WRTIMING3
                      & ~(_EBI_WRTIMING3_WRSETUP_MASK
                          | _EBI_WRTIMING3_WRSTRB_MASK
                          | _EBI_WRTIMING3_WRHOLD_MASK))
                     | writeTiming;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure address operation parameters for a selected bank.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure write timing for.
 *
 * @param[in] halfALE
 *   Enables or disables the half cycle ALE strobe in the last strobe cycle.
 ******************************************************************************/
void EBI_BankAddressTimingConfig(uint32_t banks, bool halfALE)
{
  /* Verify only valid banks are used */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  if ( banks & EBI_BANK0 ) {
    BUS_RegBitWrite(&EBI->ADDRTIMING, _EBI_ADDRTIMING_HALFALE_SHIFT, halfALE);
  }
  if ( banks & EBI_BANK1 ) {
    BUS_RegBitWrite(&EBI->ADDRTIMING1, _EBI_ADDRTIMING_HALFALE_SHIFT, halfALE);
  }
  if ( banks & EBI_BANK2 ) {
    BUS_RegBitWrite(&EBI->ADDRTIMING2, _EBI_ADDRTIMING_HALFALE_SHIFT, halfALE);
  }
  if ( banks & EBI_BANK3 ) {
    BUS_RegBitWrite(&EBI->ADDRTIMING3, _EBI_ADDRTIMING_HALFALE_SHIFT, halfALE);
  }
}

/***************************************************************************//**
 * @brief
 *   Configure timing values of address latch bus accesses.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure address timing for.
 *
 * @param[in] setupCycles
 *   Sets the number of cycles the address is held after ALE is asserted.
 *
 * @param[in] holdCycles
 *   Sets the number of cycles the address is driven onto the ADDRDAT bus before
 *   ALE is asserted. If set 0, 1 cycle is inserted by hardware.
 ******************************************************************************/
void EBI_BankAddressTimingSet(uint32_t banks, uint32_t setupCycles, uint32_t holdCycles)
{
  uint32_t addressLatchTiming;

  /* Verify only valid banks are used. */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  /* Check that timings are within limits */
  EFM_ASSERT(setupCycles <= _EBI_ADDRTIMING_ADDRSETUP_MASK >> _EBI_ADDRTIMING_ADDRSETUP_SHIFT);
  EFM_ASSERT(holdCycles <= _EBI_ADDRTIMING_ADDRHOLD_MASK >> _EBI_ADDRTIMING_ADDRHOLD_SHIFT);

  /* Configure address latch timing values. */
  addressLatchTiming = (setupCycles << _EBI_ADDRTIMING_ADDRSETUP_SHIFT)
                       | (holdCycles << _EBI_ADDRTIMING_ADDRHOLD_SHIFT);

  if (banks & EBI_BANK0) {
    EBI->ADDRTIMING = (EBI->ADDRTIMING
                       & ~(_EBI_ADDRTIMING_ADDRSETUP_MASK
                           | _EBI_ADDRTIMING_ADDRHOLD_MASK))
                      | addressLatchTiming;
  }
  if (banks & EBI_BANK1) {
    EBI->ADDRTIMING1 = (EBI->ADDRTIMING1
                        & ~(_EBI_ADDRTIMING1_ADDRSETUP_MASK
                            | _EBI_ADDRTIMING1_ADDRHOLD_MASK))
                       | addressLatchTiming;
  }
  if (banks & EBI_BANK2) {
    EBI->ADDRTIMING2 = (EBI->ADDRTIMING2
                        & ~(_EBI_ADDRTIMING2_ADDRSETUP_MASK
                            | _EBI_ADDRTIMING2_ADDRHOLD_MASK))
                       | addressLatchTiming;
  }
  if (banks & EBI_BANK3) {
    EBI->ADDRTIMING3 = (EBI->ADDRTIMING3
                        & ~(_EBI_ADDRTIMING3_ADDRSETUP_MASK
                            | _EBI_ADDRTIMING3_ADDRHOLD_MASK))
                       | addressLatchTiming;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure EBI pin polarity for selected bank(s) for devices with individual
 *   timing support.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure polarity for.
 *
 * @param[in] line
 *   Which pin/line to configure.
 *
 * @param[in] polarity
 *   Active high, or active low.
 ******************************************************************************/
void EBI_BankPolaritySet(uint32_t banks, EBI_Line_TypeDef line, EBI_Polarity_TypeDef polarity)
{
  uint32_t bankSet = 0;
  volatile uint32_t *polRegister = 0;

  /* Verify that only valid banks are used. */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  while (banks) {
    if (banks & EBI_BANK0) {
      polRegister = &EBI->POLARITY;
      bankSet = EBI_BANK0;
    }
    if (banks & EBI_BANK1) {
      polRegister = &EBI->POLARITY1;
      bankSet = EBI_BANK1;
    }
    if (banks & EBI_BANK2) {
      polRegister = &EBI->POLARITY2;
      bankSet = EBI_BANK2;
    }
    if (banks & EBI_BANK3) {
      polRegister = &EBI->POLARITY3;
      bankSet = EBI_BANK3;
    }

    /* What line to configure. */
    switch (line) {
      case ebiLineARDY:
        BUS_RegBitWrite(polRegister, _EBI_POLARITY_ARDYPOL_SHIFT, polarity);
        break;
      case ebiLineALE:
        BUS_RegBitWrite(polRegister, _EBI_POLARITY_ALEPOL_SHIFT, polarity);
        break;
      case ebiLineWE:
        BUS_RegBitWrite(polRegister, _EBI_POLARITY_WEPOL_SHIFT, polarity);
        break;
      case ebiLineRE:
        BUS_RegBitWrite(polRegister, _EBI_POLARITY_REPOL_SHIFT, polarity);
        break;
      case ebiLineCS:
        BUS_RegBitWrite(polRegister, _EBI_POLARITY_CSPOL_SHIFT, polarity);
        break;
      case ebiLineBL:
        BUS_RegBitWrite(polRegister, _EBI_POLARITY_BLPOL_SHIFT, polarity);
        break;
      case ebiLineTFTVSync:
        BUS_RegBitWrite(&(EBI->TFTPOLARITY), _EBI_TFTPOLARITY_VSYNCPOL_SHIFT, polarity);
        break;
      case ebiLineTFTHSync:
        BUS_RegBitWrite(&(EBI->TFTPOLARITY), _EBI_TFTPOLARITY_HSYNCPOL_SHIFT, polarity);
        break;
      case ebiLineTFTDataEn:
        BUS_RegBitWrite(&(EBI->TFTPOLARITY), _EBI_TFTPOLARITY_DATAENPOL_SHIFT, polarity);
        break;
      case ebiLineTFTDClk:
        BUS_RegBitWrite(&(EBI->TFTPOLARITY), _EBI_TFTPOLARITY_DCLKPOL_SHIFT, polarity);
        break;
      case ebiLineTFTCS:
        BUS_RegBitWrite(&(EBI->TFTPOLARITY), _EBI_TFTPOLARITY_CSPOL_SHIFT, polarity);
        break;
      default:
        EFM_ASSERT(0);
        break;
    }
    banks = banks & ~bankSet;
  }
}

/***************************************************************************//**
 * @brief
 *   Configure Byte Lane Enable for select banks
 *   timing support.
 *
 * @param[in] banks
 *   A mask of memory bank(s) to configure polarity for.
 *
 * @param[in] enable
 *   A flag
 ******************************************************************************/
void EBI_BankByteLaneEnable(uint32_t banks, bool enable)
{
  /* Verify only valid banks are used */
  EFM_ASSERT((banks & ~(EBI_BANK0 | EBI_BANK1 | EBI_BANK2 | EBI_BANK3)) == 0);

  /* Configure byte lane support for each selected bank */
  if (banks & EBI_BANK0) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BL_SHIFT, enable);
  }
  if (banks & EBI_BANK1) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BL1_SHIFT, enable);
  }
  if (banks & EBI_BANK2) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BL2_SHIFT, enable);
  }
  if (banks & EBI_BANK3) {
    BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_BL3_SHIFT, enable);
  }
}

/***************************************************************************//**
 * @brief
 *   Configure the Alternate Address Map support.
 *   Enables or disables 256 MB address range for all banks.
 *
 * @param[in] enable
 *   Set or clear the address map extension.
 ******************************************************************************/
void EBI_AltMapEnable(bool enable)
{
  BUS_RegBitWrite(&(EBI->CTRL), _EBI_CTRL_ALTMAP_SHIFT, enable);
}

#endif

/** @} (end addtogroup EBI) */
/** @} (end addtogroup emlib) */

#endif /* defined(EBI_COUNT) && (EBI_COUNT > 0) */
