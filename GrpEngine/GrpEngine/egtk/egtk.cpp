   /*****************************************************************************
   * @file    egtk.c                                                            *
   * @author  Nicola Guercetti                                                  *
   * @version 1.02                                                              *
   * @date    29 Apr 2022                                                       *
   * @brief   Elcos Graphik ToolKit                                             *
   ******************************************************************************
   *                   Copyright (c) 2022 Elcos S.R.L                           *
   *****************************************************************************/


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "egtk.h"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
    uint16_t   Width;
    uint16_t   Height;
    uint8_t    *pBuffer;
}egtk_GLYPH_T;

typedef struct {
    uint16_t    Width;
    uint16_t    Height;
    egtk_COL_T  *pBuffer;
}egtk_BITMAP_T;

typedef struct{
    uint16_t x;
    uint16_t y;
}egtk_POS_T;

typedef struct {
    uint8_t     Reverse:1;
    uint8_t     LineFistChr:1;
    uint8_t     ColFistChr:1;

    }egtk_FONT_STS;

typedef struct {
    uint16_t        Width;
    uint16_t        Height;
    int16_t         MaxX;
    int16_t         MaxY;
    int16_t         MinX;
    int16_t         MinY;
    egtk_POS_T      Pos;
    egtk_POS_T      PosIni;
    egtk_COL_T      ForeGrColour;
    egtk_COL_T      BackGrColour;
    const egtk_FONT* pFont;
    uint8_t         FontSpaceX;
    uint8_t         FontSpaceY;
    egtk_FONT_STS   FontFlags;

}egtk_SCREEN_T;




/*-------------------------------------------------------------------------------------------------*/
/*                               PRIVATE VARIABLES                                                 */
/*-------------------------------------------------------------------------------------------------*/
static egtk_SCREEN_T Screen;
static egtk_COL_T BuffImage[2][EGTK_BUFF_SIZE];
/*-------------------------------------------------------------------------------------------------*/
/*                               PRIVATE FUNCTIONS                                                 */
/*-------------------------------------------------------------------------------------------------*/
bool egtkGlyph2Bitmap(egtk_COL_T , egtk_COL_T , egtk_GLYPH_T *, egtk_BITMAP_T *,bool);
void egtkFitBitmap(uint16_t , uint16_t , egtk_BITMAP_T *, egtk_BITMAP_T *);




  /********************************************************************
  * @brief  egtkInitScreen
  * @param  ...
  * @retval void
  ********************************************************************/
void egtkInitScreen( uint16_t    Width,    uint16_t    Height,  egtk_COL_T ForeGrColour, egtk_COL_T BackGrColour)
{
    Screen.Width=Width;
    Screen.Height=Height;
    Screen.MaxX=Width-1;
    Screen.MaxY=Height-1;
    Screen.MinX=0;
    Screen.MinY=0;
    Screen.ForeGrColour=ForeGrColour;
    Screen.BackGrColour=BackGrColour;
    Screen.pFont = &FONT_8X14;


    egtkHalInitScreen(Width, Height, ForeGrColour, BackGrColour);


}


  /********************************************************************
  * @brief  egtkDrawLine
  * @param  ...
  * @retval void
  * @brief Draw a line using Bresenham's algorithm.
  *
  *          Clip Line:
  *          https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
  ********************************************************************/

typedef enum {
    INSIDE = 0,
    LEFT = 0x01,
    RIGHT = 0x01<<1,
    BOTTOM = 0x01<<2,
    TOP = 0x01<<3
}CODE_T;

static CODE_T code(int16_t x0, int16_t y0)
{
    CODE_T CodeRet = INSIDE;

    if (x0 < Screen.MinX )
        CodeRet  = LEFT;
    else if (x0 > Screen.MaxX)
        CodeRet  = RIGHT;
    
    if (y0 < Screen.MinY )
        CodeRet  = BOTTOM;
    else if (y0 > Screen.MaxY)
        CodeRet  = TOP;


    return CodeRet ;
}

