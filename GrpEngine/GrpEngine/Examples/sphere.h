
using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <stdint.h>

#include "..\egtk\egtk.h"

typedef struct {
	float x;
	float y;
	float z;
}Vector;

typedef struct {
	int x;
	int y;
}Point;


typedef struct {
	int cntrX;
	int cntrY;
	int Radius;
	int Control;
	int PosX;
	int PosY;
	bool selected;
}Control;


class	SpherelightClass
{
public:
	SpherelightClass() {};
private:
	Vector Light;
	Control ContLight;
	int SphereRadius;
	int mouseX, mouseY;
	float ReflectLight;
	
	

	void VectNormalize(Vector*);
	float dotProduct(Vector*, Vector*);
	void DrawSphere(uint16_t , uint16_t , uint16_t , egtk_COL_T );
	void GestControl(void);
	void DrawReference(void);
	void DrawReflexLight(void);

public:
	bool OnUserCreate(void);
	bool OnUserUpdate(float);
	void OnUserMouse(int, int, MOUSE_TYPE);
	void OnUserKeyboard(int, bool) {};
};


void InitSphere(void);

void GestSphere(void);
