   /*****************************************************************************
   * @file    egtk.h                                                            *
   * @author  Nicola Guercetti                                                  *
   * @version 1.00                                                              *
   * @date    29 March 2022                                                     *
   * @brief   header                                                            *
   *****************************************************************************/
#include "egtk_colour.h"

#define EGTK_BUFF_SIZE  500*500         //Dimension of the temp doube buffer
#define COLOUR_AVERAGE                  //When scaling image averages the colour information
#define  egtk_COL_T RGB565_TYPE         //Type of colour


typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
}tGlyph;

typedef struct {
     const uint16_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
}tBitmap;

typedef struct
{
   unsigned char* p;
   uint8_t width;
   uint8_t height;
} egtk_FONT;


   //Initialization
   void egtkInitScreen( uint16_t,    uint16_t,  egtk_COL_T, egtk_COL_T);

   //Geometrical shape
   void egtkDrawLine(int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkPutPixel(int16_t , int16_t , egtk_COL_T );
   void egtkGetPixel(int16_t, int16_t, egtk_COL_T*);
   void egtkDrawRectangle(int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkFillRectangle(int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkDrawCircle(int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkFillCircle(int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkDrawEllipse(int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkFillEllipse(int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkDrawPolygon(int16_t , int16_t *, egtk_COL_T );
   void egtkFillPolygon(int16_t , int16_t *, egtk_COL_T );
   void egtkDrawTriangle(int16_t , int16_t , int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkFillTriangle(int16_t , int16_t , int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkFillRoundedRectangle(int16_t , int16_t , int16_t , int16_t , int16_t , egtk_COL_T );
   void egtkDrawRoundedRectangle(int16_t , int16_t , int16_t , int16_t , int16_t , egtk_COL_T );

   //Utility
   void egtkClearScreen(void);
   uint16_t egtkGetScreenW(void);
   uint16_t egtkGetScreenH(void);

   //Draw a Glyph image
   void egtkDrawGlyph(int16_t , int16_t , egtk_COL_T, egtk_COL_T, const tGlyph *, bool );
   void egtkDrawFitGlyph(int16_t , int16_t , uint16_t , uint16_t , egtk_COL_T , egtk_COL_T , const tGlyph *);

   //Draw a Bitmap image
   void egtkDrawBitmap(int16_t , int16_t , const tBitmap *);
   bool egtkDrawFitBitmap(int16_t , int16_t , uint16_t , uint16_t , const tBitmap *);

   //Write Text
   void egtkPutChar(uint8_t ,bool);
   void egtkWriteString(char * , bool);
   void egtkSetFont(const egtk_FONT *, uint8_t , uint8_t , bool );
   void egtkSetPos(int16_t , int16_t );
   void egtkSetReverse(bool );
   void egtkSetColour(egtk_COL_T , egtk_COL_T );

   #include "egtk_fonts.h"
   #include "egtk_driver.h"

