#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

struct Position {
  int x, y, z;
  Position(){}
  Position(int x, int y, int z){this->x = x; this->y = y; this->z = z;}
};

struct Rotate {
  float x, y, z;
  Rotate(){}
  Rotate(float x, float y, float z){this->x = x; this->y = y; this->z = z;}
};

struct Vector2D {
  float x, y;
};

struct Index {
  int p, t, n;
};

struct BorderInfo {
  bool n1, n2, n3, n4, n5, n6;
  BorderInfo(){n1 = n2 = n3 = n4 = n5 = n6 = false;}
};

struct TextureInfo {
  IDirect3DTexture9 *texture;
  int TexType;
  bool transparent;
};

struct BlockInfo {
  int blockID, RotateType, OriginTex;
  std::vector<TextureInfo> textures;
};

struct Vertex {
  D3DXVECTOR3 p;
  D3DXVECTOR3 n;
  DWORD color;
  float tv, tu;
  init(float x, float y, float z, DWORD color, float tu, float tv)
  {p = D3DXVECTOR3(x, y, z); this->color = color; this->tu = tu; this->tv = tv;}
};

struct Triangle {
  Vertex vertex1;
  Vertex vertex2;
  Vertex vertex3;
  void ComputeNormal();
};

struct Square {
  Triangle triangle1;
  Triangle triangle2;
};

struct Cube {
  Square square1;
  Square square2;
  Square square3;
  Square square4;
  Square square5;
  Square square6;
};

class ObjectCube {
protected:
  static std::vector<std::vector<Triangle> > templ;
  static std::vector<int> templnums;
  static const int stdtempl = 9;
  //Данные объекта
  Position pos;
  Rotate rot;
  DWORD color;
  IDirect3DTexture9 *texture;
  int Type, TexType;
  //Свойства объекта
  bool is_created, transparent;
  int RotateType;
  
public:
  static void RegisterTemplate(Triangle *trianglelist, int numtriangles, int templnum);

  ObjectCube(){is_created = 0; transparent = 0;}
  ObjectCube(Position pos, Rotate rot, DWORD color, IDirect3DTexture9 *texture, int Type, int TexType)
  {this->pos = pos; this->rot = rot; this->color = color; this->texture = texture; is_created = 1; this->Type = Type; this->TexType = TexType;}
  void init(Position pos, Rotate rot, DWORD color, IDirect3DTexture9 *texture, int Type, int TexType)
  {this->pos = pos; this->rot = rot; this->color = color; this->texture = texture; is_created = 1; this->Type = Type; this->TexType = TexType;}
  void set_pos(Position pos){this->pos = pos;}
  Position get_pos(){return pos;}
  void set_rotate(Rotate rot){this->rot = rot;}
  Rotate get_rotate(){return rot;}
  int CreateCube(Triangle *trianglelist, BorderInfo bi);
  IDirect3DTexture9* GetTexture(){return texture;}
  bool is_object(){return is_created;}
  void destroy(){is_created = 0;}
  void SetTransparent(bool transparent){this->transparent = transparent;}
  bool is_transparent(){return transparent;}
  void SetRotateType(int RotateType){this->RotateType = RotateType;}
  int get_rotatetype(){return RotateType;}
  int get_type(){return Type;}
  int get_textype(){return TexType;}
  int get_numtriangles(BorderInfo bi);
};

class ObjectList {
protected:
  int currentF, currentT, TexRoutine;
  std::vector<TextureInfo> textures;
  std::vector<BlockInfo> blocks;
  bool change;
  
public:
  bool get_update(){if (change) return 1; else return 0; change = false;}

  int GetInd(int blockID) {
    for (int i = 0; i < blocks.size(); i++)
      if (blocks[i].blockID == blockID)
        return i;
    return 0;
  }

  void set_texture(IDirect3DTexture9 *texture, int TexType, bool transparent)
  {TextureInfo ti; ti.texture = texture; ti.TexType = TexType; ti.transparent = transparent; textures.push_back(ti);}

  void set_originaltexture(int blockID, IDirect3DTexture9 *texture, int TexType, bool transparent)
  {TextureInfo ti; ti.texture = texture; ti.TexType = TexType; ti.transparent = transparent; blocks[GetInd(blockID)].textures.push_back(ti);}

  void set_block(int blockID, int RotateType, int OriginTex){BlockInfo bl; bl.blockID = blockID; bl.RotateType = RotateType; bl.OriginTex = OriginTex; blocks.push_back(bl); /*if (OriginTex == 1) TexRoutine = 1;*/}

  void set_currentF(int currentF)
  {int OldTexRoutine = TexRoutine;
   this->currentF = currentF;
   if (this->currentF < 0) this->currentF = 0; if (this->currentF >= blocks.size()) this->currentF = blocks.size() - 1;
   if (blocks[this->currentF].OriginTex == 1) TexRoutine = 1; else TexRoutine = 0;
   if (OldTexRoutine != 0 || TexRoutine != 0) currentT = 0; change = true;}

  void nextF()
  {int OldTexRoutine = TexRoutine;
   currentF++; if (currentF >= blocks.size()) currentF = 0; if (blocks[currentF].OriginTex == 1) TexRoutine = 1; else TexRoutine = 0;
   if (OldTexRoutine != 0 || TexRoutine != 0) currentT = 0; change = true;}

  void previousF()
  {int OldTexRoutine = TexRoutine;
   currentF--; if (currentF < 0) currentF = blocks.size() - 1; if (blocks[currentF].OriginTex == 1) TexRoutine = 1; else TexRoutine = 0;
   if (OldTexRoutine != 0 || TexRoutine != 0) currentT = 0; change = true;}

  void set_currentT(int currentT)
  {this->currentT = currentT;
   if (TexRoutine == 0) {if (currentT < 0) this->currentT = 0; if (currentT >= textures.size()) this->currentT = textures.size() - 1;}
   if (TexRoutine == 1) {if (currentT < 0) this->currentT = 0; if (currentT >= blocks[currentF].textures.size()) this->currentT = blocks[currentF].textures.size() - 1;} change = true;}

  void nextT()
  {if (TexRoutine == 0) {currentT++; if (currentT >= textures.size()) currentT = 0;}
   if (TexRoutine == 1) {currentT++; if (currentT >= blocks[currentF].textures.size()) currentT = 0;} change = true;}

  void previousT()
  {if (TexRoutine == 0) {currentT--; if (currentT < 0) currentT = textures.size() - 1;}
   if (TexRoutine == 1) {currentT--; if (currentT < 0) currentT = blocks[currentF].textures.size() - 1;} change = true;}

  ObjectCube get_object();
  
  BlockInfo get_blockinfo() {return blocks[currentF];}
  
  TextureInfo get_textureinfo() {return textures[currentT];}
  
  void change_texroutine();
  
  void set_texroutine(bool texroutine);
  
  int get_sizeF() {return blocks.size();}
  
  int get_sizeT() {return textures.size();}
  
  int get_sizeOriginT(int blockID) {return blocks[GetInd(blockID)].textures.size();}
};