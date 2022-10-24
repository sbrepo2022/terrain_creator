#include "WindowFrame.h"

#define ALIGN_LEFT 2
#define ALIGN_CENTER 4
#define ALIGN_RIGHT 8
#define ALIGN_TOP 2
#define ALIGN_BOTTOM 8

#define WMB_STATIC 0
#define WMB_DYNAMIC 1

#define WMB_VERTICAL 16
#define WMB_HORIZONTAL 32

class WindowManagerSimple : public WindowManager {
public:
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerBlock : public WindowManager {
protected:
  UINT style; //1 - тип смещения, 2 - расположение,
  std::vector<float> control_sizes;
  
public:
  WindowManagerBlock() {style = WMB_STATIC | ALIGN_LEFT | WMB_HORIZONTAL;}
  void init(UINT style) {this->style = style;}
  void set_elem_size(float size) {control_sizes.push_back(size);}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerVertical1 : public WindowManager {  
protected:
  float WHratio, space;

public:
  void init(float WHratio, float space) {this->WHratio = WHratio; this->space = space;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerVertical2 : public WindowManager {  
protected:
  int align;
  float WHratio, space;

public:
  void init(float WHratio, float space, int align)
  {this->WHratio = WHratio; this->space = space; this->align = align;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerHorizontal1 : public WindowManager {  
protected:
  float WHratio, space;

public:
  void init(float WHratio, float space) {this->WHratio = WHratio; this->space = space;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerHorizontal2 : public WindowManager {  
protected:
  int align;
  float WHratio, space;

public:
  void init(float WHratio, float space, int align)
  {this->WHratio = WHratio; this->space = space; this->align = align;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerMatrix : public WindowManager {  
protected:
  int NumHeight;
  float space;

public:
  void init(int NumHeight, float space) {this->NumHeight = NumHeight; this->space = space;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerTab : public WindowManager {  
protected:
  int SwitchesHeight;
  float space;

public:
  void init(int SwitchesHeight, float space) {this->SwitchesHeight = SwitchesHeight; this->space = space;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowManagerWindowed : public WindowManager {
protected:
  RECT board;
  
public:
  void init(RECT board) {this->board = board;}
  void set_elements(const std::vector<Control*> &controls, D3DXVECTOR2 scroll, D3DXVECTOR2 size);
};

class WindowAnimationSimple : public WindowAnimation {
protected:
  IDirect3DDevice9 *pDevice;
  IDirect3DTexture9 *Texture;
  
public:
  void init(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *Texture);
  void draw(Control *control, D3DXVECTOR2 offset, D3DXVECTOR2 boundpos1, D3DXVECTOR2 boundpos2);
};

class WindowAnimationFill : public WindowAnimation {
protected:
  IDirect3DDevice9 *pDevice;
  IDirect3DTexture9 *Texture;
  
public:
  void init(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *Texture);
  void draw(Control *control, D3DXVECTOR2 offset, D3DXVECTOR2 boundpos1, D3DXVECTOR2 boundpos2);
};

class WindowAnimationWindowed : public WindowAnimation {
protected:
  IDirect3DDevice9 *pDevice;
  IDirect3DTexture9 *Texture;
  DWORD color1, color2;
  RECT board;
  bool using_texture;
  
public:
  void init(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *Texture, DWORD color1, DWORD color2, RECT board, bool using_texture);
  void draw(Control *control, D3DXVECTOR2 offset, D3DXVECTOR2 boundpos1, D3DXVECTOR2 boundpos2);
};

class WindowAnimationType1 : public WindowAnimation {
protected:
  IDirect3DDevice9 *pDevice;
  IDirect3DTexture9 *Texture1, *Texture2;
  
public:
  void init(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *Texture1, IDirect3DTexture9 *Texture2);
  void draw(Control *control, D3DXVECTOR2 offset, D3DXVECTOR2 boundpos1, D3DXVECTOR2 boundpos2);
};

class WindowAnimationType2 : public WindowAnimation {
protected:
  IDirect3DDevice9 *pDevice;
  IDirect3DTexture9 *TexturePict;
  float AnimationTime, CurrentAnimationTime, LastTick;
  DWORD color1, color2;
  
public:
  WindowAnimationType2() {AnimationTime = 0.001; LastTick = 0; CurrentAnimationTime = 0;}
  void init(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *TexturePict, DWORD color1, DWORD color2, float AnimationTime);
  void draw(Control *control, D3DXVECTOR2 offset, D3DXVECTOR2 boundpos1, D3DXVECTOR2 boundpos2);
};

class WindowAnimationAnimPict1 : public WindowAnimation {
protected:
  IDirect3DDevice9 *pDevice;
  IDirect3DTexture9 *TexturePict;
  float AnimationTime, CurrentAnimationTime, LastTick;
  D3DXVECTOR2 PictSize;
  int NumPictInLine, NumPict;
  
public:
  WindowAnimationAnimPict1() {AnimationTime = 0.001; LastTick = 0; CurrentAnimationTime = 0;}
  void init(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *TexturePict,
  int NumPictInLine, int NumPict, float AnimationTime);
  void draw(Control *control, D3DXVECTOR2 offset, D3DXVECTOR2 boundpos1, D3DXVECTOR2 boundpos2);
};