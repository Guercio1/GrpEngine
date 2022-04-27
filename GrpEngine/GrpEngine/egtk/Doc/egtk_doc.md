# Elcos Graphic ToolKit
Simple portable graphic toolkit for mcrocontroller or whatever you need...


## File
Library composed by:

- *egtk.cpp* Main library
- *egtk_fonts.cpp* Fonts collection
- *egtk_driver.cpp* Hardware driver
- *egtk_colour.cpp* Colour support

Each file has his header file. File are c compliant, just replace extension in c.

## Usage
Just Include *egtk.h* file and define hardware abstraction functions in *egtk_driver.cpp*.

- *egtkHalInitScreen* Initialize screen 
- *egtkHalPutPixel* PUT a coloured PIXEL
- *egtkHalGetPixel* GET a coloured PIXEL
- *egtkHalDrawHLine* DRAW an HORIZONTAL LINE
- *egtkHalDrawVLine* DRAW a VERTICAL LINE
- *egtkHalFillRectangle* FILL a RECTANGLE
- *egtkHalBlitBitmap* BLIT (block tranfer) a bitmap image TO or FROM the screen.

You need to define the dimension of the double buffer used for internal operations EGTK_BUFF_SIZE (half buffer size); This is the maximum size of the bitmap you can handle.

You can include colour averege when scaling image dimensions: COLOUR_MEAN.

You can define egtk_COL_T as RGB565 or RGB88.

## Remarks
Origin is TOP-LEFT of the screen.

## Functions

### Initialization 

#### egtkInitScreen
Initialize screeen.

- *Width* of the screen 
- *Height* of the screen
- *ForeGrColour* in text mode
- *BackGrColour* in text mode

### Geometrical

#### egtkDrawLine
Draw a line to the screen.

- *x0* initial 
- *y0* initial 
- *x1* final 
- *y1* final 
- *Colour* of the line

#### egtkPutPixel
Put a pixel on the screen.

- *x0* position
- *y0* position
- *Colour* of the point
   
   
### egtkGetPixel   
Get a pixel from the screen.
- *x0* position
- *y0* position
- *Colour* of the point

   
#### egtkDrawRectangle/egtkFillRectangle
Draw or fill a rectangle in the screen

- *x0* upper left
- *y0* upper left
- *x1* lower right
- *y1* lower right
- *Colour* of the line

#### egtkFillRoundedRectangle/egtkDrawRoundedRectangle
Draw or fill a rectangle with rounded corner.

- *x0* upper left
- *y0* upper left
- *x1* lower right
- *y1* lower right
- *r* radius
- *Colour* of the line

#### egtkDrawCircle/egtkFillCircle
Draw or fill a circle.

- *xc* center
- *yc* center
- *color* 

#### egtkDrawEllipse/egtkFillEllipse
Draw or fill an ellipse.

- *x0* center
- *y0* center
- *a*  horizontal diameter
- *b*  vertical diameter
- *color* 

#### egtkDrawTriangle
Draw or fill a triangle.

- *x0* first vertex
- *y0* first vertex
- *x1* second vertex 
- *y1* second vertex 
- *x2* tirdh vertex
- *y2* tirdh vertex
- *Colour* 

#### egtkDrawPolygon/egtkFillPolygon
Draw or fill a generic opolygon.

- *amount* of vertexes
- *vertices* 
- *color*

### Utility

#### egtkClearScreen

Clear the screen filling background color.

### egtkGetScreenW

Return the width of the screen.

### egtkGetScreenH

Return the hight of the screen.

## egtkGetForeColour/egtkGetBackColour

Return current foreground/background colour.

### Glyph Image
A glyph is a simple, graphic image that represents an idea, sored in *egtk_GLYPH_T* Type. Is defined to be monochrome. Use lcd-image-converter to generate egtk_GLYPH_T struct. A glyph is stored 1 pixel each bit, so space occupation is lower that bmp.

#### egtkDrawGlyph
Draw a glyph image, overwrite mode available.

- *x0* upper left position
- *y0* upper left position
- *BackGrColour* colour background
- *ForeGrColour* colour foreground
- *pGlyphIn*  glyph image
- *OverWrite*  overwrite/no overwrite present screen
   
#### egtkDrawFitGlyph
Fit (scale/zoom) a glyph image in defined position: olways overwrite mode.

- *x0* upper left position
- *y0* upper left position
- *dstw* destination width
- *dsth* destination high
- *BackGrColour* colour background
- *ForeGrColour* colour foreground
- *pGlyphIn*  glyph image pointer

### Bitmap   
Bitmap is a coloured image sored ad *tBitmap* type. 

### egtkDrawBitmap

- *x0* upper left position
- *y0* upper left position
- *pBitmapIn* bitmap image pointer

### egtkDrawFitBitmap

- *x0* upper left position
- *y0* upper left position
- *dstw* destination width
- *dsth* destination high
- *pBitmapIn* bitmap image pointer

### Text
You need to enable used font. 

### egtkPutChar
Print a character on the screen Overwrite or NoOverWrite morde
- *FontCode* 0...255
- *OverWrite* overwrite/no overwrite present screen

#### egtkWriteString
Print string CR+LF...
Print a string on the screen Overwrite or NoOverWrite morde
- *InString*  String to print
- *OverWrite* overwrite/no overwrite present screen

#### egtkSetFont
Set new font and features.
- *pFont* Font to set
- *FontSpaceX* space distance
- *FontSpaceY* line distance
- *Reverse* Set reverse colour

#### egtkSetPos
Set start writing position: top left.
- *PosX* 
- *PosY* 

#### egtkSetReverse
Set reverse colour.
- *Reverse* Reverse colour

#### egtkSetColour
- *ForeColour* foreground colour
- *BackColour* background colour


# License
	MIT License

	Copyright (c) 2022 Guercio1

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

# Thanks
Some inspiration from Mika Tuupola           
[https://appelsiini.net/](https://appelsiini.net/).

Use lcd-image-converter to generate tBitmap struct Thank to Vladimir riuson     
<riuson@gmail.com>      
[http://www.riuson.com/lcd-image-converter](http://www.riuson.com/lcd-image-converter )

Fonts tables from *uGUI* library. Thanks to: Achim Dobler    
[http://www.embeddedlightning.com/](http://www.embeddedlightning.com/)


### 25/04/2022
First release





