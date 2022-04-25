#include <iostream>
#include <string>
#include <windows.h>

#include "..\egtk\egtk.h"
//#include "..\egtk\elcos.h"

using namespace std;

#include <vector>

struct sPoint2D
{
	float x;
	float y;
	float lenght;

};

struct sSpline
{
	vector<sPoint2D> points;
	float fTotalSplineLenght;


	sPoint2D GetSplinePoint(float t, bool bLooped = false)
	{
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f * tt - t;
		float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q3 = -3.0f * ttt + 4.0f * tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}

	sPoint2D GetSplineGradient(float t, bool bLooped = false)
	{
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -3.0f * tt + 4.0f * t - 1;
		float q2 = 9.0f * tt - 10.0f * t;
		float q3 = -9.0f * tt + 8.0f * t + 1.0f;
		float q4 = 3.0f * tt - 2.0f * t;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}

	float CalculateSegmentLenght(int node, bool bLooped = false)
	{
		float fLenght = 0.0f;
		float fStepSize = 0.005f;

		sPoint2D old_point, new_point;
		old_point = GetSplinePoint((float)node, bLooped);

		for (float t = 0; t < 1.0f; t += fStepSize)
		{
			new_point = GetSplinePoint((float)node + t, bLooped);

			fLenght += sqrtf((new_point.x - old_point.x) * (new_point.x - old_point.x)
				+ (new_point.y - old_point.y) * (new_point.y - old_point.y));

			old_point = new_point;
		}
		return fLenght;
	}


	float GetNormalisedOffst(float p) {
		int i = 0;
		while (p > points[i].lenght)
		{
			p -= points[i].lenght;
			i++;

		}
		return(float)i + (p / points[i].lenght);


	}



};

class OneLoneCoder_Splines
{
public:

private:
	sSpline path;
	int nSelectedPoint = 0;
	float fMarker = 0.0f;
	int PressedKey;
	int MouseX, MouseY;

public://protected:
	// Called by olcConsoleGameEngine
	OneLoneCoder_Splines(void)
	{
		//path.points = { { 10, 41 },{ 40, 41 },{ 70, 41 },{ 100, 41 } };
		path.points = { { 100, 100 },{ 200, 200 },{ 300, 300 },{ 400, 400 },{ 500, 500 },{ 100, 600 }, };
		//return true;
	}

	void Mouse(int xpos, int ypos, bool Pushed)
	{
		static bool Locked = false;
		int SelNow = -1;
		float Aux1, Aux2;

		MouseX = xpos;
		MouseY = ypos;

		if (Pushed) {

			if (Locked) {
				path.points[nSelectedPoint].y = ypos;
				path.points[nSelectedPoint].x = xpos;
			}
			else {
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

		}
		else
			Locked = false;

	}

	void Keyboard(int Code, bool push)
	{
		if (push)
			PressedKey = Code;
		else
			PressedKey = 0;

	}
	
	virtual bool OnUserUpdate(float fElapsedTime)
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
		else if (path.points[nSelectedPoint].x >= egtkGetScreenW() )
			path.points[nSelectedPoint].x = egtkGetScreenW()-1;

		if (PressedKey == VK_UP)
			path.points[nSelectedPoint].y -= 1.0f /* * fElapsedTime*/;

		if (PressedKey == VK_DOWN)
			path.points[nSelectedPoint].y += 1.0f /* * fElapsedTime*/;

		if (path.points[nSelectedPoint].y < 0)
			path.points[nSelectedPoint].y = 0;
		else if (path.points[nSelectedPoint].y >= egtkGetScreenH())
			path.points[nSelectedPoint].y = egtkGetScreenH()-1;


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
			egtkWriteString(Cstr,false);
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

		int xposNow = egtkGetScreenW()- 300;

		sprintf_s(Cstr, sizeof(Cstr), "Position=%f,%f", p1.x, p1.y);
		egtkSetPos(xposNow, 10);
		egtkWriteString(Cstr, false);

		sprintf_s(Cstr, sizeof(Cstr), "Gradient=%f,%f", g1.x, g1.y);
		egtkSetPos(xposNow, 30);
		egtkWriteString(Cstr, false);

		sprintf_s(Cstr, sizeof(Cstr), "Line 1=%f,%f", line1.x, line1.y);
		egtkSetPos(xposNow, 50);
		egtkWriteString(Cstr, false);

		sprintf_s(Cstr, sizeof(Cstr), "Line 2=%f,%f", line2.x, line2.y);
		egtkSetPos(xposNow, 70);
		egtkWriteString(Cstr, false);




		sprintf_s(Cstr, sizeof(Cstr), "Mouse=%d,%d", MouseX, MouseY);
		egtkSetPos(10, egtkGetScreenH()-30);
		egtkWriteString(Cstr, false);


		return true;
	}
};

OneLoneCoder_Splines SplineReal;

void KeyboardSpline(int keycode, bool push)
{
	SplineReal.Keyboard(keycode, push);
}

void InitSpline(void)
{
	egtkInitScreen(700, 500, RGB565_WHITE, RGB565_BLACK);
}

void GestSpline(void)
{
	int TimerSys;
	static int LastTimerSys = 0;


	TimerSys = GetTickCount();

    //egtkDrawGlyph(200, 200, RGB565_WHITE, RGB565_BLACK, &Elcos,false);

	SplineReal.OnUserUpdate((TimerSys - LastTimerSys) / 100);

	LastTimerSys = TimerSys;
}

void MouseSpline(int xpos, int ypos, bool Pushed)
{

	SplineReal.Mouse(xpos, ypos, Pushed);

}