static bool clip_line(int16_t *x0, int16_t *y0, int16_t *x1, int16_t *y1)
{
    CODE_T code0 = code(*x0, *y0);
    CODE_T code1 = code(*x1, *y1);

    bool accept = false;

    while (true) {
        if (!(code0 | code1)) {
            /* Both endpoints inside clipping window, trivial accept. */
            accept = true;
            break;
        } else if (code0 & code1) {
            /* Both endpoints outside clipping window, trivial reject. */
            break;
        } else {
            /* Part of line inside clipping window, nontrivial situation. */

            int16_t x = 0;
            int16_t y = 0;
            uint8_t code3 = code0 ? code0 : code1;

            /* Find intersection point. */
            /* slope = (y1 - y0) / (x1 - x0) */
            /* x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax */
            /* y = y0 + slope * (xm - x0), where xm is xmin or xmax */
            if (code3 & TOP) {
                x = *x0 + (*x1 - *x0) * (Screen.MaxY - *y0) / (*y1 - *y0);
                y = Screen.MaxY;
            } else if (code3 & BOTTOM) {
                x = *x0 + (*x1 - *x0) * (Screen.MaxY - *y0) / (*y1 - *y0);
                y = Screen.MinY;
            } else if (code3 & RIGHT) {
                y = *y0 + (*y1 - *y0) * (Screen.MaxX - *x0) / (*x1 - *x0);
                x = Screen.MaxX;
            }  else if (code3 & LEFT) {
                y = *y0 + (*y1 - *y0) * (Screen.MinX - *x0) / (*x1 - *x0);
                x = Screen.MinX;
            }

            /* Replace the outside point with the intersection point. */
            if (code3 == code0) {
                *x0 = x;
                *y0 = y;
                code0 = code(*x0, *y0);
            } else {
                *x1 = x;
                *y1 = y;
                code1 = code(*x1, *y1);
            }
        }
    }

    return accept;
}


void egtkDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, egtk_COL_T Colour)
{
    /* Clip coordinates to fit clip window. */
    if (false == clip_line(&x0, &y0, &x1, &y1)) {
        return;
    }

    if (y0 == y1)
        egtkHalDrawHLine(MIN(x1, x0), y0, abs(x1 - x0), Colour);

    else if (x0 == x1)
        egtkHalDrawVLine(x0, MIN(y1, y0), abs(y1 - y0), Colour);
        
    else
    {
        int16_t dx;
        int16_t sx;
        int16_t dy;
        int16_t sy;
        int16_t err;
        int16_t e2;

        dx = abs(x1 - x0);
        sx = x0 < x1 ? 1 : -1;
        dy = abs(y1 - y0);
        sy = y0 < y1 ? 1 : -1;
        err = (dx > dy ? dx : -dy) / 2;

        while (1) {
            egtkPutPixel(x0, y0, Colour);

            if (x0 == x1 && y0 == y1) {
                break;
            };

            e2 = err + err;

            if (e2 > -dx) {
                err -= dy;
                x0 += sx;
            }

            if (e2 < dy) {
                err += dx;
                y0 += sy;
            }
        }
    }
}
  /********************************************************************
  * @brief  egtkPutPixel
  * @param
  * @retval
  * @brief  Put single pixel
  ********************************************************************/
void egtkPutPixel(int16_t x0, int16_t y0, egtk_COL_T color)
{
     /* x0 or y0 is before the edge, nothing to do. */
    if ((x0 < Screen.MinX) || (y0 < Screen.MinY))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > Screen.MaxX) || (y0 > Screen.MaxY)) {
        return;
    }

    /* If still in bounds set the pixel. */
    egtkHalPutPixel(x0, y0, color);
}
/********************************************************************
* @brief  egtkGetPixel
* @param
* @retval
* @brief  Get single pixel
********************************************************************/
void egtkGetPixel(int16_t x0, int16_t y0, egtk_COL_T *color)
{
    /* x0 or y0 is before the edge, nothing to do. */
    if ((x0 < Screen.MinX) || (y0 < Screen.MinY)) {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > Screen.MaxX) || (y0 > Screen.MaxY)) {
        return;
    }

    /* If still in bounds set the pixel. */
    egtkHalGetPixel(x0, y0, color);
}

  /********************************************************************
  * @brief  egtkDrawRectangle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, egtk_COL_T color)
{
    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < Screen.MinX) || (y1 < Screen.MinY))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > Screen.MaxX) || (y0 > Screen.MaxY)) {
        return;
    }

    uint16_t width = x1 - x0;
    uint16_t height = y1 - y0;

    egtkHalDrawHLine(x0, y0, width, color);
    egtkHalDrawHLine(x0, y1, width, color);
    egtkHalDrawVLine(x0, y0, height, color);
    egtkHalDrawVLine(x1, y0, height, color);
}

  /********************************************************************
  * @brief  egtkFillRectangle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkFillRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, egtk_COL_T color)
{
    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < Screen.MinX) || (y1 < Screen.MinX))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > Screen.MaxX) || (y0 > Screen.MaxY)) {
        return;
    }

    if(x0 < Screen.MinX )
        x0 = Screen.MinX;

    if( y0 <Screen.MinY )
        y0 = Screen.MinY;

    if(x1 > Screen.MaxX )
        x1 = Screen.MaxX;

    if( y1 > Screen.MaxY )
        y1 = Screen.MaxY;

    egtkHalFillRectangle( x0,  y0,  x1,  y1,  color);
}

  /********************************************************************
  * @brief  egtkDrawCircle
  * @param
  * @retval
  * @brief
  ********************************************************************/

