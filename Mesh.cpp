#include"Mesh.h"

//#define DEBUG
Mesh::Mesh(){
	this->Init();
}
Mesh::Mesh(const char* filename){
	this->Init();
	//this->loadFile(filename);
	//this->ForBetterWorld(filename);
}
void Mesh::Init(){
	this->prop = 0;
	this->vertexSize = 0;
	this->polygonSize = 0;
	this->polygonTri = 0;
	this->textureFaceSize = 0;
	this->vertexTextureSize=0;
	this->hasSlash = 0;
	this->vertex = NULL;
	this->vertexNormal = NULL;
	this->vertexTexture = NULL;

	this->polygonTri = NULL;
	this->polygonQuad = NULL;

	this->faceNormal= NULL;
	
	this->textureFaceTri = NULL;
	this->textureFaceQuad= NULL;

	this->fileLoading = false;
	this->randColor = false;
	this->drawMode = 0;
	this->polygonShape = TRIANGLE;
	this->scaleValue = 1;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(0, TextureID);
}
void Mesh::loadFile(string filename) {
	fileLoading = true;
	fileName = filename;
	string tempname = filename;
	filename = MESH_DIR + filename;
	string textureName = TEXTURE_DIR + tempname.replace(tempname.length() - 4, 4, ".bmp");
	

	char* shouldering = new char[textureName.length() + 1];
	strcpy(shouldering,textureName.c_str());
	shouldering[textureName.length()] = '\0';
	//cout << shouldering << endl;

	this->TextureID[0]= LoadGLTextures(shouldering);

	cout << TextureID[0] << endl;
	this->vertexSize = 0;
	this->vertexNormalSize=0;
	this->polygonSize = 0;
	double dummy = 0;
	this->hasSlash = 0;
	char dummmy2[256];
	cout << "LOAD_COMPLETED :: " <<filename.c_str()<< endl;
#ifdef DEBUG
	cout << "opening file :: " << filename.c_str() << endl;
#endif
	FILE*  fp = fopen(filename.c_str(), "r");
	if (fp == NULL) {
		cout << "file open error :: ";
	}
	this->fileName = filename;
	//peek part
	int pos = filename.find("quad");
	cout << "pos : " << pos << endl;
	if (pos<0 || pos>filename.length())
		polygonShape = TRIANGLE;
	else
		polygonShape = QUADS;
	while (!feof(fp)) {
		fgets(dummmy2, 256, fp);
		if (dummmy2[0] == 'v'){
			if (dummmy2[1]==' ')
				vertexSize++;
			else if (dummmy2[1] == 'n'){
				vertexNormalSize++;
			}
			else if (dummmy2[1] == 't')
				vertexTextureSize++;
			//else if (dummmy2[0] == 'v'&&dummmy2[1] == 'n')
				//vertexNormalSize++;
		}
		else if (dummmy2[0] == 'f'){
			if (polygonSize == 0){
				for (int i = 0; i < strlen(dummmy2); i++){
					if (dummmy2[i] == '/')
						hasSlash = 1;
				}
			}
			polygonSize++;
		}
	}
	cout << "COUNT_COMPLETED :: " <<filename.c_str()<< endl;
	cout << "vertex :: " << this->vertexSize<< endl;
	cout << "vertexNormal :: " << this->vertexNormalSize<< endl;
	cout << "vertexTexture :: " << this->vertexTextureSize<< endl;
	cout << "polygon :: " <<this->polygonSize<< endl;
	cout << "polygonShape :: " << this->polygonShape << endl;
	rewind(fp);
	if (vertex != NULL)
		free(vertex);
	//if (vertexNormal != NULL&&vertexNormalSize>0)
		//free(vertexNormal);
	if (polygonTri != NULL)
		free(polygonTri);
	if (polygonQuad != NULL)
		free(polygonQuad);

	if (faceNormal!= NULL)
		free(faceNormal);
	
	vertex = NULL;
	vertexNormal = NULL;
	vertexTexture = NULL;
	polygonTri = NULL;
	polygonQuad = NULL;
	faceNormal = NULL;

	vertex = (V3D*)calloc(vertexSize,sizeof(V3D));
	if (vertexSize > 0)
		//vertexNormal = (V3D*)calloc(vertexSize, sizeof(V3D));
	
	vertexTexture= (V2D*)calloc(vertexTextureSize, sizeof(V2D));

		
		
	faceNormal= (V3D*)calloc(polygonSize,sizeof(V3D));
	if (polygonShape == TRIANGLE){
		polygonTri = (V3I*)calloc(polygonSize, sizeof(V3I));
		textureFaceTri = (V3I*)calloc(polygonSize, sizeof(V3I));
	}
	else{
		polygonQuad = (V4I*)calloc(polygonSize, sizeof(V4I));
		textureFaceQuad= (V4I*)calloc(polygonSize, sizeof(V4I));
	}
	//read part
	int vertexIndex = 0;
	int vertexNormalIndex = 0; 
	int vertexTextureIndex= 0;
	int polygonIndex = 0;
	int textureFaceIndex = 0;
	double maxPositionValue[3] = { 0 };
	char chardumm,chardumm2;
	
	int slasher = 0;
	int p1, p2, p3, p4;
	int t1, t2, t3, t4;
	int a1, a2, a3, a4;
	while (!feof(fp)) {
		fgets(dummmy2, 256, fp);
		if (dummmy2[0] == 'v'){
			if (dummmy2[1] == ' '){
				//if (vertexIndex == vertexSize)
				//continue;
				fseek(fp, 0 - (strlen(dummmy2) + 1), SEEK_CUR);
				fscanf(fp, "%c %lf %lf %lf", &chardumm, &vertex[vertexIndex][0], &vertex[vertexIndex][1], &vertex[vertexIndex][2]);
				for (int i = 0; i < 3; i++)
					maxPositionValue[i] = MAX(maxPositionValue[i], vertex[vertexIndex][i]);
				vertexIndex++;
			}
			if (vertexNormalSize > 0)
			{
				if (dummmy2[1] == 'n'){
					//fseek(fp, 0 - (strlen(dummmy2) + 1), SEEK_CUR);
					//fscanf(fp, "%c %lf %lf %lf", &chardumm, &vertexNormal[vertexNormalIndex][0], &vertexNormal[vertexNormalIndex][1], &vertexNormal[vertexNormalIndex][2]);
					vertexNormalIndex++;
				}
			}
			if(dummmy2[1]=='t'){
				fseek(fp, 0 - (strlen(dummmy2) + 1), SEEK_CUR);
				fscanf(fp, "%c%c %lf %lf", &chardumm,&chardumm2,  &vertexTexture[vertexTextureIndex][0], &vertexTexture[vertexTextureIndex++][1]);
				/*for (int i = 0; i < 2; i++)
					cout << vertexTexture[vertexTextureIndex - 1][i] << " ";
					cout << endl;*/
			}
			//else if (dummmy2[1] == 'n'){
			//	//if (vertexNormalIndex == vertexNormalSize)
			//	//continue;
			//	fseek(fp, 0 - (strlen(dummmy2) + 1), SEEK_CUR);
			//	fscanf(fp, "%c%c %lf %lf %lf", &chardumm, &chardumm2, &vertexNormal[vertexNormalIndex][0], &vertexNormal[vertexNormalIndex][1], &vertexNormal[vertexNormalIndex][2]);
			//	vertexNormalIndex++;
			//}
		}		
		else if (dummmy2[0] == 'f'){
			if (polygonIndex == (this->polygonSize))
				break;
			if (hasSlash){
				slasher = 0;
				for (int i = 0; i < strlen(dummmy2); i++){
					if (dummmy2[i] == '/'){
						slasher++;						dummmy2[i] = ' ';
					}
				}
				
				char* token;
				token = strtok(dummmy2, " ");	//token이 line문자열을 공백 기준으로 분리한 결과를 담음 "f 1212 1212 2323 2323 4343 4343"
				if (slasher==6 ){
					p1 = atoi(strtok(NULL, " ")), t1 = atoi(strtok(NULL, " "));
					p2 = atoi(strtok(NULL, " ")), t2 = atoi(strtok(NULL, " "));
					p3 = atoi(strtok(NULL, " ")), t3 = atoi(strtok(NULL, " "));

					polygonTri[polygonIndex][0] = p1 - 1;		polygonTri[polygonIndex][1] = p2 - 1;		polygonTri[polygonIndex][2] = p3 - 1;
					textureFaceTri[polygonIndex][0] = t1 - 1;	textureFaceTri[polygonIndex][1] = t2 - 1;	textureFaceTri[polygonIndex++][2] = t3 - 1;
				}
				else{
					
					p1 = atoi(strtok(NULL, " ")), t1 = atoi(strtok(NULL, " ")), a1 = atoi(strtok(NULL, " "));
					p2 = atoi(strtok(NULL, " ")), t2 = atoi(strtok(NULL, " ")), a1 = atoi(strtok(NULL, " "));
					p3 = atoi(strtok(NULL, " ")), t3 = atoi(strtok(NULL, " ")), a1 = atoi(strtok(NULL, " "));
					p4 = atoi(strtok(NULL, " ")), t4 = atoi(strtok(NULL, " ")), a1 = atoi(strtok(NULL, " "));
					//printf("%d %d %d %d %d %d\n", p1, t1, p2, t2, p3, t3);
					polygonQuad[polygonIndex][0] = p1 - 1;		polygonQuad[polygonIndex][1] = p2 - 1;		polygonQuad[polygonIndex][2] = p3 - 1; polygonQuad[polygonIndex][3] = p4 - 1;
					textureFaceQuad[polygonIndex][0] = t1 - 1;	textureFaceQuad[polygonIndex][1] = t2 - 1;	textureFaceQuad[polygonIndex][2] = t3 - 1; textureFaceQuad[polygonIndex++][3] = t4 - 1;
				}
			}
			else{
				fseek(fp, 0 - (strlen(dummmy2) + 1), SEEK_CUR);
				if (polygonShape == TRIANGLE){
					fscanf(fp, "%c %d %d %d", &chardumm, &polygonTri[polygonIndex][0], &polygonTri[polygonIndex][1], &polygonTri[polygonIndex][2]);
					for (int i = 0; i < 3; i++){
						polygonTri[polygonIndex][i]--;
					}
				}
				else{
					fscanf(fp, "%c %d %d %d %d", &chardumm, &polygonQuad[polygonIndex][0], &polygonQuad[polygonIndex][1], &polygonQuad[polygonIndex][2], &polygonQuad[polygonIndex][3]);
					for (int i = 0; i < 4; i++){
						polygonQuad[polygonIndex][i]--;
					}
				}

				polygonIndex++;
			}
		}
	}
	//polygonIndex--;
	//cout << chardumm << " " << polygonTri[polygonIndex][0] << " " << polygonTri[polygonIndex][1] << " " << polygonTri[polygonIndex][2] << endl;
	//cout << "vertexIndex: " << vertexIndex << "\n" << "polygonIndex: " << polygonIndex<<endl;
	fclose(fp);
	fileLoading = false;
	this->Alignmenter();
	this->Scaler(maxPositionValue);
	this->WireSetUp();
	//get faceNormal
	
	this->FaceNormalSetUp();
	//if (vertexNormalSize==0)
	this->VertexNormalSetUp();
	cout << "LOAD_COMPLETED :: " << filename.c_str() << endl;
	//dataPrinter();
}
void Mesh::FaceNormalSetUp(){
	if (polygonShape == TRIANGLE){
		double temp = 0;
		double edge[2][3] = { 0 };
		for (int i = 0; i < polygonSize; i++){
			temp = 0;
			for (int j = 0; j < 3; j++){
				edge[0][j] = vertex[polygonTri[i][1]][j] - vertex[polygonTri[i][0]][j];//v2-v1
				edge[1][j] = vertex[polygonTri[i][2]][j] - vertex[polygonTri[i][0]][j];//v3-v2
			}
			faceNormal[i][0] = (edge[0][1] * edge[1][2]) - (edge[0][2] * edge[1][1]);
			faceNormal[i][1] = (edge[0][2] * edge[1][0]) - (edge[0][0] * edge[1][2]);
			faceNormal[i][2] = (edge[0][0] * edge[1][1]) - (edge[0][1] * edge[1][0]);

			for (int j = 0; j < 3; j++)
				temp += pow(faceNormal[i][j], 2);

			for (int j = 0; j < 3; j++)
				faceNormal[i][j] = faceNormal[i][j] / sqrt(temp)*this->scaleValue;
		}
	}
	else{
		double temp = 0;
		double edge[2][3] = { 0 };
		for (int i = 0; i < polygonSize; i++){
			temp = 0;
			for (int j = 0; j < 3; j++){
				edge[0][j] = vertex[polygonQuad[i][1]][j] - vertex[polygonQuad[i][0]][j];//v2-v1
				edge[1][j] = vertex[polygonQuad[i][2]][j] - vertex[polygonQuad[i][0]][j];//v3-v2
			}
			faceNormal[i][0] = (edge[0][1] * edge[1][2]) - (edge[0][2] * edge[1][1]);
			faceNormal[i][1] = (edge[0][2] * edge[1][0]) - (edge[0][0] * edge[1][2]);
			faceNormal[i][2] = (edge[0][0] * edge[1][1]) - (edge[0][1] * edge[1][0]);

			for (int j = 0; j < 3; j++)
				temp += pow(faceNormal[i][j], 2);

			for (int j = 0; j < 3; j++){
				faceNormal[i][j] = faceNormal[i][j] / sqrt(temp)*this->scaleValue;
			}
		}
	}
	
}
//deprecated
void Mesh::ForBetterWorld(string fileDir){
	/*
	FILE* fp = fopen(fileDir.c_str(), "r");
	if (fp == NULL){
		printf("%s file can not open", fileDir);
		exit(1);
	}
	
	char line[256];

	while (!feof(fp)){
		fgets(line, 256, fp);
		if (line[0] == 'v'){
			if (line[1] == 't'){			//texture
				this->textureSize++;
			}
			else if (line[1] == 'n'){	//normal
				vertexNormalSize++;
			}
			else if (line[1] == ' ')		//vertex
				vertexSize++;
		}
		else if (line[0] == 'f'){		//face
			polygonSize++;
		}
	}
	rewind(fp);

	polygonSize--;

	printf("number of vertices : %d\n", vertexSize);
	printf("number of polygon : %d\n", polygonSize);
	printf("number of textureCoords : %d\n", textureSize);
	
	vertex = (V3D*)malloc(sizeof(V3D)*vertexSize);		//정점 
	//texture = (V2D*)malloc(sizeof(V2D)* textureSize);	//텍스쳐 
	polygonTri = (V3I*)malloc(sizeof(V3I) * polygonSize);				//면 (정점)
	textureFaceTri = (V3I*)malloc(sizeof(V3I) * polygonSize);			//면 (텍스쳐)
	
	int j = 0, k = 0;
	int IdxFace = 0;
	int IdxTexCoord = 0;

	while (!feof(fp)){
		fgets(line, 256, fp);
		if (line[0] == 'v'){				//정점만 읽기// vt는 텍스쳐 정보 
			if (line[1] == 't'){			//texture
				//////////////////////////////////////////////////////////////////////////
				//텍스쳐 로드
				//////////////////////////////////////////////////////////////////////////
			}
			else if (line[1] == 'n'){	//normal
			}
			else{						//vertex
				fseek(fp, 0 - (strlen(line) + 1), SEEK_CUR);		//한줄 앞으로
				float x, y, z;
				fscanf(fp, "%s %f %f %f", line, &x, &y, &z);
				vertex[j][0] = x;	vertex[j][1] = y;	vertex[j++][2] = z;
				//printf("%f %f %f\n", vertices[j-3], vertices[j-2], vertices[j-1]);
			}
		}
		else if (line[0] == 'f'){		//면정보 읽기
			if (textureSize > 0){		//Obj 텍스쳐가 있으면 "/" 구분자를 구별 해야 함
				for (int i = 0; i < strlen(line); ++i){
					if (line[i] == '/')	line[i] = ' ';
				}
				char* token;
				token = strtok(line, " ");	//token이 line문자열을 공백 기준으로 분리한 결과를 담음 "f 1212 1212 2323 2323 4343 4343"

				int p1 = atoi(strtok(NULL, " ")), t1 = atoi(strtok(NULL, " "));
				int p2 = atoi(strtok(NULL, " ")), t2 = atoi(strtok(NULL, " "));
				int p3 = atoi(strtok(NULL, " ")), t3 = atoi(strtok(NULL, " "));

				//면의 정점 순서 정보
				polygonTri[IdxFace][0] = p1 - 1;		polygonTri[IdxFace][1] = p2 - 1;		polygonTri[IdxFace++][2] = p3 - 1;
				
				//면의 텍스쳐 순서 정보
				textureFaceTri[IdxTexCoord][0] = t1 - 1;	textureFaceTri[IdxTexCoord][1] = t2 - 1;	textureFaceTri[IdxTexCoord++][2] = t3 - 1;
				

				//printf("%d %d %d %d %d %d\n", p1, t1, p2, t2, p3, t3);
			}
			else{						//텍스쳐가 없는 면 정보
				int f1, f2, f3;
				fseek(fp, 0 - (strlen(line) + 1), SEEK_CUR);		//한줄 앞으로
				fscanf(fp, "%s %d %d %d", line, &f1, &f2, &f3);		//Obj 텍스쳐가 없으면 3개의 면정보만 존재 
				polygonTri[k][0] = f1 - 1;	polygonTri[k][1] = f2 - 1;	polygonTri[k++][2] = f3 - 1;
				//printf("%d %d %d\n", faces[j-3], faces[j-2], faces[j-1]);
			}
		}
	}
	//dataPrinter();
	WireSetUp();
	//노말
	*/
}
void Mesh::Scaler(double* maxPostionValue){
	double realMaxvalue = 0;;
	for (int i = 0; i < 3; i++){
		realMaxvalue = MAX(realMaxvalue, maxPostionValue[i]);
	}
	cout<<"realMaxValue::" << realMaxvalue << endl; 
	this->scaleValue = 1 / (realMaxvalue);

	for (int i = 0; i < vertexSize;i++){
		for (int j = 0; j < 3; j++)
			vertex[i][j] *= this->scaleValue;
	}
}
void Mesh::dataPrinter(){
	if (vertex == NULL){
		cout << "error:: vertex not loaded" << endl;
		return;
	}
	/*cout << "printing vertex" << endl;

	for (int i = 0; i < vertexSize; i++){
		for (int j = 0; j < ((drawMode == TRIANGLE) ? 3 : 4); j++)
			cout << vertex[i][j]<<" ";
		cout << endl;
	}*/
	cout << "printing polygon" << endl;

	for (int i = 0; i < polygonSize; i++){
		for (int j = 0; j < 3; j++)
			cout << polygonTri[i][j] << " ";
		cout << endl;
	}
}

