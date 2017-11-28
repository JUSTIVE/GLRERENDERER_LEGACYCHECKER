#include"BenGL.h"
///////////////////////////////////////////////////////////////////////////////////
//Point

//Point 클래스의 기본 생성자. 인자를 초기화함
//사용위치 : init
Point::Point(){ this->x = this->y = 0; }

///////////////////////////////////////////////////////////////////////////////////
//Screen

//Screen 클래스의 기본 생성자. 인자를 초기화함
//사용위치: init
Screen::Screen(){
	this->height = 900;
	this->width = 1200;
}
Screen::Screen(int width, int height){
	this->width= width;
	this->height= height;
}
///////////////////////////////////////////////////////////////////////////////////
//FPS_Counter

//FPS_Counter 클래스의 기본 생성자. 인자를 초기화하고 초기 시간을 측정함.
//사용위치: init
FPS_Counter::FPS_Counter(){
	timeindex = 0;
	timeFlip[timeindex] = clock();
}

//시간을 업데이트하여 title을 갱신까지 함
//사용위치: idle
void FPS_Counter::update(){
	timeFlip[timeindex] = clock();
	title = to_string((ROUND((CLOCKS_PER_SEC / ((double)(timeFlip[timeindex] - timeFlip[(timeindex + 1) & 1]))), 2)>60) ? 60.00 : ROUND((CLOCKS_PER_SEC / ((double)(timeFlip[timeindex] - timeFlip[(timeindex + 1) & 1]))), 2));
	timeindex = (timeindex+1) & 1;
}
//최종 string 형태의 title을 반환함
//사용위치: idle
string FPS_Counter::getTitle(){ 
	return "frame: "+title; 
}
///////////////////////////////////////////////////////////////////////////////////
//Flag
//Flag 클래스의 기본 생성자. 인자를 초기화함
//사용위치: init
Flag::Flag(){
	isOrtho = 0;
	isMove = 0;
	isRotate = 0;
	isRight = 0;
}

///////////////////////////////////////////////////////////////////////////////////
//Camera_Deprecated
//공간을 회전하는 방식의 카메라 이동. 아래의 gluLookat 함수를 이용하는 카메라 클래스가 이를 대체
#ifdef _USE_CAMERA_DEPRECATED
CameraDeprecated::CameraDeprecated(){
	this->mouse[0] = 0;
	this->mouse[1] = 0;
	this->zoom = 1.0;
	this->move[0] = 0;
	this->move[1] = 0;
}
void CameraDeprecated::cameraWork(int mode){
	if (mode==ZOOM||mode==MOVE_AND_ZOOM||mode==ROTATE_AND_ZOOM||mode==CAMERA_ALL)
		glScalef(this->zoom, this->zoom, this->zoom);
	if (mode == MOVE || mode == MOVE_AND_ZOOM || mode == MOVE_AND_ROTATE || mode == CAMERA_ALL)
		glTranslatef(this->move[0], this->move[1], 0);
	if (mode == ROTATE || mode == MOVE_AND_ROTATE || mode == ROTATE_AND_ZOOM || mode == CAMERA_ALL){
		glRotatef(this->rotation[0], 0, 1, 0);
		glRotatef(this->rotation[1], 1, 0, 0);
	}
}
void CameraDeprecated::cameraMoveInit(Screen* screen, int x, int y){
	this->move[0] = (x / (screen->width / 2.0)) - 1;
	this->move[1] = ((screen->height - y) / (screen->height / 2.0)) - 1;
}

void CameraDeprecated::cameraRotateInit(int x, int y){
	this->mouse[0] = x;
	this->mouse[1] = y;
	this->prevMouse[0] = this->mouse[0];
	this->prevMouse[1] = this->mouse[1];
}

void CameraDeprecated::cameraMoveAction(Screen* screen,int x, int y){
	this->move[0] = (x / (screen->width / 2.0)) - 1;
	this->move[1] = ((screen->height - y) / (screen->height / 2.0)) - 1;
}

void CameraDeprecated::cameraRotateAction(int x, int y){
	this->mouse[0] = x;
	this->mouse[1] = y;
	this->rotation[0] -= (GLdouble)(this->mouse[0] - this->prevMouse[0]);
	this->rotation[1] += (GLdouble)(this->mouse[1] - this->prevMouse[1]);
	this->prevMouse[0] = this->mouse[0];
	this->prevMouse[1] = this->mouse[1];
}

void CameraDeprecated::cameraZoomIn(){
	this->zoom*=1.2;
}
void CameraDeprecated::cameraZoomOut(){
	this->zoom/=1.2;
}

