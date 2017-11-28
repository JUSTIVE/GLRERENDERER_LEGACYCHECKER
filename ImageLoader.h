#pragma once
#include<gl\glut.h>

//#pragma comment(lib, "glaux.lib ")

AUX_RGBImageRec *LoadBMP(char *Filename);

GLuint LoadGLTextures(char* textureFileName);