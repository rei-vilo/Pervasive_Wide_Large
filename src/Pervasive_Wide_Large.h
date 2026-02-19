///
/// @file Pervasive_Wide_Large.h
/// @brief Driver for medium screens
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
/// * ApplicationNote_E5_SE_smallSize_EPD_v01_20230522
///
/// @date 21 Nov 2024
/// @version 900
///
/// @copyright (c) Pervasive Displays Inc., 2021-2026
/// @copyright All rights reserved
/// @copyright For exclusive use with Pervasive Displays screens
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
/// @see https://creativecommons.org/licenses/by-sa/4.0/
///
/// @n Consider the Evaluation or Commercial editions for professionals or organisations and for commercial usage
///
/// * Evaluation edition: for professionals or organisations, evaluation only, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///
/// * Viewer edition: for professionals or organisations
/// @n All rights reserved
///
/// * Documentation
/// @n All rights reserved
///

// SDK and configuration
#include "PDLS_Common.h"

#if (PDLS_COMMON_RELEASE < 902)
#error Required PDLS_COMMON_RELEASE 902
#endif // PDLS_COMMON_RELEASE

// Driver
#include "Driver_EPD_Virtual.h"

#ifndef PERVASIVE_WIDE_LARGE_RELEASE
///
/// @brief Library release number
///
#define PERVASIVE_WIDE_LARGE_RELEASE 902

///
/// @name List of supported screens
/// @details Wide temperature and embedded fast update
///
/// @note Normal and fast update
/// @see https://www.pervasivedisplays.com/products-e-ink-display/?_sft_etc_itc=pu%20itc&_sft_temperature_range=m15c-to-p60c
/// @{
///
#define eScreen_EPD_969_KS_06 SCREEN(SIZE_969, FILM_K, DRIVER_6)
#define eScreen_EPD_1198_KS_06 SCREEN(SIZE_1198, FILM_K, DRIVER_6) ///< 

/// @}

///
/// @name List of features
/// @{
///
#define WITH_MONOCHROME ///< With B&W only
#define WITH_FAST ///< With fast update capability
/// @}

///
/// @brief Driver variant
///
#define DRIVER_EPD_RELEASE PERVASIVE_WIDE_LARGE_RELEASE
#define DRIVER_EPD_VARIANT "Wide large"

///
/// @brief medium screens class
///
class Pervasive_Wide_Large : public Driver_EPD_Virtual
{
  public:
    ///
    /// @brief Constructor
    /// @param eScreen_EPD
    /// @param board
    ///
    Pervasive_Wide_Large(eScreen_EPD_t eScreen_EPD, pins_t board);

    /// @name General
    /// @{

    ///
    /// @brief Initialisation
    /// @details Initialise the board and read OTP
    ///
    void begin();

    ///
    /// @brief Driver reference
    ///
    /// @return STRING_CONST_TYPE scope and release number
    /// @note Example `medium v9.0.0`
    ///
    STRING_CONST_TYPE reference();

    ///
    /// @brief Normal update
    ///
    /// @param masterframe master half of next image
    /// @param slaveframe slave half of next image
    /// @param sizeFrame size of the frame
    ///
    void updateNormal(FRAMEBUFFER_CONST_TYPE masterframe, FRAMEBUFFER_CONST_TYPE slaveframe, uint32_t sizeFrame);

    ///
    /// @brief Fast update
    ///
    /// @param masterframe master half of next image
    /// @param masterframe2 master half of old image
    /// @param slaveframe slave half of next image
    /// @param slaveframe2 slave half of old image
    /// @param sizeFrame size of the frame
    ///
    void updateFast(FRAMEBUFFER_CONST_TYPE masterframe, FRAMEBUFFER_CONST_TYPE masterframe2, FRAMEBUFFER_CONST_TYPE slaveframe, FRAMEBUFFER_CONST_TYPE slaveframe2, uint32_t sizeFrame);

    /// @}

  protected:

  private:

    // Variables and functions specific to the screen
    uint8_t COG_data[128]; // OTP

    void COG_reset();
    void COG_getDataOTP();
    void COG_initial();
    void COG_sendImageDataNormal(FRAMEBUFFER_CONST_TYPE masterFrame, FRAMEBUFFER_CONST_TYPE slaveFrame, uint32_t sizeFrame);
    void COG_sendImageDataFast(FRAMEBUFFER_CONST_TYPE masterframe, FRAMEBUFFER_CONST_TYPE masterframe2, FRAMEBUFFER_CONST_TYPE slaveframe, FRAMEBUFFER_CONST_TYPE slaveframe2, uint32_t sizeFrame);
    void COG_update(uint8_t updateMode);
    void COG_stopDCDC();
};

#endif // DRIVER_MEDIUM
