  #include <stdint.h>
  #include <stdlib.h>
  #include "..\GrpEngine\VSbridge.h"
  #include "..\egtk\egtk.h"
  #include "..\egtk\Elcos.h"
  #include "..\egtk\Giako.h"
  #include "..\Examples\TestLibrary.h"


     
     void TestLibraryClass::OnUserCreate(void)
     {
     	egtkInitScreen(700, 500, RGB565_BLACK, RGB565_WHITE);
     	//srand(100);
     }
     


 bool TestLibraryClass::OnUserUpdate(float Timer)
 {
 	
 	
 #if(0)
    uint16_t x, y;

 	for (x = 0; x < egtkGetScreenW(); x++)
 	{
 		for (y = 0; y < egtkGetScreenH(); y++)
 		{
 			RGB888_TYPE Clour;
 			uint16_t colour;
 			Clour.Red = rand() % 256;
 			Clour.Green = rand() % 256;
 			Clour.Blue = rand() % 256;
 
 			colour = egtkRGB888_2_RGB565(Clour);
 
 			egtkPutPixel(x, y, colour);
 		}
 
 
 	}
 #endif
 
 	egtkSetColour(RGB565_WHITE, RGB565_BLACK);

 #if(0)
 	egtkClearScreen();
 #endif
 

 

// 	egtkSetPos(egtkGetScreenW()/3, egtkGetScreenH()/2);
// 	egtkWriteString((char*)"Test",false);

    egtkSetFont(&FONT_32X53,0, 0, 0);
    egtkSetColour(RGB565_BLACK, RGB565_RED);

    egtkSetPos(egtkGetScreenW() / 3, egtkGetScreenH() -200);
    egtkWriteString((char*)"GrpEngine", false);

    
    egtkSetFont(&FONT_10X16, 0, 0, 0);
    egtkSetColour(RGB565_BLACK, RGB565_WHITE);
    egtkSetPos(10, egtkGetScreenH() - 40);
    egtkWriteString((char*)"POWERED BY egtk", false);

 	
 	
 
 	//egtkDrawGlyph(0,0, RGB565_YELLOW, RGB565_BLUE, &Elcos,false);
 	//egtkDrawFitGlyph(Elcos.width, 0, Elcos.width/2, Elcos.height/2,  RGB565_YELLOW, RGB565_BLUE, &Elcos );
 
 	egtkDrawGlyph(egtkGetScreenW() / 2 - Elcos.width/2, 100, RGB565_RED, RGB565_BLACK, &Elcos, false);

 	//egtkPutPixel(200, 200, 0Xffff);
 	//egtkSetColour(RGB565_MAGENTA, RGB565_BLUE);
 	//egtkSetFont(&FONT_32X53, 1, 1, 1);
 	//egtkSetPos(40,10);
 	//egtkWriteString((char*)"Ciao Caro!\n\rprova gqpj");
 	//egtkDrawFitGlyph(5,5, Elcos.width/2, Elcos.height/2, RGB565_YELLOW, RGB565_BLACK, &Elcos);
 	//egtkDrawFitGlyph(5,5, Elcos.width*2, Elcos.height*2, RGB565_WHITE, RGB565_BLACK, &Elcos);
 	//egtkDrawLine(0,0,479,271, RGB565_RED);
 	//egtkDrawLine(10,10,600,200,RGB565_YELLOW);
 	//egtkDrawCircle( 200,100, 60, RGB565_YELLOW);
 	//egtkFillCircle( 200,200, 60, RGB565_RED);
 	//egtkDrawEllipse(200,200,100,20,RGB565_BLUE);
 	//egtkFillEllipse(100,100,30,50,RGB565_GREEN);
 	//egtkDrawRoundedRectangle(150,150,300,200,9,RGB565_MAGENTA);
 	//egtkFillTriangle(285,20,400,160,260,300,RGB565_CYAN );
 	//egtkFillRoundedRectangle(150,150,300,200,9,RGB565_MAGENTA);

 	//egtkDrawBitmap(5, 5, &Giako);

    return true;
 }