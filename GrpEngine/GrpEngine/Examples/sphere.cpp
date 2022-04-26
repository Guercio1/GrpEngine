#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "..\egtk\egtk.h"

typedef struct {
	float x;
	float y;
	float z;
}VECTOR;

void normalize(VECTOR* pVect)
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

void InitSphere(void)
{
	egtkInitScreen(700, 500, RGB565_BLACK, RGB565_WHITE);
}



void GestSphere(void)
{
	egtkClearScreen();




}