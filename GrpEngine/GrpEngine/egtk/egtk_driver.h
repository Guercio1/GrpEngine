   /*****************************************************************************
   * @file    egtk_driver.h                                                     *
   * @author  Nicola Guercetti                                                  *
   * @version 1.00                                                              *
   * @date    30 March 2022                                                     *
   * @brief   header                                                            *
   *                                                                            *
   ******************************************************************************
   *                   Copyright (c) 2022 Elcos S.R.L                           *
   *****************************************************************************/
   /*
   Rel 1.00    25/04/2022
               First Emission



   */

void egtkHalInitScreen(uint16_t, uint16_t, egtk_COL_T, egtk_COL_T);
void egtkHalPutPixel(int16_t , int16_t , egtk_COL_T );
void egtkHalGetPixel(int16_t, int16_t, egtk_COL_T*);
void egtkHalDrawVLine(int16_t , int16_t , uint16_t , egtk_COL_T );
void egtkHalDrawHLine(int16_t , int16_t , uint16_t , egtk_COL_T );
void egtkHalFillRectangle(int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
void egtkHalBlitBitmap(int16_t , int16_t ,  uint16_t ,  uint16_t ,egtk_COL_T *, bool);


