
using namespace std;
#include <iostream>
#include <vector>
#include <string>

#include "..\egtk\egtk.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


struct sBall
{
	float px, py;
	float vx, vy;
	float ax, ay;
	float ox, oy;
	float radius;
	float mass;

	float fSimTimeRemaining;

	int id;
};


struct sLineSegment
{
	float sx, sy;
	float ex, ey;
	float radius;
};


class CirclePhysics
{
public:
	CirclePhysics() {};
private:
	vector <sBall> vecBalls;
	sBall* pSelectedBall = nullptr;

	vector<sLineSegment> vecLines;
	sLineSegment* pSelectedLine = nullptr;
	bool bSelectedLineStart = false;

	int MouseX, MouseY;

	void AddBall(float, float, float);
	void DrawBall(sBall*);
	void DrawLineSegment(sLineSegment*);
public:
	bool OnUserCreate(void);
	bool OnUserUpdate(float);
	void OnUserMouse(int, int, MOUSE_TYPE);
	void OnUserKeyboard(int, bool);
};