void CameraDeprecated::cameraInitiate(){
	this->mouse[0] = 0;
	this->mouse[1] = 0;
	this->zoom = 1.0;
	this->rotation[0] = 0;
	this->rotation[1] = 0;
	this->move[0] = 0;
	this->move[1] = 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////////
//Camera
//Camera 클래스의 기본 생성자.
//사용위치: init
Camera::Camera(){
	this->cameraReset();
}
//vc++6.0에서 타 생성자 호출 문제로 인하여 초기화 함수를 별도로 작성
void Camera::cameraReset(){
	//local variables
	this->zoom = 1;
	this->dist = 0.2;
	this->swing = 0;
	this->elevation = 0;
	//array variables
	for (int i = 0; i < 3; i++){
		this->origUpVec[i] = 0;
		this->eyePos[i] = 0;
		this->upVec[i] = 0;
		this->origEyePos[i] = 0;
	}
	//업벡터와 거리 초기화
	this->origEyePos[2] = dist;
	this->origUpVec[1] = 1;
	//이전 좌표 초기화
	this->prevPoint.x = 0;
	this->prevPoint.y = 0;
}
//클릭되었을때 카메라값을 저장
//사용위치: mouseFunc
void Camera::cameraInit(int x, int y){
	this->prevPoint.x = x;
	this->prevPoint.x = y;
}
//카메라의 위치와 업벡터 값을 행렬의 회전변환으로 계산
//사용위치: motionFunc
void Camera::cameraUpdate(int x, int y){
	this->swing += (double)( prevPoint.x-x);
	this->elevation += (double)(prevPoint.y-y);
	double temp[3] = { 0 };
	double rot_elev[3][3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };//elev 만큼 x축으로 회전한 행렬
	double rot_swing[3][3] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };//swing 만큼 -y축으로 회전한 행렬
	rot_elev[1][1] = cos(RADIAN(elevation));    rot_elev[1][2] = -sin(RADIAN(elevation));
	rot_elev[2][1] = sin(RADIAN(elevation));    rot_elev[2][2] = cos(RADIAN(elevation));
	rot_swing[0][0] = cos(RADIAN(swing));    rot_swing[0][2] = sin(RADIAN(swing));
	rot_swing[2][0] = -sin(RADIAN(swing));   rot_swing[2][2] = cos(RADIAN(swing));
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++)
			temp[i] += rot_elev[i][j] * origEyePos[j];
	}
	for (int i = 0; i < 3; i++){
		eyePos[i] = 0;
		for (int j = 0; j < 3; j++)
			eyePos[i] += rot_swing[i][j] * temp[j];
	}
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++)
			temp[i] += rot_elev[i][j] * origUpVec[j];
	}
	
	for (int i = 0; i < 3; i++){
		upVec[i] = 0;
		for (int j = 0; j < 3; j++)
			upVec[i] += rot_swing[i][j] * temp[j];
	}

	this->prevPoint.x = x;
	this->prevPoint.y = y;
}
//실제 카메라가 작동하는 부분을 작성
//사용위치: displayFunc
void Camera::cameraWork(int flag){
	
	if ((flag & 4) == CAMERA_ZOOM)
		glScalef(zoom, zoom, zoom);
	gluLookAt(eyePos[0], eyePos[1], eyePos[2],
		centerPos[0], centerPos[1], centerPos[2],
		upVec[0],upVec[1],upVec[2]);
}
//카메라의 줌인을 담당
//사용위치: keyboardFunc
void Camera::cameraZoomIn(){
	this->zoom *= 1.2;
	
}
//카메라의 줌아웃을 담당
//사용위치: keyboardFunc
void Camera::cameraZoomOut(){
	this->zoom /= 1.2;
}

double Camera::getZoom(){ return this->zoom; }
///////////////////////////////////////////////////////////////////////////////////
//Button
//Button 클래스의 기본 생성자.
//사용위치: init
Button::Button(){
	this->ButtonReset();
}
//Button 클래스의 생성자. 위치와 크기를 입력받음
//사용위치: init
Button::Button(double x, double y, double width, double height){
	this->ButtonReset();
	this->positionX = x;
	this->positionY = y;
	this->Text = "";
	this->width = width;
	this->height = height;
	this->isPressed = 0;
	for (int i = 0; i < 3; i++){
		this->color[i] = 0.42;
		mainColor[i] = 0.42;
	}
}
//Button 클래스의 생성자. 위치와 크기와 작성될 텍스트를 입력받음
//사용위치: init
Button::Button(double x, double y, double width, double height, string Text){
	this->ButtonReset();
	positionX = x;
	positionY = y;
	this->Text = "";
	this->width = width;
	this->height = height;
	isPressed = 0;
	for (int i = 0; i < 3; i++){
		color[i] = 0.42;
		mainColor[i] = 0.42;
	}
}
//vc++6.0에서 타 생성자 호출 문제로 인하여 초기화 함수를 별도로 작성
//사용위치: init
void Button::ButtonReset(){
	positionX = 0;
	positionY = 0;
	Text = "";
	width = 1;
	height = 1;
	isPressed = 0;
	for (int i = 0; i < 3; i++){
		this->color[i] = 0.42;
		mainColor[i] = 0.42;
	}
	animVeloMultiply = 0.5;
}

