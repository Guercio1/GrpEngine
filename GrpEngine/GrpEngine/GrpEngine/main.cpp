#include <stdint.h>
#include "VSbridge.h"
#include "..\Examples\TestLibrary.h"
#include "..\Examples\spline.h"
#include "..\egtk\egtk.h"

#define TEST_SPLINE 
//#define TEST_LIBRARY

void KeyboardCallback(int keycode, bool push)
{
#ifdef TEST_SPLINE 
    KeyboardSpline(keycode, push);
#endif
}

void MouseCallback(int xpos, int ypos, bool Push)
{
#ifdef TEST_SPLINE 
    MouseSpline(xpos, ypos, Push);
#endif
}

void InitScreen(void)
{
#ifdef TEST_SPLINE 
    InitSpline();
#endif

#ifdef TEST_LIBRARY
    TestLibraryInit();
#endif
}
void DrawScreen(void)
{
#ifdef TEST_SPLINE 
    GestSpline();
#endif
#ifdef TEST_LIBRARY
    TestLibraryGest();
#endif
}