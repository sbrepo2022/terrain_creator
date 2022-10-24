/*
Здесь содержатся функции игры. Объявления функций в файле functions.h
*/

#include "defines.h"
#include "functions.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

Camera Player;  //объект камеры
ObjectList CurrentObject; //итератор блоков и текстур
char worldname[256] = "world.tc"; //название мира по умолчанию

const int CurrentLoaderVersion = 1; //версия загрузчика
const int maxX = 32, maxY = 32, maxZ = 32, maxObjTriangleNum = 16384;
//размеры карты и максимальный размер буфера полигонов
const float RenderDistance = 128.0f; //дистанция рендрединга
const int IndSizeMod = 5; // 1/IndSizeMod * размер экрана - размер индикации блоков

const int NumSkyboxes = 2;
int CurrentSkybox = 0;

int CurrentScene, CurrentFrame, OldScene, OldFrame;
RECT StartRect;

IDirect3D9 *pD3D; //directx объект
IDirect3DDevice9 *pDevice; //девайс
ObjectCube FieldArray[maxY][maxZ][maxX]; //массив карты

IDirect3DVertexBuffer9 *pVB; //буфер вершин карты
IDirect3DVertexBuffer9 *pVBS; //буфер вершин скайбокса
IDirect3DVertexBuffer9 *pVBIndicate; //буфер вершин индикации блока
//--------------
//текстуры экрана загрузки
ImgPrint LoadingScreen4_3;
ImgPrint LoadingScreen16_9;
//для вывода индикации блоков
ID3DXSprite *IndicateSprite;
IDirect3DTexture9 *IndicateTexture;
//перекрестие
IDirect3DTexture9 *CrossTexture;
//текстуры скайбокса
IDirect3DTexture9 *SkyBoxT1[NumSkyboxes];
IDirect3DTexture9 *SkyBoxT2[NumSkyboxes];
IDirect3DTexture9 *SkyBoxT3[NumSkyboxes];
IDirect3DTexture9 *SkyBoxT4[NumSkyboxes];
IDirect3DTexture9 *SkyBoxT5[NumSkyboxes];
IDirect3DTexture9 *SkyBoxT6[NumSkyboxes];
//текстуры кнопок
IDirect3DTexture9 *TextureButtonPlay;
IDirect3DTexture9 *TextureButtonSettings;
IDirect3DTexture9 *TextureButtonCreator;
IDirect3DTexture9 *TextureButtonExit;

IDirect3DTexture9 *TextureButtonContinue;
IDirect3DTexture9 *TextureButtonNew;
IDirect3DTexture9 *TextureButtonLoad;
IDirect3DTexture9 *TextureButtonSave;
IDirect3DTexture9 *TextureButtonToMenu;

IDirect3DTexture9 *TextureButtonClose;
IDirect3DTexture9 *TextureButtonScroll1;
IDirect3DTexture9 *TextureButtonScroll2;
IDirect3DTexture9 *TextureButtonScroll3;
IDirect3DTexture9 *TextureButtonScroll4;
IDirect3DTexture9 *TextureButtonEffect;
IDirect3DTexture9 *TextureSpecialRender;
IDirect3DTexture9 *TextureSwitchTexture;
IDirect3DTexture9 *TextureSwitchOriginTexture;
IDirect3DTexture9 *TextureCreditsPict;
//текстуры кнопок меню выбора блоков
std::vector<IDirect3DTexture9*> TexturesBlockChoose;
//текстуры блоков
IDirect3DTexture9 *Texture[14];
//Frames
WindowFrame Background;
Group GroupBackground;
Group GroupSimple;
//---
WindowManager ManagerStd;
WindowManagerSimple ManagerSimple;
WindowManagerWindowed ManagerWindowed;
WindowManagerHorizontal2 ManagerWindowBoard;
WindowAnimation AnimationStd;
WindowAnimationSimple AnimationSimple;
WindowAnimationFill AnimationFill;

WindowFrame MainMenu;
Group GroupMainMenu;
WindowManagerVertical1 ManagerMainMenu;
Button ButtonPlay, ButtonSettings, ButtonCreator, ButtonExit;
WindowAnimationType2 AnimationMainMenu[4];
//---
WindowFrame PlayMenu;
Group GroupPlayMenu;
WindowManagerVertical1 ManagerPlayMenu;
Button ButtonContinue, ButtonNew, ButtonLoad, ButtonSave, ButtonToMenu;
WindowAnimationType2 AnimationPlayMenu[5];
//---
WindowFrame BlockChooseMenu;
WindowAnimationWindowed AnimationBlockChooseWindow;
Group GroupBlockChooseMenu, GroupBlockChooseBoard, GroupBlockChooseCanvas;

Button ButtonBlockChooseExit;
WindowAnimationType2 AnimationBlockChooseExit;

Tab TabBlockChoose;
WindowManagerTab ManagerTabBlockChoose;
Group GroupSwitches;
WindowManagerHorizontal2 ManagerSwitches;
WindowAnimationType2 AnimationGroupSwitches;
Group GroupBlocks, GroupTextures, GroupOriginTextures;
WindowManagerMatrix ManagerGroupChoose;
Button ButtonSwitchBlocks, ButtonSwitchTextures, ButtonSwitchOriginTextures;
WindowAnimationType1 AnimationSwitchBlocks, AnimationSwitchTextures, AnimationSwitchOriginTextures;
Button *ButtonBlocks, *ButtonTextures;
WindowAnimationType1 *AnimationButtonBlocks, *AnimationButtonTextures;
Group *GroupsOriginTextures;
Button **ButtonOriginTextures;
WindowAnimationType1 **AnimationButtonOriginTextures;

Group GroupCanvasSeparate;
WindowManagerBlock ManagerCanvasSeparate;
Group GroupScrollBlock;
WindowManagerHorizontal2 ManagerScrollBlock;
Group GroupScroll;
WindowManagerBlock ManagerScroll;
Button ButtonScrollUp, ButtonScrollDown;
WindowAnimationType1 AnimationButtonScrollUp, AnimationButtonScrollDown;
//---
WindowFrame CreditsMenu;
WindowAnimationWindowed AnimationCreditsWindow;
WindowManagerHorizontal2 ManagerCreditsPict;
Group GroupCreditsMenu, GroupCreditsBoard, GroupCreditsCanvas;

Button ButtonCreditsExit, ButtonCreditsPict;
WindowAnimationType2 AnimationCreditsExit;
WindowAnimationSimple AnimationCreditsPict;
//---
WindowFrame SettingsMenu;
WindowAnimationWindowed AnimationSettingsWindow;
WindowManagerBlock ManagerSkyBChoose; 
WindowManagerHorizontal2 ManagerSkyB;
Group GroupSettingsMenu, GroupSettingsBoard, GroupSettingsCanvas, GroupSkyB, GroupSkyBIn, GroupSkyBChoose;

WindowAnimationType1 AnimationSkyBLeft, AnimationSkyBRight;
WindowAnimationSimple AnimationSkyBChoose[NumSkyboxes];
Button ButtonSettingsExit, ButtonSkyBLeft, ButtonSkyBRight, ButtonSkyBChoose[NumSkyboxes];
WindowAnimationType2 AnimationSettingsExit;
//--------------
void Render1() {
  pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
  if (SUCCEEDED(pDevice->BeginScene())) {
    Background.draw();
	
    switch (CurrentFrame) {
      case 1:
        MainMenu.draw();
        break;
		
	  case 2:
	    SettingsMenu.draw();
	    break;
		
	  case 3:
	    CreditsMenu.draw();
	    break;
    }
    
    pDevice->EndScene();
  }
  
  pDevice->Present(NULL, NULL, NULL, NULL);
}

void Render2() {
  IDirect3DSurface9 *BackSurface; //бэкбуффер
  pDevice->GetRenderTarget(0, &BackSurface);
  
  IDirect3DSurface9 *IndicateSurface; //поверхность для рендера в текстуру
  IndicateTexture->GetSurfaceLevel(0,&IndicateSurface);
  //--
  //рендер основной сцены
  pDevice->SetRenderTarget(0, BackSurface);
  pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
  pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
  if (SUCCEEDED(pDevice->BeginScene())) {
    
    UpdateMaterial();
    UpdateLight();
    UpdateView();
    DrawSkyBox();
    DrawObject();
    
    pDevice->EndScene();
  }
  //рендер сцены индикации блока 
  pDevice->SetRenderTarget(0, IndicateSurface);
  pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(255, 255, 255, 32), 1.0f, 0);
  pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
  if (SUCCEEDED(pDevice->BeginScene())) {
    
    DrawIndicate((float)GetSystemMetrics(SM_CYSCREEN)/GetSystemMetrics(SM_CXSCREEN), 0, NULL);
    
    pDevice->EndScene();
  }
  //--
  pDevice->SetRenderTarget(0, BackSurface);
  
  //отрисовка индикации и перекрестия блока
  if (SUCCEEDED(pDevice->BeginScene())) {
    
    D3DSURFACE_DESC info;
    CrossTexture->GetLevelDesc(0, &info);
    D3DXVECTOR2 CrossSize(StartRect.right / 2 - info.Width / 2, StartRect.bottom / 2 - info.Height / 2);
  
    D3DXMATRIX Mat;
    IndicateSprite->Begin(0);
    
    D3DXMatrixTransformation2D(&Mat, NULL, 0, NULL, NULL, 0, NULL);
    IndicateSprite->SetTransform(&Mat);
    IndicateSprite->Draw(IndicateTexture, NULL, NULL, NULL, 0xFFFFFFFF);
    
    D3DXMatrixTransformation2D(&Mat, NULL, 0, NULL, NULL, 0, &CrossSize);
    IndicateSprite->SetTransform(&Mat);
    IndicateSprite->Draw(CrossTexture, NULL, NULL, NULL, 0xFFFFFFFF);
    
    IndicateSprite->End();
  
    pDevice->EndScene();
  }
  //--
  
  //отрисовка фреймов
  if (SUCCEEDED(pDevice->BeginScene())) {
    
    switch (CurrentFrame) {
      case 11:
        PlayMenu.draw();
        break;
        
      case 12:
        BlockChooseMenu.draw();
        break;
    }
    
    pDevice->EndScene();
  }
  
  pDevice->Present(NULL, NULL, NULL, NULL);
}

void DrawLoadingScreen(HWND hwnd) {
  RECT rc;
  int size;
  GetClientRect(hwnd, &rc);
  
  HDC hdc = GetDC(hwnd);
  if ((float)rc.bottom/rc.right < (9.0f/16)) {
	size = rc.right / 16 * 9;
	rc.right = size / 9 * 16;
	LoadingScreen16_9.set_pos(0, (rc.bottom - size) / 2);
    LoadingScreen16_9.print_ex(hdc, rc.right, size);
  }
  else {
	size = rc.bottom / 9 * 16;
	rc.bottom = size / 16 * 9;
	LoadingScreen16_9.set_pos((rc.right - size) / 2, 0);
    LoadingScreen16_9.print_ex(hdc, size, rc.bottom);
  }
  ReleaseDC(hwnd, hdc);
}