//마우스가 눌린 상태를 적용
//사용위치: mouseFunc
void Button::Press(){
	this->isPressed = 2;
}
//마우스의 좌표가 버튼 위에 있는지 확인하는 함수. 정규화되지 않은 위치의 버튼은 스크린의 크기를 참조하여 확인
//사용위치: mouseFunc, passiveMotionFunc
bool Button::isHover(Screen* screen,double x, double y,int norm){
	//정규화되지 않은 좌표
	if (!norm){
		if ((x > positionX&&x<positionX + width) && (((screen->height - y)<positionY) && ((screen->height - y)>positionY - height))){
			isPressed = 1;
			return true;
		}
		isPressed = 0;
	}
	//정규화된 좌표
	else{
		if ((x > positionX&&x<positionX + width) && (((y)<positionY) && ((y)>positionY - height))){
			isPressed = 1;
			return true;
		}
		isPressed = 0;
	}
	return false;	
}
//사각형 버튼을 그리는 함수
//사용위치: displayFunc
void Button::drawButton(){
	//눌린 상태의 버튼
	if (isPressed == 2)
		for (int i = 0; i < 3; i++)
			this->color[i] = this->color[i] - 0.015*animVeloMultiply < (this->mainColor[i] - 0.1) ? (this->mainColor[i] - 0.1) : this->color[i] - 0.015*animVeloMultiply;
	//마우스가 올라간 상태의 버튼
	else if (isPressed == 1)
		for (int i = 0; i < 3; i++)
			this->color[i] = this->color[i] + 0.015*animVeloMultiply >(this->mainColor[i] + 0.1) ? (this->mainColor[i] + 0.1) : this->color[i] + 0.015*animVeloMultiply;
	//위의 두 상태가 아닌 경우
	else
		for (int i = 0; i < 3; i++)
			this->color[i] = this->color[i] + 0.005*animVeloMultiply >= (this->mainColor[i] + 0.005*animVeloMultiply) ? this->color[i] - 0.005*animVeloMultiply : this->color[i] + 0.005*animVeloMultiply;
	//그리는 부분
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix(); {
		glTranslatef(positionX, positionY, 0);
		glutSolidRectangle(width, height, SQUARE_LEFT_TOP);
		glColor3f(0, 0, 0);
		glutDrawText(Text, 1);
	}
	glPopMatrix();
}
//모서리가 둥근 사각형 버튼을 그리는 함수
//사용위치: displayFunc
void Button::drawRoundButton(double rad){
	if (isPressed == 2){
		for (int i = 0; i < 3; i++){
			this->color[i] = this->color[i] - 0.015*animVeloMultiply < (this->mainColor[i] - 0.1) ? (this->mainColor[i] - 0.1) : this->color[i] - 0.015*animVeloMultiply;
		}
	}
	else if (isPressed == 1){
		for (int i = 0; i < 3; i++){
			this->color[i] = this->color[i] + 0.015*animVeloMultiply >(this->mainColor[i] + 0.1) ? (this->mainColor[i] + 0.1) : this->color[i] + 0.015*animVeloMultiply;
		}
	}

	else{
		for (int i = 0; i < 3; i++){
			this->color[i] = this->color[i] + 0.005*animVeloMultiply >= (this->mainColor[i] + 0.005*animVeloMultiply) ? this->color[i] - 0.005*animVeloMultiply : this->color[i] + 0.005*animVeloMultiply;
		}
	}
	glColor3f(color[0], color[1], color[2]);
	glPushMatrix(); {
		glTranslatef(positionX, positionY, 0);
		glutSolidRoundRectangle(width, height,rad, SQUARE_LEFT_TOP);
		glColor3f(0, 0, 0);
		glutDrawText(Text, 1);
	}
	glPopMatrix();
}
//버튼을 누를 때의 역할을 담당하는 함수. 초기화할때 지정하려 했으나 여러 클래스의 메소드를 받는 데에 한계가 있어 수정이 필요
//TODO:: 
void Button::action(function<void(void)>function){
	function();
}
//버튼의 idle 상태의 색을 바꾸는 함수
void Button::setColor(double* color){
	for (int i = 0; i < 3; i++)
		this->mainColor[i] = color[i];
}
//버튼의 좌표를 가져오는 함수
Screen Button::getPosition(){
	Screen screen(this->positionX, this->positionY);
	return screen;
}
//버튼의 크기를 가져오는 함수
Screen Button::getSize(){
	Screen screen(this->width, this->height);
	return screen;
}
//버튼의 애니메이션 속도 배율을 지정하는 함수. 빠른 배수로 지정할 경우 버튼의 색이 튀는 경우가 발생한다.
void Button::setVelo(double multiplyValue){
	this->animVeloMultiply = multiplyValue;
}
///////////////////////////////////////////////////////////////////////////////////
//Light
//Light 클래스의 기본 생성자.
//사용위치: init
const GLfloat Light::ambient[] = {0,1,0,1};
const GLfloat Light::diffuse[] = { 1, 0, 0, 1 };
const GLfloat Light::specular[] = { 1, 1, 1, 1 };
const GLfloat Light::position[] = { 2, 1, 3, 1};