string Mesh::objPolisher(string filename){

	if (filename.find("_polished") != string::npos)
		return filename;
	string prev_filename = filename;
	string new_filename = filename.insert(filename.length() - 4, "_polished");
	FILE* fp2 = fopen(new_filename.c_str(), "r");
	if (fp2 == NULL){
		FILE* fp2 = fopen(new_filename.c_str(), "w");
		FILE* fp = fopen(prev_filename.c_str(), "r");
		//file checker
		if (fp2 == NULL) {
			cout << "file open error :: " << new_filename.c_str() << endl;
			assert(false);
		}
		if (fp == NULL) {
			cout << "file open error :: " << filename.c_str() << endl;
			assert(false);
		}
		//
		char dummy;
		while (!feof(fp)){
			fscanf(fp, "%c", &dummy);
			if (dummy == '#'){//주석처리
				while (dummy != '\n'){
					fscanf(fp, "%c", &dummy);
					cout << dummy;
				}				
			}
			//cout << dummy;
			fprintf(fp2, "%c", dummy);
		}
		cout << "file created :: " << new_filename.c_str() << endl;
		return new_filename;
		fclose(fp);
	}
	else{
		cout << "file created :: " << "already polished" << endl;
		return filename;
	}
	fclose(fp2);
}

void Mesh::setProp(int number){
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];
	GLfloat shininess;
	GLfloat emission;
	switch (number){
	case 0://emerald
		ambient[0] = ambient[2] = 0.0215;
		ambient[1] = 0.1745;
		diffuse[0] = diffuse[2] = 0.07568;
		diffuse[1] = 0.61424;
		specular[0] = specular[2] = 0.633;
		specular[1] = 0.727811;
		shininess = 0.6;
		
		break;
	case 1://obsidian
		ambient[0] = 0.05375;
		ambient[1] = 0.05;
		ambient[2] = 0.6625;

		diffuse[0] = 0.18275;
		diffuse[1] = 0.17;
		diffuse[2] = 0.22525;

		specular[0] = 0.332741;
		specular[1] = 0.328634;
		specular[2] = 0.346435;
		shininess = 0.3;
		break;
	case 2://brass
		ambient[0] = 0.329412;
		ambient[1] = 0.223529;
		ambient[2] = 0.027451;

		diffuse[0] = 0.780392;
		diffuse[1] = 0.568627;
		diffuse[2] = 0.113725;

		specular[0] = 0.992157;
		specular[1] = 0.941176;
		specular[2] = 0.807843;
		shininess = 0.21794872;
		break;
	case 3://chrome
		ambient[0] = ambient[1] = ambient[2] = 0.25;
		 
		diffuse[0] = diffuse[1] = diffuse[2] = 0.4;
		
		specular[0] = specular[1] = specular[2] = 0.774597;
		
		shininess = 0.6;
		break;
	///////////////////////////////////////////////////
	case 4://gold
		ambient[0] = 0.24725;
		ambient[1] = 0.1995;
		ambient[2] = 0.0745;

		diffuse[0] = 0.75164;
		diffuse[1] = 0.60648;
		diffuse[2] = 0.22648;

		specular[0] = 0.628281;
		specular[1] = 0.555802;
		specular[2] = 0.366065;
		shininess = 0.4;
		break;

	case 5://red rubber
		ambient[1] = ambient[2] = 0.0;
		ambient[0] = 0.05;
		diffuse[1] = diffuse[2] = 0.4;
		diffuse[0] = 0.5;
		specular[1] = specular[2] = 0.04;
		specular[0] = 0.7;
		shininess = 0.078125;
		break;
	case 6://silver
		ambient[0] = ambient[1] = ambient[2] = 0.19225;
		
		diffuse[0] = diffuse[1] = diffuse[2] = 0.50754;

		specular[0] = specular[1] = specular[2] = 0.508273;

		shininess = 0.4;
		break;
	case 7://pearl
		ambient[0] = 0.25;
		ambient[1] = 0.20725;
		ambient[2] = 0.20725;

		diffuse[0] = 1;
		diffuse[1] = 0.829;
		diffuse[2] = 0.829;

		specular[0] = specular[1] = specular[2] = 0.296648;
		shininess = 0.088;
		break;
	///////////////////////////////////////////////////
	case 8://turquoise
		ambient[0] = 0.1;
		ambient[1] = 0.18725;
		ambient[2] = 0.1745;

		diffuse[0] = 0.396;
		diffuse[1] = 0.74151;
		diffuse[2] = 0.69102;

		specular[0] = 0.297254;
		specular[1] = 0.30829;
		specular[2] = 0.306678;
		shininess = 0.1;
		break;
	case 9: //yellow rubber
		ambient[0] = ambient[1] = 0.05;
		ambient[2] = 0;
		diffuse[0] = diffuse[1] = 0.5;
		diffuse[2] = 0.4;
		specular[1] = specular[0] = 0.7;
		specular[2] = 0.04;
		shininess = 0.078125;
		break;
	case 10://black
		ambient[0] = ambient[1] = ambient[2] = 0.000;

		diffuse[0] = diffuse[1] = diffuse[2] = 0.000;

		specular[0] = specular[1] = specular[2] = 0;
		shininess = 1;
		break;
	default://white
		ambient[0] = ambient[1] = ambient[2] = 0.5;

		diffuse[0] = diffuse[1] = diffuse[2] = 0.9;

		specular[0] = specular[1] = specular[2] =0;
		shininess = 1;
		break;
	}
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);;
	glMaterialf(GL_FRONT, GL_SHININESS, shininess*128.0);
	
	glMaterialf(GL_FRONT_AND_BACK, GL_EMISSION, 128.0);
}
void Mesh::changeProp(){
	this->prop = (prop + 1) % 12;
}
void colorPalette(int prop){
	switch (prop){
	case 0:
		glColor3f(0.5215, 0.5745, 0.5215);
		break;
	case 1:
		glColor3f(0.05375, 0.05, 0.06625);
		break;
	}
}
void Mesh::render(){
	
	glPushMatrix(); {
		glEnable(GL_TEXTURE_2D);
		if (polygonSize != 0&&this->fileLoading!=true){
			//if (Wire!=NULL)
			Node<Line*>* currentNode = Wire->List;
		//	glEnableClientState(GL_VERTEX_ARRAY);
			//glEnableClientState(GL_NORMAL_ARRAY);
			
			//glScalef(this->scaleValue, this->scaleValue, this->scaleValue);
			//setProp(this->prop);
			//glDisable(GL_COLOR_MATERIAL);
			
			if ((fileName != "") && (fileLoading == false)){
				//colorPalette(this->prop);
				switch (drawMode){
				case 0: //poly
					
					//glEnable(GL_TEXTURE_2D);
					//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					
					
					for (int i = 0; i < polygonSize; i++){
						if (randColor){
							switch (i % 3)
							{
							case 0:
								glColor3f(1, 0, 0);
								break;
							case 1:
								glColor3f(0, 1, 0);
								break;
							default:
								glColor3f(0, 0, 1);
								break;
							}
						}
						//glColor3f(255.0/255.0, 217.0/255.0, 170.0/255.0);
						//glColor3f(1, 1, 1);
						
						if (polygonShape == TRIANGLE){
							
							glBegin(GL_TRIANGLES); {								
								
								for (int j = 2; j >= 0; j--){
									glNormal3f(vertexNormal[polygonTri[i][j]][0], vertexNormal[polygonTri[i][j]][1], vertexNormal[polygonTri[i][j]][2]);
									//glVertexPointer(3, GL_FLOAT, 0, vertex[polygonTri[i][j] - 1]);
									glVertex3f(vertex[polygonTri[i][j]][0], vertex[polygonTri[i][j]][1], vertex[polygonTri[i][j]][2]);
								}
							}
							glEnd();
						}
						else{

							//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
							glBindTexture(GL_TEXTURE_2D, TextureID[0]);
							
							glBegin(GL_QUADS); {
								for (int j = 0; j < 4; j++){
									glNormal3f(vertexNormal[polygonQuad[i][j]][0], vertexNormal[polygonQuad[i][j]][1], vertexNormal[polygonQuad[i][j]][2]);
									//if(this->TextureID!=0)
									glTexCoord2f(vertexTexture[textureFaceQuad[i][j]][0], vertexTexture[textureFaceQuad[i][j]][1]);
									glVertex3f(vertex[polygonQuad[i][j]][0], vertex[polygonQuad[i][j]][1], vertex[polygonQuad[i][j]][2]);
								}
							}
							glEnd();							
						}
					}
					
					break;
				case 1://wireframe
					glLineWidth(0.5);
						glBegin(GL_LINES); {
							//if (Wire != NULL){
								for (int i = 0; i < Wire->getSize(); i++){
									if (randColor) {
										switch (i % 3)
										{
										case 0:
											glColor3f(1, 0, 0);
											break;
										case 1:
											glColor3f(0, 1, 0);
											break;
										default:
											glColor3f(0, 0, 1);
											break;
										}
									}
									glNormal3f(vertexNormal[currentNode->data->a][0], vertexNormal[currentNode->data->a][1], vertexNormal[currentNode->data->a][2]);
									glVertex3f(vertex[currentNode->data->a][0], vertex[currentNode->data->a][1], vertex[currentNode->data->a][2]);

									glNormal3f(vertexNormal[currentNode->data->b][0], vertexNormal[currentNode->data->b][1], vertexNormal[currentNode->data->b][2]);
									glVertex3f(vertex[currentNode->data->b][0], vertex[currentNode->data->b][1], vertex[currentNode->data->b][2]);
									currentNode = currentNode->Link;
								}
							//}
						}
						glEnd();
					
					break;
				case 2://points
					glPointSize(1);
					
					glBegin(GL_POINTS); {
						for (int i = 0; i < vertexSize; i++){
							glNormal3f(vertexNormal[i][0], vertexNormal[i][1], vertexNormal[i][2]);
							glVertex3f(vertex[i][0], vertex[i][1], vertex[i][2]);
						}
					}
					glEnd();

					break;
				default:
					break;
				}
			}
		}
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}
	glPopMatrix();
}