void egtkDrawCircle(int16_t xc, int16_t yc, int16_t r, egtk_COL_T color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    egtkPutPixel(xc + x, yc + y, color);
    egtkPutPixel(xc + x, yc - y, color);
    egtkPutPixel(xc + y, yc + x, color);
    egtkPutPixel(xc + y, yc - x, color);

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        egtkPutPixel(xc + x, yc + y, color);
        egtkPutPixel(xc - x, yc + y, color);
        egtkPutPixel(xc + x, yc - y, color);
        egtkPutPixel(xc - x, yc - y, color);
        egtkPutPixel(xc + y, yc + x, color);
        egtkPutPixel(xc - y, yc + x, color);
        egtkPutPixel(xc + y, yc - x, color);
        egtkPutPixel(xc - y, yc - x, color);
    }
}


  /********************************************************************
  * @brief  egtkFillCircle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkFillCircle(int16_t x0, int16_t y0, int16_t r, egtk_COL_T color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x) {
        egtkDrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        egtkDrawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
        egtkDrawLine(x0 - y, y0 + x, x0 + y, y0 + x,color);
        egtkDrawLine(x0 - y, y0 - x, x0 + y, y0 - x,color);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

  /********************************************************************
  * @brief  egtkDrawEllipse
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawEllipse(int16_t x0, int16_t y0, int16_t a, int16_t b, egtk_COL_T color)
{
    int16_t wx, wy;
    int32_t xa, ya;
    int32_t t;
    int32_t asq = a * a;
    int32_t bsq = b * b;

    egtkPutPixel(x0, y0 + b, color);
    egtkPutPixel(x0, y0 - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    t = asq / 4 - asq * b;

    while (1) {
        t += xa + bsq;

        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya) {
            break;
        }

        egtkPutPixel(x0 + wx, y0 - wy, color);
        egtkPutPixel(x0 - wx, y0 - wy, color);
        egtkPutPixel(x0 + wx, y0 + wy, color);
        egtkPutPixel(x0 - wx, y0 + wy, color);
    }

    egtkPutPixel(x0 + a, y0, color);
    egtkPutPixel(x0 - a, y0, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    t = bsq / 4 - bsq * a;

    while (1) {
        t += ya + asq;

        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa) {
            break;
        }

        egtkPutPixel(x0 + wx, y0 - wy, color);
        egtkPutPixel(x0 - wx, y0 - wy, color);
        egtkPutPixel(x0 + wx, y0 + wy, color);
        egtkPutPixel(x0 - wx, y0 + wy, color);
    }
}

  /********************************************************************
  * @brief  egtkFillEllipse
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkFillEllipse(int16_t x0, int16_t y0, int16_t a, int16_t b, egtk_COL_T color)
{
    int16_t wx, wy;
    int32_t xa, ya;
    int32_t t;
    int32_t asq = a * a;
    int32_t bsq = b * b;

    egtkPutPixel(x0, y0 + b, color);
    egtkPutPixel(x0, y0 - b, color);

    wx = 0;
    wy = b;
    xa = 0;
    ya = asq * 2 * b;
    t = asq / 4 - asq * b;

    while (1) {
        t += xa + bsq;

        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }

        xa += bsq * 2;
        wx++;

        if (xa >= ya) {
            break;
        }

        egtkDrawLine(x0 - wx, y0 - wy, x0 + wx, y0 - wy, color);
        egtkDrawLine(x0 - wx, y0 + wy, x0 + wx, y0 + wy, color);
    }

    egtkDrawLine(x0 - a, y0, x0 + a, y0, color);

    wx = a;
    wy = 0;
    xa = bsq * 2 * a;

    ya = 0;
    t = bsq / 4 - bsq * a;

    while (1) {
        t += ya + asq;

        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }

        ya += asq * 2;
        wy++;

        if (ya > xa) {
            break;
        }

        egtkDrawLine(x0 - wx, y0 - wy, x0 + wx, y0 - wy, color);
        egtkDrawLine(x0 - wx, y0 + wy, x0 + wx, y0 + wy, color);
    }
}

  /********************************************************************
  * @brief  egtkDrawPolygon
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawPolygon(int16_t amount, int16_t *vertices, egtk_COL_T color)
{

    for(int16_t i = 0; i < amount - 1; i++) {
        egtkDrawLine(
            vertices[(i << 1 ) + 0],
            vertices[(i << 1 ) + 1],
            vertices[(i << 1 ) + 2],
            vertices[(i << 1 ) + 3],
            color
        );
    }
    egtkDrawLine(
        vertices[0],
        vertices[1],
        vertices[(amount <<1 ) - 2],
        vertices[(amount <<1 ) - 1],
        color
    );
}

  /********************************************************************
  * @brief  egtkFillPolygon
  * @param
  * @retval
  * @brief  http://alienryderflex.com/polygon_fill/
  ********************************************************************/