Light::Light(){

	GLfloat localView[] = { 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient); //Ambient 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular); //Diffuse 조명의 성질을 설정한다.
	glLightfv(GL_LIGHT0, GL_POSITION, position); //조명의 위치(광원)를 설정한다.

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);
}

void Light::lightOn(){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glDisable(GL_COLOR_MATERIAL);
}

void Light::lightOff(){
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//class over 클래스 선언 종료
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


//화면에 glBegin(GL_LINES)를 이용한 텍스트를 그리는 함수. Antialiasing 의 효과를 적용받는다.
//작성할 텍스트/텍스트의 크기
void glutDrawText(string text, double size){
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);
	glPushMatrix(); {
		glScalef(size / 300.0, size / 300.0, size / 300.0);
		for (int i = 0; i < text.length(); i++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		}
	}
	glPopMatrix();
	//glDisable(GL_LINE_SMOOTH);
	//glBlendFunc(NULL, NULL);
}
//화면에 비트맵 텍스트를 그리는 함수. 크기를 지정할 수 없다.
//작성할 텍스트/텍스트의 좌표
void glutDrawBitmapText(string input, int x, int y){
	glRasterPos2d(x, y);
	for (int i = 0; i<input.length(); i++){
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,input.c_str()[i]);
	}
}
//화면에 호를 그리는 함수
//호의 반지름/호의 각/정밀도
void glutDrawArc(double radius, double angle, int slice){
	if (angle>360.0)
		glutWireCircle(radius,slice);
	else{
		slice = (slice > 50) ? 50 : slice;
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= slice; i++)
			glVertex3f(0 * cos((double)((angle / slice)*i / (180.0 / M_PI))) - radius*sin((double)((angle / slice)*i / (180.0 / M_PI))),
			0 * sin((double)((angle / slice)*i / (180.0 / M_PI))) + radius*cos((double)((angle/ slice)*i / (180.0 / M_PI))),
			0);
		glEnd();
	}
}

//pie
void glutDrawPie(double radius, int slice, double startAngle,double endAngle){
	if (startAngle == endAngle)
		return;
	
	if (endAngle-startAngle>360.0){
		glutSolidCircle(radius, slice);
	}
	else{
		//slice = (slice > 50) ? 50 : slice;
		glRotatef(startAngle, 0, 0, 1);
		glBegin(GL_TRIANGLE_FAN); {
			glVertex3f(0, 0, 0);
			for (int i = 0; i <= slice; i++)
				glVertex3f(0 * cos((double)(((endAngle - startAngle) / slice)*i / (180.0 / M_PI))) - radius*sin((double)(((endAngle - startAngle) / slice)*i / (180.0 / M_PI))),
				0 * sin((double)(((endAngle - startAngle) / slice)*i / (180.0 / M_PI))) + radius*cos((double)(((endAngle - startAngle) / slice)*i / (180.0 / M_PI))),
				0);
		}glEnd();
	}
	//glRotatef(angle[j], 0, 0, 1);
	
}

