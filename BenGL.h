#pragma once
#define _USE_MATH_DEFINES

#include"Mesh.h"
#include<string>

#include<functional>
#include<math.h>
#include<time.h>

enum CAMERA_MODE{ CAMERA_MOVE = 1, CAMERA_ROTATE = 2, CAMERA_ZOOM = 4 };
enum SQUARE_OFFSET{ SQUARE_CENTER, SQUARE_LEFT_TOP };
//arithmetical macro functions
#define RADIAN(x) x*M_PI/180.0
#define ROUND(x,y) ((double)(round((x*pow(10,y)))/(pow(10,y))))

//for fast coloring
#define GLUT_PAINT_RED glColor3f(1,0,0);
#define GLUT_PAINT_GREEN glColor3f(0,1,0);
#define GLUT_PAINT_BLUE glColor3f(0,0,1);
#define GLUT_PAINT_WHITE glColor3f(1,1,1);
#define GLUT_PAINT_GRAY glColor3f(0.5,0.5,0.5);
using namespace std;
class Point{
public:
	Point();
	long double x, y;
};
class Screen {
public:
	Screen();
	Screen(int width,int height);
	int width;
	int height;
};
class FPS_Counter{
private:
	int timeindex;
	time_t timeFlip[2];
	string title;
public:
	FPS_Counter();
	string getTitle();
	void update();
};
class Flag{
private:
	static Flag* var;
public:
	Flag();
	Flag*getFlag();
	int isOrtho;
	int isMove;
	int isRotate;
	int isRight;
};
#ifdef _USE_CAMERA_DEPRECATED
class Camera {
	GLint mouse[2];
	GLint prevMouse[2];
	GLdouble zoom;
	GLdouble rotation[2];
	GLdouble move[2];
public:
	enum CAMERA_MODE{ MOVE, ROTATE, ZOOM, MOVE_AND_ZOOM, ROTATE_AND_ZOOM, MOVE_AND_ROTATE, CAMERA_ALL };
	CameraDeprecated();
	void cameraWork(int mode);
	void cameraMoveInit(Screen* screen,int x, int y);
	void cameraRotateInit(int x, int y);
	void cameraMoveAction(Screen* screen,int x, int y);
	void cameraRotateAction(int x, int y);
	void cameraZoomIn();
	void cameraZoomOut();
	void cameraInitiate();
};
#endif
class Camera{
private:
	double zoom;
	double origEyePos[3],origUpVec[3];
	double swing, elevation,dist;
	double eyePos[3], centerPos[3], upVec[3];
	Point prevPoint;
public:
	
	Camera();
	void cameraReset();
	void cameraInit(int x, int y);
	void cameraWork(int flag);
	
	void cameraUpdate(int x, int y);
	void cameraZoomIn();
	void cameraZoomOut();
	
	double getZoom();
};
class Button{
private:
	double positionX;
	double positionY;
	string Text;
	double width;
	double height;
	int isPressed;
	double color[3];
	double mainColor[3];
	double animVeloMultiply;
public:
	Button();
	Button(double x, double y, double width, double height);
	Button(double x, double y, double width, double height,string Text);
	void ButtonReset();
	Screen getPosition();
	Screen getSize();
	bool isHover(Screen* Screen,double x, double y,int norm);
	void Press();
	void drawButton();
	void drawRoundButton(double rad);
	void action(function<void(void)>function);
	void setColor(double* color);
	void setVelo(double multiplyValue);
};
class Light{
public:
	Light();
	static const GLfloat ambient[4];
	static const GLfloat diffuse[4];
	static const GLfloat specular[4];
	static const GLfloat position[4];
	void lightOn();
	void lightOff();
};
//text
void glutDrawText(string input, double size);
void glutDrawBitmapText(string input,int x, int y);

//arc
void glutDrawArc(double radius, double angle, int slice);

//pie
void glutDrawPie(double radius, int slice, double startAngle,double endAngle);

//circle
void glutSolidCircle(double radius, int slice);
void glutWireCircle(double radius, int slice);

//cylinder
void glutSolidCylinder(double topRadius, double height, double bottomRadius, int slice);

//rectangle
void glutWireRectangle(double width,double height);
void glutWireRectangle(double width, double height, int offset);
void glutSolidRectangle(double width, double height);
void glutSolidRectangle(double width, double height, int offset);

//square
void glutWireSquare(double side);
void glutSolidSquare(double side);

//roundRectangle
void glutWireRoundRectangle(double width, double height, double rad);
void glutSolidRoundRectangle(double width, double height, double rad);
void glutWireRoundRectangle(double width, double height,double rad,int offset);
void glutSolidRoundRectangle(double width, double height, double rad, int offset);

//roundSquare
void glutWireRoundSquare(double side, double rad);
void glutSolidRoundSquare(double side, double rad);

//round Rectangle/SquareColumn
void glutSolidRoundRectangleColumn(double topWidth, double topHeight, double bottomWidth, double bottomHeight, double elevation, double rad);
void glutSolidRoundSquareColumn(double topSide, double bottomSide, double elevation, double rad);

//axis
void glutDrawAxisMini();
void glutDrawGrid(double size);
void glutDrawAxis();


void emitter();
void drawScene();