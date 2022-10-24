#include "ImgPrint.h"

void ImgPrint::copy(const ImgPrint &src) {
  this->left = src.left;
  this->Img = src.Img;
  this->cut_x = src.cut_x;
  this->cut_y = src.cut_y;
  this->Bitmap = src.Bitmap;
  this->cut_width = src.cut_width;
  this->cut_height = src.cut_height;
  this->IsTransp = src.IsTransp;

  if (this->hBitmap)
    DeleteObject(this->hBitmap);

  this->hBitmap = CopyImage(src.hBitmap, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
}

ImgPrint::ImgPrint(std::string Img, int left, int top) {
  this->left = left;
  this->top = top;
  this->Img = Img;
  this->cut_x = this->cut_y = 0;
  this->IsTransp = 0;

  this->hBitmap = LoadImage(NULL, this->Img.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  GetObject(this->hBitmap, sizeof(BITMAP), &this->Bitmap);
  this->cut_width = Bitmap.bmWidth;
  this->cut_height = Bitmap.bmHeight;
}

ImgPrint::ImgPrint(const ImgPrint &src) {
  this->left = src.left;
  this->top = src.top;
  this->Img = src.Img;
  this->cut_x = src.cut_x;
  this->cut_y = src.cut_y;
  this->Bitmap = src.Bitmap;
  this->cut_width = src.cut_width;
  this->cut_height = src.cut_height;
  this->IsTransp = src.IsTransp;

  if (this->hBitmap)
    DeleteObject(this->hBitmap);

  this->hBitmap = CopyImage(src.hBitmap, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
}

ImgPrint::~ImgPrint() {
  DeleteObject(this->hBitmap);
  DeleteObject(this->hBitmapMask);
}

void ImgPrint::init(std::string Img, int left, int top) {
  this->left = left;
  this->top = top;
  this->Img = Img;
  this->cut_x = this->cut_y = 0;
  this->IsTransp = 0;
}

void ImgPrint::load() {
  if (this->hBitmap)
    DeleteObject(this->hBitmap);

  this->hBitmap = LoadImage(NULL, this->Img.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

  GetObject(this->hBitmap, sizeof(BITMAP), &this->Bitmap);
  this->cut_width = Bitmap.bmWidth;
  this->cut_height = Bitmap.bmHeight;
}

void ImgPrint::destroy() {
  DeleteObject(this->hBitmap);
  DeleteObject(this->hBitmapMask);
}

void ImgPrint::set_img(std::string Img) {
  this->Img = Img;
}

void ImgPrint::set_pos(int left, int top) {
  this->left = left;
  this->top = top;
}

void ImgPrint::set_cut(int cut_x, int cut_y, int cut_width, int cut_height) {
  this->cut_x = cut_x;
  this->cut_y = cut_y;
  if (cut_width == 0)
    this->cut_width = this->Bitmap.bmWidth;
  else
    this->cut_width = cut_width;

  if (cut_height == 0)
    this->cut_height = this->Bitmap.bmHeight;
  else
    this->cut_height = cut_height;
}

void ImgPrint::set_angle(int angle) {
  this->angle = angle;
}

void ImgPrint::set_mask(HANDLE hBitmapMask, bool IsTransp) {
  this->IsTransp = IsTransp;
  this->hBitmapMask = CopyImage(hBitmapMask, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
}

void ImgPrint::get_himg(HANDLE *hBitmap) {
  *hBitmap = this->hBitmap;
}

void ImgPrint::get_pos(int *left, int *top) {
  *left = this->left;
  *top = this->top;
}

void ImgPrint::get_cut(int *cut_x, int *cut_y, int *cut_width, int *cut_height) {
  *cut_x = this->cut_x;
  *cut_y = this->cut_y;
  *cut_width = this->cut_width;
  *cut_height = this->cut_height;
}

void ImgPrint::get_angle(int *angle) {
  *angle = this->angle;
}

bool ImgPrint::get_state() {
  if (hBitmap)
    return 1;
  else
    return 0;
}

void ImgPrint::print_std(HDC hdc) {
  XFORM xform;
  double angle_rotate;
  int centerX = cut_width/2 + left + cut_x;
  int centerY = cut_height/2 + top + cut_y;
  int LastMode;

  angle_rotate = angle * M_PI / 180.0;
  xform.eM11 = (float)cos(angle_rotate);
  xform.eM12 = (float)sin(angle_rotate);
  xform.eM21 = (float)-sin(angle_rotate);
  xform.eM22 = (float)cos(angle_rotate);
  xform.eDx = (float)(centerX - cos(angle_rotate)*centerX + sin(angle_rotate)*centerY);
  xform.eDy = (float)(centerY - cos(angle_rotate)*centerY - sin(angle_rotate)*centerX);

  HDC hDC, hDC2;
  hDC = CreateCompatibleDC(hdc);
  SelectObject(hDC, this->hBitmap);
  if (this->IsTransp) {
    hDC2 = CreateCompatibleDC(hdc);
    SelectObject(hDC2, this->hBitmapMask);
  }

  LastMode = SetGraphicsMode(hdc, GM_ADVANCED);
  SetWorldTransform(hdc, &xform);
  if (! this->IsTransp) {
    StretchBlt(hdc, this->left, this->top, this->cut_width, this->cut_height, hDC, this->cut_x, this->cut_y, this->cut_width, this->cut_height, SRCCOPY);
  }
  else {
    StretchBlt(hdc, this->left, this->top, this->cut_width, this->cut_height, hDC2, this->cut_x, this->cut_y, this->cut_width, this->cut_height, SRCPAINT);
    StretchBlt(hdc, this->left, this->top, this->cut_width, this->cut_height, hDC, this->cut_x, this->cut_y, this->cut_width, this->cut_height, SRCAND);
  }

  xform.eM11 = 1.0f;
  xform.eM12 = 0.0f;
  xform.eM21 = 0.0f;
  xform.eM22 = 1.0f;
  xform.eDx = 0.0f;
  xform.eDy = 0.0f;
  SetWorldTransform(hdc, &xform);
  SetGraphicsMode(hdc, LastMode);

  DeleteDC(hDC);
  DeleteDC(hDC2);
}

void ImgPrint::print_ex(HDC hdc, int width, int height) {
  XFORM xform;
  double angle_rotate;
  int centerX = cut_width/2 + left + cut_x;
  int centerY = cut_height/2 + top + cut_y;
  int LastMode;

  angle_rotate = angle * M_PI / 180.0;
  xform.eM11 = (float)cos(angle_rotate);
  xform.eM12 = (float)sin(angle_rotate);
  xform.eM21 = (float)-sin(angle_rotate);
  xform.eM22 = (float)cos(angle_rotate);
  xform.eDx = (float)(centerX - cos(angle_rotate)*centerX + sin(angle_rotate)*centerY);
  xform.eDy = (float)(centerY - cos(angle_rotate)*centerY - sin(angle_rotate)*centerX);

  HDC hDC, hDC2;
  hDC = CreateCompatibleDC(hdc);
  SelectObject(hDC, this->hBitmap);
  if (this->IsTransp) {
    hDC2 = CreateCompatibleDC(hdc);
    SelectObject(hDC2, this->hBitmapMask);
  }

  LastMode = SetGraphicsMode(hdc, GM_ADVANCED);
  SetWorldTransform(hdc, &xform);
  if (! this->IsTransp) {
    StretchBlt(hdc, this->left, this->top, width, height, hDC, this->cut_x, this->cut_y, this->cut_width, this->cut_height, SRCCOPY);
  }
  else {
    StretchBlt(hdc, this->left, this->top, width, height, hDC2, this->cut_x, this->cut_y, this->cut_width, this->cut_height, SRCPAINT);
    StretchBlt(hdc, this->left, this->top, width, height, hDC, this->cut_x, this->cut_y, this->cut_width, this->cut_height, SRCAND);
  }

  xform.eM11 = 1.0f;
  xform.eM12 = 0.0f;
  xform.eM21 = 0.0f;
  xform.eM22 = 1.0f;
  xform.eDx = 0.0f;
  xform.eDy = 0.0f;
  SetWorldTransform(hdc, &xform);
  SetGraphicsMode(hdc, LastMode);

  DeleteDC(hDC);
  DeleteDC(hDC2);
}

ImgPrint& ImgPrint::operator=(const ImgPrint &src) {
  this->copy(src);
  return *this;
}


void ImgPrintRC::copy(const ImgPrintRC &src) {
  this->hinst = src.hinst;
  this->left = src.left;
  this->Img = src.Img;
  this->cut_x = src.cut_x;
  this->cut_y = src.cut_y;
  this->Bitmap = src.Bitmap;
  this->cut_width = src.cut_width;
  this->cut_height = src.cut_height;
  this->IsTransp = src.IsTransp;

  if (this->hBitmap)
    DeleteObject(this->hBitmap);

  this->hBitmap = CopyImage(src.hBitmap, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
}

ImgPrintRC::ImgPrintRC(HINSTANCE hinst, std::string Img, int left, int top) {
  this->hinst = hinst;
  this->left = left;
  this->top = top;
  this->Img = Img;
  this->cut_x = this->cut_y = 0;
  this->IsTransp = 0;

  this->hBitmap = LoadBitmap(this->hinst, this->Img.c_str());

  GetObject(this->hBitmap, sizeof(BITMAP), &this->Bitmap);
  this->cut_width = Bitmap.bmWidth;
  this->cut_height = Bitmap.bmHeight;
}

ImgPrintRC::ImgPrintRC(const ImgPrintRC &src) {
  this->hinst = src.hinst;
  this->left = src.left;
  this->top = src.top;
  this->Img = src.Img;
  this->cut_x = src.cut_x;
  this->cut_y = src.cut_y;
  this->Bitmap = src.Bitmap;
  this->cut_width = src.cut_width;
  this->cut_height = src.cut_height;
  this->IsTransp = src.IsTransp;

  if (this->hBitmap)
    DeleteObject(this->hBitmap);

  this->hBitmap = CopyImage(src.hBitmap, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
}

void ImgPrintRC::init(HINSTANCE hinst, std::string Img, int left, int top) {
  this->hinst = hinst;
  this->left = left;
  this->top = top;
  this->Img = Img;
  this->cut_x = this->cut_y = 0;
  this->IsTransp = 0;
}

void ImgPrintRC::load() {
  if (this->hBitmap)
    DeleteObject(this->hBitmap);

  this->hBitmap = LoadBitmap(this->hinst, this->Img.c_str());

  GetObject(this->hBitmap, sizeof(BITMAP), &this->Bitmap);
  this->cut_width = Bitmap.bmWidth;
  this->cut_height = Bitmap.bmHeight;
}

ImgPrintRC& ImgPrintRC::operator=(const ImgPrintRC &src) {
  this->copy(src);
  return *this;
}