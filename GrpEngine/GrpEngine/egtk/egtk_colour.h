   /*****************************************************************************
   * @file    egtk_colour.h                                                     *
   * @author  Nicola Guercetti                                                  *
   * @version 1.00                                                              *
   * @date    29 March 2022                                                     *
   * @brief   include                                                           *
   *                                                                            *
   ******************************************************************************
   *                   Copyright (c) 2022 Elcos S.R.L                           *
   *****************************************************************************/


typedef struct {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
}RGB888_TYPE;

#define  RGB565_TYPE uint16_t

RGB888_TYPE egtkRGB565_2_RGB888(RGB565_TYPE);
RGB565_TYPE egtkRGB888_2_RGB565(RGB888_TYPE);
//         colours

#define RGB565_RED          0xF800
#define RGB565_GREEN        0x07E0
#define RGB565_GREEN_5      0x07C0
#define RGB565_BLUE         0x001F

#define RGB565_WHITE        0xFFFF
#define RGB565_BLACK        0x0000

#define RGB565_YELLOW       0xFFE0
#define RGB565_CYAN         0x07FF
#define RGB565_MAGENTA      0xF81F


#define RGB565_GREY_50      (RGB565_RED/2+RGB565_GREEN_5/2+RGB565_BLUE/2)


