
using namespace std;

#include <iostream>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>

#include "..\egtk\egtk.h"



struct vec3d
{
	float x;
	float y;
	float z;
};

struct triangle
{
	vec3d p[3];

	RGB888_TYPE col;
};

struct mesh
{
	vector<triangle>tris;

	bool LoadFromObjectFile(string);
};

struct mat4x4
{
	float m[4][4] = { 0 };
};



class	eng3DClass
{
public:
	eng3DClass() {};
private:
	mesh meshCube;
	mat4x4 matProj;

	// Projection Matrix
	float fNear;
	float fFar;
	float fFov;
	float fAspectRatio;
	float fFovRad;
	float fTheta;
	vec3d vCamera;

	void MultiplyMatrixVector(vec3d&, vec3d&, mat4x4&);


public:
	bool OnUserCreate(void) ;
	bool OnUserUpdate(float) ;
	void OnUserMouse(int, int, MOUSE_TYPE) {};
	void OnUserKeyboard(int, bool) {};
};