void egtkFillPolygon(int16_t amount, int16_t *vertices, egtk_COL_T color)
{
    uint16_t nodes[64];
    int16_t y;

    float x0;
    float y0;
    float x1;
    float y1;

    int16_t miny = Screen.Height;
    int16_t maxy = 0;

    for (uint8_t i = 0; i < amount; i++) {
        if (miny > vertices[(i << 1) + 1]) {
            miny = vertices[(i << 1) + 1];
        }
        if (maxy < vertices[(i << 1) + 1]) {
            maxy = vertices[(i << 1) + 1];
        }
    }

    /*  Loop through the rows of the image. */
    for (y = miny; y <= maxy; y++) {

        /*  Build a list of nodes. */
        int16_t count = 0;
        int16_t j = amount - 1;

        for (int16_t i = 0; i < amount; i++) {
            x0 = vertices[(i << 1) + 0];
            y0 = vertices[(i << 1) + 1];
            x1 = vertices[(j << 1) + 0];
            y1 = vertices[(j << 1) + 1];

            if (
                (y0 < (float)y && y1 >= (float)y) ||
                (y1 < (float)y && y0 >= (float)y)
            ) {
                nodes[count] = (int16_t)(x0 + (y - y0) / (y1 - y0) * (x1 - x0));
                count++;
            }
            j = i;
        }

        /* Sort the nodes, via a simple “Bubble” sort. */
        int16_t i = 0;
        while (i < count - 1) {
            if (nodes[i] > nodes[i + 1]) {
                int16_t swap = nodes[i];
                nodes[i] = nodes[i + 1];
                nodes[i + 1] = swap;
                if (i) {
                    i--;
                }
            } else {
                i++;
            }
        }

        /* Draw lines between nodes. */
        for (int16_t i = 0; i < count; i += 2) {
            //int16_t width = nodes[i + 1] - nodes[i];
            egtkDrawLine(nodes[i], y, nodes[i + 1], y, color);
        }
    }
}

  /********************************************************************
  * @brief  egtkDrawTriangle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, egtk_COL_T color)
{
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    egtkDrawPolygon(3, vertices, color);
};

  /********************************************************************
  * @brief  egtkFillTriangle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, egtk_COL_T color)
{
    int16_t vertices[6] = {x0, y0, x1, y1, x2, y2};
    egtkFillPolygon(3, vertices, color);
}


  /********************************************************************
  * @brief  egtkDrawRoundedRectangle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawRoundedRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, egtk_COL_T color)
{
    int16_t x, y, d, r1, r2;

    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < Screen.MinX) || (y1 < Screen.MinY))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > Screen.MaxX) || (y0 > Screen.MaxY)) {
        return;
    }

    /* Max radius is half of shortest edge. */
    r1 = (x1 - x0 + 1)/2;
    r2 = (y1 - y0 + 1)/2;

    if(r1<r)
        r=r1;
    if(r2<r)
        r=r2;

    egtkDrawLine(x0 + r, y0, x1 - r, y0, color);
    egtkDrawLine(x0 + r, y1, x1 - r, y1, color);
    egtkDrawLine(x0, y0 + r, x0, y1 - r, color);
    egtkDrawLine(x1, y0 + r, x1, y1 - r, color);

    x = 0;
    y = r;
    d = 3 - 2 * r;

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        /* Top right */
        egtkPutPixel(x1 - r + x, y0 + r - y, color);
        egtkPutPixel(x1 - r + y, y0 + r - x, color);

        /* Top left */
        egtkPutPixel(x0 + r - x, y0 + r - y, color);
        egtkPutPixel(x0 + r - y, y0 + r - x, color);

        /* Bottom right */
        egtkPutPixel(x1 - r + x, y1 - r + y, color);
        egtkPutPixel(x1 - r + y, y1 - r + x, color);

        /* Bottom left */
        egtkPutPixel(x0 + r - x, y1 - r + y, color);
        egtkPutPixel(x0 + r - y, y1 - r + x, color);
    }
};

  /********************************************************************
  * @brief  egtkFillRoundedRectangle
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkFillRoundedRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, egtk_COL_T color)
{


    int16_t rx0, ry0, rx1, x, y, d, r1, r2;

    /* Make sure x0 is smaller than x1. */
    if (x0 > x1) {
        x0 = x0 + x1;
        x1 = x0 - x1;
        x0 = x0 - x1;
    }

    /* Make sure y0 is smaller than y1. */
    if (y0 > y1) {
        y0 = y0 + y1;
        y1 = y0 - y1;
        y0 = y0 - y1;
    }

    /* x1 or y1 is before the edge, nothing to do. */
    if ((x1 < Screen.MinX) || (y1 < Screen.MinY))  {
        return;
    }

    /* x0 or y0 is after the edge, nothing to do. */
    if ((x0 > Screen.MaxX) || (y0 > Screen.MaxY)) {
        return;
    }




    /* Max radius is half of shortest edge. */
    r1 = (x1 - x0 + 1)/2;
    r2 = (y1 - y0 + 1)/2;

    if(r1<r)
        r=r1;

    if(r2<r)
        r=r2;

    x = 0;
    y = r;
    d = 3 - 2 * r;

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        /* Top  */
        ry0 = y0 + r - x;
        rx0 = x0 + r - y;
        rx1 = x1 - r + y;
        egtkDrawLine(rx0, ry0, rx1, ry0, color);

        ry0 = y0 + r - y;
        rx0 = x0 + r - x;
        rx1 = x1 - r + x;
        egtkDrawLine(rx0, ry0, rx1, ry0, color);

        /* Bottom */
        ry0 = y1 - r + y;
        rx0 = x0 + r - x;
        rx1 = x1 - r + x;
        egtkDrawLine(rx0, ry0, rx1, ry0, color);

        ry0 = y1 - r + x;
        rx0 = x0 + r - y;
        rx1 = x1 - r + y;
        egtkDrawLine(rx0, ry0, rx1, ry0, color);
    }

    /* Center */
    egtkFillRectangle(x0, y0 + r, x1, y1 - r, color);
};

  /********************************************************************
  * @brief  egtkClearScreen
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkClearScreen(void)
{
    egtkFillRectangle(Screen.MinX,Screen.MinY,Screen.MaxX,Screen.MaxY,Screen.BackGrColour);
}



/********************************************************************
  * @brief  egtkGetScreenW
  * @param
  * @retval
  * @brief
  ********************************************************************/
