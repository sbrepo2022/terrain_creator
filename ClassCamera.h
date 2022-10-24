#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

class Camera {
private:
  int MoveType;

  D3DXVECTOR3 Position;
  D3DXVECTOR3 LookAt;
  D3DXVECTOR3 Right;
  D3DXVECTOR3 Up;
  bool Change;
  float RotateAroundLookAt;
  float RotateAroundRight;
  float RotateAroundUp;
  float Dist;
  D3DXMATRIX MatrixView;
  
  float distance;
  int cmd;
  
public:
  Camera();
  void SetMoveType(int MoveType);
  void SetPosition(D3DXVECTOR3 Pos);
  void SetAngle(float RotateAroundLookAt, float RotateAroundRight, float RotateAroundUp);
  void SetDist(float NewDist);
  void SetDistance(float distance);
  void SetCmd(int cmd);
  D3DXVECTOR3 GetPosition();
  D3DXVECTOR3 GetLookAt();
  float GetDistance();
  int GetCmd();
  void MoveForward();
  void MoveBackward();
  void MoveRight();
  void MoveLeft();
  void MoveUp();
  void MoveDown();
  void Roll(float angle);
  void RotateUpDown(float angle);
  void RotateLeftRight(float angle);
  void UpdateCameraStd(IDirect3DDevice9 *pDevice, bool is_change);
  void UpdateCameraMan(IDirect3DDevice9 *pDevice, bool is_change);
};

