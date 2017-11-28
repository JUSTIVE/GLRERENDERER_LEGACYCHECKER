#pragma warning(disable:4996)
#include<iostream>
#include"BenGL.h"

#include<vector>
#include<Windows.h>
#include<tchar.h>
#include<cassert>
#include<time.h>
#include<ctype.h>
#include<thread>
//#define SIDE_WIDTH 


void loadFileList(const char* folder);
void printFileList();
void drawUI(int UIOPEN);
void displayFunc();
void irine_Idler(int value);
void mouseFunc(int button, int state, int x, int y);
void reshapeFunc(int width, int height);
void createMenu(int value);
void changeShading();
void init();
void lightInit();

enum Quality{ LOW, HIGH };
int quality;

int SIDE_WIDTH=300;
string materialPropTitle[]= {"Emerald","Obsidian","Brass","Chrome","Gold","Red Rubber","Silver","Pearl","Turquoise","Yellow Rubber","BLACK","WHITE"};
string DRAWINGMODE[3] = {"POLYGON","WIREFRAME","POINTS"};
string EnableText[2] = { "TRUE", "FALSE" };
string SHADINGMODETEXT[3] = { "FLAT", "GOURAUD", "PHONG" };

Screen* screen;
FPS_Counter fps_counter;
Mesh* mesh;


#ifndef _USE_CAMERA_DEPRECATED
Camera camera;
#endif
int antialiasEnable = 1;
//IU
Button* UIopener;
Button* drawmodeButton;
Button* propButton;
Button* axisButton;
Button* shadingButton;
Button** Tools;
Button* antiAliasingButton;
int drawaxis=1;

int shadingMode;
//files
vector<string> FILES;
int currentFile=-1;
//lights
Light* light;
//GLfloat AmbientLightValue[] = { 0, 0, 0, 1.0};
//GLfloat DiffuseLightValue[] = { 1.0, 1.0, 1.0,1 };
//GLfloat SpecularLightValue[] = { 1.0, 1.0, 1.0, 1 };
//GLfloat PositionLightValue[] = { 2, 0, 1, 0 };


int UIOPEN=1;

bool dofirst = true;
//load file from directory
void loadFileList(const char* folder){
	char search_path[200];
	wchar_t search_path_t[260];
	LPCWSTR ptr;
	sprintf(search_path, "%s*.obj", folder);
	mbstowcs(search_path_t, search_path, strlen(search_path)+1);
	ptr = search_path_t;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(ptr, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				wstring ws(fd.cFileName);
				string filename(ws.begin(), ws.end());
				FILES.push_back(filename);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}	
}
//show file list
void printFileList(){
	cout << "file amount : "<<FILES.size()<<endl;
	for (int i = 0; i < FILES.size(); i++){
		cout << FILES[i] << endl;
	}
}

