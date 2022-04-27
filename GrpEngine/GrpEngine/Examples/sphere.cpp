



#include <math.h>

#include "..\GrpEngine\VSbridge.h"
#include "..\Examples\sphere.h"

void SpherelightClass::DrawReference(void)
{

	int lineRefLen = 30;
	int OrigX = 30;
	int OrigY = 30;
	int TriDim = 3;
	egtkSetFont(&FONT_8X14, 0, 0, 1);

	//x
	egtkDrawLine(OrigX, OrigY, OrigX + lineRefLen, OrigY, egtkGetForeColour());
	/*egtkFillTriangle(
		OrigX + lineRefLen,
		OrigY - TriDim,
		OrigX + lineRefLen,
		OrigY + TriDim,
		OrigX + lineRefLen + TriDim,
		OrigY,
		egtkGetForeColour());
	egtkSetPos(OrigX + lineRefLen + TriDim+3, OrigY - TriDim-3);*/
	egtkSetPos(OrigX + lineRefLen + 3, OrigY - 6);
	egtkPutChar('x', false);

	//y
	egtkDrawLine(OrigX, OrigY, OrigX, OrigY + lineRefLen, egtkGetForeColour());
	/*egtkFillTriangle(
		OrigX + TriDim,
		OrigY + lineRefLen,
		OrigX - TriDim,
		OrigY + lineRefLen,
		OrigX,
		OrigY + lineRefLen + TriDim,
		egtkGetForeColour());
	egtkSetPos(OrigX - TriDim, OrigY + lineRefLen + TriDim+3);*/
	egtkSetPos(OrigX  - 4, OrigY + lineRefLen + 3); 
	egtkPutChar('y', false);

	//z
	egtkDrawLine(OrigX, OrigY, OrigX - lineRefLen / 2, OrigY + lineRefLen / 2, egtkGetForeColour());
	/*egtkFillTriangle(
		OrigX - lineRefLen / 2 - TriDim / 2,
		OrigY + lineRefLen / 2 - TriDim / 2,
		OrigX - lineRefLen / 2 + TriDim / 2,
		OrigY + lineRefLen / 2 + TriDim / 2,
		OrigX - lineRefLen / 2 - TriDim / 2,
		OrigY + lineRefLen / 2 + TriDim / 2,
		egtkGetForeColour());
	egtkSetPos(OrigX - lineRefLen / 2 - TriDim / 2, OrigY + lineRefLen / 2 + TriDim / 2);*/
	egtkSetPos(OrigX - lineRefLen / 2 - 7, OrigY + lineRefLen / 2 + 2); 
	egtkPutChar('z', false);
}

void SpherelightClass::DrawSphere(uint16_t ui16Radius, uint16_t ui16CntrX, uint16_t ui16CntrY, egtk_COL_T Colour)
{
	int16_t x, y;
	Vector vSurface;
	float reflectLight;
	egtk_COL_T ColSphere;

	for (x = -ui16Radius; x <= ui16Radius; x++)
	{
		for (y = -ui16Radius; y <= ui16Radius; y++)
		{
			if ((x * x + y * y) <= ui16Radius * ui16Radius)
			{
				vSurface.x = x;
				vSurface.y = y;
				vSurface.z = sqrt(ui16Radius * ui16Radius - x * x - y * y);

				VectNormalize(&vSurface);

				reflectLight = pow(dotProduct(&Light, &vSurface),9);



				//If mouse pointer on the sphere
				if( ((mouseX - ui16CntrX) == x) && ((mouseY - ui16CntrY) == y))
					ReflectLight = reflectLight;

				RGB888_TYPE colourSphere = { 128,128,128 };

				if (reflectLight > 0)
				{
					colourSphere.Red += 128 * reflectLight;
					colourSphere.Green += 128 * reflectLight;
					colourSphere.Blue += 128 * reflectLight;
				}
				else
				{
					colourSphere.Red += 64 * reflectLight;
					colourSphere.Green += 64 * reflectLight;
					colourSphere.Blue += 64 * reflectLight;
				}


				egtkPutPixel(x + ui16CntrX, y + ui16CntrY, egtkRGB888_2_RGB565(colourSphere));
			}
		}
	}
}

