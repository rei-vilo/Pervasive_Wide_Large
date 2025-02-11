/*
	demoImageData.h
  DEMO set of Image Data
*/

// Load Global Update Images
#if (SCRN == 970)
#define frameSize	40320
#define FastPic_1_m        (uint8_t *)&Image_970_Masterfm_01
#define FastPic_2_m        (uint8_t *)&Image_970_Masterfm_02
#define FastPic_1_s         (uint8_t *)&Image_970_Slavefm_01
#define FastPic_2_s         (uint8_t *)&Image_970_Slavefm_02
#else
#define frameSize	46080
#define FastPic_1_m        (uint8_t *)&Image_1200_Masterfm_01
#define FastPic_2_m        (uint8_t *)&Image_1200_Masterfm_02
#define FastPic_1_s         (uint8_t *)&Image_1200_Slavefm_01
#define FastPic_2_s         (uint8_t *)&Image_1200_Slavefm_02
#endif