void displayFunc(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	drawUI(UIOPEN);
	glPushMatrix();{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,  1);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		if (currentFile == FILES.size()){
			//gluPerspective(60, 1, 3,100);
			//cout << "git" << endl;
			glFrustum(-10, 10, -10, 10, 5, 1000);
			glDisable(GL_LIGHT0);
		}
		else{
			glOrtho(-2, 2, -2, 2, -100, 100);
		}
		glViewport(SIDE_WIDTH, 0, 900, screen->height);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix(); {
			
		//MAIN
			//SIDE OPEN MENU BUTTON
			glPushMatrix(); {
				glColor3f(1, 1, 1);
				glTranslatef(-1.8, (double)(SIDE_WIDTH/300.0)*0.1-(0.05*((double)(300-SIDE_WIDTH)/300.0)), 0);
				for (int i = 0; i < 3; i++){
					glutSolidSphere(0.01, 50, 50);
					glRotatef(120 - (SIDE_WIDTH*120.0 / 300.0), 0, 0, 1);
					glTranslatef(0, -0.1, 0);
				}
				glEnd();
			}
			glPopMatrix();
			//rendering
			Mesh::setProp(mesh->prop);
			
			
			if (currentFile == FILES.size()){ 
				camera.cameraWork(CAMERA_ZOOM);
				//gluPerspective(60, 1, -4, 4);
				drawScene(); 
			}
			else{
				camera.cameraWork(CAMERA_MOVE | CAMERA_ZOOM | CAMERA_ROTATE);
				mesh->render(); 
			}
		}glPopMatrix();
		/*glPushMatrix(); {
			glTranslatef(light->position[0], light->position[1], light->position[2]);
			glutSolidSphere(0.1, 50, 50);
		}glPopMatrix();*/
		
		glPushMatrix(); {
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHTING);
			camera.cameraWork(CAMERA_MOVE | CAMERA_ZOOM | CAMERA_ROTATE);
			if (drawaxis){
				glutDrawGrid(10);
				glutDrawAxis();
			}
			glTranslatef(0.7, -0.7, 0);
		}glPopMatrix();
	}glPopMatrix();
	//drawOBject on lightsource
	//right-below overlay components
	glPushMatrix(); {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.8,-0.8,0);
		glPushMatrix(); {
			GLUT_PAINT_WHITE
			glTranslatef(-0.05, -0.1, -1);
			glutDrawBitmapText(fps_counter.getTitle(),0,0);
			glTranslatef(0, -0.02, 0);
			//zoom text, 100+((log(camera.getZoom()/log(12))*20) ?
			//glutDrawBitmapText("zoom : " + to_string(100 +((log(camera.getZoom()) / log(12)) * 20)), 0, 0);
			glutDrawBitmapText("zoom : " + to_string(100 + ((log(camera.getZoom()) / log(1.2)) * 20)), 0, 0);
			//glutDrawBitmapText("zoom : " + to_string(100 + ((log(camera.getZoom()) / log(1.2)))*20) + "%", 0, 0);
		}
		glPopMatrix();
		camera.cameraWork(CAMERA_MOVE | CAMERA_ROTATE);
		glutDrawAxisMini();
	}glPopMatrix();
	glPushMatrix();{

	}glPopMatrix();
	
	glutSwapBuffers();
}
void drawUI(int UIOPEN){
	if (UIOPEN == 1){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 300, 0, screen->height, -100, 100.0);
		glViewport(0, 0, SIDE_WIDTH, screen->height);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(1, 1, 1);
		glPushMatrix(); {
			drawmodeButton->drawButton();
			axisButton->drawButton();
			propButton->drawButton();
			antiAliasingButton->drawButton();
			shadingButton->drawButton();
		}glPopMatrix();
		for (int i = 0; i < 12; i++){
			Tools[i]->drawRoundButton(10);
		}
		glEnable(GL_LIGHTING);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glEnable(GL_LIGHT0);
		GLfloat lighttemp[3];
		for (int i = 0; i < 12; i++){
			glPushMatrix(); {
				glTranslatef(Tools[i]->getPosition().width + (Tools[i]->getSize().width / 2), Tools[i]->getPosition().height - (Tools[i]->getSize().height / 2), 0);
				Mesh::setProp(i);
				glColor3f(1, 1, 1);
				glutSolidSphere(20, 50, 3);
			}
			glPopMatrix();
		}
		glColor3f(0.9, 0.9, 0.9);
		glPushMatrix(); {
			glTranslatef(0, screen->height, 0);
			glutSolidRectangle(300, screen->height, SQUARE_LEFT_TOP);
		}glPopMatrix();
		//glColor3f(1, 1, 1);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glPushMatrix(); {
			glTranslatef(10, (screen->height) - 30, 0.2);
			glutDrawBitmapText("FILENAME : ", 0,0);
			glTranslatef(0, -30, 0);
			if (currentFile != -1){
				if(currentFile==FILES.size())
					glutDrawBitmapText("BONUS", 0, 0);
				else
					glutDrawBitmapText(FILES[currentFile], 0, 0);
			}
			else{
				glutDrawBitmapText("NULL", 0, 0);
			}
			glTranslatef(0, -60, 0);
			glutDrawBitmapText("DRAWMODE : ", 0, 0);
			glTranslatef(0, -30, 0);
			if (currentFile != -1){
				glutDrawBitmapText(DRAWINGMODE[mesh->getDrawMode()], 0, 0);
			}
			else{
				glutDrawBitmapText("NULL", 0, 0);
			}

			glTranslatef(0, -30, 0);
			glutDrawBitmapText("Material Property : ", 0, 0);
			glTranslatef(0, -30, 0);
			glutDrawBitmapText(materialPropTitle[mesh->prop], 0, 0);

			glTranslatef(0, -30, 0);
			glutDrawBitmapText("DrawAxis : ", 0, 0);
			glTranslatef(0, -30, 0);
			glutDrawBitmapText(EnableText[1-drawaxis], 0, 0);

			glTranslatef(0, -30, 0);
			glutDrawBitmapText("Antialiasing : ", 0, 0);
			glTranslatef(0, -30, 0);
			glutDrawBitmapText(EnableText[1 - antialiasEnable], 0, 0);

			glTranslatef(0, -30, 0);
			glutDrawBitmapText("Shading : ", 0, 0);
			glTranslatef(0, -30, 0);
			glutDrawBitmapText(SHADINGMODETEXT[shadingMode], 0, 0);
		}
		glPopMatrix();
	}
}
void createMenu(int value){
	currentFile = value - 1;
	if (value<FILES.size()+1){
		//glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		string temp = FILES[value - 1];
		mesh->loadFile(temp); 
		//mesh->ForBetterWorld(temp);
		drawaxis = 1;
	}
	else{
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		//drawaxis = 0;
	}
	camera.cameraReset();
	camera.cameraInit(0, 0);
	camera.cameraUpdate(0, 0);
	//camera.cameraReset();
//	mesh->ForBetterWorld(temp);
}

