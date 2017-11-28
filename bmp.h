
#pragma once
#include <windows.h>		// Header File For Windows - has structures for BMP format

#include <stdio.h>	    	// Header File For Standard Input/Output
#include <stdlib.h>
#pragma warning(disable:4996)
class AUX_RGBImageRec {
	void convertBGRtoRGB();
public:
	byte *data;
	DWORD sizeX;
	DWORD sizeY;
	bool NoErrors;
	AUX_RGBImageRec() : NoErrors(false), data(NULL) {};
	AUX_RGBImageRec(const char *FileName);
	~AUX_RGBImageRec();
	bool loadFile(const char *FileName);
	friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);
};
