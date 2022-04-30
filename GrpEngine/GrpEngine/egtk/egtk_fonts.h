/*****************************************************************************
* @file    egtk_fontsr.h                                                     *
* @author  Nicola Guercetti                                                  *
* @version 1.00                                                              *
* @date    25 April 2022                                                     *
* @brief   fonts                                                             *
*                                                                            *
******************************************************************************
*                   Copyright (c) 2022 Elcos S.R.L                           *
*****************************************************************************/


/*
Rel 1.00    25/04/2022
            First Emission



*/



#define  USE_FONT_8X14 1
#define USE_FONT_32X53 1
#define USE_FONT_10X16 1

unsigned char reverse_byte(unsigned char );

#ifdef USE_FONT_4X6
    extern const unsigned char font_4x6[256][6]; 
#endif

#ifdef USE_FONT_5X8
    extern const unsigned char font_5x8[256][8];
#endif

#ifdef USE_FONT_5X12
    extern const unsigned char font_5x12[256][12];
#endif

#ifdef USE_FONT_6X8
    extern const unsigned char font_6x8[256][8];
#endif

#ifdef USE_FONT_6X10
    extern const unsigned char font_6x10[256][10];
#endif

#ifdef USE_FONT_7X12
    extern const unsigned char font_7x12[256][12];
#endif

#ifdef USE_FONT_8X8
    extern const unsigned char font_8x8[256][8];
#endif


#ifdef USE_FONT_8X12
    extern const unsigned char font_8x12[256][12];
#endif

#ifdef USE_FONT_8X14
    extern const unsigned char font_8x14[256][14];
#endif

#ifdef USE_FONT_10X16
    extern const unsigned char font_10x16[256][32];
#endif

#ifdef USE_FONT_12X16
    extern const unsigned char font_12x16[256][32];
#endif


#ifdef USE_FONT_12X20
    extern const unsigned char font_12x20[256][40];
#endif

#ifdef USE_FONT_16X26
extern const unsigned char font_16x26[256][52];
#endif

#ifdef USE_FONT_22X36
    extern const unsigned char font_22x36[256][108];
#endif

#ifdef USE_FONT_24X40
    extern const unsigned char font_24x40[256][120];
#endif

#ifdef USE_FONT_32X53
    extern const unsigned char font_32x53[256][212];
#endif


#ifdef USE_FONT_4X6
    extern const egtk_FONT FONT_4X6 ;
#endif
#ifdef USE_FONT_5X8
    extern const egtk_FONT FONT_5X8 ;
#endif
#ifdef USE_FONT_5X12
    extern const egtk_FONT FONT_5X12;
#endif
#ifdef USE_FONT_6X8
    extern const egtk_FONT FONT_6X8 ;
#endif
#ifdef USE_FONT_6X10
    extern const egtk_FONT FONT_6X10 ;
#endif
#ifdef USE_FONT_7X12
    extern const egtk_FONT FONT_7X12;
#endif
#ifdef USE_FONT_8X8
    extern const egtk_FONT FONT_8X8 ;
#endif
#ifdef USE_FONT_8X12
    extern const egtk_FONT FONT_8X12;
#endif
#ifdef USE_FONT_8X14
    extern const egtk_FONT FONT_8X14 ;
#endif
#ifdef USE_FONT_10X16
    extern const egtk_FONT FONT_10X16;
#endif
#ifdef USE_FONT_12X16
    extern const egtk_FONT FONT_12X16;
#endif
#ifdef USE_FONT_12X20
    extern const egtk_FONT FONT_12X20;
#endif
#ifdef USE_FONT_16X26
    extern const egtk_FONT FONT_16X26;
#endif
#ifdef USE_FONT_22X36
    extern const egtk_FONT FONT_22X36;
#endif
#ifdef USE_FONT_24X40
    extern const egtk_FONT FONT_24X40;
#endif
#ifdef USE_FONT_32X53
    extern const egtk_FONT FONT_32X53;
#endif



