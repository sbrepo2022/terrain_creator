#include "ClassCamera.h"

Camera::Camera() {
  MoveType = 0;
  Position = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
  LookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
  Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
  Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
  RotateAroundLookAt = RotateAroundRight = RotateAroundUp = 0;
  Dist = 0.1f;
  D3DXMatrixIdentity(&MatrixView);
  Change = true;
}

void Camera::SetMoveType(int MoveType) {
  this->MoveType = MoveType;
}

void Camera::SetPosition(D3DXVECTOR3 Pos) {
  Position = Pos;
  Change = true;
}

void Camera::SetAngle(float RotateAroundLookAt, float RotateAroundRight, float RotateAroundUp) {
  this->RotateAroundLookAt = RotateAroundLookAt;
  this->RotateAroundRight = RotateAroundRight;
  this->RotateAroundUp = RotateAroundUp;
  Change = true;
}

void Camera::SetDist(float NewDist) {
  Dist = NewDist;
}

void Camera::SetDistance(float distance) {
  this->distance = distance;
}

void Camera::SetCmd(int cmd) {
  this->cmd = cmd;
}

D3DXVECTOR3 Camera::GetPosition() {
  return Position;
}

D3DXVECTOR3 Camera::GetLookAt() {
  return LookAt;
}

float Camera::GetDistance() {
  return distance;
}

int Camera::GetCmd() {
  return cmd;
}

void Camera::MoveForward() {
  D3DXVECTOR3 LookAt;
  switch (MoveType) {
    case 0:
      LookAt = this->LookAt;
      break;
      
    case 1:
      LookAt = this->LookAt;
      LookAt.y = 0;
      D3DXVec3Normalize(&LookAt, &LookAt);
      break;
  }
  Position += Dist * LookAt;
  Change = true;
}

void Camera::MoveBackward() {
  D3DXVECTOR3 LookAt;
  switch (MoveType) {
    case 0:
      LookAt = this->LookAt;
      break;
      
    case 1:
      LookAt = this->LookAt;
      LookAt.y = 0;
      D3DXVec3Normalize(&LookAt, &LookAt);
      break;
  }
  Position -= Dist * LookAt;
  Change = true;
}

void Camera::MoveRight() {
  D3DXVECTOR3 Right;
  switch (MoveType) {
    case 0:
      Right = this->Right;
      break;
      
    case 1:
      Right = this->Right;
      Right.y = 0;
      D3DXVec3Normalize(&Right, &Right);
      break;
  }
  Position += Dist * Right;
  Change = true;
}

void Camera::MoveLeft() {
  D3DXVECTOR3 Right;
  switch (MoveType) {
    case 0:
      Right = this->Right;
      break;
      
    case 1:
      Right = this->Right;
      Right.y = 0;
      D3DXVec3Normalize(&Right, &Right);
      break;
  }
  Position -= Dist * Right;
  Change = true;
}

void Camera::MoveUp() {
  D3DXVECTOR3 Up;
  switch (MoveType) {
    case 0:
      Up = this->Up;
      break;
      
    case 1:
      Up = this->Up;
      Up.x = Up.z = 0;
      Up.y = 1;
      break;
  }
  Position += Dist * Up;
  Change = true;
}

void Camera::MoveDown() {
  D3DXVECTOR3 Up;
  switch (MoveType) {
    case 0:
      Up = this->Up;
      break;
      
    case 1:
      Up = this->Up;
      Up.x = Up.z = 0;
      Up.y = 1;
      break;
  }
  Position -= Dist * Up;
  Change = true;
}

void Camera::Roll(float angle) {
  RotateAroundLookAt += angle;
  Change = true;
}

void Camera::RotateUpDown(float angle) {
  RotateAroundRight += angle;
  Change = true;
}

void Camera::RotateLeftRight(float angle) {
  RotateAroundUp += angle;
  Change = true;
}