uint16_t egtkGetScreenW(void)
{
    return Screen.Width;
}

/********************************************************************
  * @brief  egtkGetScreenH
  * @param
  * @retval
  * @brief
  ********************************************************************/
uint16_t egtkGetScreenH(void)
{
    return Screen.Height;
}

/********************************************************************
  * @brief  egtGetkForeColour
  * @param
  * @retval
  * @brief
  ********************************************************************/
uint16_t egtkGetForeColour(void)
{
    return Screen.ForeGrColour;
}
/********************************************************************
  * @brief  egtkGetBackColour
  * @param
  * @retval
  * @brief
  ********************************************************************/
uint16_t egtkGetBackColour(void)
{
    return Screen.BackGrColour;
}



  /********************************************************************
  * @brief  egtkGlyph2Bitmap
  * @param
  * @retval
  * @brief
  ********************************************************************/
bool egtkGlyph2Bitmap(egtk_COL_T BackGrColour, egtk_COL_T ForeGrColour, egtk_GLYPH_T *pGlyph, egtk_BITMAP_T *pBitmap,bool OverWrite)
{
    uint16_t   WidthG2B;
    uint16_t   HeightG2B;
    uint8_t    *pBuffGlyph;
    egtk_COL_T *pBuffBitmap;

    WidthG2B = pBitmap->Width =  pGlyph->Width;
    HeightG2B = pBitmap->Height =  pGlyph->Height;

    pBuffGlyph =  pGlyph->pBuffer;
    pBuffBitmap = pBitmap->pBuffer;

    if( (HeightG2B*WidthG2B) > EGTK_BUFF_SIZE)
        return false;
    else {

        for (uint16_t y = 0; y < HeightG2B; y++) {
            for (uint16_t x = 0; x < WidthG2B; x++ ) {
                if( pBuffGlyph[x/8] & (0x80 >> (x % 8)) )
                    *pBuffBitmap = ForeGrColour;
                else if (OverWrite)
                    *pBuffBitmap = BackGrColour;
                pBuffBitmap++;
            }
            pBuffGlyph += WidthG2B/8;

            if(WidthG2B%8)
                pBuffGlyph++;
        }
        return true;
    }

}

  /********************************************************************
  * @brief  egtkBlitBitmap
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkBlitBitmap(int16_t x0, int16_t y0, egtk_BITMAP_T *pBitmap, bool ToScreen)
{
    /* Check if bitmap is inside clip windows bounds */
    if (
        (x0 < Screen.MinX) ||
        (y0 < Screen.MinY) ||
        (x0 + pBitmap->Width  > Screen.MaxX) ||
        (y0 + pBitmap->Height > Screen.MaxY)
    ) {
        /* Out of bounds, use local pixel fallback. */
        egtk_COL_T *ptr = pBitmap->pBuffer;

        for (uint16_t y = 0; y < pBitmap->Height; y++) {
            for (uint16_t x = 0; x < pBitmap->Width; x++) {
                if(ToScreen)
                    egtkPutPixel(x0 + x , y0 + y , *ptr);
                else
                    egtkGetPixel(x0 + x, y0 + y, ptr);

                ptr++;
            }
        }
    } else {
        /* Inside of bounds, can use HAL provided blit. */
        egtkHalBlitBitmap(x0, y0, pBitmap->Width, pBitmap->Height, pBitmap->pBuffer, ToScreen);
    }

};

  /********************************************************************
  * @brief  egtkDrawGlyph
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawGlyph(int16_t x0, int16_t y0, egtk_COL_T ForeGrColour, egtk_COL_T BackGrColour, const tGlyph *pGlyphIn, bool OverWrite)
{
    egtk_GLYPH_T LocalGlyph;
    egtk_BITMAP_T LocalBitmap;

    LocalGlyph.Width = pGlyphIn->width;
    LocalGlyph.Height = pGlyphIn->height;
    LocalGlyph.pBuffer = (uint8_t *)pGlyphIn->data;

    LocalBitmap.Width = pGlyphIn->width;
    LocalBitmap.Height = pGlyphIn->height;
    LocalBitmap.pBuffer = BuffImage[1];

    if (!OverWrite)
        egtkBlitBitmap(x0, y0, &LocalBitmap, false);

    if( egtkGlyph2Bitmap(BackGrColour, ForeGrColour, &LocalGlyph, &LocalBitmap, OverWrite) )
        egtkBlitBitmap(x0,y0, &LocalBitmap,true);
}


  /********************************************************************
  * @brief  egtkDrawBitmap
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawBitmap(int16_t x0, int16_t y0, const tBitmap *pBitmapIn)
{
    egtk_BITMAP_T LocalBitmap;

    LocalBitmap.Width = pBitmapIn->width;
    LocalBitmap.Height = pBitmapIn->height;
    LocalBitmap.pBuffer = ( egtk_COL_T*)pBitmapIn->data;

    egtkBlitBitmap(x0,y0, &LocalBitmap,true);
}
  /********************************************************************
  * @brief  egtkFitBitmap
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkFitBitmap(uint16_t dstw, uint16_t dsth, egtk_BITMAP_T *src, egtk_BITMAP_T *dst)
{
    uint16_t px, py;
    uint16_t srcw = src->Width;
    uint16_t srch = src->Height;
    uint32_t x_ratio = (uint32_t)((srcw << 16) / dstw);
    uint32_t y_ratio = (uint32_t)((srch << 16) / dsth);

    egtk_COL_T *dstptr =  dst->pBuffer;

    egtk_COL_T *srcptr =  src->pBuffer;

    dst->Width = dstw;
    dst->Height = dsth;

    int xSample;
    int ySample;
    int tSample;

    xSample = srcw/dstw;
    if(!xSample)
        xSample = 1;
    ySample = srch/dsth;
    if(!ySample)
        ySample = 1;
    tSample = ySample*xSample;

    RGB888_TYPE ColRGB;

    for (uint16_t y = 0; y < dsth; y++) {
        for (uint16_t x = 0; x < dstw; x++) {
            px = ((x * x_ratio) >> 16);
            py = ((y * y_ratio) >> 16);

            #ifdef COLOUR_AVERAGE
            {
                uint32_t MeanColR=0,MeanColG=0,MeanColB = 0;

                for(uint8_t jx = 0; jx<xSample ; jx++ ){
                    for(uint8_t jy = 0; jy<ySample ; jy++ ){
                            egtk_COL_T ColVal;

                            ColVal = srcptr[((py+jy) * srcw) + px + jx];
                            ColRGB = egtkRGB565_2_RGB888( ColVal );
                            MeanColR += ColRGB.Red;
                            MeanColG += ColRGB.Green;
                            MeanColB += ColRGB.Blue;
                    }
                }
                ColRGB.Red = MeanColR/tSample;
                ColRGB.Green = MeanColG/tSample;
                ColRGB.Blue = MeanColB/tSample;

                *(dstptr++) = egtkRGB888_2_RGB565(ColRGB);
            }
            #else
                *(dstptr++) = srcptr[(py * srcw) + px];
            #endif          
        }
    }
}
  /********************************************************************
  * @brief  egtkDrawFitBitmap
  * @param
  * @retval
  * @brief
  ********************************************************************/
