#include "ClassObject.h"

std::vector<std::vector<Triangle> > ObjectCube::templ;
std::vector<int> ObjectCube::templnums;

void Triangle::ComputeNormal() {
  D3DXVECTOR3 out;
  D3DXVECTOR3 u = vertex2.p - vertex1.p;
  D3DXVECTOR3 v = vertex3.p - vertex1.p;
  D3DXVec3Cross(&out, &u, &v);
  D3DXVec3Normalize(&out, &out);
  vertex1.n = vertex2.n = vertex3.n = out;
}

int ObjectCube::CreateCube(Triangle *trianglelist, BorderInfo bi) {
  int numtriangles = 0, index = 0;
  switch (Type) {
    case 0://куб
      if (! bi.n1) {
        trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
        trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
        trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
        trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
      }
      if (! bi.n2) {
        trianglelist[numtriangles].vertex1.init(0, 1, 0, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(0, 1, 1, color, 0.0f, 0.0f);
        trianglelist[numtriangles].vertex3.init(1, 1, 1, color, 0.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
        trianglelist[numtriangles].vertex1.init(0, 1, 0, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(1, 1, 1, color, 0.0f, 1.0f);
        trianglelist[numtriangles].vertex3.init(1, 1, 0, color, 1.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
      }
      if (! bi.n3) {
        trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(0, 1, 1, color, 0.0f, 0.0f);
        trianglelist[numtriangles].vertex3.init(0, 1, 0, color, 0.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
        trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 1.0f);
        trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
      }
      if (! bi.n4) {
        trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(1, 1, 1, color, 0.0f, 0.0f);
        trianglelist[numtriangles].vertex3.init(0, 1, 1, color, 0.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
        trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(0, 1, 1, color, 0.0f, 1.0f);
        trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
      }
      if (! bi.n5) {
        trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 0.0f);
        trianglelist[numtriangles].vertex3.init(1, 1, 1, color, 0.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
        trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(1, 1, 1, color, 0.0f, 1.0f);
        trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
      }
      if (! bi.n6) {
        trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 0.0f);
        trianglelist[numtriangles].vertex3.init(1, 1, 0, color, 0.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
        trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
        trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 1.0f);
        trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
        trianglelist[numtriangles].ComputeNormal();
        numtriangles++;
      }
      break;
    
    case 1://пирамида
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0.5f, 1, 0.5f, color, 0.0f, 0.5f);
      trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0.5f, 1, 0.5f, color, 0.0f, 0.5f);
      trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0.5f, 1, 0.5f, color, 0.0f, 0.5f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0.5f, 1, 0.5f, color, 0.0f, 0.5f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      break;
    
    case 2://крыша
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      break;
      
    case 3://угол крыши
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      break;
      
    case 4://угол крыши внутрь
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 1, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;

      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 1, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;

      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(0, 1, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 1, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 1, 1, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      break;
      
    case 5://верхушка крыши
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 0.5f, color, 0.5f, 0.5f);
      trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 0.5f, color, 0.5f, 0.0f);
      trianglelist[numtriangles].vertex3.init(0, 0.5f, 0.5f, color, 0.5f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 0.5f, color, 0.5f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 0.5f, color, 0.5f, 0.5f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 0.5f, color, 0.5f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0.5f, 0.5f, color, 0.5f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 0.5f, color, 0.5f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      break;
      
    case 6:
      break;
      
    case 7:
      break;
      
    case 8://полублок
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0, 0, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0, 0, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0.5f, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 1, color, 0.0f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0.5f, 1, color, 0.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0.5f, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 1, color, 0.0f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0.5f, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 1, color, 0.5f, 0.0f);
      trianglelist[numtriangles].vertex3.init(0, 0.5f, 0, color, 0.5f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 0, color, 0.5f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 1, color, 0.5f, 0.0f);
      trianglelist[numtriangles].vertex3.init(0, 0.5f, 1, color, 0.5f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 1, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 1, color, 0.5f, 1.0f);
      trianglelist[numtriangles].vertex3.init(0, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 0, color, 0.5f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0.5f, 1, color, 0.5f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(1, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 1, color, 0.5f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 1, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(0, 0.5f, 0, color, 0.5f, 0.0f);
      trianglelist[numtriangles].vertex3.init(1, 0.5f, 0, color, 0.5f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      trianglelist[numtriangles].vertex1.init(0, 0, 0, color, 1.0f, 0.0f);
      trianglelist[numtriangles].vertex2.init(1, 0.5f, 0, color, 0.5f, 1.0f);
      trianglelist[numtriangles].vertex3.init(1, 0, 0, color, 1.0f, 1.0f);
      trianglelist[numtriangles].ComputeNormal();
      numtriangles++;
      break;
    
    default:
      if (Type >= stdtempl) {
        for (int i = 0; i < templnums.size(); i++) {
          if (templnums[i] == Type)
            index = i;
        }
        for (int i = 0; i < templ[index].size(); i++) {
          trianglelist[i] = templ[index][i];
          trianglelist[i].vertex1.color = color;
          trianglelist[i].vertex2.color = color;
          trianglelist[i].vertex3.color = color;
        }
        numtriangles = templ[index].size();
      }
  }
  
  float x, y, angle;
  for (int i = 0; i < numtriangles; i++) {
    angle = rot.x * M_PI / 180;
    x = trianglelist[i].vertex1.p.z - 0.5f;
    y = trianglelist[i].vertex1.p.y - 0.5f;
    trianglelist[i].vertex1.p.z = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex1.p.y = cos(angle) * y + sin(angle) * x + 0.5f;
    
    x = trianglelist[i].vertex2.p.z - 0.5f;
    y = trianglelist[i].vertex2.p.y - 0.5f;
    trianglelist[i].vertex2.p.z = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex2.p.y = cos(angle) * y + sin(angle) * x + 0.5f;
    
    x = trianglelist[i].vertex3.p.z - 0.5f;
    y = trianglelist[i].vertex3.p.y - 0.5f;
    trianglelist[i].vertex3.p.z = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex3.p.y = cos(angle) * y + sin(angle) * x + 0.5f;
  }
  
  for (int i = 0; i < numtriangles; i++) {
    angle = rot.y * M_PI / 180;
    x = trianglelist[i].vertex1.p.x - 0.5f;
    y = trianglelist[i].vertex1.p.z - 0.5f;
    trianglelist[i].vertex1.p.x = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex1.p.z = cos(angle) * y + sin(angle) * x + 0.5f;
    
    x = trianglelist[i].vertex2.p.x - 0.5f;
    y = trianglelist[i].vertex2.p.z - 0.5f;
    trianglelist[i].vertex2.p.x = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex2.p.z = cos(angle) * y + sin(angle) * x + 0.5f;
    
    x = trianglelist[i].vertex3.p.x - 0.5f;
    y = trianglelist[i].vertex3.p.z - 0.5f;
    trianglelist[i].vertex3.p.x = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex3.p.z = cos(angle) * y + sin(angle) * x + 0.5f;
  }
  
  for (int i = 0; i < numtriangles; i++) {
    angle = rot.z * M_PI / 180;
    x = trianglelist[i].vertex1.p.y - 0.5f;
    y = trianglelist[i].vertex1.p.x - 0.5f;
    trianglelist[i].vertex1.p.y = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex1.p.x = cos(angle) * y + sin(angle) * x + 0.5f;
    
    x = trianglelist[i].vertex2.p.y - 0.5f;
    y = trianglelist[i].vertex2.p.x - 0.5f;
    trianglelist[i].vertex2.p.y = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex2.p.x = cos(angle) * y + sin(angle) * x + 0.5f;
    
    x = trianglelist[i].vertex3.p.y - 0.5f;
    y = trianglelist[i].vertex3.p.x - 0.5f;
    trianglelist[i].vertex3.p.y = cos(angle) * x - sin(angle) * y + 0.5f;
    trianglelist[i].vertex3.p.x = cos(angle) * y + sin(angle) * x + 0.5f;
  }
  
  D3DXVECTOR3 offset(pos.x, pos.y, pos.z);
  for (int i = 0; i < numtriangles; i++) {
    trianglelist[i].vertex1.p += offset;
    trianglelist[i].vertex2.p += offset;
    trianglelist[i].vertex3.p += offset;
    trianglelist[i].ComputeNormal();
  }
  
  return numtriangles;
}

int ObjectCube::get_numtriangles(BorderInfo bi) {
  int numtriangles = 0, index = 0;
  switch (Type) {
    case 0:
      if (! bi.n1) {
        numtriangles += 2;
      }
      if (! bi.n2) {
        numtriangles += 2;
      }
      if (! bi.n3) {
        numtriangles += 2;
      }
      if (! bi.n4) {
        numtriangles += 2;
      }
      if (! bi.n5) {
        numtriangles += 2;
      }
      if (! bi.n6) {
        numtriangles += 2;
      }
      break;
      
    case 1:
      numtriangles = 6;
      break;
      
    case 2:
      numtriangles = 8;
      break;
      
    case 3:
      numtriangles = 6;
      break;
      
    case 4:
      numtriangles = 10;
      break;
      
    case 5:
      numtriangles = 8;
      break;
      
    case 6:
      break;
      
    case 7:
      break;
      
    case 8:
      numtriangles = 12;
      break;
      
    default:
      if (Type >= stdtempl) {
        for (int i = 0; i < templnums.size(); i++) {
          if (templnums[i] == Type)
            index = i;
        }
        numtriangles = templ[index].size();
      }
  }
  return numtriangles;
}

void ObjectCube::RegisterTemplate(Triangle *trianglelist, int numtriangles, int templnum) {
  std::vector<Triangle> trianglevector;
  for (int i = 0; i < numtriangles; i++) {
    trianglevector.push_back(trianglelist[i]);
  }
  templ.push_back(trianglevector);
  templnums.push_back(templnum);
}

ObjectCube ObjectList::get_object() {
  ObjectCube obj;
  if (TexRoutine == 0) {
    obj.init(Position(0, 0, 0), Rotate(0, 0, 0), 0xffffffff, textures[currentT].texture, blocks[currentF].blockID, textures[currentT].TexType);
    obj.SetTransparent(textures[currentT].transparent);
  }
  else {
    obj.init(Position(0, 0, 0), Rotate(0, 0, 0), 0xffffffff, blocks[currentF].textures[currentT].texture, blocks[currentF].blockID, blocks[currentF].textures[currentT].TexType);
    obj.SetTransparent(blocks[currentF].textures[currentT].transparent);
  }
  
  obj.SetRotateType(blocks[currentF].RotateType);
  return obj;
}

void ObjectList::change_texroutine() {
  if (TexRoutine == 0) {
    if (blocks[currentF].textures.size() > 0)
      TexRoutine = 1;
  }
  else {
    if (blocks[currentF].OriginTex == 0) 
      TexRoutine = 0; 
  }
  currentT = 0;
  change = true;
}

void ObjectList::set_texroutine(bool texroutine) {
  if (texroutine == 0) {
    if (blocks[currentF].OriginTex == 0) 
      TexRoutine = 0; 
  }
  else {
    if (blocks[currentF].textures.size() > 0)
      TexRoutine = 1;
  }
  currentT = 0;
  change = true;
}