void Camera::UpdateCameraStd(IDirect3DDevice9 *pDevice, bool is_change) {
  if (Change || is_change) {
    D3DXMATRIX MatTotal;
    D3DXMATRIX MatRotateAroundLookAt;
    D3DXMATRIX MatRotateAroundRight;
    D3DXMATRIX MatRotateAroundUp;
    
    //вычисление матрицы по углу отклонения оси
    D3DXMatrixRotationAxis(&MatRotateAroundLookAt, &LookAt, RotateAroundLookAt);
    D3DXMatrixRotationAxis(&MatRotateAroundRight, &Right, RotateAroundRight);
    D3DXMatrixRotationAxis(&MatRotateAroundUp, &Up, RotateAroundUp);
    D3DXMatrixMultiply(&MatTotal, &MatRotateAroundLookAt, &MatRotateAroundRight);
    D3DXMatrixMultiply(&MatTotal, &MatRotateAroundUp, &MatTotal);
    //вычисление вектора по матрице
    D3DXVec3TransformCoord(&Right, &Right, &MatTotal);
    D3DXVec3TransformCoord(&Up, &Up, &MatTotal);
    D3DXVec3Cross(&LookAt, &Right, &Up);

    if (fabs(D3DXVec3Dot(&Up, &Right)) > 0.01) {
      D3DXVec3Cross(&Up, &LookAt, &Right);
    }

    D3DXVec3Normalize(&LookAt, &LookAt);
    D3DXVec3Normalize(&Right, &Right);
    D3DXVec3Normalize(&Up, &Up);
    
    float f41, f42, f43;
    f41 = -D3DXVec3Dot(&Right, &Position);
    f42 = -D3DXVec3Dot(&Up, &Position);
    f43 = -D3DXVec3Dot(&LookAt, &Position);
    
    D3DXMATRIX MatView;
    MatView = D3DXMATRIX(Right.x, Up.x, LookAt.x, 0.0f,
                         Right.y, Up.y, LookAt.y, 0.0f,
                         Right.z, Up.z, LookAt.z, 0.0f,
                         f41, f42, f43, 1.0f);
    
    pDevice->SetTransform(D3DTS_VIEW, &MatView);
    
    RotateAroundLookAt = RotateAroundRight = RotateAroundUp = 0;
    Change = false;
  }
}

void Camera::UpdateCameraMan(IDirect3DDevice9 *pDevice, bool is_change) {
  if (Change || is_change) {
    D3DXMATRIX MatTotal;
    D3DXMATRIX MatRotateAroundLookAt;
    D3DXMATRIX MatRotateAroundRight;
    D3DXMATRIX MatRotateAroundUp;
    
    D3DXVECTOR3 UpRotate(0.0f, 1.0f, 0.0f);
    
    D3DXMatrixRotationAxis(&MatRotateAroundUp, &UpRotate, RotateAroundUp);
    D3DXVec3TransformCoord(&LookAt, &LookAt, &MatRotateAroundUp);
    D3DXVec3TransformCoord(&Right, &Right, &MatRotateAroundUp);
    D3DXVec3Cross(&Up, &LookAt, &Right);
    
    D3DXMatrixRotationAxis(&MatRotateAroundRight, &Right, RotateAroundRight);
    D3DXVec3TransformCoord(&Up, &Up, &MatRotateAroundRight);
    D3DXVec3TransformCoord(&LookAt, &LookAt, &MatRotateAroundRight);
    D3DXVec3Cross(&Right, &Up, &LookAt);
    
    D3DXMatrixRotationAxis(&MatRotateAroundLookAt, &LookAt, RotateAroundLookAt);
    D3DXVec3TransformCoord(&Right, &Right, &MatRotateAroundLookAt);
    D3DXVec3TransformCoord(&Up, &Up, &MatRotateAroundLookAt);
    D3DXVec3Cross(&LookAt, &Right, &Up);

    D3DXVec3Normalize(&LookAt, &LookAt);
    D3DXVec3Normalize(&Right, &Right);
    D3DXVec3Normalize(&Up, &Up);
    
    float f41, f42, f43;
    f41 = -D3DXVec3Dot(&Right, &Position);
    f42 = -D3DXVec3Dot(&Up, &Position);
    f43 = -D3DXVec3Dot(&LookAt, &Position);
    
    D3DXMATRIX MatView;
    MatView = D3DXMATRIX(Right.x, Up.x, LookAt.x, 0.0f,
                         Right.y, Up.y, LookAt.y, 0.0f,
                         Right.z, Up.z, LookAt.z, 0.0f,
                         f41, f42, f43, 1.0f);
    
    pDevice->SetTransform(D3DTS_VIEW, &MatView);
    
    RotateAroundLookAt = RotateAroundRight = RotateAroundUp = 0;
    Change = false;
  }
}