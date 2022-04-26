
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "..\GrpEngine\VSbridge.h"
#include "..\Examples\spline2.h"
#include <cstdio>
#include <cwchar>
#include "..\egtk\egtk.h"



OneLoneCoder_Splines::OneLoneCoder_Splines(void)
{

}

void OneLoneCoder_Splines::OnUserCreate()
{
	egtkInitScreen(700, 500, RGB565_WHITE, RGB565_BLACK);

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

	float ang = 0;

	float radius = MIN(egtkGetScreenW(), egtkGetScreenH()) / 3;

	float middleY = egtkGetScreenH() / 2;
	float middleX = egtkGetScreenW() / 2;
	sPoint2D SplinePoin;


	


	for (int i = 0; i < 10; i++)
	{
		SplinePoin.x = middleX + radius * cos(ang);
		SplinePoin.y = middleY + radius * sin(ang);

		path.points.push_back(SplinePoin);

		ang += 2 * 3.14159f / 10;

	}



	//path.points = { { 10, 41 },{ 40, 41 },{ 70, 41 },{ 100, 41 } };
	//path.points = { { 100, 100 },{ 200, 200 },{ 300, 300 },{ 400, 400 },{ 500, 500 },{ 100, 600 }, };
	//return true;
}

void OneLoneCoder_Splines::OnUserMouse(int xpos, int ypos, MOUSE_TYPE MousAction)
{
	static bool Locked = false;
	int SelNow = -1;
	float Aux1, Aux2;

	MouseX = xpos;
	MouseY = ypos;

	if (MousAction == MOUSE_LFT_PSH) {

		for (int i = 0; i < path.points.size(); i++) {
			Aux1 = abs(path.points[i].x - xpos);
			Aux1 *= Aux1;

			Aux2 = abs(path.points[i].y - ypos);
			Aux2 *= Aux2;

			Aux2 += Aux1;

			Aux1 = sqrt(Aux2);

			if (Aux1 <= 10)
			{
				Locked = true;
				path.points[i].y = ypos;
				path.points[i].x = xpos;
				nSelectedPoint = i;
				break;
			}
		}
	}
	else if (MousAction == MOUSE_MOV_L_PSH) {
		if (Locked) {
			path.points[nSelectedPoint].y = ypos;
			path.points[nSelectedPoint].x = xpos;
		}
	}
	else
		Locked = false;

}

