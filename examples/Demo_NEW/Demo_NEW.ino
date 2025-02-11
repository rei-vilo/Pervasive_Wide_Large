#include "Pervasive_Wide_Large.h"

// DEMO Image Set <Comment out when in User Mode>
// Screen Size: 970, 1200
#define SCRN 970
#include "globalupdate_src/demoImageData.h"
#include "fastupdate_src/fastImageData.h"

Pervasive_Wide_Large myDriver(eScreen_EPD_969_KS_06, boardRaspberryPiPico_RP2040);

void setup()
{
    hV_HAL_begin();

    myDriver.begin();

    myDriver.updateNormal(Masterfm1, Slavefm1, frameSize);

    myDriver.updateFast(FastPic_2_m, FastPic_1_m, FastPic_2_s, FastPic_1_s, frameSize);
    myDriver.updateFast(FastPic_1_m, FastPic_2_m, FastPic_1_s, FastPic_2_s, frameSize);
    myDriver.updateFast(FastPic_2_m, FastPic_1_m, FastPic_2_s, FastPic_1_s, frameSize);
    myDriver.updateFast(FastPic_1_m, FastPic_2_m, FastPic_1_s, FastPic_2_s, frameSize);
    myDriver.updateFast(FastPic_2_m, FastPic_1_m, FastPic_2_s, FastPic_1_s, frameSize);
    myDriver.updateFast(FastPic_1_m, FastPic_2_m, FastPic_1_s, FastPic_2_s, frameSize);
    myDriver.updateFast(FastPic_2_m, FastPic_1_m, FastPic_2_s, FastPic_1_s, frameSize);
    myDriver.updateFast(FastPic_1_m, FastPic_2_m, FastPic_1_s, FastPic_2_s, frameSize);

    hV_HAL_exit();
}

void loop()
{
    hV_HAL_delayMilliseconds(1000);
}