//circle
void glutSolidCircle(double radius, int slice){
	int i;
	//slice = (slice>50) ? 50 : slice;
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		for (i = 0; i <= (slice * 2)-1; i++)
			glVertex3f(0 * cos((double)((360.0 / slice)*i / (180.0 / M_PI))) - radius*sin((double)((360.0 / slice)*i / (180.0 / M_PI))),
				0 * sin((double)((360.0 / slice)*i / (180.0 / M_PI))) + radius*cos((double)((360.0 / slice)*i / (180.0 / M_PI))),
				0);
	glEnd();
}
void glutWireCircle(double radius, int slice){
	int i;
	slice = (slice>50) ? 50 : slice;
	glBegin(GL_LINE_LOOP); {
		for (i = 0; i <= slice; i++)
			glVertex3f(0 * cos((double)((360.0 / slice)*i / (180.0 / M_PI))) - radius*sin((double)((360.0 / slice)*i / (180.0 / M_PI))),
				0 * sin((double)((360.0 / slice)*i / (180.0 / M_PI))) + radius*cos((double)((360.0 / slice)*i / (180.0 / M_PI))),
				0);
	}glEnd();
}
//cylinder
void glutSolidCylinder(double topRadius, double height, double bottomRadius, int slice){
	int i, j, step = 0;
	glPushMatrix(); {
		glutSolidCircle(topRadius, slice);
		glPointSize(2);
		glBegin(GL_QUAD_STRIP); {
			for (i = 0; i <= ((slice)* 2); i++){
				for (j = 0; j < 2; j++){
					step = (step + 1) & 1;
					if (step)
						glVertex3f(0 * cos((double)((360.0 / slice)*i / (180.0 / M_PI))) - topRadius*sin((double)((360.0 / slice)*i / (180.0 / M_PI))),
							0 * sin((double)((360.0 / slice)*i / (180.0 / M_PI))) + topRadius*cos((double)((360.0 / slice)*i / (180.0 / M_PI))),
							0);
					else
						glVertex3f(0 * cos((double)((360.0 / slice)*i / (180.0 / M_PI))) - bottomRadius*sin((double)((360.0 / slice)*i / (180.0 / M_PI))),
							0 * sin((double)((360.0 / slice)*i / (180.0 / M_PI))) + bottomRadius*cos((double)((360.0 / slice)*i / (180.0 / M_PI))),
							height);
					i--;
				}
				i += 2;
			}
		}glEnd();
		glTranslatef(0, 0, height);
		glutSolidCircle(bottomRadius, slice);
	}
	glPopMatrix();
}


//rectangle
void glutWireRectangle(double width, double height){
	glBegin(GL_LINE_LOOP); {
		glVertex3f(-(width/2),-(height/2),0);
		glVertex3f((width / 2), -(height / 2), 0);
		glVertex3f((width / 2), (height / 2), 0);
		glVertex3f(-(width / 2), (height / 2), 0);
	}
	glEnd();
}
void glutWireRectangle(double width, double height,int offset){
	if (offset == SQUARE_CENTER){
		glBegin(GL_LINE_LOOP); {
			glVertex3f(-(width / 2), -(height / 2), 0);
			glVertex3f((width / 2), -(height / 2), 0);
			glVertex3f((width / 2), (height / 2), 0);
			glVertex3f(-(width / 2), (height / 2), 0);
		}
		glEnd();
	}
	else if (offset == SQUARE_LEFT_TOP){
		glBegin(GL_QUADS); {
			glVertex3f(0, -height, 0);
			glVertex3f(width, -height, 0);
			glVertex3f(width, 0, 0);
			glVertex3f(0, 0, 0);
		}
		glEnd();
	}
}
void glutSolidRectangle(double width, double height){
	glBegin(GL_QUADS); {
		glNormal3f(0, 0, 1);
		glVertex3f(-(width / 2), -(height / 2), 0);
		glNormal3f(0, 0, 1);
		glVertex3f((width / 2), -(height / 2), 0);
		glNormal3f(0, 0, 1);
		glVertex3f((width / 2), (height / 2), 0);
		glNormal3f(0, 0, 1);
		glVertex3f(-(width / 2), (height / 2), 0);
	}
	glEnd();
}
void glutSolidRectangle(double width, double height,int offset){
	if (offset == SQUARE_CENTER){
		glBegin(GL_QUADS); {
			glVertex3f(-(width / 2), -(height / 2), 0);
			glVertex3f((width / 2), -(height / 2), 0);
			glVertex3f((width / 2), (height / 2), 0);
			glVertex3f(-(width / 2), (height / 2), 0);
		}
		glEnd();
	}
	else if(offset==SQUARE_LEFT_TOP){
		glBegin(GL_QUADS); {
			glVertex3f(0, -height, 0);
			glVertex3f(width, -height, 0);
			glVertex3f(width, 0, 0);
			glVertex3f(0, 0, 0);
		}
		glEnd();
	}
}
//square
void glutWireSquare(double side){
	glBegin(GL_LINE_LOOP); {
		glVertex3f(-(side / 2), -(side/ 2), 0);
		glVertex3f((side/ 2), -(side / 2), 0);
		glVertex3f((side / 2), (side / 2), 0);
		glVertex3f(-(side / 2), (side/ 2), 0);
	}
	glEnd();
}
void glutSolidSquare(double side){
	glBegin(GL_QUADS); {
		glVertex3f(-(side / 2), -(side / 2), 0);
		glVertex3f((side / 2), -(side / 2), 0);
		glVertex3f((side / 2), (side / 2), 0);
		glVertex3f(-(side / 2), (side / 2), 0);
	}
	glEnd();
}