void DrawObject() {
  //установка трансформаций
  D3DXMATRIX Transform;
  D3DXMatrixIdentity(&Transform);
  D3DXMatrixTranslation(&Transform, 0.0f, 0.0f, 0.0f);
  pDevice->SetTransform(D3DTS_WORLD, &Transform);
  //установка состояния рендера
  pDevice->SetRenderState(D3DRS_LIGHTING, true);
  pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
  pDevice->SetStreamSource(0, pVB, 0, sizeof(Vertex));
  pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
  //отрисовка непрозрачных блоков
  int pos = 0;
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object()) {
          if (FieldArray[j][f][i].is_transparent()) {
            pos += FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f)) * 3;
            continue;
          }
          pDevice->SetTexture(0, FieldArray[j][f][i].GetTexture());
          pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, pos, FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f)));
          pos += FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f)) * 3;
        }
      }
    }
  }
  //отрисовка прозрачных блоков
  pos = 0;
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object()) {
          if (! FieldArray[j][f][i].is_transparent()) {
            pos += FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f)) * 3;
            continue;
          }
          pDevice->SetTexture(0, FieldArray[j][f][i].GetTexture());
          pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, pos, FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f)));
          pos += FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f)) * 3;
        }
      }
    }
  }
}

void DrawSkyBox() {
  //перенос центра вывода в позицию игрока
  D3DXVECTOR3 center = Player.GetPosition();
  
  D3DXMATRIX Transform;
  D3DXMatrixIdentity(&Transform);
  D3DXMatrixTranslation(&Transform, center.x, center.y, center.z);
  pDevice->SetTransform(D3DTS_WORLD, &Transform);
  //установка состояния рендера
  pDevice->SetRenderState(D3DRS_LIGHTING, false);
  pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
  pDevice->SetStreamSource(0, pVBS, 0, sizeof(Vertex));
  pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
  //отрисовка скайбокса
  pDevice->SetTexture(0, SkyBoxT1[CurrentSkybox]);
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*0, 2);
  pDevice->SetTexture(0, SkyBoxT2[CurrentSkybox]);
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*1, 2);
  pDevice->SetTexture(0, SkyBoxT3[CurrentSkybox]);
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*2, 2);
  pDevice->SetTexture(0, SkyBoxT4[CurrentSkybox]);
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*3, 2);
  pDevice->SetTexture(0, SkyBoxT5[CurrentSkybox]);
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*4, 2);
  pDevice->SetTexture(0, SkyBoxT6[CurrentSkybox]);
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 6*5, 2);
}

void DrawIndicate(float Vh, int routine, IDirect3DTexture9 *Texture) {
  //установка трансформаций
  D3DXMATRIX Result, Transform, Rotate;
  D3DXMatrixIdentity(&Result);
  D3DXMatrixIdentity(&Transform);
  D3DXMatrixIdentity(&Rotate);
  D3DXMatrixTranslation(&Transform, -0.5f, -0.5f, -0.5f);
  switch (routine) {
    case 0:
      D3DXMatrixRotationY(&Rotate, D3DXToRadian((GetTickCount() / 180)%360));
      break;
      
    case 1:
      D3DXMatrixRotationY(&Rotate, 0);
      break;
  }
  D3DXMatrixMultiply(&Result, &Transform, &Rotate);
  pDevice->SetTransform(D3DTS_WORLD, &Result);
  //установка камеры
  D3DXMATRIX View;
  D3DXVECTOR3 Eye;
  switch (routine) {
    case 0:
      Eye = D3DXVECTOR3(1.5f, 1.2f, 1.5f);
      break;
      
    case 1:
      Eye = D3DXVECTOR3(1.0f, 0.8f, 1.0f);
      break;
  }
  D3DXVECTOR3 LookAt(0.0f, 0.0f, 0.0f);
  D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
  D3DXMatrixLookAtLH(&View, &Eye, &LookAt, &Up);
  pDevice->SetTransform(D3DTS_VIEW, &View);
  //матрица проекций
  float Zn = 0.5f;
  float Zf = 100.0f;
  float Vw = 1.0f;

  D3DMATRIX Projection = {
    2*Zn/Vw, 0, 0, 0,
    0, 2*Zn/Vh, 0, 0,
    0, 0, Zf/(Zf-Zn), 1,
    0, 0, -Zf/(Zf-Zn)*Zn, 0,
  };
  pDevice->SetTransform(D3DTS_PROJECTION, &Projection);
  //установка состояния рендера
  pDevice->SetRenderState(D3DRS_LIGHTING, false);
  pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
  pDevice->SetStreamSource(0, pVBIndicate, 0, sizeof(Vertex));
  pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
  //отрисовка индикации блока
  switch (routine) {
    case 0:
      pDevice->SetTexture(0, CurrentObject.get_object().GetTexture());
      break;
      
    case 1:
      pDevice->SetTexture(0, Texture);
      break;
  }
  pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, CurrentObject.get_object().get_numtriangles(BorderInfo()));
  UpdateView();
}

bool InitResources() {
  //загрузка ресурсов
  if (! InitIndicateSprite())
    return false;
  
  if (! InitTextures())
    return false;
  
  if (! InitObjects())
    return false;
  
  //стандартные установки
  SetBlocks();
  if (! InitTexturesBlockChoose())
    return false;
  Player.SetDistance(10.0f);
  Player.SetMoveType(1);
  OldScene = CurrentScene = 1;
  OldFrame = CurrentFrame = 1;
  
  if (! InitFrames())
    return false;
  
  if (! InitScene())
    return false;
  
  return true;
}

void DeleteResources() {
  DeleteFrames();
}

bool InitLoadingScreen() {
  LoadingScreen4_3.init("special\\screen4_3.bmp", 0, 0);
  LoadingScreen4_3.load();
  LoadingScreen16_9.init("special\\screen16_9.bmp", 0, 0);
  LoadingScreen16_9.load();
}

bool InitIndicateSprite() {
  int Width = GetSystemMetrics(SM_CXSCREEN) / IndSizeMod;
  int Height = GetSystemMetrics(SM_CYSCREEN) / IndSizeMod;
  pDevice->CreateTexture(Width,Height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&IndicateTexture,NULL);
  D3DXCreateSprite(pDevice, &IndicateSprite);
  return true;
}

bool InitTextures() {
  HRESULT hr;
  
  hr = D3DXCreateTextureFromFile(pDevice, "special\\cross.png", &CrossTexture);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox1.png", &SkyBoxT1[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox2.png", &SkyBoxT2[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox3.png", &SkyBoxT3[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox4.png", &SkyBoxT4[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox5.png", &SkyBoxT5[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox6.png", &SkyBoxT6[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox21.png", &SkyBoxT1[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox22.png", &SkyBoxT2[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox23.png", &SkyBoxT3[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox24.png", &SkyBoxT4[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox25.png", &SkyBoxT5[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "skyboxt\\skybox26.png", &SkyBoxT6[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\ground.jpg", &Texture[0]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\wall.jpg", &Texture[1]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\brick.jpg", &Texture[2]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\wood.jpg", &Texture[3]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\iron.jpg", &Texture[4]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\wool.jpg", &Texture[5]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\grass.jpg", &Texture[6]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\glass.png", &Texture[7]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\gold.jpg", &Texture[8]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\silver.jpg", &Texture[9]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\bronze.jpg", &Texture[10]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\roof.jpg", &Texture[11]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\roof2.jpg", &Texture[12]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "textures\\marble.jpg", &Texture[13]);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Play.png", &TextureButtonPlay);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Settings.png", &TextureButtonSettings);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Creator.png", &TextureButtonCreator);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Exit.png", &TextureButtonExit);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Continue.png", &TextureButtonContinue);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\New.png", &TextureButtonNew);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Load.png", &TextureButtonLoad);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Save.png", &TextureButtonSave);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\ToMenu.png", &TextureButtonToMenu);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Close.png", &TextureButtonClose);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Scroll1.png", &TextureButtonScroll1);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Scroll2.png", &TextureButtonScroll2);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Scroll3.png", &TextureButtonScroll3);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Scroll4.png", &TextureButtonScroll4);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\Effect.png", &TextureButtonEffect);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\SpecialRender.png", &TextureSpecialRender);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\SwitchTexture.png", &TextureSwitchTexture);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "ButtonTextures\\SwitchOriginTexture.png", &TextureSwitchOriginTexture);
  if (FAILED(hr)) {
    return false;
  }
  
  hr = D3DXCreateTextureFromFile(pDevice, "Special\\credits.png", &TextureCreditsPict);
  if (FAILED(hr)) {
    return false;
  }
  
  return true;
}

bool InitTexturesBlockChoose() {
  IDirect3DSurface9 *BackSurface; //бэкбуффер
  pDevice->GetRenderTarget(0, &BackSurface);
  IDirect3DTexture9 *Texture;
  IDirect3DSurface9 *Surface; //поверхность для рендера в текстуру
  
  //CurrentObject.set_currentT(CurrentObject.GetInd(BlockChooseTNum));
  //рендер сцены индикации блока 
  for (int i = 0; i < CurrentObject.get_sizeF(); i++) {
    CurrentObject.set_currentF(i);
    UpdateIndicate();
    pDevice->CreateTexture(128,128,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&Texture,NULL);
    Texture->GetSurfaceLevel(0,&Surface);
    pDevice->SetRenderTarget(0, Surface);
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(255, 255, 255, 0), 1.0f, 0);
    pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
    if (SUCCEEDED(pDevice->BeginScene())) {
    
      DrawIndicate(1.0f, 1, TextureSpecialRender);
    
      pDevice->EndScene();
    }
    TexturesBlockChoose.push_back(Texture);
  }
  //--
  pDevice->SetRenderTarget(0, BackSurface);
  
  ButtonBlocks = new Button[CurrentObject.get_sizeF()];
  AnimationButtonBlocks = new WindowAnimationType1[CurrentObject.get_sizeF()];
  CurrentObject.set_currentT(0);
  CurrentObject.set_currentF(0);
  for (int i = 0; i < CurrentObject.get_sizeF(); i++) {
    AnimationButtonBlocks[i].init(pDevice, TexturesBlockChoose[i], TextureButtonEffect);
    ButtonBlocks[i].init(i, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationButtonBlocks[i]);
    ButtonBlocks[i].set_msgstr("block");
    ButtonBlocks[i].set_callbackf(&CallbackBlockChooseMenu);
    GroupBlocks.add_control(&ButtonBlocks[i]);
  }
  GroupBlocks.show_control(true);
  
  ButtonTextures = new Button[CurrentObject.get_sizeT()];
  AnimationButtonTextures = new WindowAnimationType1[CurrentObject.get_sizeT()];
  CurrentObject.set_currentT(0);
  CurrentObject.set_currentF(0);
  TextureInfo textureinfo;
  for (int i = 0; i < CurrentObject.get_sizeT(); i++) {
    textureinfo = CurrentObject.get_textureinfo();
    AnimationButtonTextures[i].init(pDevice, textureinfo.texture, TextureButtonEffect);
    ButtonTextures[i].init(i, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationButtonTextures[i]);
    ButtonTextures[i].set_msgstr("texture");
    ButtonTextures[i].set_callbackf(&CallbackBlockChooseMenu);
    GroupTextures.add_control(&ButtonTextures[i]);
    CurrentObject.nextT();
  }
  GroupTextures.show_control(false);
  
  GroupsOriginTextures = new Group[CurrentObject.get_sizeF()];
  ButtonOriginTextures = new Button*[CurrentObject.get_sizeF()];
  AnimationButtonOriginTextures = new WindowAnimationType1*[CurrentObject.get_sizeF()];
  CurrentObject.set_currentT(0);
  CurrentObject.set_currentF(0);
  BlockInfo blockinfo;
  for (int i = 0; i < CurrentObject.get_sizeF(); i++) {
    blockinfo = CurrentObject.get_blockinfo();
    AnimationButtonOriginTextures[i] = new WindowAnimationType1[blockinfo.textures.size()];
    ButtonOriginTextures[i] = new Button[blockinfo.textures.size()];
    GroupsOriginTextures[i].init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerGroupChoose);
    for (int j = 0; j < blockinfo.textures.size(); j++) {
      AnimationButtonOriginTextures[i][j].init(pDevice, blockinfo.textures[j].texture, TextureButtonEffect);
      ButtonOriginTextures[i][j].init(j, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationButtonOriginTextures[i][j]);
      ButtonOriginTextures[i][j].set_msgstr("origintex");
      ButtonOriginTextures[i][j].set_callbackf(&CallbackBlockChooseMenu);
      GroupsOriginTextures[i].add_control(&ButtonOriginTextures[i][j]);
      GroupsOriginTextures[i].lock_control(true);
    }
    GroupOriginTextures.add_control(&GroupsOriginTextures[i]);
    CurrentObject.nextF();
  }
  GroupOriginTextures.show_control(false);
  
  BlockChooseMenu.set_size(BlockChooseMenu.get_size());
  
  CurrentObject.set_currentT(0);
  CurrentObject.set_currentF(0);
  UpdateIndicate();
  
  return true;
}

