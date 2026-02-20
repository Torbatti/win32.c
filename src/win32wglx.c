#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#define GLAD_GL_IMPLEMENTATION
#define GLAD_WGL_IMPLEMENTATION
#include <glad/gl33core.h>
#include <glad/wgl10x.h>

LRESULT CALLBACK
win32_callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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

  // Set the pixel format for the device context:
  PIXELFORMATDESCRIPTOR pixelformatd = {};
  pixelformatd.nSize                 = sizeof(pixelformatd);
  pixelformatd.nSize                 = sizeof(PIXELFORMATDESCRIPTOR);
  pixelformatd.dwFlags
      = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pixelformatd.iPixelType = PFD_TYPE_RGBA;
  pixelformatd.cColorBits = 32;
  pixelformatd.cDepthBits = 32;
  pixelformatd.iLayerType = PFD_MAIN_PLANE;

  int format = ChoosePixelFormat(HDeviceContext, &pixelformatd);
  assert(format != 0);

  int setpixelformat_result
      = SetPixelFormat(HDeviceContext, format, &pixelformatd);
  assert(setpixelformat_result != FALSE);

  HGLRC temp_gl_context = wglCreateContext(HDeviceContext);
  assert(temp_gl_context != NULL);

  wglMakeCurrent(HDeviceContext, temp_gl_context);

  int gladloadwgl_result = gladLoaderLoadWGL(HDeviceContext);
  assert(gladloadwgl_result);

  int attributes []
      = {WGL_CONTEXT_MAJOR_VERSION_ARB,
         3,
         WGL_CONTEXT_MINOR_VERSION_ARB,
         3,
         WGL_CONTEXT_FLAGS_ARB,
         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
         0};

  HGLRC gl_context
      = wglCreateContextAttribsARB(HDeviceContext, NULL, attributes);
  assert(gl_context != NULL);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(temp_gl_context);
  wglMakeCurrent(HDeviceContext, gl_context);

  int gladloadgl_result = gladLoaderLoadGL();
  assert(gladloadgl_result);

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

    // @todo: add xinput
    // @brief: polling and handling xinput

    glClearColor(0.65f, 0.0f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SwapBuffers(HDeviceContext);
  }

  if(gl_context)
    wglDeleteContext(gl_context);
  if(HDeviceContext)
    ReleaseDC(HWindow, HDeviceContext);
  if(HWindow)
    DestroyWindow(HWindow);

  return 0;
}

LRESULT CALLBACK
win32_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
    case WM_QUIT :
    case WM_CLOSE   : DestroyWindow(hWnd); break;
    case WM_DESTROY : PostQuitMessage(0); break;
    default         : return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}
