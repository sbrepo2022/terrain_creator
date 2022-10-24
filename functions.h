//регистрация класса окна
bool WndClass(HINSTANCE, WNDPROC, LPCTSTR);
//создание окна
HWND CreateMainWindow(HINSTANCE hInst, LPCSTR title, int width, int height);
//оконная функция обратного вызова
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//инициализация Direct3D устройства
bool D3DInit(HWND hWnd, IDirect3D9 **pD3D, IDirect3DDevice9 **pDevice);
//освобождение ресурсов
void D3DRelease(IDirect3D9 **pD3D, IDirect3DDevice9 **pDevice);
//--------------
//рендрединг сцены (меню)
void Render1();
//рендрединг сцены (игра)
void Render2();
//отрисовка экрана загрузки
void DrawLoadingScreen(HWND hwnd);
//отрисовка объектов блоков
void DrawObject();
//отрисовка скайбокса
void DrawSkyBox();
//отрисовка индикации блоков
void DrawIndicate(float Vh, int routine, IDirect3DTexture9 *Texture);
//--------------
//инициализация игровых ресурсов
bool InitResources();
//удаление игровых ресурсов
void DeleteResources();
//инициализация экрана загрузки
bool InitLoadingScreen();
//инициализация спрайта индикации
bool InitIndicateSprite();
//инициализация текстур
bool InitTextures();
//инициализация текстур выбора блоков
bool InitTexturesBlockChoose();
//инициализация шаблонов объектов
bool InitObjects();
//инициализация фреймов
bool InitFrames();
//удаление фреймов
void DeleteFrames();
//загрузка объектов из файла .obj
int LoadObject(char *filename, Triangle *trianglelist);
//инициализация итератора блоков
void SetBlocks();
//установка сцены
bool InitScene();
//загрузка сцены
bool LoadScene(char *filename);
//сохранение сцены
bool SaveScene(char *filename);
//создание сцены
bool CreateScene(char *filename);
//установка/удаление блоков со сцены
bool UpdateScene();
//обновление буфера вершин сцены
bool UpdateObject();
//обновление буфера вершин скайбокса
bool UpdateSkyBox();
//обновление буфера вершин индикации блоков
bool UpdateIndicate();
//очистка буферов вершин
void DeleteBuff();
//начальная установка камеры съемки
void SetView();
//обновление камеры съемки
void UpdateView();
//обновление материалов
void UpdateMaterial();
//обновление освещения
void UpdateLight();
//---------------
//получение куба для проверки пересечения
void CubeIntersect(Cube *cube, Position pos);
//проверка пересечения направления взгляда с кубом
float GetIntersect(Position *block, Position *before, Camera Player);
//поворот блока при установке
void RotateObject(ObjectCube *oc, Position block, Position before, Camera Player);
//получение информации о соседних блоках
BorderInfo GetBorderInfo(int i, int j, int f);
//открытие диалога загрузки файлов
bool OpenFileDialog(HWND hwnd, char *filename);
//открытие диалога сохранения файлов
bool SaveFileDialog(HWND hwnd, char *filename);
//курсор в центр экрана
void SetCursorToCenter(HWND hWnd);
//получить фрейм по индексу
WindowFrame* GetFrame(int scene, int frame);
//---------------
//поворот камеры
void RotatePlayer(HWND hWnd, Camera *Player, bool *change);
//движение камеры
void MovePlayer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Camera *Player);
//--
//сообщения для меню
void MessagesToFrame(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, WindowFrame *WF);
//---------------
void CallbackMainMenu(ControlMessage msg);
void CallbackPlayMenu(ControlMessage msg);
void CallbackBlockChooseMenu(ControlMessage msg);
void CallbackSettingsMenu(ControlMessage msg);
void CallbackCreditsMenu(ControlMessage msg);
