//
// Pervasive_Wide_Large.cpp
// Class library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 21 Nov 2024
//
// Copyright (c) Rei Vilo, 2010-2024
// Licence All rights reserved
//
// See Pervasive_Wide_Large.h for references
//
// Release 611: Added proxy functions to normal update
// Release 611: Added support for red and yellow colour screens
// Release 614: Improved BWRY examples
// Release 702: Added support for 417-QS-0A
// Release 700: Refactored screen and board functions
// Release 800: Read OTP memory
// Release 801: Improved OTP implementation
// Release 802: Added references to application notes
// Release 802: Refactored CoG functions
// Release 900: Added new driver library
//

// Header
#include "Pervasive_Wide_Large.h"

//
// === COG section
//
//

//
// --- Medium screens with K film
//
void Pervasive_Wide_Large::COG_reset()
{
    b_reset(5, 2, 4, 20, 5); // large
}

void Pervasive_Wide_Large::COG_getDataOTP()
{
    // Read OTP
    uint8_t ui8 = 0;
    uint16_t _readBytes = 0;
    u_flagOTP = false;

    COG_reset();

    _readBytes = 128;
    hV_HAL_SPI_end(); // With unicity check
    hV_HAL_SPI3_begin(); // Define 3-wire SPI pins
    hV_HAL_GPIO_clear(b_pin.panelDC); // Command
    hV_HAL_GPIO_clear(b_pin.panelCS); // Select
    hV_HAL_SPI3_write(0xb9);
    hV_HAL_delayMilliseconds(5); // !!! Use hV_HAL_Peripherals
    // mySerial.println("Command sent...");
    hV_HAL_GPIO_set(b_pin.panelDC); // Data
    ui8 = hV_HAL_SPI3_read(); // Dummy
    // hV_HAL_log(LEVEL_DEBUG, "Dummy read 0x%02x", ui8);
    // mySerial.println(formatString("Dummy read 0x%02x", ui8));
    // Populate COG_data
    for (uint16_t index = 0; index < _readBytes; index += 1)
    {
        COG_data[index] = hV_HAL_SPI3_read(); // Read OTP
        // mySerial.print(index, HEX);
        // mySerial.print("\t");
        // mySerial.println(COG_data[index], HEX);
        // mySerial.println(formatString("%d OTP 0x%02x", index, COG_data[index]));
    }
    // mySerial.println("OTP populated...");
    // End of OTP reading
    hV_HAL_GPIO_set(b_pin.panelCS); // Unselect

    // Check
    uint8_t _chipId = 0x96;

    u_flagOTP = (COG_data[1] == _chipId); // !!! COG Type(0x01): 0x96 -> dual-chip
    if (u_flagOTP == false)
    {
        hV_HAL_Serial_crlf();
        hV_HAL_log(LEVEL_CRITICAL, "OTP check failed - First byte 0x%02x, expected 0x%02x", COG_data[0], _chipId);
        hV_HAL_exit(0x01); // !!! Don't by-pass checks
    }
    hV_HAL_SPI3_end();
}

void Pervasive_Wide_Large::COG_initial()
{
    uint8_t workDCTL[2];
    workDCTL[0] = COG_data[0x10]; // DCTL
    workDCTL[1] = 0x00;
    b_sendIndexDataSelect(0x01, workDCTL, 2, PANEL_CS_BOTH);
}