bool InitObjects() {
  Triangle *trianglelist; //массив полигонов для загрузки шаблона
  trianglelist = new Triangle[maxObjTriangleNum];
  int numtriangles = 0; //кол-во треугольников в загружаемом объекте
  char filename[256];
  
  strcpy(filename, "meshes\\sphere.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 20);
  
  strcpy(filename, "meshes\\chair.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 21);
  
  strcpy(filename, "meshes\\table.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 22);
  
  strcpy(filename, "meshes\\fence.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 23);
  
  strcpy(filename, "meshes\\fence_corner.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 24);
  
  strcpy(filename, "meshes\\cylinder.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 25);
  
  strcpy(filename, "meshes\\text.obj");
  numtriangles = LoadObject(filename, trianglelist);
  if (! numtriangles)
    return 0;
  ObjectCube::RegisterTemplate(trianglelist, numtriangles, 2017);
  
  delete [] trianglelist;
  
  return 1;
}

bool InitFrames() {
  RECT board;
  board.left = 10;
  board.right = 10;
  board.bottom = 10;
  board.top = 40;
  ManagerWindowed.init(board);
  ManagerWindowBoard.init(2, 0.1f, ALIGN_RIGHT);
  GroupSimple.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerStd);
//--------
  ManagerMainMenu.init(3, 0.5f);
  AnimationMainMenu[0].init(pDevice, TextureButtonPlay, D3DCOLOR_ARGB(128, 255, 254, 0), D3DCOLOR_ARGB(200, 255, 254, 0), 0.25f);
  AnimationMainMenu[1].init(pDevice, TextureButtonSettings, D3DCOLOR_ARGB(128, 0, 0, 255), D3DCOLOR_ARGB(200, 0, 0, 255), 0.25f);
  AnimationMainMenu[2].init(pDevice, TextureButtonCreator, D3DCOLOR_ARGB(128, 0, 254, 0), D3DCOLOR_ARGB(200, 0, 254, 0), 0.25f);
  AnimationMainMenu[3].init(pDevice, TextureButtonExit, D3DCOLOR_ARGB(128, 255, 0, 0), D3DCOLOR_ARGB(200, 255, 0, 0), 0.25f);
  AnimationSimple.init(pDevice, Texture[1]);
  AnimationFill.init(pDevice, Texture[3]);
  
  GroupBackground.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationFill, &ManagerSimple);
  Background.init(&GroupBackground, D3DXVECTOR2(0, 0), D3DXVECTOR2(StartRect.right, StartRect.bottom));
  
  GroupMainMenu.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerMainMenu);
  
  ButtonPlay.init(ID_BUTTONPLAY, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationMainMenu[0]);
  ButtonPlay.set_callbackf(&CallbackMainMenu);
  GroupMainMenu.add_control(&ButtonPlay);
  
  ButtonSettings.init(ID_BUTTONSETTINGS, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationMainMenu[1]);
  ButtonSettings.set_callbackf(&CallbackMainMenu);
  GroupMainMenu.add_control(&ButtonSettings);
  
  ButtonCreator.init(ID_BUTTONCREATOR, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationMainMenu[2]);
  ButtonCreator.set_callbackf(&CallbackMainMenu);
  GroupMainMenu.add_control(&ButtonCreator);
  
  ButtonExit.init(ID_BUTTONEXIT, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationMainMenu[3]);
  ButtonExit.set_callbackf(&CallbackMainMenu);
  GroupMainMenu.add_control(&ButtonExit);
  
  MainMenu.init(&GroupMainMenu, D3DXVECTOR2(0, 0), D3DXVECTOR2(StartRect.right, StartRect.bottom));
//--------
  ManagerPlayMenu.init(4, 0.5f);
  AnimationPlayMenu[0].init(pDevice, TextureButtonContinue, D3DCOLOR_ARGB(128, 0, 0, 0), D3DCOLOR_ARGB(196, 0, 0, 0), 0.25f);
  AnimationPlayMenu[1].init(pDevice, TextureButtonNew, D3DCOLOR_ARGB(128, 0, 0, 0), D3DCOLOR_ARGB(196, 0, 0, 0), 0.25f);
  AnimationPlayMenu[2].init(pDevice, TextureButtonLoad, D3DCOLOR_ARGB(128, 0, 0, 0), D3DCOLOR_ARGB(196, 0, 0, 0), 0.25f);
  AnimationPlayMenu[3].init(pDevice, TextureButtonSave, D3DCOLOR_ARGB(128, 0, 0, 0), D3DCOLOR_ARGB(196, 0, 0, 0), 0.25f);
  AnimationPlayMenu[4].init(pDevice, TextureButtonToMenu, D3DCOLOR_ARGB(128, 0, 0, 0), D3DCOLOR_ARGB(196, 0, 0, 0), 0.25f);
  
  GroupPlayMenu.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerPlayMenu);
  
  ButtonContinue.init(ID_BUTTONCONTINUE, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationPlayMenu[0]);
  ButtonContinue.set_callbackf(&CallbackPlayMenu);
  GroupPlayMenu.add_control(&ButtonContinue);
  
  ButtonNew.init(ID_BUTTONNEW, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationPlayMenu[1]);
  ButtonNew.set_callbackf(&CallbackPlayMenu);
  GroupPlayMenu.add_control(&ButtonNew);
  
  ButtonLoad.init(ID_BUTTONLOAD, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationPlayMenu[2]);
  ButtonLoad.set_callbackf(&CallbackPlayMenu);
  GroupPlayMenu.add_control(&ButtonLoad);
  
  ButtonSave.init(ID_BUTTONSAVE, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationPlayMenu[3]);
  ButtonSave.set_callbackf(&CallbackPlayMenu);
  GroupPlayMenu.add_control(&ButtonSave);
  
  ButtonToMenu.init(ID_BUTTONTOMENU, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationPlayMenu[4]);
  ButtonToMenu.set_callbackf(&CallbackPlayMenu);
  GroupPlayMenu.add_control(&ButtonToMenu);
  
  PlayMenu.init(&GroupPlayMenu, D3DXVECTOR2(0, 0), D3DXVECTOR2(StartRect.right, StartRect.bottom));
//--------
  ManagerSwitches.init(1, 0.1f, ALIGN_LEFT);
  ManagerTabBlockChoose.init(100, 0.1f);
  ManagerGroupChoose.init(6, 0.2f);
  AnimationBlockChooseWindow.init(pDevice, NULL, D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(128, 0, 0, 0), board, 0);
  AnimationBlockChooseExit.init(pDevice, TextureButtonClose, D3DCOLOR_ARGB(255, 220, 220, 220), D3DCOLOR_ARGB(255, 196, 0, 0), 0.25f);
  AnimationGroupSwitches.init(pDevice, NULL, D3DCOLOR_ARGB(128, 0, 0, 0), D3DCOLOR_ARGB(128, 0, 0, 0), 0.25f);
  AnimationSwitchBlocks.init(pDevice, TexturesBlockChoose[0], TextureButtonEffect);
  AnimationSwitchTextures.init(pDevice, TextureSwitchTexture, TextureButtonEffect);
  AnimationSwitchOriginTextures.init(pDevice, TextureSwitchOriginTexture, TextureButtonEffect);
  
  GroupBlockChooseMenu.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationBlockChooseWindow, &ManagerWindowed);
  GroupBlockChooseBoard.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerWindowBoard);
   
  ButtonBlockChooseExit.init(ID_BUTTONMENUCLOSE, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationBlockChooseExit);
  ButtonBlockChooseExit.set_callbackf(&CallbackBlockChooseMenu);
  GroupBlockChooseBoard.add_control(&ButtonBlockChooseExit);
   
  GroupBlockChooseMenu.add_control(&GroupBlockChooseBoard);
  
  GroupBlockChooseCanvas.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerSimple);
  
  GroupSwitches.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationGroupSwitches, &ManagerSwitches);

  TabBlockChoose.init(&GroupSwitches, 0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerTabBlockChoose);
  
  ButtonSwitchBlocks.init(ID_SWITCHTAB, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSwitchBlocks);
  ButtonSwitchBlocks.set_callbackf(&CallbackBlockChooseMenu);
  GroupBlocks.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerGroupChoose);
  TabBlockChoose.set_controls(&ButtonSwitchBlocks, &GroupBlocks);
  
  ButtonSwitchTextures.init(ID_SWITCHTAB, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSwitchTextures);
  ButtonSwitchTextures.set_callbackf(&CallbackBlockChooseMenu);
  GroupTextures.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerGroupChoose);
  TabBlockChoose.set_controls(&ButtonSwitchTextures, &GroupTextures);
  
  ButtonSwitchOriginTextures.init(ID_SWITCHTAB, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSwitchOriginTextures);
  ButtonSwitchOriginTextures.set_callbackf(&CallbackBlockChooseMenu);
  GroupOriginTextures.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerSimple);
  TabBlockChoose.set_controls(&ButtonSwitchOriginTextures, &GroupOriginTextures);
  
  GroupBlockChooseCanvas.add_control(&TabBlockChoose);
  
  ManagerCanvasSeparate.init(WMB_STATIC | WMB_HORIZONTAL | ALIGN_TOP);
  GroupCanvasSeparate.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerCanvasSeparate);
  
  ManagerScrollBlock.init(0.8f, 0, ALIGN_RIGHT);
  GroupScrollBlock.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerScrollBlock);
  
  ManagerScroll.init(WMB_DYNAMIC | WMB_HORIZONTAL | ALIGN_TOP);
  GroupScroll.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerScroll);
  
  AnimationButtonScrollUp.init(pDevice, TextureButtonScroll1, TextureButtonEffect);
  ButtonScrollUp.init(ID_BUTTONSCROLLUP, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationButtonScrollUp);
  ButtonScrollUp.set_callbackf(&CallbackBlockChooseMenu);
  GroupScroll.add_control(&ButtonScrollUp);
  ManagerScroll.set_elem_size(0.5f);
  
  AnimationButtonScrollDown.init(pDevice, TextureButtonScroll2, TextureButtonEffect);
  ButtonScrollDown.init(ID_BUTTONSCROLLDOWN, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationButtonScrollDown);
  ButtonScrollDown.set_callbackf(&CallbackBlockChooseMenu);
  GroupScroll.add_control(&ButtonScrollDown);
  ManagerScroll.set_elem_size(0.5f);
  
  GroupScrollBlock.add_control(&GroupScroll);
  GroupCanvasSeparate.add_control(&GroupScrollBlock);
  ManagerCanvasSeparate.set_elem_size(100);
  GroupBlockChooseCanvas.add_control(&GroupCanvasSeparate);
  
  GroupBlockChooseMenu.add_control(&GroupBlockChooseCanvas);
  
  BlockChooseMenu.init(&GroupBlockChooseMenu, D3DXVECTOR2(StartRect.right * 0.1f, StartRect.bottom * 0.1f), D3DXVECTOR2(StartRect.right * 0.8f, StartRect.bottom * 0.8f));
  //---
  AnimationCreditsWindow.init(pDevice, NULL, D3DCOLOR_ARGB(128, 255, 255, 255), D3DCOLOR_ARGB(128, 0, 0, 0), board, 0);
  AnimationCreditsExit.init(pDevice, TextureButtonClose, D3DCOLOR_ARGB(255, 220, 220, 220), D3DCOLOR_ARGB(255, 196, 0, 0), 0.25f);
  AnimationCreditsPict.init(pDevice, TextureCreditsPict);
  ManagerCreditsPict.init(16.0f/9, 0, ALIGN_CENTER);
  
  GroupCreditsMenu.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationCreditsWindow, &ManagerWindowed);
  GroupCreditsBoard.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerWindowBoard);
   
  ButtonCreditsExit.init(ID_BUTTONMENUCLOSE, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationCreditsExit);
  ButtonCreditsExit.set_callbackf(&CallbackCreditsMenu);
  GroupCreditsBoard.add_control(&ButtonCreditsExit);
   
  GroupCreditsMenu.add_control(&GroupCreditsBoard);
  
  GroupCreditsCanvas.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerCreditsPict);
  
  ButtonCreditsPict.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationCreditsPict);
  GroupCreditsCanvas.add_control(&ButtonCreditsPict);
  
  GroupCreditsMenu.add_control(&GroupCreditsCanvas);
  
  CreditsMenu.init(&GroupCreditsMenu, D3DXVECTOR2(StartRect.right * 0.1f, StartRect.bottom * 0.1f), D3DXVECTOR2(StartRect.right * 0.8f, StartRect.bottom * 0.8f));
  //---
  AnimationSettingsWindow.init(pDevice, NULL, D3DCOLOR_ARGB(128, 255, 255, 255), D3DCOLOR_ARGB(128, 0, 0, 0), board, 0);
  AnimationSettingsExit.init(pDevice, TextureButtonClose, D3DCOLOR_ARGB(255, 220, 220, 220), D3DCOLOR_ARGB(255, 196, 0, 0), 0.25f);
  AnimationSkyBLeft.init(pDevice, TextureButtonScroll3, TextureButtonScroll3);
  AnimationSkyBRight.init(pDevice, TextureButtonScroll4, TextureButtonScroll4);
  ManagerSkyBChoose.init(WMB_DYNAMIC | WMB_VERTICAL | ALIGN_LEFT);
  ManagerSkyB.init(1.0f, 0, ALIGN_CENTER);
  for (int i = 0; i < NumSkyboxes; i++) {
    AnimationSkyBChoose[i].init(pDevice, SkyBoxT6[i]);
  }
  
  GroupSettingsMenu.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSettingsWindow, &ManagerWindowed);
  GroupSettingsBoard.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerWindowBoard);
   
  ButtonSettingsExit.init(ID_BUTTONMENUCLOSE, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationCreditsExit);
  ButtonSettingsExit.set_callbackf(&CallbackSettingsMenu);
  GroupSettingsBoard.add_control(&ButtonSettingsExit);
   
  GroupSettingsMenu.add_control(&GroupSettingsBoard);
  
  GroupSettingsCanvas.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerSimple);
  
  GroupSkyB.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerSkyB);
  GroupSkyBIn.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerSimple);
  GroupSkyB.add_control(&GroupSkyBIn);
  for (int i = 0; i < NumSkyboxes; i++) {
    ButtonSkyBChoose[i].init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSkyBChoose[i]);
    GroupSkyBIn.add_control(&ButtonSkyBChoose[i]);
	ButtonSkyBChoose[i].show_control(false);
  }
  ButtonSkyBChoose[CurrentSkybox].show_control(true);
  GroupSettingsCanvas.add_control(&GroupSkyB);
  
  GroupSkyBChoose.init(0, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationStd, &ManagerSkyBChoose);
  
  ButtonSkyBLeft.init(ID_BUTTONSKYBLEFT, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSkyBLeft);
  ButtonSkyBLeft.set_callbackf(&CallbackSettingsMenu);
  GroupSkyBChoose.add_control(&ButtonSkyBLeft);
  ManagerSkyBChoose.set_elem_size(0.1f);
  
  GroupSkyBChoose.add_control(&GroupSimple);
  ManagerSkyBChoose.set_elem_size(0.8f);
  
  ButtonSkyBRight.init(ID_BUTTONSKYBRIGHT, D3DXVECTOR2(0, 0), D3DXVECTOR2(0, 0), &AnimationSkyBRight);
  ButtonSkyBRight.set_callbackf(&CallbackSettingsMenu);
  GroupSkyBChoose.add_control(&ButtonSkyBRight);
  ManagerSkyBChoose.set_elem_size(0.1f);
  
  GroupSkyBIn.add_control(&GroupSkyBChoose);
  
  GroupSettingsMenu.add_control(&GroupSettingsCanvas);
  
  SettingsMenu.init(&GroupSettingsMenu, D3DXVECTOR2(StartRect.right * 0.1f, StartRect.bottom * 0.1f), D3DXVECTOR2(StartRect.right * 0.8f, StartRect.bottom * 0.8f));
  return 1;
}

