   /*****************************************************************************
   * @file    egtk_driver.c                                                     *
   * @author  Nicola Guercetti                                                  *
   * @version 1.00                                                              *
   * @date    30 March 2022                                                     *
   * @brief   low level driver                                                  *
   *                                                                            *
   ******************************************************************************
   *                   Copyright (c) 2022 Elcos S.R.L                           *
   *****************************************************************************/


/*---------- DVIVER INIT ------------*/
extern void* memory;
extern int client_width;
extern int client_height;
/*----------- DRIVER END ---------------*/


#include <stdint.h>
#include "egtk.h"


/*---------- DVIVER INIT ------------*/
uint32_t GetColorToPrint(egtk_COL_T egtkColor)
{
    RGB888_TYPE RetVal;

    RetVal = egtkRGB565_2_RGB888(egtkColor);

    return ((RetVal.Red<<16)+(RetVal.Green<<8)+RetVal.Blue);
}

egtk_COL_T GetColorToRead(uint32_t inColor)
{
    egtk_COL_T RetVal;
	RGB888_TYPE halkColor;
	
	halkColor.Red = 0x00FF&(inColor>>16);
	halkColor.Green = 0x00FF&(inColor>>8);
	halkColor.Blue = 0x00FF&inColor;

    RetVal = egtkRGB888_2_RGB565(halkColor);

    return RetVal;
}

/*----------- DRIVER END ---------------*/



  /********************************************************************
  * @brief  egtkHalPutPixel
  * @param  ...
  * @retval
  * @brief  Put single pixel
  ********************************************************************/
void egtkHalInitScreen(uint16_t    Width, uint16_t    Height, egtk_COL_T ForeGrColour, egtk_COL_T BackGrColour)
{

    /*---------- DVIVER INIT ------------*/

    client_width = Width;
    client_height = Height;


    /*----------- DRIVER END ---------------*/

}

  /********************************************************************
  * @brief  egtkHalPutPixel
  * @param  ...
  * @retval
  * @brief  Put single pixel
  ********************************************************************/
void egtkHalPutPixel(int16_t x0, int16_t y0, egtk_COL_T Colour)
{

/*---------- DVIVER INIT ------------*/

    uint32_t* pixel = (uint32_t*)memory;
    pixel += (client_height -1-y0) * client_width + x0;
    *pixel = GetColorToPrint(Colour);

/*----------- DRIVER END ---------------*/
}

/********************************************************************
* @brief  egtkHalGettPixel
* @param  ...
* @retval
* @brief  Get single pixel
********************************************************************/
void egtkHalGetPixel(int16_t x0, int16_t y0, egtk_COL_T *Colour)
{

    /*---------- DVIVER INIT ------------*/

    uint32_t* pixel = (uint32_t*)memory;
    pixel += (client_height - 1 - y0) * client_width + x0;
    
    *Colour = GetColorToRead(*pixel);

    /*----------- DRIVER END ---------------*/
}


  /********************************************************************
  * @brief  egtkHalDrawHLine
  * @param  ...
  * @retval
  * @brief
  ********************************************************************/
void egtkHalDrawHLine(int16_t x0, int16_t y0, uint16_t width, egtk_COL_T Colour)
{

/*---------- DVIVER INIT ------------*/
    uint32_t* pixel = (uint32_t*)memory;
    pixel += ((client_height -1-y0) * client_width + x0);

    do
    {
        *pixel = GetColorToPrint(Colour);
        pixel ++;
    }
    while (width--);
/*----------- DRIVER END ---------------*/

}


  /********************************************************************
  * @brief  egtkHalDrawVLine
  * @param  ...
  * @retval
  * @brief
  ********************************************************************/
void egtkHalDrawVLine(int16_t x0, int16_t y0, uint16_t height, egtk_COL_T Colour)
{
/*---------- DVIVER INIT ------------*/

    uint32_t* pixel = (uint32_t*)memory;
    pixel += (client_height -y0-1) * client_width + x0;

    do
    {
        *pixel = GetColorToPrint(Colour);
        pixel -= client_width;
    } while (height--);

/*----------- DRIVER END ---------------*/

}


  /********************************************************************
  * @brief  egtkHalFillRectangle
  * @param  ...
  * @retval
  * @brief
  ********************************************************************/
void egtkHalFillRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, egtk_COL_T Colour)
{

/*---------- DVIVER INIT ------------*/


    uint16_t y;

    for(y=y0;y<=y1;y++)
        egtkHalDrawHLine(x0, y, (x1 - x0), Colour);



/*----------- DRIVER END ---------------*/
}

  /************************
  * @brief  egtkHalBlitBitmap
  * @param  ...
  * @retval
  * @brief
  ********************************************************************/
void egtkHalBlitBitmap(int16_t x0, int16_t y0, uint16_t width, uint16_t height, egtk_COL_T *pBitmap, bool ToScreen)
{
/*---------- DVIVER INIT ------------*/
        egtk_COL_T *ptr = pBitmap;
        uint32_t* pixel = (uint32_t*)memory;

        for (uint16_t y = 0; y < height; y++) {
            pixel = (uint32_t*)memory + (client_height-1-y0-y) * client_width + x0;
            for (uint16_t x = 0; x < width; x++) {
                
				if(ToScreen)
					*pixel =  GetColorToPrint(*ptr);
				else
                    (*ptr) = GetColorToRead(*pixel);

				ptr++;
                pixel++;
            }
        }

/*----------- DRIVER END ---------------*/
}