bool egtkDrawFitBitmap(int16_t x0, int16_t y0, uint16_t dstw, uint16_t dsth, const tBitmap *pBitmapIn)
{
    if( (dstw*dsth)> EGTK_BUFF_SIZE)
        return false;
    else {
        egtk_BITMAP_T LocalBitmapSrc;
        egtk_BITMAP_T LocalBitmapDst;

        LocalBitmapSrc.Width = pBitmapIn->width;
        LocalBitmapSrc.Height = pBitmapIn->height;
        LocalBitmapSrc.pBuffer = ( egtk_COL_T*)pBitmapIn->data;


        LocalBitmapDst.pBuffer = BuffImage[0];

        egtkFitBitmap(dstw, dsth, &LocalBitmapSrc, &LocalBitmapDst);

        egtkBlitBitmap(x0,y0, &LocalBitmapDst,true);

        return true;
    }
}


  /********************************************************************
  * @brief  egtkDrawFitGlyph
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkDrawFitGlyph(int16_t x0, int16_t y0, uint16_t dstw, uint16_t dsth, egtk_COL_T ForeGrColour, egtk_COL_T BackGrColour, const tGlyph *pGlyphIn)
{
    egtk_GLYPH_T LocalGlyph;
    egtk_BITMAP_T LocalBitmapSrc;
    egtk_BITMAP_T LocalBitmapDst;

    LocalGlyph.Width = pGlyphIn->width;
    LocalGlyph.Height = pGlyphIn->height;
    LocalGlyph.pBuffer = (uint8_t *)pGlyphIn->data;

    LocalBitmapSrc.Width = pGlyphIn->width;
    LocalBitmapSrc.Height = pGlyphIn->height;
    LocalBitmapSrc.pBuffer = BuffImage[0];

    LocalBitmapDst.pBuffer = BuffImage[1];

    if( egtkGlyph2Bitmap(BackGrColour, ForeGrColour, &LocalGlyph, &LocalBitmapSrc,true) )
    {
        egtkFitBitmap(dstw, dsth, &LocalBitmapSrc, &LocalBitmapDst);

        egtkBlitBitmap(x0,y0, &LocalBitmapDst,true);
    }
}



  /********************************************************************
  * @brief  egtkPutChar
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkPutChar(uint8_t FontCode,bool OverWrite)
{
    unsigned char *pFntTbl;
    unsigned char *pLocalBuffGlyph;
    uint16_t FntChrSze;
    uint16_t FntChrW;
    egtk_COL_T FG_Colour;
    egtk_COL_T BG_Colour;

    egtk_GLYPH_T LocalGlyph;
    egtk_BITMAP_T LocalBitmap;

    LocalGlyph.Width = Screen.pFont->width;
    LocalGlyph.Height = Screen.pFont->height;
    LocalGlyph.pBuffer = pLocalBuffGlyph = (uint8_t*)BuffImage[1];

    LocalBitmap.pBuffer = BuffImage[0];

    FntChrW = Screen.pFont->width/8;
    if(Screen.pFont->width%8)
        FntChrW++;

    FntChrSze = FntChrW*Screen.pFont->height;

    pFntTbl = Screen.pFont->p;
    pFntTbl += FontCode*FntChrSze;

    for( uint8_t j = 0; j<FntChrSze; j++ )
        pLocalBuffGlyph[j] = reverse_byte(pFntTbl[j]);


    if(Screen.FontFlags.Reverse) {
        BG_Colour = Screen.BackGrColour;
        FG_Colour = Screen.ForeGrColour;
    } else  {
        FG_Colour = Screen.BackGrColour;
        BG_Colour = Screen.ForeGrColour;
    }

    if (!OverWrite)
    {
        LocalBitmap.Width = Screen.pFont->width;
        LocalBitmap.Height = Screen.pFont->height;

        egtkBlitBitmap(Screen.Pos.x, Screen.Pos.y, &LocalBitmap, false);

    }

    if( egtkGlyph2Bitmap(BG_Colour, FG_Colour, &LocalGlyph, &LocalBitmap, OverWrite) )
        egtkBlitBitmap(Screen.Pos.x, Screen.Pos.y, &LocalBitmap, true);
}


  /********************************************************************
  * @brief  egtkWriteString
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkWriteString(char* InString, bool OverWrite)
{
    Screen.Pos.x = Screen.PosIni.x ;
    Screen.Pos.y = Screen.PosIni.y ;

    while(*InString)
    {
        if(*InString == 0x0A)
        {
            Screen.FontFlags.LineFistChr = 1;
            Screen.FontFlags.ColFistChr = 0;
            Screen.Pos.y += Screen.pFont->height + Screen.FontSpaceY;
        }

        else if(*InString == 0x0D)
        {
            Screen.FontFlags.LineFistChr = 1;
            Screen.Pos.x = Screen.PosIni.x;
        }

        else if(    (Screen.Pos.x+Screen.pFont->width < Screen.MaxX) &&
                    (Screen.Pos.y+Screen.pFont->height < Screen.MaxY)       )
        {

            //separazione
            if(Screen.FontFlags.LineFistChr)
                Screen.FontFlags.LineFistChr = 0;
            else
                Screen.Pos.x += Screen.FontSpaceX;

            egtkPutChar(*InString, OverWrite);

            Screen.Pos.x += Screen.pFont->width;

        }

        InString++;

    }
}

  /********************************************************************
  * @brief  egtkSetFont
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkSetFont(const egtk_FONT*pFont, uint8_t FontSpaceX, uint8_t FontSpaceY,  bool Reverse)
{
    Screen.FontSpaceX = FontSpaceX;
    Screen.FontSpaceY = FontSpaceY;


    if(Reverse)
        Screen.FontFlags.Reverse = 1;
    else
        Screen.FontFlags.Reverse = 0;

    Screen.pFont = pFont;


}

  /********************************************************************
  * @brief  egtkSetPos
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkSetPos(int16_t PosX, int16_t PosY)
{
    Screen.PosIni.x = Screen.Pos.x = PosX;
    Screen.PosIni.y = Screen.Pos.y = PosY;

    Screen.FontFlags.ColFistChr = 1;
    Screen.FontFlags.LineFistChr = 1;

}

  /********************************************************************
  * @brief  SetReverse
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkSetReverse(bool Reverse)
{
    if(Reverse)
        Screen.FontFlags.Reverse= 1;
    else
        Screen.FontFlags.Reverse = 0;
}

  /********************************************************************
  * @brief  SetReverse
  * @param
  * @retval
  * @brief
  ********************************************************************/
void egtkSetColour(egtk_COL_T ForeColour, egtk_COL_T BackColour)
{
    Screen.BackGrColour = BackColour;

    Screen.ForeGrColour = ForeColour;

}



