//roundSquare
void glutWireRoundRectangle(double width, double height, double rad){
	glBegin(GL_LINES); {
		glVertex3f(-((width / 2) - rad), -(height / 2), 0);
		glVertex3f((width / 2) - rad, -(height / 2), 0);

		glVertex3f((width / 2), -((height / 2) - rad), 0);
		glVertex3f((width / 2), ((height / 2) - rad), 0);

		glVertex3f((width / 2) - rad, (height / 2), 0);
		glVertex3f(-((width / 2) - rad), (height / 2), 0);

		glVertex3f(-(width / 2), ((height / 2) - rad), 0);
		glVertex3f(-(width / 2), -((height / 2) - rad), 0);
	}
	glEnd();
	glPushMatrix(); {
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		glRotatef(90, 0, 0, 1);
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		glRotatef(90, 0, 0, 1);
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
		glRotatef(90, 0, 0, 1);
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
			glScalef(width / 4, height / 4, 1);
			glutDrawArc(1, 90, 50);
		}glPopMatrix();
	}glPopMatrix();
}
void glutWireRoundRectangle(double width, double height, double rad, int offset){
	if (offset == SQUARE_CENTER){
		glBegin(GL_LINES); {
			glVertex3f(-((width / 2) - rad), -(height / 2), 0);
			glVertex3f((width / 2) - rad, -(height / 2), 0);

			glVertex3f((width / 2), -((height / 2) - rad), 0);
			glVertex3f((width / 2), ((height / 2) - rad), 0);

			glVertex3f((width / 2) - rad, (height / 2), 0);
			glVertex3f(-((width / 2) - rad), (height / 2), 0);

			glVertex3f(-(width / 2), ((height / 2) - rad), 0);
			glVertex3f(-(width / 2), -((height / 2) - rad), 0);
		}
		glEnd();
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
		}glPopMatrix();
	}
	//TODO:: REMAP POSITION
	else if (offset == SQUARE_LEFT_TOP){
		glBegin(GL_LINES); {
			glVertex3f(-((width / 2) - rad), -(height / 2), 0);
			glVertex3f((width / 2) - rad, -(height / 2), 0);

			glVertex3f((width / 2), -((height / 2) - rad), 0);
			glVertex3f((width / 2), ((height / 2) - rad), 0);

			glVertex3f((width / 2) - rad, (height / 2), 0);
			glVertex3f(-((width / 2) - rad), (height / 2), 0);

			glVertex3f(-(width / 2), ((height / 2) - rad), 0);
			glVertex3f(-(width / 2), -((height / 2) - rad), 0);
		}
		glEnd();
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((width / 2) - rad), (height / 2) - rad, 0);
				glScalef(width / 4, height / 4, 1);
				glutDrawArc(1, 90, 50);
			}glPopMatrix();
		}glPopMatrix();
	}
}