void Pervasive_Wide_Large::COG_sendImageDataNormal(FRAMEBUFFER_CONST_TYPE masterFrame, FRAMEBUFFER_CONST_TYPE slaveFrame, uint32_t sizeFrame) // First frame, blackBuffer
{
    // Application note § 3.2 Input image to the EPD

    // Send image data
    b_sendIndexDataSelect(0x13, &COG_data[0x15], 6, PANEL_CS_BOTH); // DUW
    b_sendIndexDataSelect(0x90, &COG_data[0x0c], 4, PANEL_CS_BOTH); // DRFW
    b_sendCommandDataSelect8(0x91, 0x12, PANEL_CS_BOTH);

    // Next frame
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_MASTER); // RAM_RW
    b_sendIndexDataSelect(0x10, masterFrame, sizeFrame, PANEL_CS_MASTER); // Next frame

    // Previous frame = dummy
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_MASTER); // RAM_RW
    b_sendIndexFixedSelect(0x11, 0x00, sizeFrame, PANEL_CS_MASTER); // Previous frame = dummy

    // Next frame
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_SLAVE); // RAM_RW
    b_sendIndexDataSelect(0x10, slaveFrame, sizeFrame, PANEL_CS_SLAVE); // Next frame

    // Previous frame = dummy
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_SLAVE); // RAM_RW
    b_sendIndexFixedSelect(0x11, 0x00, sizeFrame, PANEL_CS_SLAVE); // Previous frame = dummy
}

void Pervasive_Wide_Large::COG_sendImageDataFast(FRAMEBUFFER_CONST_TYPE masterframe, FRAMEBUFFER_CONST_TYPE masterframe2, FRAMEBUFFER_CONST_TYPE slaveframe, FRAMEBUFFER_CONST_TYPE slaveframe2, uint32_t sizeFrame) // First frame, blackBuffer
{
    // Application note § 3.2 Input image to the EPD

    // Send image data
    b_sendIndexDataSelect(0x13, &COG_data[0x15], 6, PANEL_CS_BOTH); // DUW
    b_sendIndexDataSelect(0x90, &COG_data[0x0c], 4, PANEL_CS_BOTH); // DRFW
    b_sendCommandDataSelect8(0x91, 0x12, PANEL_CS_BOTH);

    // Next frame
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_MASTER); // RAM_RW
    b_sendIndexDataSelect(0x10, masterframe, sizeFrame, PANEL_CS_MASTER); // Next frame

    // Previous frame = dummy
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_MASTER); // RAM_RW
    b_sendIndexDataSelect(0x11, masterframe2, sizeFrame, PANEL_CS_MASTER); // Next frame

    // Next frame
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_SLAVE); // RAM_RW
    b_sendIndexDataSelect(0x10, slaveframe, sizeFrame, PANEL_CS_SLAVE); // Next frame

    // Previous frame = dummy
    b_sendIndexDataSelect(0x12, &COG_data[0x12], 3, PANEL_CS_SLAVE); // RAM_RW
    b_sendIndexDataSelect(0x11, slaveframe2, sizeFrame, PANEL_CS_SLAVE); // Next frame
}

