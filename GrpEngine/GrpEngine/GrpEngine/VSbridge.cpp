#define WIN32_LEAN_AND_MEAN
//#define UNICODE
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windowsx.h>
#include "VSbridge.h"


typedef uint32_t u32;

#include "setup.h"


void* memory;
int client_width;
int client_height;

LRESULT CALLBACK
window_proc(HWND window,
    UINT message,
    WPARAM w_param,
    LPARAM l_param)
{
    int xpos, ypos;
    MOUSE_TYPE MouseAct = MOUSE_RGH_NONE;
    bool KeyPush; 

    switch (message)
    {
        

        case WM_MOUSEMOVE:
            if (w_param == MK_LBUTTON)
                MouseAct = MOUSE_MOV_L_PSH;

            else if (w_param == MK_RBUTTON)
                MouseAct = MOUSE_MOV_R_PSH;

            else
                MouseAct = MOUSE_MOV;

            goto MouseCoord;
        case WM_RBUTTONUP:
            MouseAct = MOUSE_RGH_REL;
            goto MouseCoord;
        case WM_RBUTTONDOWN:
            MouseAct = MOUSE_RGH_PSH;
            goto MouseCoord;
        case WM_LBUTTONUP:
            MouseAct = MOUSE_LFT_REL;
            goto MouseCoord;
        case WM_LBUTTONDOWN:
        {
            MouseAct = MOUSE_LFT_PSH;

            MouseCoord:
            xpos = GET_X_LPARAM(l_param);
            ypos = GET_Y_LPARAM(l_param);

            MY_TEST.OnUserMouse(xpos, ypos, MouseAct);
            //MouseCallback(xpos, ypos, MouseAct);
            break;
        }
        case WM_KEYUP:
            KeyPush = false;
            goto KeyCode;
        case WM_KEYDOWN:
            KeyPush = true;
            KeyCode:
            MY_TEST.OnUserKeyboard(w_param, KeyPush);
            break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;

        default:
        {
            return DefWindowProc(window,
                message,
                w_param,
                l_param);
        }
    }

    return 0;
}

int WINAPI
wWinMain(HINSTANCE instance,
    HINSTANCE prev_instance,
    PWSTR cmd_line,
    int cmd_show)
{
    // window creation

    WNDCLASS window_class = {};

    const wchar_t class_name[] = L"MyWindowClass";

    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = instance;
    window_class.lpszClassName = class_name;
    window_class.hCursor = LoadCursor(0, IDC_CROSS);

    if (!RegisterClass(&window_class))
    {
        MessageBox(0, L"RegisterClass failed", 0, 0);
        return GetLastError();
    }

    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    MY_TEST.OnUserCreate();
    //InitScreen();


    HWND window = CreateWindowEx(0,
        class_name,
        L"egtk",
        WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        instance,
        0);


    if (!window)
    {
        MessageBox(0, L"CreateWindowEx failed", 0, 0);
        return GetLastError();
    }

    // allocate memory
    memory = VirtualAlloc(0,
        client_width * client_height * 4,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    // create BITMAPINFO struct for StretchDIBits

    BITMAPINFO bitmap_info;
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = client_width;
    bitmap_info.bmiHeader.biHeight = client_height;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(window);

    // loop

    {

        DWORD dwStyle = GetWindowLongPtr(window, GWL_STYLE);
        DWORD dwExStyle = GetWindowLongPtr(window, GWL_EXSTYLE);
        HMENU menu = GetMenu(window);

        RECT rc = { 0, 0, client_width, client_height };

        AdjustWindowRectEx(&rc, dwStyle, menu ? TRUE : FALSE, dwExStyle);

        SetWindowPos(window, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);

    }


    bool running = true;

    while (running)
    {
        MSG msg;
        static uint32_t TimerLast;
        static uint32_t TimerRevious;
        uint32_t TimerNow;
        static uint16_t CntFrame = 0;
        wchar_t  buf[200];


        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        CntFrame++;
        TimerNow = GetTickCount();

        float Counter = (float)(TimerNow - TimerRevious) / 1000;
        const float CounterMax = 0.1f;
        if (Counter > CounterMax)
            Counter = CounterMax;

        MY_TEST.OnUserUpdate(Counter);

        TimerRevious = TimerNow;

        if ((TimerNow - TimerLast) > 1000)
        {
            wsprintf(buf, L"Screen %dx%d FPS = %d", client_width, client_height, CntFrame);
            //wsprintf(buf, L"Screen %dx%d TIME = %d", client_width, client_height, (TimerNow - TimerLast));
            SetWindowText(window, buf);

            TimerLast = TimerNow;
            CntFrame--;
        }

        StretchDIBits(hdc,
            0,
            0,
            client_width,
            client_height,
            0,
            0,
            client_width,
            client_height,
            memory,
            &bitmap_info,
            DIB_RGB_COLORS,
            SRCCOPY
        );

    }

    return 0;
}