void irine_Idler(int value){
	//thread t(&drawUI, UIOPEN);
	//t.join();
	int callNext = 1;
	if (UIOPEN==-1){
		if (quality == HIGH){
			
			if (SIDE_WIDTH < 300)
				SIDE_WIDTH+=5;
			if (SIDE_WIDTH == 300){
				callNext = 0;
				UIOPEN = 1;
			}
			
		}
		else{
			SIDE_WIDTH = 300;
			callNext = 0;
			UIOPEN = 1;
		}
		glutReshapeWindow(900 + SIDE_WIDTH, 900);
	}
	if (UIOPEN == -2){
		if (quality == HIGH){
			if (SIDE_WIDTH > 0)
				SIDE_WIDTH -= 5;
			if (SIDE_WIDTH == 0){
				callNext = 0;
				UIOPEN = 0;
			}
		}
		else{
			SIDE_WIDTH = 0;
			callNext = 0;
			UIOPEN = 0;
		}
		glutReshapeWindow(900 + SIDE_WIDTH, 900);
	}
	fps_counter.update();
	if (callNext) {}
	glutTimerFunc(10, irine_Idler, 0);
		
	glutPostRedisplay();
}
double selectedColor[] = {0.2,0.7,0.9};
double normalColor[] = { 0.42,0.42,0.42 };
void mouseFunc(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (x > 300)
			camera.cameraInit(x, y);
		
		switch (state)
		{
		case GLUT_UP:
			if (UIopener->isHover(screen, ((double)((x - (SIDE_WIDTH*UIOPEN)) - 450) * 4 / 900.0), ((double)(450 - y) * 4 / 900.0), 1) == true)
			{
				glutTimerFunc(10, irine_Idler, 0);
				if (UIOPEN == 0)
					UIOPEN = -1;
				else if (UIOPEN == 1)
					UIOPEN = -2;
			}
			if (UIOPEN == 1){
				if (currentFile != -1 && (drawmodeButton->isHover(screen, x, y, 0) == true)){
					
					mesh->modeChange();
				}
				if (currentFile != -1 && (propButton->isHover(screen, x, y, 0) == true)){
					Tools[mesh->prop]->setColor(normalColor);
					mesh->changeProp();
					Tools[mesh->prop]->setColor(selectedColor);

				}
				if (axisButton->isHover(screen, x, y, 0) == true){
					drawaxis = (drawaxis + 1) & 1;
				}
				if (shadingButton ->isHover(screen, x, y, 0) == true){
					changeShading();
				}
				if (antiAliasingButton->isHover(screen, x, y, 0) == true){
					antialiasEnable = (antialiasEnable + 1) & 1;
					if (antialiasEnable){
						glEnable(GL_LINE_SMOOTH);
						glLineWidth(2);
					}
					else {
						glDisable(GL_LINE_SMOOTH);
						glLineWidth(1);
					}
				}
				for (int i = 0; i < 12; i++){
					if ((Tools[i]->isHover(screen, x, y, 0) == true)){
						Tools[mesh->prop]->setColor(normalColor);
						mesh->prop = i;
						Tools[i]->setColor(selectedColor);
						
					}
				}
			}
			break;
		case GLUT_DOWN:
			fps_counter.update();
			if (UIOPEN == 1){
				if (drawmodeButton->isHover(screen, x, y,0)){
					drawmodeButton->Press();
				}
				if (propButton->isHover(screen, x, y,0)){
					propButton->Press();
				}
				if (axisButton->isHover(screen, x, y,0)){
					axisButton->Press();
				}
				if (antiAliasingButton->isHover(screen, x, y, 0)){
					antiAliasingButton->Press();
				}

				if (shadingButton->isHover(screen,x,y,0)){
					shadingButton->Press();
				}
				if (UIopener->isHover(screen, ((double)((x - (SIDE_WIDTH*UIOPEN)) - 450) * 4 / 900.0), ((double)(450 - y) * 4 / 900.0), 1))
					UIopener->Press();
				for (int i = 0; i < 12; i++){
					if (Tools[i]->isHover(screen, x, y,0))
						Tools[i]->Press();
				}
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void motionFunc(int x,int y){
	
	if (x>SIDE_WIDTH){
		//if (currentFile != FILES.size())
			camera.cameraUpdate(x,y);
	}
	glutPostRedisplay();
}

void reshapeFunc(int width, int height) {

	screen = new Screen();
	glutReshapeWindow(900+SIDE_WIDTH,900);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glutPostRedisplay();
}

void init(){
	
	screen = new Screen();
	mesh = new Mesh();
	loadFileList(MESH_DIR);

	//buttons initiating
	drawmodeButton = new Button(0,(screen->height-100),SIDE_WIDTH,60);
	propButton = new Button(0, (screen->height - 160), SIDE_WIDTH, 60);
	axisButton = new Button(0, (screen->height - 220), SIDE_WIDTH, 60);
	antiAliasingButton = new Button(0, (screen->height - 280), SIDE_WIDTH, 60);
	shadingButton = new Button(0, (screen->height - 340), SIDE_WIDTH, 60);

	UIopener = new Button(-1.9, 0.1, 0.2, 0.2);


	double ToolsColor[4] = { 0.42, 0.42, 0.42 };

	Tools = new Button*[12];
	for (int i = 0; i < 12; i++){
		Tools[i] = new Button(0 + ((SIDE_WIDTH / 4) * ((i % 4))) + (10), (screen->height - 680) - ((SIDE_WIDTH / 4)* (i / 4)), (SIDE_WIDTH / 4) - 20, (SIDE_WIDTH / 4) - 20);
		Tools[i]->setColor(ToolsColor);
		Tools[i]->setVelo(3);
	}
	Tools[0]->setColor(selectedColor);
	quality = HIGH;
	//light = new Light();
}
void glSetup(){
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	
	//glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_PHONG_WIN);
	glHint(GL_PHONG_HINT_WIN, GL_NICEST);
	
	//AA
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	
	
	

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_COLOR_MATERIAL_FACE);

	
	
	camera.cameraUpdate(0, 0);
	glClearColor(0.0, 0.0, 0.0, 0);
}
void changeShading(){
	shadingMode = (shadingMode + 1)%3;
	if (shadingMode == 0){
		glShadeModel(GL_FLAT);
	}
	else if (shadingMode == 1){
		glShadeModel(GL_SMOOTH);
	}
	else{
		glShadeModel(GL_PHONG_WIN);
		glHint(GL_PHONG_HINT_WIN, GL_NICEST);
	}
}

void keyFunc(unsigned char key, int x, int y){
	if (isupper(key))
		key = tolower(key);
	switch (key){
	case '+':
		camera.cameraZoomIn();
		/*for (int i = 0; i < 3; i++)
			PositionLightValue[i] *= 1.44;*/
			
		break;
	case '-':
		camera.cameraZoomOut();
		/*for (int i = 0; i < 3; i++)
			PositionLightValue[i] /= 1.44;*/
		
		break;
	case 'i':
#ifdef _USE_CAMERA_DEPRECATED
		camera.cameraInitiate();
#endif
		camera.cameraReset();
		camera.cameraInit(0, 0);
		camera.cameraUpdate(0, 0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

const GLfloat ambient[] = { 0, 0, 0, 1 };
const GLfloat diffuse[] = { 1, 1, 1, 1 };
const GLfloat specular[] = { 1, 1, 1, 1 };
const GLfloat position[] = { 2, 0, 1, 0 };

const GLfloat ambient1[] = { 0, 0, 0, 1 };
const GLfloat diffuse1[] = { 1, 1, 0.9, 0.5 };
const GLfloat specular1[] = { 1, 1, 1, 0.3 };
const GLfloat position1[] = { -1, 1.5, 0, 0.5 };
const GLfloat direction[] = { 1, -2, 0, 1};

const GLfloat ambient2[] = { 0, 0, 0, 1 };
const GLfloat diffuse2[] = { 1, 0, 0, 0.3 };
const GLfloat specular2[] = { 1, 0, 0, 0.3 };
const GLfloat position2[] = { 0.2, 0, 0, 0.5};

const GLfloat ambient3[] = { 0, 0, 0, 1 };
const GLfloat diffuse3[] = { 0, 1, 0, 0.3 };
const GLfloat specular3[] = { 0, 1, 0, 0.3 };
const GLfloat position3[] = { -0.2, 0, 0, 0.5 };
void lightInit(){
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_POSITION, position); //조명의 위치(광원)를 설정한다.

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT1, GL_POSITION, position1); //조명의 위치(광원)를 설정한다.
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular2); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT2, GL_POSITION, position2); //조명의 위치(광원)를 설정한다.

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular3); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT3, GL_POSITION, position3); //조명의 위치(광원)를 설정한다.

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glDisable(GL_COLOR_MATERIAL);
}
void passiveFunc(int x, int y){
	drawmodeButton->isHover(screen,x, y,0);
	propButton->isHover(screen, x, y,0);
	axisButton->isHover(screen, x, y,0);
	antiAliasingButton->isHover(screen, x, y, 0);
	shadingButton->isHover(screen, x, y, 0);
	UIopener->isHover(screen, ((double)((x - (SIDE_WIDTH*UIOPEN))-450) *4 / 900.0), ((double)(450 - y) * 4 / 900.0),1);
	
	for (int i = 0; i < 12; i++){
		Tools[i]->isHover(screen, x, y,0);

	}
}
void idler(){
	fps_counter.update();
	glutPostRedisplay();
}

void main(int argc,char* argv[]) {
	glutInit(&argc, argv);
	init();
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE|GLUT_DEPTH|GLUT_ALPHA);
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glutInitWindowPosition(0,0);
	glutInitWindowSize(screen->width, screen->height);
	glutCreateWindow("OpenGL OBJ LOADER");
	
	glSetup();
	//light->lightOn();
	lightInit();
	
	//fileload
	glutCreateMenu(createMenu);
	for (int i = 0; i < FILES.size(); i++)
		glutAddMenuEntry(FILES[i].c_str(),i+1);
	glutAddMenuEntry("Scene0", FILES.size() + 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	//glutIdleFunc(idler);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutKeyboardFunc(keyFunc);
	glutPassiveMotionFunc(passiveFunc);
	
	if (dofirst){
		SIDE_WIDTH = 0;
		UIOPEN = 0;
		dofirst = false;
		glutReshapeWindow(900 + SIDE_WIDTH, 900);
	}
	glutMainLoop();
}