void glutSolidRoundRectangle(double width, double height, double rad){
	rad = (rad > (width / 2)) ? width / 2 : rad;
	rad = (rad > (height / 2)) ? width / 2 : rad;
	glPushMatrix(); {
		glTranslatef(-((width / 2) - rad), -((height / 2) - rad), 0);
		glutSolidCircle(rad, 50);
		glTranslatef(width - (2 * rad), 0, 0);
		glutSolidCircle(rad, 50);
		glTranslatef(0, height - (2 * rad), 0);
		glutSolidCircle(rad, 50);
		glTranslatef(-(width - (2 * rad)), 0, 0);
		glutSolidCircle(rad, 50);
	}glPopMatrix();
	glutSolidRectangle(width, height - (2 * rad));
	glutSolidRectangle(width - (2 * rad), height);
}
void glutSolidRoundRectangle(double width, double height, double rad,int offset){
	if (offset == SQUARE_CENTER){
		rad = (rad > (width / 2)) ? width / 2 : rad;
		rad = (rad > (height / 2)) ? width / 2 : rad;
		glPushMatrix(); {
			glTranslatef(-((width / 2) - rad), -((height / 2) - rad), 0);
			glutSolidCircle(rad, 50);
			glTranslatef(width - (2 * rad), 0, 0);
			glutSolidCircle(rad, 50);
			glTranslatef(0, height - (2 * rad), 0);
			glutSolidCircle(rad, 50);
			glTranslatef(-(width - (2 * rad)), 0, 0);
			glutSolidCircle(rad, 50);
		}glPopMatrix();
		glutSolidRectangle(width, height - (2 * rad));
		glutSolidRectangle(width - (2 * rad), height);
	}
	else if (offset == SQUARE_LEFT_TOP){
		rad = (rad > (width / 2)) ? width / 2 : rad;
		rad = (rad > (height / 2)) ? width / 2 : rad;
		glPushMatrix(); {
			glTranslatef(rad, - rad, 0);
			glutSolidCircle(rad, 50);
			glTranslatef(0, -height+(2*rad), 0);
			glutSolidCircle(rad, 50);
			glTranslatef(width-(2*rad), 0, 0);
			glutSolidCircle(rad, 50);
			glTranslatef(0, height-2*rad, 0);
			glutSolidCircle(rad, 50);
		}glPopMatrix();
		glPushMatrix(); {
			glTranslatef(0,-rad,0);
			glutSolidRectangle(width, height - (2 * rad),SQUARE_LEFT_TOP);
			glTranslatef(rad, rad, 0);
			glutSolidRectangle(width - (2 * rad), height,SQUARE_LEFT_TOP);
		}
		glPopMatrix();
	}
}

//roundSquare
void glutWireRoundSquare(double side, double rad){
	glutWireRoundRectangle(side, side, rad);
}
void glutSolidRoundSquare(double side, double rad){
	glutSolidRoundRectangle(side, side, rad);
}


//roundSquareColumn
void glutSolidRoundRectangleColumn(double topWidth, double topHeight, double bottomWidth, double bottomHeight, double elevation, double rad){
	glPushMatrix(); {
		glutSolidRoundRectangle(topWidth,topHeight,rad);
		
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(-((topWidth / 2) - rad), (topWidth / 2) - rad, 0);
				//glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad,elevation,rad,50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topWidth/ 2) - rad), (topHeight / 2) - rad, 0);
				glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topWidth / 2) - rad), (topHeight / 2) - rad, 0);
				glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topWidth / 2) - rad), (topHeight / 2) - rad, 0);
				glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();

		}glPopMatrix();
		glTranslatef(0,0,elevation);
		glutSolidRoundRectangle(bottomWidth, bottomHeight, rad);
	}glPopMatrix();
}

void glutSolidRoundSquareColumn(double topSide, double bottomSide, double elevation, double rad){
	glPushMatrix(); {
		glutSolidRoundSquare(topSide, rad);
		glPushMatrix(); {
			glPushMatrix(); {
				glTranslatef(-((topSide / 2) - rad), (topSide/ 2) - rad, 0);
				//glScalef(topWidth / 4, topHeight / 4, 1);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topSide/ 2) - rad), (topSide / 2) - rad, 0);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topSide/ 2) - rad), (topSide/ 2) - rad, 0);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
			glRotatef(90, 0, 0, 1);
			glPushMatrix(); {
				glTranslatef(-((topSide/ 2) - rad), (topSide/ 2) - rad, 0);
				glutSolidCylinder(rad, elevation, rad, 50);
			}glPopMatrix();
		}glPopMatrix();
		glTranslatef(0, 0, elevation);
		glutSolidRoundSquare(bottomSide,rad);
	}glPopMatrix();
}

//axis
void glutDrawAxisMini(){
	glPushMatrix(); {
		glScalef(0.7, 0.7, 0.7);
		double fontsize = 0.1;
		glLineWidth(2);
		GLUT_PAINT_RED
			glBegin(GL_LINES); {
			glVertex3f(0, 0, 0);
			glVertex3f(0.15, 0, 0);
		}
		glEnd();
		glPushMatrix(); {
			glTranslatef(0.2, 0, 0);
			glutDrawText("X", fontsize);
		}glPopMatrix();
		/////////////
		GLUT_PAINT_GREEN
			glBegin(GL_LINES); {
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0.15, 0);
		}
		glEnd();
		glPushMatrix(); {
			glTranslatef(0, 0.2, 0);
			glutDrawText("Y", fontsize);
		}glPopMatrix();
		/////////////
		GLUT_PAINT_BLUE
			glBegin(GL_LINES); {
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 0.15);
		}
		glEnd();
		glPushMatrix(); {
			glTranslatef(0, 0, 0.2);
			glutDrawText("Z", fontsize);
		}glPopMatrix();
	}
	glPopMatrix();
}

