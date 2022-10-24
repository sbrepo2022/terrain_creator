//����������� ������ ����
bool WndClass(HINSTANCE, WNDPROC, LPCTSTR);
//�������� ����
HWND CreateMainWindow(HINSTANCE hInst, LPCSTR title, int width, int height);
//������� ������� ��������� ������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//������������� Direct3D ����������
bool D3DInit(HWND hWnd, IDirect3D9 **pD3D, IDirect3DDevice9 **pDevice);
//������������ ��������
void D3DRelease(IDirect3D9 **pD3D, IDirect3DDevice9 **pDevice);
//--------------
//���������� ����� (����)
void Render1();
//���������� ����� (����)
void Render2();
//��������� ������ ��������
void DrawLoadingScreen(HWND hwnd);
//��������� �������� ������
void DrawObject();
//��������� ���������
void DrawSkyBox();
//��������� ��������� ������
void DrawIndicate(float Vh, int routine, IDirect3DTexture9 *Texture);
//--------------
//������������� ������� ��������
bool InitResources();
//�������� ������� ��������
void DeleteResources();
//������������� ������ ��������
bool InitLoadingScreen();
//������������� ������� ���������
bool InitIndicateSprite();
//������������� �������
bool InitTextures();
//������������� ������� ������ ������
bool InitTexturesBlockChoose();
//������������� �������� ��������
bool InitObjects();
//������������� �������
bool InitFrames();
//�������� �������
void DeleteFrames();
//�������� �������� �� ����� .obj
int LoadObject(char *filename, Triangle *trianglelist);
//������������� ��������� ������
void SetBlocks();
//��������� �����
bool InitScene();
//�������� �����
bool LoadScene(char *filename);
//���������� �����
bool SaveScene(char *filename);
//�������� �����
bool CreateScene(char *filename);
//���������/�������� ������ �� �����
bool UpdateScene();
//���������� ������ ������ �����
bool UpdateObject();
//���������� ������ ������ ���������
bool UpdateSkyBox();
//���������� ������ ������ ��������� ������
bool UpdateIndicate();
//������� ������� ������
void DeleteBuff();
//��������� ��������� ������ ������
void SetView();
//���������� ������ ������
void UpdateView();
//���������� ����������
void UpdateMaterial();
//���������� ���������
void UpdateLight();
//---------------
//��������� ���� ��� �������� �����������
void CubeIntersect(Cube *cube, Position pos);
//�������� ����������� ����������� ������� � �����
float GetIntersect(Position *block, Position *before, Camera Player);
//������� ����� ��� ���������
void RotateObject(ObjectCube *oc, Position block, Position before, Camera Player);
//��������� ���������� � �������� ������
BorderInfo GetBorderInfo(int i, int j, int f);
//�������� ������� �������� ������
bool OpenFileDialog(HWND hwnd, char *filename);
//�������� ������� ���������� ������
bool SaveFileDialog(HWND hwnd, char *filename);
//������ � ����� ������
void SetCursorToCenter(HWND hWnd);
//�������� ����� �� �������
WindowFrame* GetFrame(int scene, int frame);
//---------------
//������� ������
void RotatePlayer(HWND hWnd, Camera *Player, bool *change);
//�������� ������
void MovePlayer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Camera *Player);
//--
//��������� ��� ����
void MessagesToFrame(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, WindowFrame *WF);
//---------------
void CallbackMainMenu(ControlMessage msg);
void CallbackPlayMenu(ControlMessage msg);
void CallbackBlockChooseMenu(ControlMessage msg);
void CallbackSettingsMenu(ControlMessage msg);
void CallbackCreditsMenu(ControlMessage msg);
