#include <windows.h>
#include <string>
#include <cmath>

//����� ����������� �� ����� .bmp
class ImgPrint {
protected:		//������, ��� ������������
  int left, top;		//����� ������� ���� ������
  std::string Img;		//��� �����������
  int cut_x, cut_y, cut_width, cut_height; //�������
  int angle;			//���� ��������
  bool IsTransp;		//������������
			//������, ��� �������
  HANDLE hBitmap, hBitmapMask;	//���������� �����������
  BITMAP Bitmap;		//������ �����������

  virtual void copy(const ImgPrint &src);

public:
  ImgPrint(){};
  ImgPrint(std::string Img, int left, int top);	//�����������, ��������
  ImgPrint(const ImgPrint &src);	//�����������, �����������
  ~ImgPrint();				//����������, �������� �����������
  void init(std::string Img, int left, int top);//�������������
  virtual void load();				//��������
  void destroy();			//��������

  void set_img(std::string Img);	//��������� ����� �����������
  void set_pos(int left, int top);	//��������� ����� ������
  void set_cut(int cut_x, int cut_y, int cut_width, int cut_height);
  void set_angle(int angle);		//��������� ����
  void set_mask(HANDLE hBitmapMask, bool IsTransp);//��������� �����
  void get_himg(HANDLE *hBitmap);	//��������� ����������� �����������
  void get_pos(int *left, int *top);	//��������� ����� ������
  void get_cut(int *cut_x, int *cut_y, int *cut_width, int *cut_height);
  void get_angle(int *angle);		//��������� ����
  bool get_state();

  void print_std(HDC hdc);		//�����, ����������� ������
  void print_ex(HDC hdc, int width, int height);//�����, ������������� ������

  virtual ImgPrint &operator=(const ImgPrint &src);
};


//����� ����������� �� ����� ��������
class ImgPrintRC : public ImgPrint {
protected:		//������, ��� ������������
  HINSTANCE hinst;		//���������� ����������

  void copy(const ImgPrintRC &src);

public:
  ImgPrintRC(){}
  ImgPrintRC(HINSTANCE hinst, std::string Img, int left, int top);//�����������, ��������
  ImgPrintRC(const ImgPrintRC &src);	//�����������, �����������
  void init(HINSTANCE hinst, std::string Img, int left, int top);//�������������
  void load();				//��������

  ImgPrintRC &operator=(const ImgPrintRC &src);
};