void glutDrawGrid(double size){
	glPushMatrix(); {
		glLineWidth(0.25);
		glColor4f(0.5, 0.5, 0.5, 1);
		glScalef(size, size, size);
		for (int k = 0; k < 3; k++){
			if (k==1)
				glRotatef(90, 1, 0, 0);
			else if (k==2)
				glRotatef(90, 0, 1, 0);
			glPushMatrix(); {
				for (int j = 0; j < 2; j++){
					glBegin(GL_LINES); {
						for (int i = 0; i < 30; i++)
							if (-1 + (2 / 30.0)*i != 0){
								glVertex3f(-1+(2/30.0)*i,1,0);
								glVertex3f(-1 + (2 / 30.0)*i, -1, 0);
							}
					}glEnd();
					glRotatef(90, 0, 0, 1);
				}
			}glPopMatrix();
		}
	}
	glPopMatrix();
}
void glutDrawAxis(){
	glLineWidth(5);
	for (int i = 0; i < 3; i++)
	{
		GLUT_PAINT_RED
		if (i == 1){
			GLUT_PAINT_GREEN
			glRotatef(90, 0, 0, 1);
		}
		else if (i == 2){
			GLUT_PAINT_BLUE
			glRotatef(90, 0, 1, 0);
		}
		glPushMatrix(); {
			glBegin(GL_LINES); {
				glVertex3f(-4, 0, 0);
				glVertex3f(4, 0, 0);
			}
			glEnd();
		}
		glPopMatrix();
	}
}

void emitter(int amount){
	glMaterialf(GL_FRONT, GL_EMISSION, amount*128.0);
}

void drawScene(){
	glPushMatrix(); {
		//glScalef(0.5, 0.5, 0.5);
		//box;
		glPushMatrix(); {
			glPushMatrix(); {
				Mesh::setProp(11);
				glTranslatef(0, 0, -2);
				
				GLUT_PAINT_WHITE
					glPushMatrix(); {
					glScalef(1, 1, 0.001);
					glutSolidCube(4);
				}glPopMatrix();
			}glPopMatrix();

			glPushMatrix(); {
				emitter(128);
				glRotatef(90, 0, 1, 0);
				glTranslatef(0, 0, -2);
				GLUT_PAINT_GREEN
					glPushMatrix(); {
					glScalef(1, 1, 0.001);
					glutSolidCube(4);
				}glPopMatrix();
			}glPopMatrix();

			glPushMatrix(); {
				emitter(128);
				glRotatef(-90, 0, 1, 0);
				glTranslatef(0, 0, -2); 
				GLUT_PAINT_RED
					glPushMatrix(); {
					glScalef(1, 1, 0.001);
					glutSolidCube(4);
				}glPopMatrix();
			}glPopMatrix();

			glPushMatrix(); {
				glRotatef(-90, 1, 0, 0);
				glTranslatef(0, 0, -2);
				glRotatef(180, 0, 1, 0);
				GLUT_PAINT_WHITE
					glPushMatrix(); {
					glScalef(1, 1, 0.001);
					glutSolidCube(4);
				}glPopMatrix();
			}glPopMatrix();

			glPushMatrix(); {

				glRotatef(90, 1, 0, 0);
				glTranslatef(0, 0, -2);
				glRotatef(180, 0, 1, 0);
				GLUT_PAINT_WHITE
				glPushMatrix(); {
					glScalef(1, 1, 0.001);
					glutSolidCube(4);
				}glPopMatrix();
			}glPopMatrix();
			Mesh::setProp(11);
			glPopMatrix();
			glTranslatef(0, 0, 0);
			GLUT_PAINT_WHITE
			glutSolidSphere(0.75, 50, 50);
		}
		
	}
	glPopMatrix();
}

AUX_RGBImageRec *LoadBMP(char *Filename)    // Loads A Bitmap Image
{
	FILE *File = NULL;         // File Handle
	if (!Filename)          // Make Sure A Filename Was Given
	{
		return NULL;         // If Not Return NULL
	}
	File = fopen(Filename, "r");       // Check To See If The File Exists
	if (File)           // Does The File Exist?
	{
		fclose(File);         // Close The Handle
		return auxDIBImageLoad(Filename);    // Load The Bitmap And Return A Pointer
	}
	return NULL;          // If Load Failed Return NULL
}

GLuint LoadGLTextures(char* textureFileName)
{
	glEnable(GL_TEXTURE_2D);


	GLuint texID;

	AUX_RGBImageRec *TextureImage[1];

	memset(TextureImage, 0, sizeof(void *) * 1);
	if (TextureImage[0] = LoadBMP(textureFileName))
	{
		glGenTextures(1, &texID);					// Create The Texture

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return texID;											// Return The Status
}