void Pervasive_Wide_Large::COG_update(uint8_t updateMode)
{
    // Initial COG
    // Application note § 3.1 Initial flow chart
    b_sendCommandDataSelect8(0x05, 0x7d, PANEL_CS_BOTH);
    hV_HAL_delayMilliseconds(50);
    b_sendCommandDataSelect8(0x05, 0x00, PANEL_CS_BOTH);
    hV_HAL_delayMilliseconds(1);
    b_sendCommandDataSelect8(0xd8, COG_data[0x1c], PANEL_CS_BOTH); // MS_SYNC
    b_sendCommandDataSelect8(0xd6, COG_data[0x1d], PANEL_CS_BOTH); // BVSS

    b_sendCommandDataSelect8(0xa7, 0x10, PANEL_CS_BOTH);
    hV_HAL_delayMilliseconds(2);
    b_sendCommandDataSelect8(0xa7, 0x00, PANEL_CS_BOTH);
    hV_HAL_delayMilliseconds(10);

    b_sendCommandDataSelect8(0x44, 0x00, PANEL_CS_MASTER);
    b_sendCommandDataSelect8(0x45, 0x80, PANEL_CS_MASTER);

    b_sendCommandDataSelect8(0xa7, 0x10, PANEL_CS_MASTER);
    hV_HAL_delayMilliseconds(2);
    b_sendCommandDataSelect8(0xa7, 0x00, PANEL_CS_MASTER);
    hV_HAL_delayMilliseconds(10);

    uint8_t indexTemperature;

    switch (updateMode)
    {
        case UPDATE_FAST:

            indexTemperature = (u_temperature + 0x28) + 0x80;
            // indexTemperature = (u_temperature > 50) ? 0xda : indexTemperature;
            // indexTemperature = (u_temperature < 0) ? 0xa8 : indexTemperature;
            indexTemperature = checkRange(indexTemperature, (uint8_t)0xa8, (uint8_t)0xda);
            break;

        case UPDATE_NORMAL:

            indexTemperature = u_temperature + 0x28; // Temperature 0x41@25C
            // indexTemperature = (u_temperature > 60) ? 0x64 : indexTemperature;
            // indexTemperature = (u_temperature < -15) ? 0x19 : indexTemperature;
            indexTemperature = checkRange(indexTemperature, (uint8_t)0x19, (uint8_t)0x64);
            break;

        default:

            break;
    }

    b_sendCommandDataSelect8(0x44, 0x06, PANEL_CS_MASTER);
    b_sendCommandDataSelect8(0x45, indexTemperature, PANEL_CS_MASTER);

    b_sendCommandDataSelect8(0xa7, 0x10, PANEL_CS_MASTER);
    hV_HAL_delayMilliseconds(2);
    b_sendCommandDataSelect8(0xa7, 0x00, PANEL_CS_MASTER);
    hV_HAL_delayMilliseconds(10);

    b_sendCommandDataSelect8(0x44, 0x00, PANEL_CS_SLAVE);
    b_sendCommandDataSelect8(0x45, 0x80, PANEL_CS_SLAVE);

    b_sendCommandDataSelect8(0xa7, 0x10, PANEL_CS_SLAVE);
    hV_HAL_delayMilliseconds(2);
    b_sendCommandDataSelect8(0xa7, 0x00, PANEL_CS_SLAVE);
    hV_HAL_delayMilliseconds(10);

    b_sendCommandDataSelect8(0x44, 0x06, PANEL_CS_SLAVE);
    b_sendCommandDataSelect8(0x45, indexTemperature, PANEL_CS_SLAVE);

    b_sendCommandDataSelect8(0xa7, 0x10, PANEL_CS_SLAVE);
    hV_HAL_delayMilliseconds(2);
    b_sendCommandDataSelect8(0xa7, 0x00, PANEL_CS_SLAVE);
    hV_HAL_delayMilliseconds(10);

    b_sendCommandDataSelect8(0x60, COG_data[0x0b], PANEL_CS_BOTH); // TCON
    b_sendCommandDataSelect8(0x61, COG_data[0x1b], PANEL_CS_BOTH); // STV_DIR
    // No DCTL here
    b_sendCommandDataSelect8(0x02, COG_data[0x11], PANEL_CS_BOTH); // VCOM

    // DC/DC Soft-start
    // Application note § 3.3 DC/DC soft-start
    // DRIVER_B = 0x28, DRIVER_8 = 0x20
    uint8_t offsetFrame = 0x28;

    // Filter for register 0x09
    uint8_t _filter09 = 0xff;

    for (uint8_t stage = 0; stage < 4; stage += 1)
    {
        uint8_t offset = offsetFrame + 0x08 * stage;
        // uint8_t FORMAT = COG_data[offset] & 0x80;
        uint8_t REPEAT = COG_data[offset] & 0x7f;

        uint8_t PHL_PHH[2];
        PHL_PHH[0] = COG_data[offset + 1]; // PHL_INI
        PHL_PHH[1] = COG_data[offset + 2]; // PHH_INI
        uint8_t PHL_VAR = COG_data[offset + 3];
        uint8_t PHH_VAR = COG_data[offset + 4];
        uint8_t BST_SW_a = COG_data[offset + 5] & _filter09;
        uint8_t BST_SW_b = COG_data[offset + 6] & _filter09;
        uint8_t DELAY_SCALE = COG_data[offset + 7] & 0x80;
        uint16_t DELAY_VALUE = COG_data[offset + 7] & 0x7f;

        for (uint8_t i = 0; i < REPEAT; i += 1)
        {
            b_sendCommandDataSelect8(0x09, BST_SW_a, PANEL_CS_BOTH);
            PHL_PHH[0] += PHL_VAR; // PHL
            PHL_PHH[1] += PHH_VAR; // PHH
            b_sendIndexDataSelect(0x51, PHL_PHH, 2, PANEL_CS_BOTH);
            b_sendCommandDataSelect8(0x09, BST_SW_b, PANEL_CS_BOTH);

            hV_HAL_delayMilliseconds(DELAY_VALUE); // ms
        }
    }

    // Display Refresh Start
    // Application note § 4 Send updating command
    b_waitBusy();
    b_sendCommandDataSelect8(0x15, 0x3c, PANEL_CS_BOTH);
}

