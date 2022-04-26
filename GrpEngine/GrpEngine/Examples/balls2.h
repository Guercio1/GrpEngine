
using namespace std;
#include <iostream>
#include <vector>
#include <string>

#include "..\egtk\egtk.h"


struct sBall
{
	float px, py;
	float vx, vy;
	float ax, ay;
	float radius;
	float mass;

	int id;
};

class CirclePhysics
{
public:
	CirclePhysics();
private:
	vector <sBall> vecBalls;
	sBall* pSelectedBall = nullptr;
	int MouseX, MouseY;

	void AddBall(float, float, float);
	void DrawBall(sBall*);
public:
	bool OnUserCreate(void);
	bool OnUserUpdate(float);
	void OnUserMouse(int, int, MOUSE_TYPE);
	void OnUserKeyboard(int, bool);
};