void SpherelightClass::GestControl(void)
{
	int xCntr, yCntr;
	Vector NewLight;

	char Str[100];
	
	egtkDrawCircle(ContLight.cntrX, ContLight.cntrY, ContLight.Radius, RGB565_WHITE);

	egtkFillCircle(ContLight.PosX, ContLight.PosY, ContLight.Control, RGB565_RED);

	NewLight.x = ContLight.PosX - ContLight.cntrX;
	NewLight.y = ContLight.PosY - ContLight.cntrY;

	NewLight.z = sqrt(ContLight.Radius* ContLight.Radius - NewLight.x* NewLight.x - NewLight.y* NewLight.y);

	VectNormalize(&NewLight);

	Light = NewLight;

	sprintf_s(Str, "x:%f\n\ry:%f\n\rz:%f", Light.x, Light.y, Light.z);

	egtkSetPos(ContLight.cntrX - ContLight.Radius, ContLight.cntrY + ContLight.Radius + 10);
	egtkSetFont(&FONT_8X14, 0, 0, 1);
	egtkWriteString(Str, false);

	
}

void SpherelightClass::VectNormalize(Vector *pVect)
{
	float fAux;

	fAux = pVect->x * pVect->x;
	fAux += pVect->y * pVect->y;
	fAux += pVect->z * pVect->z;

	fAux = sqrt(fAux);

	pVect->x /= fAux;
	pVect->y /= fAux;
	pVect->z /= fAux;
}

float SpherelightClass::dotProduct(Vector* pVectA, Vector* pVectB)
{
	float VectC;

	VectC = pVectA->x * pVectB->x + 
			pVectA->y * pVectB->y + 
			pVectA->z * pVectB->z;

	return VectC /*< 0 ? -VectC : 0*/;

}

void SpherelightClass::DrawReflexLight(void)
{
	char StrTmp[100];
	
	egtkSetFont(&FONT_8X14, 0, 0, 1);

	sprintf_s(StrTmp, "Light=%f", ReflectLight);
	egtkSetPos(10, egtkGetScreenH() - 40);
	egtkWriteString(StrTmp, true);


}
// mandatory functions
bool SpherelightClass::OnUserCreate(void) {

	egtkInitScreen(700, 500, RGB565_WHITE,RGB565_BLACK );

	ContLight.Radius = 40;
	ContLight.Control = 5;
	ContLight.cntrX = egtkGetScreenW() - ContLight.Radius - 30;
	ContLight.cntrY = ContLight.Radius + 30;
	ContLight.PosX = ContLight.cntrX;
	ContLight.PosY = ContLight.cntrY;
	ContLight.selected = false;

	VectNormalize(&Light);


	SphereRadius = 150;

	return true;
}

bool SpherelightClass::OnUserUpdate(float fTimer)
{
	egtkClearScreen();

	GestControl();

	DrawSphere(SphereRadius, egtkGetScreenW() / 2, egtkGetScreenH() / 2,  RGB565_BLUE );

	DrawReference();

	DrawReflexLight();

	return true;
}

void SpherelightClass::OnUserMouse(int xpos, int ypos, MOUSE_TYPE MousAction)
{
	mouseX = xpos;
	mouseY = ypos;

	if (MousAction == MOUSE_LFT_PSH)
	{
		if (((xpos - ContLight.PosX) * (xpos - ContLight.PosX) + (ypos - ContLight.PosY) * (ypos - ContLight.PosY)) <= (ContLight.Control * ContLight.Control))
			ContLight.selected = true;
	}
	else if(MousAction == MOUSE_LFT_REL)
		ContLight.selected = false;
	else if (MousAction == MOUSE_MOV_L_PSH)
	{
		if (ContLight.selected)
		{
			if (((xpos - ContLight.cntrX) * (xpos - ContLight.cntrX) + (ypos - ContLight.cntrY) * (ypos - ContLight.cntrY)) <= (ContLight.Radius * ContLight.Radius))
			{
				ContLight.PosX = xpos;
				ContLight.PosY = ypos;
			}
		}
	}
}