void Pervasive_Wide_Large::COG_stopDCDC()
{
    // Application note § 7. Turn-off DC/DC
    // Application note § 5. Turn-off DC/DC

    // DC-DC off
    b_waitBusy();

    b_sendCommandDataSelect8(0x09, 0x7f, PANEL_CS_BOTH);
    b_sendCommandDataSelect8(0x05, 0x3d, PANEL_CS_BOTH);
    b_sendCommandDataSelect8(0x09, 0x7e, PANEL_CS_BOTH);
    hV_HAL_delayMilliseconds(60);
    b_sendCommandDataSelect8(0x09, 0x00, PANEL_CS_BOTH);
}
//
// --- End of Small screens with Q film
//
/// @endcond
//
// === End of COG section
//

Pervasive_Wide_Large::Pervasive_Wide_Large(eScreen_EPD_t eScreen_EPD, pins_t board)
{
    d_COG = COG_WIDE_LARGE;
    u_eScreen_EPD = eScreen_EPD;
    b_pin = board;
}

void Pervasive_Wide_Large::begin()
{
    b_begin(b_pin, FAMILY_LARGE, b_delayCS);
    b_resume(); // GPIO
    COG_reset(); // Reset

    COG_getDataOTP(); // 3-wire SPI read OTP memory
}

STRING_CONST_TYPE Pervasive_Wide_Large::reference()
{
    return formatString("%s v%i.%i.%i", DRIVER_EPD_VARIANT, DRIVER_EPD_RELEASE / 100, (DRIVER_EPD_RELEASE / 10) % 10, DRIVER_EPD_RELEASE % 10);
}

void Pervasive_Wide_Large::updateNormal(FRAMEBUFFER_CONST_TYPE masterframe, FRAMEBUFFER_CONST_TYPE slaveframe, uint32_t sizeFrame)
{
    b_resume(); // GPIO
    COG_reset(); // Reset

    // Start SPI
    hV_HAL_SPI_begin(16000000); // Fast 16 MHz, with unicity check

    COG_initial(); // Initialise
    COG_sendImageDataNormal(masterframe, slaveframe, sizeFrame);

    COG_update(UPDATE_NORMAL); // Update
    COG_stopDCDC(); // Power off

    // hV_HAL_SPI_end(); // With unicity check
}

void Pervasive_Wide_Large::updateFast(FRAMEBUFFER_CONST_TYPE masterframe, FRAMEBUFFER_CONST_TYPE masterframe2, FRAMEBUFFER_CONST_TYPE slaveframe, FRAMEBUFFER_CONST_TYPE slaveframe2, uint32_t sizeFrame)
{
    b_resume(); // GPIO
    COG_reset(); // Reset

    // Start SPI
    hV_HAL_SPI_begin(16000000); // Fast 16 MHz, with unicity check

    COG_initial(); // Initialise
    COG_sendImageDataFast(masterframe, masterframe2, slaveframe, slaveframe2, sizeFrame);

    COG_update(UPDATE_FAST); // Update
    COG_stopDCDC(); // Power off

    // hV_HAL_SPI_end(); // With unicity check
}

