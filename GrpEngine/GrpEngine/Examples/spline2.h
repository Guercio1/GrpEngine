using namespace std;
#include <iostream>
#include <string>
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
	OneLoneCoder_Splines(void);

private:
	sSpline path;
	int nSelectedPoint = 0;
	float fMarker = 0.0f;
	int PressedKey;
	int MouseX, MouseY;

public:
	void OnUserCreate(void);
	void OnUserMouse(int, int, MOUSE_TYPE);
	void OnUserKeyboard(int, bool);
	bool OnUserUpdate(float);
};