void DeleteFrames() {
  delete [] ButtonBlocks;
  delete [] AnimationButtonBlocks;
  delete [] ButtonTextures;
  delete [] AnimationButtonTextures;
}

int LoadObject(char *filename, Triangle *trianglelist) {
  std::ifstream infile;
  infile.open(filename);
  if (!infile)
    return 0;
  
  std::vector<D3DXVECTOR3> p; //вектор позиций вершин
  D3DXVECTOR3 pos;
  std::vector<D3DXVECTOR3> n; //вектор нормалей вершин
  D3DXVECTOR3 normal;
  std::vector<Vector2D> t; //вектор текстурных координат вершин
  Vector2D texture;
  std::vector<Index> ind; //вектор индексов вершин
  Index index;
  
  StrParse parser, parser2;
  parser.set_separate(' ');
  parser2.set_separate('/');
  char buff[19], str[62], str2[32];
  while (! infile.eof()) {
    strcpy(str, "");
    infile.getline(str, 61);
    parser.set_text(str);
    //координаты
    if (str[0] == 'v' && str[1] == ' ') {
      parser.get_lexem(buff, 18, 1);
      pos.x = atof(buff);
      parser.get_lexem(buff, 18, 2);
      pos.y = atof(buff);
      parser.get_lexem(buff, 18, 3);
      pos.z = atof(buff);
      p.push_back(pos);
    }
    //нормали
    if (str[0] == 'v' && str[1] == 'n') {
      parser.get_lexem(buff, 18, 1);
      normal.x = atof(buff);
      parser.get_lexem(buff, 18, 2);
      normal.y = atof(buff);
      parser.get_lexem(buff, 18, 3);
      normal.z = atof(buff);
      n.push_back(normal);
    }
    //текстурные координаты
    if (str[0] == 'v' && str[1] == 't') {
      parser.get_lexem(buff, 18, 1);
      texture.x = atof(buff);
      parser.get_lexem(buff, 18, 2);
      texture.y = atof(buff);
      t.push_back(texture);
    }
    //индексы
    if (str[0] == 'f' && str[1] == ' ') {
      parser.get_lexem(str2, 31, 1);
      parser2.set_text(str2);
      parser2.get_lexem(buff, 18, 0);
      index.p = atoi(buff);
      parser2.get_lexem(buff, 18, 1);
      index.t = atoi(buff);
      parser2.get_lexem(buff, 18, 2);
      index.n = atoi(buff);
      ind.push_back(index);
      
      parser.get_lexem(str2, 31, 2);
      parser2.set_text(str2);
      parser2.get_lexem(buff, 18, 0);
      index.p = atoi(buff);
      parser2.get_lexem(buff, 18, 1);
      index.t = atoi(buff);
      parser2.get_lexem(buff, 18, 2);
      index.n = atoi(buff);
      ind.push_back(index);
      
      parser.get_lexem(str2, 31, 3);
      parser2.set_text(str2);
      parser2.get_lexem(buff, 18, 0);
      index.p = atoi(buff);
      parser2.get_lexem(buff, 18, 1);
      index.t = atoi(buff);
      parser2.get_lexem(buff, 18, 2);
      index.n = atoi(buff);
      ind.push_back(index);
    }
  }
  //создание массива треугольников при помощи значений из индексного буфера
  int num = 0;
  for (int i = 0; i < ind.size()/3; i++) {
    trianglelist[i].vertex1.p = p[ind[num].p - 1];
    trianglelist[i].vertex1.n = n[ind[num].n - 1];
    trianglelist[i].vertex1.tv = t[ind[num].t - 1].x;
    trianglelist[i].vertex1.tu = t[ind[num].t - 1].y;
    num++;
    trianglelist[i].vertex2.p = p[ind[num].p - 1];
    trianglelist[i].vertex2.n = n[ind[num].n - 1];
    trianglelist[i].vertex2.tv = t[ind[num].t - 1].x;
    trianglelist[i].vertex2.tu = t[ind[num].t - 1].y;
    num++;
    trianglelist[i].vertex3.p = p[ind[num].p - 1];
    trianglelist[i].vertex3.n = n[ind[num].n - 1];
    trianglelist[i].vertex3.tv = t[ind[num].t - 1].x;
    trianglelist[i].vertex3.tu = t[ind[num].t - 1].y;
    num++;
  }
  return num / 3;
}