void OneLoneCoder_Splines::OnUserKeyboard(int Code, bool push)
{
	if (push)
		PressedKey = Code;
	else
		PressedKey = 0;

}

 bool OneLoneCoder_Splines::OnUserUpdate(float fElapsedTime)
{

	char Cstr[100];
	// Clear Screen
	egtkClearScreen();

	// Handle input
	if (PressedKey == 'A')
	{
		PressedKey = 0;
		nSelectedPoint++;
		if (nSelectedPoint >= path.points.size())
			nSelectedPoint = 0;
	}
	if (PressedKey == 'Z')
	{
		PressedKey = 0;
		nSelectedPoint--;
		if (nSelectedPoint < 0)
			nSelectedPoint = path.points.size() - 1;
	}
	if (PressedKey == VK_LEFT)
		path.points[nSelectedPoint].x -= 1.0f /* * fElapsedTime*/;

	if (PressedKey == VK_RIGHT)
		path.points[nSelectedPoint].x += 1.0f /* * fElapsedTime*/;

	if (path.points[nSelectedPoint].x < 0)
		path.points[nSelectedPoint].x = 0;
	else if (path.points[nSelectedPoint].x >= egtkGetScreenW())
		path.points[nSelectedPoint].x = egtkGetScreenW() - 1;

	if (PressedKey == VK_UP)
		path.points[nSelectedPoint].y -= 1.0f /* * fElapsedTime*/;

	if (PressedKey == VK_DOWN)
		path.points[nSelectedPoint].y += 1.0f /* * fElapsedTime*/;

	if (path.points[nSelectedPoint].y < 0)
		path.points[nSelectedPoint].y = 0;
	else if (path.points[nSelectedPoint].y >= egtkGetScreenH())
		path.points[nSelectedPoint].y = egtkGetScreenH() - 1;


	if (PressedKey == 'X')
		fMarker -= 5.0f /* * fElapsedTime*/;

	if (PressedKey == 'S')
		fMarker += 5.0f /* * fElapsedTime*/;

	if (fMarker >= (float)path.fTotalSplineLenght)
		fMarker -= (float)path.fTotalSplineLenght;

	if (fMarker < 0.0f)
		fMarker += (float)path.fTotalSplineLenght;

	// Draw Spline
	for (float t = 0; t < (float)path.points.size(); t += 0.005f)
	{
		sPoint2D pos = path.GetSplinePoint(t, true);
		egtkPutPixel(pos.x, pos.y, RGB565_WHITE);
	}

	egtkSetFont(&FONT_8X14, 0, 0, 1);

	// Draw Control Points

	egtkSetColour(RGB565_YELLOW, RGB565_BLACK);
	path.fTotalSplineLenght = 0.0f;
	for (int i = 0; i < path.points.size(); i++)
	{
		egtkSetPos(path.points[i].x + 20, path.points[i].y - 20);
		//egtkPutChar(i+0x30);
		path.fTotalSplineLenght += (path.points[i].lenght = path.CalculateSegmentLenght(i, true));
		egtkFillCircle(path.points[i].x, path.points[i].y, 10, RGB565_RED);
		egtkPutPixel(path.points[i].x, path.points[i].y, RGB565_BLACK);
		sprintf_s(Cstr, sizeof(Cstr), "%f", path.points[i].lenght);
		egtkWriteString(Cstr, false);
	}

	egtkSetColour(RGB565_WHITE, RGB565_BLACK);

	// Highlight control point
	egtkFillCircle(path.points[nSelectedPoint].x, path.points[nSelectedPoint].y, 10, RGB565_YELLOW);
	egtkPutPixel(path.points[nSelectedPoint].x, path.points[nSelectedPoint].y, RGB565_BLACK);

	// Draw agent to demonstrate gradient
	float fOffset = path.GetNormalisedOffst(fMarker);
	sPoint2D p1 = path.GetSplinePoint(fOffset, true);
	sPoint2D g1 = path.GetSplineGradient(fOffset, true);
	float r = atan2(-g1.y, g1.x);


	sPoint2D line1, line2;
	line1.x = 10.0f * sin(r) + p1.x;
	line1.y = 10.0f * cos(r) + p1.y;
	line2.x = -10.0f * sin(r) + p1.x;
	line2.y = -10.0f * cos(r) + p1.y;
	egtkDrawLine(static_cast<int>(line1.x), static_cast<int>(line1.y), static_cast<int>(line2.x), static_cast<int>(line2.y), RGB565_BLUE);

	sprintf_s(Cstr, sizeof(Cstr), "Marker=%f", fMarker);
	egtkSetPos(10, 10);
	egtkWriteString(Cstr, false);
	sprintf_s(Cstr, sizeof(Cstr), "Offset=%f", fOffset);
	egtkSetPos(10, 30);
	egtkWriteString(Cstr, false);

	int xposNow = egtkGetScreenW() - 300;

	sprintf_s(Cstr, sizeof(Cstr), "Position=%f,%f", p1.x, p1.y);
	egtkSetPos(xposNow, 10);
	egtkWriteString(Cstr, false);

	sprintf_s(Cstr, sizeof(Cstr), "Gradient=%f,%f", g1.x, g1.y);
	egtkSetPos(xposNow, 30);
	egtkWriteString(Cstr, false);

	/*
			sprintf_s(Cstr, sizeof(Cstr), "Line 1=%f,%f", line1.x, line1.y);
			egtkSetPos(xposNow, 50);
			egtkWriteString(Cstr, false);

			sprintf_s(Cstr, sizeof(Cstr), "Line 2=%f,%f", line2.x, line2.y);
			egtkSetPos(xposNow, 70);
			egtkWriteString(Cstr, false);
	*/



	sprintf_s(Cstr, sizeof(Cstr), "Mouse=%d,%d", MouseX, MouseY);
	egtkSetPos(10, egtkGetScreenH() - 30);
	egtkWriteString(Cstr, false);


	return true;
}
	


