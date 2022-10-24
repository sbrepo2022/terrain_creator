#include <windows.h>
#include <string>
#include <cmath>

//вывод изображения из файла .bmp
class ImgPrint {
protected:		//данные, для пользователя
  int left, top;		//левый верхний угол вывода
  std::string Img;		//имя изображения
  int cut_x, cut_y, cut_width, cut_height; //обрезка
  int angle;			//угол поворота
  bool IsTransp;		//прозрачность
			//данные, для объекта
  HANDLE hBitmap, hBitmapMask;	//дескриптор изображения
  BITMAP Bitmap;		//данные изображения

  virtual void copy(const ImgPrint &src);

public:
  ImgPrint(){};
  ImgPrint(std::string Img, int left, int top);	//конструктор, загрузка
  ImgPrint(const ImgPrint &src);	//конструктор, копирование
  ~ImgPrint();				//деструктор, удаление изображения
  void init(std::string Img, int left, int top);//инициализация
  virtual void load();				//загрузка
  void destroy();			//удаление

  void set_img(std::string Img);	//установка имени изображения
  void set_pos(int left, int top);	//установка точки вывода
  void set_cut(int cut_x, int cut_y, int cut_width, int cut_height);
  void set_angle(int angle);		//установка угла
  void set_mask(HANDLE hBitmapMask, bool IsTransp);//установка маски
  void get_himg(HANDLE *hBitmap);	//получение дескриптора изображения
  void get_pos(int *left, int *top);	//получение точки вывода
  void get_cut(int *cut_x, int *cut_y, int *cut_width, int *cut_height);
  void get_angle(int *angle);		//получение угла
  bool get_state();

  void print_std(HDC hdc);		//вывод, стандартный размер
  void print_ex(HDC hdc, int width, int height);//вывод, нестандартный размер

  virtual ImgPrint &operator=(const ImgPrint &src);
};


//вывод изображения из файла ресурсов
class ImgPrintRC : public ImgPrint {
protected:		//данные, для пользователя
  HINSTANCE hinst;		//дескриптор приложения

  void copy(const ImgPrintRC &src);

public:
  ImgPrintRC(){}
  ImgPrintRC(HINSTANCE hinst, std::string Img, int left, int top);//конструктор, загрузка
  ImgPrintRC(const ImgPrintRC &src);	//конструктор, копирование
  void init(HINSTANCE hinst, std::string Img, int left, int top);//инициализация
  void load();				//загрузка

  ImgPrintRC &operator=(const ImgPrintRC &src);
};
