#include "defines.h"
#include "functions.h"

extern Camera Player;
extern ObjectList CurrentObject;
extern char worldname[];

extern IDirect3D9 *pD3D;
extern IDirect3DDevice9 *pDevice;

extern int CurrentScene, CurrentFrame, OldScene, OldFrame;
extern RECT StartRect;

HINSTANCE GlobalhInst;
char chClassName[] = "WindClass";
//-------
extern WindowFrame MainMenu;
extern WindowFrame PlayMenu;
extern WindowFrame BlockChooseMenu;
//-------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpszCmdLine, int nCmdShow) {
  //---
  bool change_to_scene = 1, enable_cursor = 1;
  //---
  srand(time(NULL));
  GlobalhInst = hInstance;
  if (! WndClass(hInstance, WndProc, chClassName))
    return 0;
  
  HWND hWnd = CreateMainWindow(hInstance, "TerrainCreator v3.0.0 (beta) SBstudio © 2017", CW_USEDEFAULT, CW_USEDEFAULT);
  ShowWindow(hWnd, SW_SHOWMAXIMIZED);
  
  RECT rc;
  GetClientRect(hWnd, &rc);
  StartRect = rc;
  
  int StartLTime, CurrentLTime;
  CurrentLTime = StartLTime = (float)clock() / CLOCKS_PER_SEC;
  InitLoadingScreen();
  DrawLoadingScreen(hWnd);
  
  if (! D3DInit(hWnd, &pD3D, &pDevice))
    return 0;
  
  if (! InitResources())
    return 0;

  while (CurrentLTime - StartLTime < 4.0f) {
	CurrentLTime = (float)clock() / CLOCKS_PER_SEC;
  }
  
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  while(msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      DispatchMessage(&msg);
      TranslateMessage(&msg);
    }
    else { //действия в каждом цикле игры
      if (GetForegroundWindow() != hWnd && CurrentScene == 2)
        CurrentFrame = 11;
      if (GetForegroundWindow() != hWnd || CurrentScene != 2 || CurrentFrame != 0) {
        change_to_scene = 1;
        if (! enable_cursor) {
          ShowCursor(true);
          enable_cursor = true;
        }
      }
      else {
        if (enable_cursor) {
          ShowCursor(false);
          enable_cursor = false;
        }
      }
      //input (ввод данных)
      if (CurrentScene == 1) {
        switch (CurrentFrame) {
          case 0:
            break;
        }
      }
      if (CurrentScene == 2) {
        switch (CurrentFrame) {
          case 0:
            RotatePlayer(hWnd, &Player, &change_to_scene);
            //MovePlayer();
            break;
        }
      }
      //обновления
      if (CurrentObject.get_update())
          UpdateIndicate();
      if (! UpdateScene())
        return 0;
      //draw (отрисовка)
      if (CurrentScene == 1) {
        Render1();
      }
      if (CurrentScene == 2) {
        Render2();
      }
    }
  }
  DeleteResources();
  D3DRelease(&pD3D, &pDevice);
  return msg.wParam;
}

bool WndClass(HINSTANCE hInst, WNDPROC WindowProc, LPCTSTR szClassName) {
  WNDCLASS wc;
  wc.lpfnWndProc = (WNDPROC)WindowProc;
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = wc.cbWndExtra = 0;
  wc.hInstance = hInst;
  wc.hIcon = LoadIcon(hInst, "icon");
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
  wc.lpszMenuName = (LPCTSTR)NULL;
  wc.lpszClassName = szClassName;
  return RegisterClass(&wc);
}

HWND CreateMainWindow(HINSTANCE hInst, LPCSTR title, int width, int height) {
  return CreateWindow(chClassName, title, WS_OVERLAPPEDWINDOW,
			   CW_USEDEFAULT, CW_USEDEFAULT,
			   width, height,
			   0, 0, hInst, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
static RECT rc;
char strfile[256]; 
//input
if (CurrentScene == 2 && CurrentFrame == 0)
  MovePlayer(hWnd, msg, wParam, lParam, &Player);

if (CurrentScene && CurrentFrame)
  MessagesToFrame(hWnd, msg, wParam, lParam, GetFrame(CurrentScene, CurrentFrame));

if (OldScene && OldFrame && (OldScene != CurrentScene || OldFrame != CurrentFrame))
  GetFrame(OldScene, OldFrame)->rect_out();

OldScene = CurrentScene;
OldFrame = CurrentFrame;

switch(msg) {
  case WM_KEYDOWN:
    switch(LOWORD(wParam)) {
      case VK_ESCAPE:
        if (CurrentScene == 2) {
          if (CurrentFrame == 0)
            CurrentFrame = 11;
          else
              CurrentFrame = 0;
        }
        break;
        
      case 0x45:
        if (CurrentScene == 2) {
          if (CurrentFrame == 0)
            CurrentFrame = 12;
          else
            if (CurrentFrame == 12) {
              CurrentFrame = 0;
            }
        }
        break;
    }
    break;
  
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
    
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
return 0;
}

bool D3DInit(HWND hWnd, IDirect3D9 **pD3D, IDirect3DDevice9 **pDevice) {
  if (NULL == (*pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    return false;

  D3DCAPS9 caps;
  DWORD vp = 0;
  (*pD3D)->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
  if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
	vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
  }
  else {
	vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
  }
  
  
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory(&d3dpp, sizeof(d3dpp));
  d3dpp.Windowed = true;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  d3dpp.EnableAutoDepthStencil = true;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  HRESULT hr;
  hr = (*pD3D)->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                          vp, &d3dpp, &(*pDevice));
  if( FAILED(hr) || !(*pDevice))
    return false;
  
  return true;
}

void D3DRelease(IDirect3D9 **pD3D, IDirect3DDevice9 **pDevice) {
  DeleteBuff();
  
  if (*pDevice)
    (*pDevice)->Release();
  if (*pD3D)
    (*pD3D)->Release();
}