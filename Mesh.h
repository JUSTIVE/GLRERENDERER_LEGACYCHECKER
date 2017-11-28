#pragma once
#pragma warning(disable:4996)

#include<iostream>

#include<gl\glut.h>
#include<gl\GLU.h>
#include<gl\GL.h>
#include "bmp.h"

#include"BenList.h"
#include<cmath>
#include<map>
#include<cassert>
#include<vector>
#include<array>
#define MESH_DIR "../meshes/"
#define TEXTURE_DIR "../textures/"
#define MAX(x,y) (x>y)?x:y
#define NORMALIZE(x) x[0]=x[0]/(sqrt((x[0]*x[0])+(x[1]*x[1])+(x[2]*x[2]))) ,\
	x[1]=x[1]/(sqrt((x[0]*x[0])+(x[1]*x[1])+(x[2]*x[2]))),\
	x[2]=x[2]/(sqrt((x[0]*x[0])+(x[1]*x[1])+(x[2]*x[2])))
typedef GLint V3I[3];
typedef GLint V4I[4];
typedef double V2D[2];
typedef double V3D[3];
enum POLYGON{ TRIANGLE = 3, QUADS = 4 };
enum SHADING{FLAT_SHADING =0,GOURAUD_SHADING=1,PHONG_SHADING=2};
using namespace std;
struct Line{
	int a;
	int b;
};
class Mesh{
private:
	bool fileLoading;
	V3D* vertex;
	V3D* vertexNormal;
	V2D* vertexTexture;

	V4I* polygonQuad;
	V3I* polygonTri;

	V3D* faceNormal;
	
	
	V3I* textureFaceTri;
	V4I* textureFaceQuad;
		
	int vertexSize;
	int vertexNormalSize;
	int vertexTextureSize;
	int polygonSize;
	int polygonShape;//poly, quads?
	int textureFaceSize;

	BenList<Line*>* Wire;

	string fileName;
	bool randColor;
	int drawMode;
	double scaleValue;
	
	int hasSlash;
	void FaceNormalSetUp();
	void WireSetUp();
	void VertexNormalSetUp();
	void Scaler(double* maxPostionValue);
	void Alignmenter();
	
	void WireCleanup();
public:
	
	int prop;
	GLuint TextureID[1];
	Mesh();
	Mesh(const char * filename);
	void Init();
	void dataPrinter();

	void ForBetterWorld(string fileDir);
	void loadFile(string filename);

	void render();
	void modeChange();
	void setRandColor(bool value);
	string objPolisher(string filename);
	void changeProp();
	static void setProp(int number);
	int getDrawMode();
	
};

AUX_RGBImageRec *LoadBMP(char *Filename);
GLuint LoadGLTextures(char* textureFileName);