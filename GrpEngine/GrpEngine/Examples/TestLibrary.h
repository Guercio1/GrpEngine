using namespace std;
#include <iostream>



class TestLibraryClass
{
public:
	TestLibraryClass(void) {};

public:
	void OnUserCreate(void);
	void OnUserMouse(int, int, MOUSE_TYPE) {};
	void OnUserKeyboard(int, bool) {};
	bool OnUserUpdate(float) ;
};
