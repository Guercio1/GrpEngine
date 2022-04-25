#include <stdint.h>
#include "VSbridge.h"
#include "..\Examples\TestLibrary.h"
#include "..\Examples\spline.h"
#include "..\egtk\egtk.h"


void KeyboardCallback(int keycode, bool push)
{
    //KeyboardSpline(keycode, push);
}

void MouseCallback(int xpos, int ypos, bool Push)
{
    //MouseSpline(xpos, ypos, Push);
}

void InitScreen(void)
{
    //InitSpline();

    TestLibraryInit();
}
void DrawScreen(void)
{
    //GestSpline();

    TestLibraryGest();
}