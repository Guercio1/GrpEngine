   /*****************************************************************************
   * @file    egtk_colour.c                                                     *
   * @author  Nicola Guercetti                                                  *
   * @version 1.00                                                              *
   * @date    29 March 2022                                                     *
   * @brief   colour                                                            *
   *                                                                            *
   ******************************************************************************
   *                   Copyright (c) 2022 Elcos S.R.L                           *
   *****************************************************************************/
   /*
   Rel 1.00    25/04/2022
               First Emission



   */

#include <stdint.h>
#include "egtk_colour.h"


RGB888_TYPE egtkRGB565_2_RGB888( uint16_t Color )
{
    RGB888_TYPE RetVal;

    RetVal.Red   = ((Color & RGB565_RED)   >> 8);
    RetVal.Green = ((Color & RGB565_GREEN) >> 3);
    RetVal.Blue  = ((Color & RGB565_BLUE)  << 3);

    return RetVal;
}

uint16_t egtkRGB888_2_RGB565(RGB888_TYPE  Color )
{
    uint16_t RetVal;
    uint16_t TmpVal;

    RetVal = 0;

    TmpVal = Color.Red;
    TmpVal <<= 8;
    TmpVal &= RGB565_RED;
    RetVal += TmpVal;

    TmpVal = Color.Green;
    TmpVal <<= 3;
    TmpVal &= RGB565_GREEN;
    RetVal += TmpVal;

    TmpVal = Color.Blue;
    TmpVal >>= 3;
    TmpVal &= RGB565_BLUE;
    RetVal += TmpVal;

    return RetVal;
}