void SetBlocks() {
  //установка текстур
  CurrentObject.set_texture(Texture[0], 0, false);
  CurrentObject.set_texture(Texture[1], 1, false);
  CurrentObject.set_texture(Texture[2], 2, false);
  CurrentObject.set_texture(Texture[3], 3, false);
  CurrentObject.set_texture(Texture[4], 4, false);
  CurrentObject.set_texture(Texture[5], 5, false);
  CurrentObject.set_texture(Texture[6], 6, false);
  CurrentObject.set_texture(Texture[7], 7, true);
  CurrentObject.set_texture(Texture[13], 13, false);
  //начальная текстура
  CurrentObject.set_currentT(0);
  //стандартные
  CurrentObject.set_block(0, 0, 0);
  CurrentObject.set_block(1, 5, 0);
  CurrentObject.set_block(2, 2, 0);
  CurrentObject.set_block(3, 4, 0);
  CurrentObject.set_block(4, 4, 0);
  CurrentObject.set_block(5, 2, 0);
  CurrentObject.set_block(8, 5, 0);
  //загруженные
  CurrentObject.set_block(25, 5, 0);
  CurrentObject.set_block(20, 0, 0);
  CurrentObject.set_block(21, 1, 0);
  CurrentObject.set_block(22, 0, 0);
  CurrentObject.set_block(23, 1, 0);
  CurrentObject.set_block(24, 3, 0);
  CurrentObject.set_block(2017, 1, 1);
  //уникальные текстуры
  CurrentObject.set_originaltexture(2017, Texture[8], 8, false);
  CurrentObject.set_originaltexture(2017, Texture[9], 9, false);
  CurrentObject.set_originaltexture(2017, Texture[10], 10, false);
  
  CurrentObject.set_originaltexture(2, Texture[11], 11, false);
  CurrentObject.set_originaltexture(3, Texture[11], 11, false);
  CurrentObject.set_originaltexture(4, Texture[11], 11, false);
  CurrentObject.set_originaltexture(5, Texture[11], 11, false);
  
  CurrentObject.set_originaltexture(2, Texture[12], 12, false);
  CurrentObject.set_originaltexture(3, Texture[12], 12, false);
  CurrentObject.set_originaltexture(4, Texture[12], 12, false);
  CurrentObject.set_originaltexture(5, Texture[12], 12, false);
  //начальный блок
  CurrentObject.set_currentF(0);
}

bool InitScene() {
  //загрузка сцены и установка камеры
  if (! CreateScene(worldname))
    return false;
  
  SetView();
  pDevice->SetRenderState(D3DRS_LIGHTING, false);
  pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  
  return true;
}

bool LoadScene(char *filename) {
  std::ifstream infile;
  infile.open(filename);
  if (!infile)
    return 0;
  //очистка сцены
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object())
          FieldArray[j][f][i].destroy();
      }
    }
  }
  //переменные для загрузки данных
  Position pos;
  Rotate rot;
  int Type, TexType, transparent, LoaderVersion;
  StrParse parser;
  char buff[11], str[55];
  //определение версии загрузчика для файла
  strcpy(str, "");
  infile.getline(str, 54, ';');
  if (str[0] == '>') {
    parser.set_separate('>');
    parser.set_text(str);
    parser.get_lexem(buff, 11, 1);
    LoaderVersion = atoi(buff);
  }
  else {
    LoaderVersion = 0;
  }
  //загрузка данных
  parser.set_separate(',');
  switch (LoaderVersion) {
    case 0:
      while (! infile.eof()) {
        strcpy(str, "");
        infile.getline(str, 54, ';');
        parser.set_text(str);
        parser.get_lexem(buff, 11, 0);
        pos.x = atoi(buff);
        parser.get_lexem(buff, 11, 1);
        pos.y = atoi(buff);
        parser.get_lexem(buff, 11, 2);
        pos.z = atoi(buff);
        parser.get_lexem(buff, 11, 3);
        Type = atoi(buff);
        parser.get_lexem(buff, 11, 4);
        TexType = atoi(buff);
        parser.get_lexem(buff, 11, 5);
        transparent = atoi(buff);
        if (pos.x < maxX && pos.y < maxY && pos.z < maxZ) {
          FieldArray[pos.y][pos.z][pos.x].init(pos, Rotate(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255), Texture[TexType], Type, TexType);
          FieldArray[pos.y][pos.z][pos.x].SetTransparent(transparent);
        }
      }
      break;
      
    case 1:
      while (! infile.eof()) {
        strcpy(str, "");
        infile.getline(str, 54, ';');
        parser.set_text(str);
        parser.get_lexem(buff, 11, 0);
        pos.x = atoi(buff);
        parser.get_lexem(buff, 11, 1);
        pos.y = atoi(buff);
        parser.get_lexem(buff, 11, 2);
        pos.z = atoi(buff);
        parser.get_lexem(buff, 11, 3);
        rot.x = atoi(buff);
        parser.get_lexem(buff, 11, 4);
        rot.y = atoi(buff);
        parser.get_lexem(buff, 11, 5);
        rot.z = atoi(buff);
        parser.get_lexem(buff, 11, 6);
        Type = atoi(buff);
        parser.get_lexem(buff, 11, 7);
        TexType = atoi(buff);
        parser.get_lexem(buff, 11, 8);
        transparent = atoi(buff);
        if (pos.x < maxX && pos.y < maxY && pos.z < maxZ) {
          FieldArray[pos.y][pos.z][pos.x].init(pos, rot, D3DCOLOR_XRGB(255, 255, 255), Texture[TexType], Type, TexType);
          FieldArray[pos.y][pos.z][pos.x].SetTransparent(transparent);
        }
      }
      break;
  }
  return 1;
}

bool SaveScene(char *filename) {
  std::ofstream outfile;
  outfile.open(filename);
  if (! outfile)
    return 0;
  
  Position pos;
  Rotate rot;
  char buff[11];
  //сохранение данных
  itoa(CurrentLoaderVersion, buff, 10);
  outfile << ">" << buff << ";";
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object()) {
          pos = FieldArray[j][f][i].get_pos();
          itoa(pos.x, buff, 10);
          outfile << buff << ",";
          itoa(pos.y, buff, 10);
          outfile << buff << ",";
          itoa(pos.z, buff, 10);
          outfile << buff << ",";
          rot = FieldArray[j][f][i].get_rotate();
          itoa(rot.x, buff, 10);
          outfile << buff << ",";
          itoa(rot.y, buff, 10);
          outfile << buff << ",";
          itoa(rot.z, buff, 10);
          outfile << buff << ",";
          itoa(FieldArray[j][f][i].get_type(), buff, 10);
          outfile << buff << ",";
          itoa(FieldArray[j][f][i].get_textype(), buff, 10);
          outfile << buff << ",";
          itoa((int)FieldArray[j][f][i].is_transparent(), buff, 10);
          outfile << buff << ";";
        }
      }
    }
  }
  return 1;
}

bool CreateScene(char *filename) {
  /*for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (rand() % 10 == 0) {
          FieldArray[j][f][i].init(Position(i, j, f), Rotate(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255), Texture[6], 0, 6);
        }
      }
    }
  }*/
  //очистка карты
  strcpy(filename, "world.tc");
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object())
          FieldArray[j][f][i].destroy();
      }
    }
  }
  //создание плоской поверхности
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY && j < 1; j++) {
      for (int f = 0; f < maxZ; f++) {
        FieldArray[j][f][i].init(Position(i, j, f), Rotate(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255), Texture[6], 0, 6);
      }
    }
  }
  if (! UpdateObject())
    return false;
  
  if (! UpdateSkyBox())
    return false;
  
  return true;
}

bool UpdateScene() {
  Position block, before; /*позиция блока, на который указывает перекрестие и
  позиция блока перед гранью, на которое указывает перекрестие*/
  float distance; //растояние до блока, на который указывает перекрестие
  if (Player.GetCmd()) {
    distance = GetIntersect(&block, &before, Player);
    if (distance != -1.0f && distance < Player.GetDistance()) {
      switch (Player.GetCmd()) {
        //удаление блока
        case 1:
          FieldArray[block.y][block.z][block.x].destroy();
          if (! UpdateObject())
            return false;
          break;
        //установка блока
        case 2:
          if (before.x >= 0 && before.x < maxX && before.y >= 0 && before.y < maxY
          && before.z >= 0 && before.z < maxZ) {
            FieldArray[before.y][before.z][before.x] = CurrentObject.get_object();
            FieldArray[before.y][before.z][before.x].set_pos(Position(before.x, before.y, before.z));
            RotateObject(&FieldArray[before.y][before.z][before.x], block, before, Player);
            if (! UpdateObject())
              return false;
          }
      }
    }
    Player.SetCmd(0);
  }
  return true;
}

bool UpdateObject() {
  //очистка буфера
  if(pVB)
    pVB->Release();
  pVB = 0;
  int NumPolygons = 0; //число полигонов в сцене

  int num = 0, numbuff = 0; //счетные переменные цикла
  Triangle *OutArray, *buffer; //массив треугольников на вывод и буфера
  //определение общего числа полигонов
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object()) {
          NumPolygons += FieldArray[j][f][i].get_numtriangles(GetBorderInfo(i, j, f));
        }
      }
    }
  }
  OutArray = new Triangle[NumPolygons];
  buffer = new Triangle[maxObjTriangleNum];
  //заполнение массива треугольников для рендера сцены
  NumPolygons = 0;
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object()) {
          NumPolygons += numbuff = FieldArray[j][f][i].CreateCube(buffer, GetBorderInfo(i, j, f));
          for (int l = 0; l < numbuff; l++) {
            OutArray[num] = buffer[l];
            num++;
          }
        }
      }
    }
  }
  
  HRESULT hr;
  void *pBuf;
  //заполнение буфера вершин
  hr = pDevice->CreateVertexBuffer(sizeof(Triangle) * NumPolygons, 0, D3DFVF_CUSTOMVERTEX,
                                   D3DPOOL_DEFAULT, &pVB, NULL);
  if (FAILED(hr))
    return false;
  hr = pVB->Lock(0, sizeof(Triangle) * NumPolygons, &pBuf, 0);
  if(FAILED(hr))
    return false;
  memcpy(pBuf, OutArray, sizeof(Triangle) * NumPolygons);
  pVB->Unlock();
  
  delete [] OutArray;
  delete [] buffer;

  return true;
}