void Mesh::modeChange(){
	this->drawMode = (drawMode + 1) % 3;
}

void Mesh::setRandColor(bool value){
	this->randColor = value;
}
int Mesh::getDrawMode(){
	return this->drawMode;
};

void Mesh::Alignmenter(){
	double maxValues[] = { 0,0,0 };
	double minValues[] = { vertex[0][0], vertex[0][1], vertex[0][2] };
	double medivalValue[3];
	double average[3] = {0};
	for (int i = 0; i < vertexSize; i++){
		for (int j = 0; j < 3; j++){
			maxValues[j] = (maxValues[j]>vertex[i][j]) ? maxValues[j] : vertex[i][j];
			minValues[j] = (minValues[j]<vertex[i][j]) ? minValues[j] : vertex[i][j];
			average[j] += vertex[i][j];
		}
	}
	for (int i = 0; i < 3; i++){
		medivalValue[i] = (maxValues[i] + minValues[i]) / 2.0;
		average[i] /= vertexSize;
		average[i] += medivalValue[i];
		average[i] /= 2.0;
	}
	for (int i = 0; i < vertexSize; i++){
		for (int j = 0; j < 3; j++){
			vertex[i][j] -= average[j];
		}
	}
}
void Mesh::WireSetUp(){
	int i, j;
	int order[2];
	Line* temp;
	if (Wire != NULL){
		//delete Wire;
		Wire->cleanUp();
		delete Wire;
	}
	this->Wire = new BenList<Line*>();

	BenList<int>** list;
	list =new BenList<int>*[vertexSize];

	for (int i = 0; i < vertexSize; i++){
		list[i] = new BenList<int>();
	}
	cout << "Wiresetup::vertexsize" << vertexSize<< endl;
	cout << "Wiresetup::polygonsize" <<polygonSize<< endl;
	if (polygonShape == TRIANGLE){
		for (i = 0; i < polygonSize-30;i++){
			for (j = 0; j < 3; j++){
				order[0] = ((polygonTri[i][(j + 1) % 3])>(polygonTri[i][j % 3]) ) ? (polygonTri[i][j % 3] ) : (polygonTri[i][(j + 1) % 3] );
				order[1] = ((polygonTri[i][(j + 1) % 3]) < (polygonTri[i][j % 3]) ) ? (polygonTri[i][j % 3] ) : (polygonTri[i][(j + 1) % 3]);
				order[0] = (order[0] < 0) ? 0 : order[0];
				order[1] = (order[1] > polygonSize - 1) ? polygonSize - 1 : order[1];
				if (list[order[0]]->isThereThisGuy(order[1]) == false && list[order[1]]->isThereThisGuy(order[0]) == false){
					temp = (Line*)malloc(sizeof(Line));
					temp->a = order[0];
					temp->b = order[1];
					Wire->append(temp);
					
					list[order[0]]->append(order[1]);
					list[order[1]]->append(order[0]);
				}				
			}
		}
	}
	else{
		for (i = 0; i < polygonSize-30; i++){
			//cout << "poly " <<i<< endl;
			for (j = 0; j < 3; j++){
				order[0] = ((polygonQuad[i][(j + 1) % 3] )>(polygonQuad[i][j % 3]) ) ? (polygonQuad[i][j % 3] ) : (polygonQuad[i][(j + 1) % 3] );
				order[1] = ((polygonQuad[i][(j + 1) % 3] ) < (polygonQuad[i][j % 3]) ) ? (polygonQuad[i][j % 3] ) : (polygonQuad[i][(j + 1) % 3]);
				
				order[0] = (order[0] < 0) ? 0 : order[0];
				order[1] = (order[1] < 0) ? 0 : order[1];
				order[1] = (order[1] > polygonSize - 1) ? polygonSize - 1 : order[1];
				//cout << order[1] << " " << order[0] << endl;
				if (list[order[0]]->isThereThisGuy(order[1]) == false && list[order[1]]->isThereThisGuy(order[0]) == false){
					temp = (Line*)malloc(sizeof(Line));
					temp->a = order[0];
					temp->b = order[1];
					Wire->append(temp);

					list[order[0]]->append(order[1]);
					list[order[1]]->append(order[0]);
				}
			}
		}
	}
	cout << "WIRESIZE=" << Wire->getSize() << endl;

	for (int i = 0; i < vertexSize; i++){
		list[i]->cleanUp();
		delete list[i];
	}
	delete[] list;
}

void Mesh::VertexNormalSetUp(){
	if (vertexNormal != NULL)
		free(vertexNormal);
	if (polygonShape== TRIANGLE){
		vertexNormalSize = vertexSize;
			free(vertexNormal);
		vertexNormal = (V3D*)calloc(vertexNormalSize, sizeof(V3D));

		for (int i = 0; i < polygonSize-30; i++){//모든 폴리곤들에 대해
			for (int j = 0; j < 3; j++){//모든 정점으로 
				for (int k = 0; k < 3;k++)//xyz에 대해
					vertexNormal[polygonTri[i][j]][k] += faceNormal[i][k];
			}
		}
	}
	else{
		vertexNormalSize = vertexSize;
			free(vertexNormal);
		vertexNormal = (V3D*)calloc(vertexNormalSize, sizeof(V3D));

		for (int i = 0; i < polygonSize - 30; i++){//모든 폴리곤들에 대해
			for (int j = 0; j < 4; j++){//모든 정점으로 
				for (int k = 0; k < 3; k++)//xyz에 대해
					vertexNormal[polygonQuad[i][j]][k] += faceNormal[i][k];
			}
		}
	}
}