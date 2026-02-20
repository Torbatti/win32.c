#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK
win32_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int
main()
{
  // @brief: if main is used instead of WinMain as the main entry point
  //         need to get hInstance from GetModuleHandle()
  HMODULE hInstance = GetModuleHandleA(NULL);
  assert(hInstance != NULL);

  WNDCLASSEXA window_class_exa   = {0};
  window_class_exa.cbSize        = sizeof(WNDCLASSEXA);
  window_class_exa.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  window_class_exa.lpfnWndProc   = win32_callback;
  window_class_exa.cbClsExtra    = 0;
  window_class_exa.cbWndExtra    = 0;
  window_class_exa.hInstance     = hInstance;
  window_class_exa.hIcon         = 0;
  window_class_exa.hCursor       = 0;
  window_class_exa.hbrBackground = 0;
  window_class_exa.lpszMenuName  = 0;
  window_class_exa.lpszClassName = "MainWindowClass";
  window_class_exa.hIconSm       = 0;

  ATOM wndclass = RegisterClassExA(&window_class_exa);
  assert(wndclass != 0);

  LPCSTR win32_mainWindowName = "Main Window";
  int    window_x             = CW_USEDEFAULT;
  int    window_y             = CW_USEDEFAULT;
  int    window_width         = CW_USEDEFAULT;
  int    window_height        = CW_USEDEFAULT;
  HWND   HWindow              = CreateWindowExA(
      0, window_class_exa.lpszClassName, win32_mainWindowName,
      WS_OVERLAPPEDWINDOW | WS_VISIBLE, window_x, window_y,
      window_width, window_height, 0, 0, hInstance, 0
  );
  assert(HWindow != NULL);

  HDC HDeviceContext = GetDC(HWindow);
  assert(HDeviceContext != NULL);

  ShowWindow(HWindow, SW_SHOWDEFAULT);
  UpdateWindow(HWindow);

  int quit = 0;
  MSG Message;
  while(quit != 1)
  {
    // @brief: peek for messages
    int peek_message = 1;
    while(peek_message == 1)
    {
      BOOL peek_message_resault
          = PeekMessageA(&Message, 0, 0, 0, PM_REMOVE);

      // @brief: check if any message is available, 0 means no message
      if(peek_message_resault == 0)
      {
        peek_message = 0;
      }

      // @brief: handle messages
      switch(Message.message)
      {
        case WM_QUIT :
          {
            quit = 1;
          }
          break;
      }

      // @todo: handle error
      // @todo: what `TranslateMessage()` and `DispatchMessageA()` are supposed to do
      TranslateMessage(&Message);
      DispatchMessageA(&Message);
    }
  }

  // @todo: add xinput
  // @brief: polling and handling xinput

  if(HDeviceContext != NULL)
    ReleaseDC(HWindow, HDeviceContext);
  if(HWindow != NULL)
    DestroyWindow(HWindow);
}

LRESULT CALLBACK
win32_callback(HWND HWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
    case WM_QUIT :
    case WM_CLOSE   : DestroyWindow(HWindow); break;
    case WM_DESTROY : PostQuitMessage(0); break;
    default         : return DefWindowProc(HWindow, uMsg, wParam, lParam);
  }

  return 0;
}

//
// @links:
// - GetModuleHandleA(): https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea
// - RegisterClassExA(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
// - CreateWindowExA(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
// - GetDC(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdc
// - ShowWindow(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
// - UpdateWindow(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-updatewindow
// - PeekMessageA(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
// - TranslateMessage(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
// - DispatchMessageA(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessagea
// - ReleaseDC(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-releasedc
// - DestroyWindow(): https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroywindow
//