bool UpdateSkyBox() {
  //очистка буфера
  if (pVBS)
    pVBS->Release();
  pVBS = 0;
  
  float leight = (int)(RenderDistance / sqrt(3)); //вычисление длины грани исходя из дистанции рендера
  Cube SkyBox; //куб с гранями внутрь
  D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    SkyBox.square1.triangle1.vertex1.init(pos.x-leight, pos.y+leight, pos.z+leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square1.triangle1.vertex2.init(pos.x-leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 0.0f);
    SkyBox.square1.triangle1.vertex3.init(pos.x+leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square1.triangle1.ComputeNormal();
    SkyBox.square1.triangle2.vertex1.init(pos.x-leight, pos.y+leight, pos.z+leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square1.triangle2.vertex2.init(pos.x+leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square1.triangle2.vertex3.init(pos.x+leight, pos.y+leight, pos.z+leight, 0xffffffff, 1.0f, 1.0f);
    SkyBox.square1.triangle2.ComputeNormal();
    
    SkyBox.square2.triangle1.vertex1.init(pos.x-leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square2.triangle1.vertex2.init(pos.x-leight, pos.y-leight, pos.z+leight, 0xffffffff, 0.0f, 0.0f);
    SkyBox.square2.triangle1.vertex3.init(pos.x+leight, pos.y-leight, pos.z+leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square2.triangle1.ComputeNormal();
    SkyBox.square2.triangle2.vertex1.init(pos.x-leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square2.triangle2.vertex2.init(pos.x+leight, pos.y-leight, pos.z+leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square2.triangle2.vertex3.init(pos.x+leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 1.0f);
    SkyBox.square2.triangle2.ComputeNormal();
    
    SkyBox.square3.triangle1.vertex1.init(pos.x+leight, pos.y-leight, pos.z+leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square3.triangle1.vertex2.init(pos.x+leight, pos.y+leight, pos.z+leight, 0xffffffff, 0.0f, 0.0f);
    SkyBox.square3.triangle1.vertex3.init(pos.x+leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square3.triangle1.ComputeNormal();
    SkyBox.square3.triangle2.vertex1.init(pos.x+leight, pos.y-leight, pos.z+leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square3.triangle2.vertex2.init(pos.x+leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square3.triangle2.vertex3.init(pos.x+leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 1.0f);
    SkyBox.square3.triangle2.ComputeNormal();
    
    SkyBox.square4.triangle1.vertex1.init(pos.x+leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square4.triangle1.vertex2.init(pos.x+leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 0.0f);
    SkyBox.square4.triangle1.vertex3.init(pos.x-leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square4.triangle1.ComputeNormal();
    SkyBox.square4.triangle2.vertex1.init(pos.x+leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square4.triangle2.vertex2.init(pos.x-leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square4.triangle2.vertex3.init(pos.x-leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 1.0f);
    SkyBox.square4.triangle2.ComputeNormal();
    
    SkyBox.square5.triangle1.vertex1.init(pos.x-leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square5.triangle1.vertex2.init(pos.x-leight, pos.y+leight, pos.z-leight, 0xffffffff, 0.0f, 0.0f);
    SkyBox.square5.triangle1.vertex3.init(pos.x-leight, pos.y+leight, pos.z+leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square5.triangle1.ComputeNormal();
    SkyBox.square5.triangle2.vertex1.init(pos.x-leight, pos.y-leight, pos.z-leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square5.triangle2.vertex2.init(pos.x-leight, pos.y+leight, pos.z+leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square5.triangle2.vertex3.init(pos.x-leight, pos.y-leight, pos.z+leight, 0xffffffff, 1.0f, 1.0f);
    SkyBox.square5.triangle2.ComputeNormal();
    
    SkyBox.square6.triangle1.vertex1.init(pos.x-leight, pos.y-leight, pos.z+leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square6.triangle1.vertex2.init(pos.x-leight, pos.y+leight, pos.z+leight, 0xffffffff, 0.0f, 0.0f);
    SkyBox.square6.triangle1.vertex3.init(pos.x+leight, pos.y+leight, pos.z+leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square6.triangle1.ComputeNormal();
    SkyBox.square6.triangle2.vertex1.init(pos.x-leight, pos.y-leight, pos.z+leight, 0xffffffff, 1.0f, 0.0f);
    SkyBox.square6.triangle2.vertex2.init(pos.x+leight, pos.y+leight, pos.z+leight, 0xffffffff, 0.0f, 1.0f);
    SkyBox.square6.triangle2.vertex3.init(pos.x+leight, pos.y-leight, pos.z+leight, 0xffffffff, 1.0f, 1.0f);
    SkyBox.square6.triangle2.ComputeNormal();
  
  HRESULT hr;
  void *pBuf;
  //заполнение буфера вершин
  hr = pDevice->CreateVertexBuffer(sizeof(Triangle) * 12, 0, D3DFVF_CUSTOMVERTEX,
                                   D3DPOOL_DEFAULT, &pVBS, NULL);
  if (FAILED(hr))
    return false;
  hr = pVBS->Lock(0, sizeof(Triangle) * 12, &pBuf, 0);
  if(FAILED(hr))
    return false;
  memcpy(pBuf, &SkyBox, sizeof(Triangle) * 12);
  pVBS->Unlock();

  return true;
}

bool UpdateIndicate() {
  //очистка буфера
  if(pVBIndicate)
    pVBIndicate->Release();
  pVBIndicate = 0;
  int NumPolygons = 0;

  Triangle *OutArray; //массив треугольников для отрисовки сцены
  OutArray = new Triangle[maxObjTriangleNum];
  
  ObjectCube oc; //текущий блок
  oc = CurrentObject.get_object();
  oc.set_pos(Position(0.0f, 0.0f, 0.0f));
  NumPolygons = oc.CreateCube(OutArray, BorderInfo());
  
  HRESULT hr;
  void *pBuf;
  //заполнение буфера вершин
  hr = pDevice->CreateVertexBuffer(sizeof(Triangle) * NumPolygons, 0, D3DFVF_CUSTOMVERTEX,
                                   D3DPOOL_DEFAULT, &pVBIndicate, NULL);
  if (FAILED(hr))
    return false;
  hr = pVBIndicate->Lock(0, sizeof(Triangle) * NumPolygons, &pBuf, 0);
  if(FAILED(hr))
    return false;
  memcpy(pBuf, OutArray, sizeof(Triangle) * NumPolygons);
  pVBIndicate->Unlock();
  
  delete [] OutArray;

  return true;
}

void DeleteBuff() {
  if(pVB)
    pVB->Release();
  pVB = 0;
  
  if (pVBS)
    pVBS->Release();
  pVBS = 0;
  
  if (pVBIndicate)
    pVBIndicate->Release();
  pVBIndicate = 0;
}

void SetView() {
  Player.SetPosition(D3DXVECTOR3(maxX/2, 2, maxZ/2));
  Player.SetAngle(0, 0, 0);
}

void UpdateView() {
  D3DXMATRIX Transform;
  D3DXMatrixIdentity(&Transform);
  D3DXMatrixTranslation(&Transform, 0.0f, 0.0f, 0.0f);
  pDevice->SetTransform(D3DTS_WORLD, &Transform);
  
  float Zn = 0.5f;//0.5f;
  float Zf = RenderDistance;//100.0f;
  float Vw = 1.0f;//1.0f;
  float Vh = (float)GetSystemMetrics(SM_CYSCREEN)/GetSystemMetrics(SM_CXSCREEN);//3.0f/4;

  D3DMATRIX Projection = {
    2*Zn/Vw, 0, 0, 0,
    0, 2*Zn/Vh, 0, 0,
    0, 0, Zf/(Zf-Zn), 1,
    0, 0, -Zf/(Zf-Zn)*Zn, 0,
  };
  pDevice->SetTransform(D3DTS_PROJECTION, &Projection);
  //---------
  Player.UpdateCameraMan(pDevice, true);
}

void UpdateMaterial() {
  D3DMATERIAL9 Material;
  ZeroMemory(&Material, sizeof(D3DMATERIAL9));
  Material.Diffuse.r = 1;
  Material.Diffuse.b = 1;
  Material.Diffuse.g = 1;
  Material.Ambient.r = 1;
  Material.Ambient.b = 1;
  Material.Ambient.g = 1;
  Material.Power = 1;
  pDevice->SetMaterial(&Material);
}

void UpdateLight() {
  D3DLIGHT9 light;
  ZeroMemory(&light, sizeof(light));
  
  switch (CurrentSkybox) {
	case 0:
      light.Type = D3DLIGHT_DIRECTIONAL;
      light.Diffuse.r = 0.8f;
      light.Diffuse.g = 0.8f;
      light.Diffuse.b = 0.8f;
      light.Direction = D3DXVECTOR3(-0.8f, -0.8f, -1.0f);
      light.Range = 100.0f;
      pDevice->SetLight(0 ,&light);
      pDevice->LightEnable(0, true);
	  break;
	  
	case 1:
	  light.Type = D3DLIGHT_DIRECTIONAL;
      light.Diffuse.r = 0.6f;
      light.Diffuse.g = 0.6f;
      light.Diffuse.b = 0.6f;
      light.Direction = D3DXVECTOR3(-1.0f, -0.8f, -0.1f);
      light.Range = 100.0f;
      pDevice->SetLight(0 ,&light);
      pDevice->LightEnable(0, true);
	  break;
  }

  
  
  pDevice->SetRenderState(D3DRS_LIGHTING, true);
  pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA((int)(255 * 0.2f), (int)(255 * 0.2f), (int)(255 * 0.2f), 255));
  pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
}
//-----------------
void CubeIntersect(Cube *cube, Position pos) {
  //создание кубика с позицией pos
    cube->square1.triangle1.vertex1.init(pos.x, pos.y, pos.z+1, 0xffffffff, 1.0f, 0.0f);
    cube->square1.triangle1.vertex2.init(pos.x, pos.y, pos.z, 0xffffffff, 0.0f, 0.0f);
    cube->square1.triangle1.vertex3.init(pos.x+1, pos.y, pos.z, 0xffffffff, 0.0f, 1.0f);
    cube->square1.triangle1.ComputeNormal();
    cube->square1.triangle2.vertex1.init(pos.x, pos.y, pos.z+1, 0xffffffff, 1.0f, 0.0f);
    cube->square1.triangle2.vertex2.init(pos.x+1, pos.y, pos.z, 0xffffffff, 0.0f, 1.0f);
    cube->square1.triangle2.vertex3.init(pos.x+1, pos.y, pos.z+1, 0xffffffff, 1.0f, 1.0f);
    cube->square1.triangle2.ComputeNormal();
    
    cube->square2.triangle1.vertex1.init(pos.x, pos.y+1, pos.z, 0xffffffff, 1.0f, 0.0f);
    cube->square2.triangle1.vertex2.init(pos.x, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 0.0f);
    cube->square2.triangle1.vertex3.init(pos.x+1, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 1.0f);
    cube->square2.triangle1.ComputeNormal();
    cube->square2.triangle2.vertex1.init(pos.x, pos.y+1, pos.z, 0xffffffff, 1.0f, 0.0f);
    cube->square2.triangle2.vertex2.init(pos.x+1, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 1.0f);
    cube->square2.triangle2.vertex3.init(pos.x+1, pos.y+1, pos.z, 0xffffffff, 1.0f, 1.0f);
    cube->square2.triangle2.ComputeNormal();
    
    cube->square3.triangle1.vertex1.init(pos.x, pos.y, pos.z+1, 0xffffffff, 1.0f, 0.0f);
    cube->square3.triangle1.vertex2.init(pos.x, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 0.0f);
    cube->square3.triangle1.vertex3.init(pos.x, pos.y+1, pos.z, 0xffffffff, 0.0f, 1.0f);
    cube->square3.triangle1.ComputeNormal();
    cube->square3.triangle2.vertex1.init(pos.x, pos.y, pos.z+1, 0xffffffff, 1.0f, 0.0f);
    cube->square3.triangle2.vertex2.init(pos.x, pos.y+1, pos.z, 0xffffffff, 0.0f, 1.0f);
    cube->square3.triangle2.vertex3.init(pos.x, pos.y, pos.z, 0xffffffff, 1.0f, 1.0f);
    cube->square3.triangle2.ComputeNormal();
    
    cube->square4.triangle1.vertex1.init(pos.x+1, pos.y, pos.z+1, 0xffffffff, 1.0f, 0.0f);
    cube->square4.triangle1.vertex2.init(pos.x+1, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 0.0f);
    cube->square4.triangle1.vertex3.init(pos.x, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 1.0f);
    cube->square4.triangle1.ComputeNormal();
    cube->square4.triangle2.vertex1.init(pos.x+1, pos.y, pos.z+1, 0xffffffff, 1.0f, 0.0f);
    cube->square4.triangle2.vertex2.init(pos.x, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 1.0f);
    cube->square4.triangle2.vertex3.init(pos.x, pos.y, pos.z+1, 0xffffffff, 1.0f, 1.0f);
    cube->square4.triangle2.ComputeNormal();
    
    cube->square5.triangle1.vertex1.init(pos.x+1, pos.y, pos.z, 0xffffffff, 1.0f, 0.0f);
    cube->square5.triangle1.vertex2.init(pos.x+1, pos.y+1, pos.z, 0xffffffff, 0.0f, 0.0f);
    cube->square5.triangle1.vertex3.init(pos.x+1, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 1.0f);
    cube->square5.triangle1.ComputeNormal();
    cube->square5.triangle2.vertex1.init(pos.x+1, pos.y, pos.z, 0xffffffff, 1.0f, 0.0f);
    cube->square5.triangle2.vertex2.init(pos.x+1, pos.y+1, pos.z+1, 0xffffffff, 0.0f, 1.0f);
    cube->square5.triangle2.vertex3.init(pos.x+1, pos.y, pos.z+1, 0xffffffff, 1.0f, 1.0f);
    cube->square5.triangle2.ComputeNormal();
    
    cube->square6.triangle1.vertex1.init(pos.x, pos.y, pos.z, 0xffffffff, 1.0f, 0.0f);
    cube->square6.triangle1.vertex2.init(pos.x, pos.y+1, pos.z, 0xffffffff, 0.0f, 0.0f);
    cube->square6.triangle1.vertex3.init(pos.x+1, pos.y+1, pos.z, 0xffffffff, 0.0f, 1.0f);
    cube->square6.triangle1.ComputeNormal();
    cube->square6.triangle2.vertex1.init(pos.x, pos.y, pos.z, 0xffffffff, 1.0f, 0.0f);
    cube->square6.triangle2.vertex2.init(pos.x+1, pos.y+1, pos.z, 0xffffffff, 0.0f, 1.0f);
    cube->square6.triangle2.vertex3.init(pos.x+1, pos.y, pos.z, 0xffffffff, 1.0f, 1.0f);
    cube->square6.triangle2.ComputeNormal();
}

float GetIntersect(Position *block, Position *before, Camera Player) {
  Cube cube; //куб для проверки пересечений
  D3DXVECTOR3 pos, lookat;
  pos = Player.GetPosition(); //позиция игрока
  lookat = Player.GetLookAt(); //вектор взгляда
  float MinDistance = -1.0f, distance, add; //дистанция до ближайшего кубика, дистанция до пересечения с полигоном, доп. вектор
  for (int i = 0; i < maxX; i++) {
    for (int j = 0; j < maxY; j++) {
      for (int f = 0; f < maxZ; f++) {
        if (FieldArray[j][f][i].is_object()) {
          CubeIntersect(&cube, Position(i, j, f));
          if (D3DXIntersectTri(&(cube.square1.triangle1.vertex1.p), &(cube.square1.triangle1.vertex2.p), &(cube.square1.triangle1.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j - 1, f);}
          if (D3DXIntersectTri(&(cube.square1.triangle2.vertex1.p), &(cube.square1.triangle2.vertex2.p), &(cube.square1.triangle2.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j - 1, f);}
          if (D3DXIntersectTri(&(cube.square2.triangle1.vertex1.p), &(cube.square2.triangle1.vertex2.p), &(cube.square2.triangle1.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j + 1, f);}
          if (D3DXIntersectTri(&(cube.square2.triangle2.vertex1.p), &(cube.square2.triangle2.vertex2.p), &(cube.square2.triangle2.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j + 1, f);}
          if (D3DXIntersectTri(&(cube.square3.triangle1.vertex1.p), &(cube.square3.triangle1.vertex2.p), &(cube.square3.triangle1.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i - 1, j, f);}
          if (D3DXIntersectTri(&(cube.square3.triangle2.vertex1.p), &(cube.square3.triangle2.vertex2.p), &(cube.square3.triangle2.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i - 1, j, f);}
          if (D3DXIntersectTri(&(cube.square4.triangle1.vertex1.p), &(cube.square4.triangle1.vertex2.p), &(cube.square4.triangle1.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j, f + 1);}
          if (D3DXIntersectTri(&(cube.square4.triangle2.vertex1.p), &(cube.square4.triangle2.vertex2.p), &(cube.square4.triangle2.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j, f + 1);}
          if (D3DXIntersectTri(&(cube.square5.triangle1.vertex1.p), &(cube.square5.triangle1.vertex2.p), &(cube.square5.triangle1.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i + 1, j, f);}
          if (D3DXIntersectTri(&(cube.square5.triangle2.vertex1.p), &(cube.square5.triangle2.vertex2.p), &(cube.square5.triangle2.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i + 1, j, f);}
          if (D3DXIntersectTri(&(cube.square6.triangle1.vertex1.p), &(cube.square6.triangle1.vertex2.p), &(cube.square6.triangle1.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j, f - 1);}
          if (D3DXIntersectTri(&(cube.square6.triangle2.vertex1.p), &(cube.square6.triangle2.vertex2.p), &(cube.square6.triangle2.vertex3.p),
              &pos, &lookat, &add, &add, &distance))
            if (MinDistance == -1.0f || distance < MinDistance)
            {MinDistance = distance; *block = Position(i, j, f); *before = Position(i, j, f - 1);}
        }
      }
    }
  }
  return MinDistance;
}

void RotateObject(ObjectCube *oc, Position block, Position before, Camera Player) {
  /*Режимы поворотов
  0.без поворотов
  1.поворот в 4 стороны
  2.поворот в 4 стороны + вверх/вниз
  3.поворот на угол в 4 стороны
  4.поворот на угол в 4 стороны + вверх/вниз
  5.присоединение к стороне кубика
  */
  D3DXVECTOR3 LookAt = Player.GetLookAt();
  switch (oc->get_rotatetype()) {
    case 1:
      if (LookAt.z < 0 && fabs(LookAt.z) > fabs(LookAt.x)) {
        oc->set_rotate(Rotate(0, 0, 0));
      }
      if (LookAt.x > 0 && fabs(LookAt.x) > fabs(LookAt.z)) {
        oc->set_rotate(Rotate(0, 90, 0));
      }
      if (LookAt.z > 0 && fabs(LookAt.z) > fabs(LookAt.x)) {
        oc->set_rotate(Rotate(0, 180, 0));
      }
      if (LookAt.x < 0 && fabs(LookAt.x) > fabs(LookAt.z)) {
        oc->set_rotate(Rotate(0, 270, 0));
      }
      break;
      
    case 2:
      if (LookAt.z < 0 && fabs(LookAt.z) > fabs(LookAt.x) && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 0, 0));
      }
      if (LookAt.x > 0 && fabs(LookAt.x) > fabs(LookAt.z) && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 90, 0));
      }
      if (LookAt.z > 0 && fabs(LookAt.z) > fabs(LookAt.x) && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 180, 0));
      }
      if (LookAt.x < 0 && fabs(LookAt.x) > fabs(LookAt.z) && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 270, 0));
      }
      if (LookAt.z < 0 && fabs(LookAt.z) > fabs(LookAt.x) && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 0, 180));
      }
      if (LookAt.x < 0 && fabs(LookAt.x) > fabs(LookAt.z) && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 90, 180));
      }
      if (LookAt.z > 0 && fabs(LookAt.z) > fabs(LookAt.x) && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 180, 180));
      }
      if (LookAt.x > 0 && fabs(LookAt.x) > fabs(LookAt.z) && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 270, 180));
      }
      break;
    
    case 3:
      if (LookAt.x < 0 && LookAt.z < 0) {
        oc->set_rotate(Rotate(0, 0, 0));
      }
      if (LookAt.x > 0 && LookAt.z < 0) {
        oc->set_rotate(Rotate(0, 90, 0));
      }
      if (LookAt.x > 0 && LookAt.z > 0) {
        oc->set_rotate(Rotate(0, 180, 0));
      }
      if (LookAt.x < 0 && LookAt.z > 0) {
        oc->set_rotate(Rotate(0, 270, 0));
      }
      break;
    
    case 4:
      if (LookAt.x < 0 && LookAt.z < 0 && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 0, 0));
      }
      if (LookAt.x > 0 && LookAt.z < 0 && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 90, 0));
      }
      if (LookAt.x > 0 && LookAt.z > 0 && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 180, 0));
      }
      if (LookAt.x < 0 && LookAt.z > 0 && LookAt.y < 0) {
        oc->set_rotate(Rotate(0, 270, 0));
      }
      if (LookAt.x < 0 && LookAt.z < 0 && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 90, 180));
      }
      if (LookAt.x > 0 && LookAt.z < 0 && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 0, 180));
      }
      if (LookAt.x > 0 && LookAt.z > 0 && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 270, 180));
      }
      if (LookAt.x < 0 && LookAt.z > 0 && LookAt.y > 0) {
        oc->set_rotate(Rotate(0, 180, 180));
      }
      break;
      
    case 5:
      if (block.y == before.y - 1) {
        oc->set_rotate(Rotate(0, 0, 0));
      }
      if (block.y == before.y + 1) {
        oc->set_rotate(Rotate(0, 0, 180));
      }
      if (block.x == before.x - 1) {
        oc->set_rotate(Rotate(0, 0, 90));
      }
      if (block.z == before.z + 1) {
        oc->set_rotate(Rotate(90, 0, 0));
      }
      if (block.x == before.x + 1) {
        oc->set_rotate(Rotate(0, 0, -90));
      }
      if (block.z == before.z - 1) {
        oc->set_rotate(Rotate(-90, 0, 0));
      }
      break;
  }
}

BorderInfo GetBorderInfo(int i, int j, int f) {
  BorderInfo bi; //информация о соседних блоках
  
  if (j - 1 >= 0)
    if (FieldArray[j-1][f][i].is_object() && (! FieldArray[j-1][f][i].is_transparent() || FieldArray[j][f][i].is_transparent()) && FieldArray[j-1][f][i].get_type() == 0)
      bi.n1 = true;
  if (j + 1 < maxY)
    if (FieldArray[j+1][f][i].is_object() && (! FieldArray[j+1][f][i].is_transparent() || FieldArray[j][f][i].is_transparent()) && FieldArray[j+1][f][i].get_type() == 0)
      bi.n2 = true;
  if (i - 1 >= 0)
    if (FieldArray[j][f][i-1].is_object() && (! FieldArray[j][f][i-1].is_transparent() || FieldArray[j][f][i].is_transparent()) && FieldArray[j][f][i-1].get_type() == 0)
      bi.n3 = true;
  if (f + 1 < maxZ)
    if (FieldArray[j][f+1][i].is_object() && (! FieldArray[j][f+1][i].is_transparent() || FieldArray[j][f][i].is_transparent()) && FieldArray[j][f+1][i].get_type() == 0)
      bi.n4 = true;
  if (i + 1 < maxX)
    if (FieldArray[j][f][i+1].is_object() && (! FieldArray[j][f][i+1].is_transparent() || FieldArray[j][f][i].is_transparent()) && FieldArray[j][f][i+1].get_type() == 0)
      bi.n5 = true;
  if (f - 1 >= 0)
    if (FieldArray[j][f-1][i].is_object() && (! FieldArray[j][f-1][i].is_transparent() || FieldArray[j][f][i].is_transparent()) && FieldArray[j][f-1][i].get_type() == 0)
      bi.n6 = true;
    
  return bi;
}

bool OpenFileDialog(HWND hwnd, char *filename) {
  OPENFILENAME ofn = {0}; //диалог загрузки файла
  char szFile[256], szFileName[256], CustomFilter[256];
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  *(ofn.lpstrFile) = 0;
  ofn.nMaxFile =sizeof(szFile);
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = szFileName;
  *(ofn.lpstrFileTitle) = 0;
  ofn.nMaxFileTitle = sizeof(szFileName);
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_EXPLORER;
  char szFilter[256] = "Terrain Creator File (*.tc)\0*.tc\0";
  ofn.lpstrFilter = szFilter;
  
  if (GetOpenFileName(&ofn)) {
    strcpy(filename, szFile);
    return 1;
  }
  else {
    strcpy(filename, "");
    return 0;
  }
}

bool SaveFileDialog(HWND hwnd, char *filename) {
  OPENFILENAME ofn = {0}; //диалог сохранения файла
  char szFile[256], szFileName[256], CustomFilter[256];
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  *(ofn.lpstrFile) = 0;
  ofn.nMaxFile =sizeof(szFile);
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = szFileName;
  *(ofn.lpstrFileTitle) = 0;
  ofn.nMaxFileTitle = sizeof(szFileName);
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_EXPLORER;
  char szFilter[256] = "Terrain Creator File (*.tc)\0*.tc\0";
  ofn.lpstrFilter = szFilter;
  
  if (GetSaveFileName(&ofn)) {
    strcpy(filename, szFile);
    return 1;
  }
  else {
    strcpy(filename, "");
    return 0;
  }
}

void SetCursorToCenter(HWND hWnd) {
  RECT rc;
  GetClientRect(hWnd, &rc);
  SetCursorPos(rc.right/2, rc.bottom/2);
}

WindowFrame* GetFrame(int scene, int frame) {
  if (scene == 1) {
    switch (frame) {
      case 1:
        return &MainMenu;
        break;
		
	  case 2:
        return &SettingsMenu;
        break;
		
	  case 3:
	    return &CreditsMenu;
		break;
    }
  }
  if (scene == 2) {
    switch (frame) {
      case 11:
        return &PlayMenu;
        break;
      
      case 12:
        return &BlockChooseMenu;
        break;
    }
  }
}

void RotatePlayer(HWND hWnd, Camera *Player, bool *change) {
  int offsetX, offsetY;
  POINT Pos;
  RECT rc;
  
  if (*change) {
    SetCursorToCenter(hWnd);
    *change = 0;
  }
  
  GetCursorPos(&Pos);
  GetClientRect(hWnd, &rc);
    
  offsetX = Pos.x - rc.right/2;
  offsetY = Pos.y - rc.bottom/2;
  Player->RotateLeftRight(D3DXToRadian(offsetX/4));
  Player->RotateUpDown(D3DXToRadian(offsetY/4));
  
  SetCursorPos(rc.right/2, rc.bottom/2);
}

void MovePlayer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Camera *Player) {
  switch(msg) {
    case WM_LBUTTONDOWN:
      Player->SetCmd(1);
      break;
    
    case WM_RBUTTONDOWN:
      Player->SetCmd(2);
      break;
    
    case WM_KEYDOWN:
      switch(LOWORD(wParam)) {
        case 0x41://A
          Player->MoveLeft();
          break;
        
        case 0x44://D
          Player->MoveRight();
          break;
          
        case 0x57://W
          Player->MoveForward();
          break;
          
        case 0x53://S
          Player->MoveBackward();
          break;
          
        case VK_SPACE:
          Player->MoveUp();
          break;
          
        case VK_SHIFT:
          Player->MoveDown();
          break;
        
        case VK_LEFT:
          CurrentObject.previousT();
          break;
        
        case VK_RIGHT:
          CurrentObject.nextT();
          break;
        
        case VK_DOWN:
          CurrentObject.previousF();
          break;
        
        case VK_UP:
          CurrentObject.nextF();
          break;
        
        case 0x51:
          CurrentObject.change_texroutine();
          break;
        break;
      }
  }
}

void MessagesToFrame(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, WindowFrame *WF) {
  D3DXVECTOR2 CurPos;
  switch (msg) {
    case WM_LBUTTONDOWN:
      CurPos.x = LOWORD(lParam);
      CurPos.y = HIWORD(lParam);
      WF->button_down(CurPos);
    break;

    case WM_LBUTTONUP:
      CurPos.x = LOWORD(lParam);
      CurPos.y = HIWORD(lParam);
      WF->button_up(CurPos);
      break;
    
    case WM_MOUSEMOVE:
      CurPos.x = LOWORD(lParam);
      CurPos.y = HIWORD(lParam);
      WF->mouse_move(CurPos);
  }
}

void CallbackMainMenu(ControlMessage msg) {
  switch (msg.message) {
    case MSG_BUTTONUP:
      switch (msg.ThisObject->get_id()) {
        case ID_BUTTONPLAY:
          InitScene();
          
          CurrentScene = 2;
          CurrentFrame = 0;
          break;
          
        case ID_BUTTONSETTINGS:
	  CurrentScene = 1;
          CurrentFrame = 2;
          break;
          
        case ID_BUTTONCREATOR:
	  CurrentScene = 1;
          CurrentFrame = 3;
          break;
          
        case ID_BUTTONEXIT:
          PostQuitMessage(0);
          break;
      }
      break;
  }
}

void CallbackPlayMenu(ControlMessage msg) {
  char strfile[256];
  switch (msg.message) {
    case MSG_BUTTONUP:
      switch (msg.ThisObject->get_id()) {
        case ID_BUTTONCONTINUE:
          CurrentScene = 2;
          CurrentFrame = 0;
          break;
          
        case ID_BUTTONNEW:
          if (! SaveScene(worldname))
              MessageBox(NULL, "Ошибка сохранения!", "Ошибка", MB_OK | MB_ICONERROR);
          if (! CreateScene(worldname)) {
            MessageBox(NULL, "Ошибка генерации!", "Ошибка", MB_OK | MB_ICONERROR);
            break;
          }
          UpdateObject();
          
          CurrentScene = 2;
          CurrentFrame = 0;
          break;
          
        case ID_BUTTONLOAD:
          if (OpenFileDialog(NULL, strfile)) {
            strcpy(worldname, strfile);
            if (! LoadScene(worldname)) {
              MessageBox(NULL, "Ошибка загрузки!", "Ошибка", MB_OK | MB_ICONERROR);
              break;
            }
            UpdateObject();
          }
          
          CurrentScene = 2;
          CurrentFrame = 0;
          break;
          
        case ID_BUTTONSAVE:
          if (SaveFileDialog(NULL, strfile)) {
            strcpy(worldname, strfile);
            if (! SaveScene(worldname))
              MessageBox(NULL, "Ошибка сохранения!", "Ошибка", MB_OK | MB_ICONERROR);
          }
          
          CurrentScene = 2;
          CurrentFrame = 0;
          break;
          
        case ID_BUTTONTOMENU:
          if (! SaveScene(worldname))
              MessageBox(NULL, "Ошибка сохранения!", "Ошибка", MB_OK | MB_ICONERROR);
        
          CurrentScene = 1;
          CurrentFrame = 1;
          break;
      }
      break;
  }
}

void CallbackBlockChooseMenu(ControlMessage msg) {
  static D3DXVECTOR2 scroll(0, 0);
  switch (msg.message) {
    case MSG_BUTTONUP://поменять блок
      if (strcmp(msg.ThisObject->get_msgstr().c_str(), "block") == 0) {
        CurrentObject.set_currentF(msg.ThisObject->get_id());
        for (int i = 0; i < CurrentObject.get_sizeF(); i++)
          GroupsOriginTextures[i].lock_control(true);
        GroupsOriginTextures[msg.ThisObject->get_id()].lock_control(false);
        GroupsOriginTextures[msg.ThisObject->get_id()].show_control(true);
        if (CurrentObject.get_blockinfo().OriginTex) {//установить отображение текстур в меню
          GroupTextures.lock_control(true);
        }
        else {
          GroupTextures.lock_control(false);
        }
        break;
      }
      
      if (strcmp(msg.ThisObject->get_msgstr().c_str(), "texture") == 0) {
        CurrentObject.set_texroutine(0);
        CurrentObject.set_currentT(msg.ThisObject->get_id());
        break;
      }
      
      if (strcmp(msg.ThisObject->get_msgstr().c_str(), "origintex") == 0) {
        CurrentObject.set_texroutine(1);
        CurrentObject.set_currentT(msg.ThisObject->get_id());
        break;
      }
      
      switch (msg.ThisObject->get_id()) {          
        case ID_BUTTONMENUCLOSE:
          CurrentScene = 2;
          CurrentFrame = 0;
          break;
          
        case ID_BUTTONSCROLLUP:
          scroll += D3DXVECTOR2(0, 50);
          if (scroll.y > 0) scroll.y = 0;
          if (GroupBlocks.get_state().visible) {
            GroupBlocks.set_scroll(scroll);
          }
          if (GroupTextures.get_state().visible) {
            GroupTextures.set_scroll(scroll);
          }
          for (int i = 0; i < CurrentObject.get_sizeF(); i++)
            if (GroupsOriginTextures[i].get_state().visible) {
              GroupsOriginTextures[i].set_scroll(scroll);
            }
          break;
        
        case ID_BUTTONSCROLLDOWN:
          scroll += D3DXVECTOR2(0, -50);
          if (GroupBlocks.get_state().visible) {
            GroupBlocks.set_scroll(scroll);
          }
          if (GroupTextures.get_state().visible) {
            GroupTextures.set_scroll(scroll);
          }
          for (int i = 0; i < CurrentObject.get_sizeF(); i++)
            if (GroupsOriginTextures[i].get_state().visible) {
              GroupsOriginTextures[i].set_scroll(scroll);
            }
          break;
          
        case ID_SWITCHTAB:
          scroll = D3DXVECTOR2(0, 0);
          GroupBlocks.set_scroll(scroll);
          GroupTextures.set_scroll(scroll);
          for (int i = 0; i < CurrentObject.get_sizeF(); i++)
            GroupsOriginTextures[i].set_scroll(scroll);
          BlockChooseMenu.set_size(BlockChooseMenu.get_size());
          break;
      }
      break;
  }
}

void CallbackSettingsMenu(ControlMessage msg) {
  switch (msg.message) {
    case MSG_BUTTONUP://поменять блок      
      switch (msg.ThisObject->get_id()) {          
        case ID_BUTTONMENUCLOSE:
          CurrentScene = 1;
          CurrentFrame = 1;
          break;
		  
		case ID_BUTTONSKYBLEFT:
		  if (CurrentSkybox == 0)
			CurrentSkybox = NumSkyboxes - 1;
		  else
			CurrentSkybox -= 1;
		
		  for (int i = 0; i < NumSkyboxes; i++) {
			ButtonSkyBChoose[i].show_control(false);
		  }
		  ButtonSkyBChoose[CurrentSkybox].show_control(true);
		  break;
		  
		case ID_BUTTONSKYBRIGHT:
		  if (CurrentSkybox == NumSkyboxes - 1)
			CurrentSkybox = 0;
		  else
			CurrentSkybox += 1;
		
		  for (int i = 0; i < NumSkyboxes; i++) {
			ButtonSkyBChoose[i].show_control(false);
		  }
		  ButtonSkyBChoose[CurrentSkybox].show_control(true);
		  break;
      }
      break;
  }
}

void CallbackCreditsMenu(ControlMessage msg) {
  switch (msg.message) {
    case MSG_BUTTONUP://поменять блок      
      switch (msg.ThisObject->get_id()) {          
        case ID_BUTTONMENUCLOSE:
          CurrentScene = 1;
          CurrentFrame = 1;
          break;
      }
      break;
  }
}

