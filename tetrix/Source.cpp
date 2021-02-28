#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <string>
#include <cmath>
#include <math.h>
#include <vector>
#include <fstream>
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/GL.h>
#include <GL/glut.h>


using namespace std;

// 

ofstream myfile("Result.txt", ios::app);


int Width = 800, Height = 800;

float Block_diffuse[] = { 0, 1, 0 }; //
float Next_Block_diffuse[] = { 1, 0, 0 }; //
float Floor_Block_diffuse[] = { 0.3, 0.3, 0.3 }; //
float Frame_diffuse[] = { 0.1, 0.1, 0.1 }; //
float Frame_ambient[] = { 0.9, 0.9, 0.9 };//


int arr[7][11][7] = { 0, };  // 
int B_height = 10; // 
int B_x = 3, B_z = 3; // 
int x_1, y_1, z_1, x_2, y_2, z_2, x_3, y_3, z_3; // 
int Next_choose; // 

bool first = 1; //
bool FastDown = false; //
bool GameOver = false;//
// 
int ManipulateMode = 0;
int StartPt[2];
int score = 0;
//char score1[100];

//
float Axis[3] = { 1.0, 0.0, 0.0 };
float Angle = 0.0;
float RotMat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float Zoom = -60.0;

// 
void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void MouseMove(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Render();
void SetupViewTransform();
void SetupViewVolume();
void Timer(int id);
void* font = GLUT_BITMAP_HELVETICA_18;

//

void InitGL();
void MakeWall();
void RenderFrame();
void RenderCubes();
void RenderBlock();
void RenderNextB();
void res(int key, int x, int y);
void drawCube(float x, float y, float z);
void drawBlock(int x, int y, int z);
bool CollisionBlock(int x, int y, int z); // 
bool CollisionCube(int x, int y, int z); // 
void floorDelete(int y);  // 
void resetBlock(); // 
void GetSphereCoord(int x, int y, float* px, float* py, float* pz);
void RenderInterFace();
void text(float x, float y, float z, const char* string)
{
	glColor3f(0, 0, 0);
	glRasterPos3f(x, y, z);
	int m = 0;
	for (; *string != '\0'; string++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *string);
		m++;
	}
}

void text_1(float x, float y,float z, char* str)
{
	glColor3f(0, 0, 0);
	glRasterPos3f(x, y, z);
	int s = 0;
	for (; *str != '\0'; str++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *str);
		s++;
	}
}

void printTxt(float x, float y, char* String) {
	char* c;

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 10, 0, 10);

	glColor3f(9.0f, 0.7f, 0.4f);
	glRasterPos2f(x, y);
	(GL_LIGHTING);

	for (c = String; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}



int main(int argc, char** argv)
{
	setlocale(LC_ALL, "RUS");
	
		MakeWall();

		srand(time(NULL));

		//

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

		// 

		glutInitWindowSize(Width, Height);
		glutCreateWindow("Курсовая работа");
		InitGL();
		//

		//
		glutReshapeFunc(Reshape);
		glutMouseFunc(Mouse);
		glutMotionFunc(MouseMove);
		glutKeyboardFunc(Keyboard);
		glutDisplayFunc(Render);
		//glutIdleFunc(Render);
		glutSpecialFunc(res);

		//

		glutTimerFunc(1000, Timer, 1); //
		glutTimerFunc(5, Timer, 2); //
		glutTimerFunc(100, Timer, 3);//

		//

		glutMainLoop();
		return 0;
	
}

void Timer(int id) // 
{

	if (id == 1 && !GameOver) //
	{
		if (!FastDown)
		{
			int y = B_height;
			if (y > B_height + y_1)
				y = B_height + y_1;

			if (y > B_height + y_2)
				y = B_height + y_2;

			if (y > B_height + y_3)
				y = B_height + y_3; //

			if (y == 1 || CollisionBlock(B_x, B_height - 1, B_z)) //
			{
				//
				arr[B_x][B_height][B_z] = 1;
				arr[B_x + x_1][B_height + y_1][B_z + z_1] = 1;
				arr[B_x + x_2][B_height + y_2][B_z + z_2] = 1;
				arr[B_x + x_3][B_height + y_3][B_z + z_3] = 1;

				floorDelete(y);
				score++;
				//
				resetBlock();
			}
			//
			else
			{
				B_height--; //

				glutPostRedisplay();
			}
		}
		glutTimerFunc(2000, Timer, 1);
	}

	if (id == 2 && !GameOver) //
	{
		if (FastDown)
		{
			int y = B_height;
			if (y > B_height + y_1)
				y = B_height + y_1;

			if (y > B_height + y_2)
				y = B_height + y_2;

			if (y > B_height + y_3)
				y = B_height + y_3; // 

			if (y == 1 || CollisionBlock(B_x, B_height - 1, B_z)) //
			{
				//
				arr[B_x][B_height][B_z] = 1;
				arr[B_x + x_1][B_height + y_1][B_z + z_1] = 1;
				arr[B_x + x_2][B_height + y_2][B_z + z_2] = 1;
				arr[B_x + x_3][B_height + y_3][B_z + z_3] = 1;
				//
				floorDelete(y);
				score++;
				//
				resetBlock();
				FastDown = false; //
			}
			//
			else
			{
				B_height--; //
			}
			glutPostRedisplay();
		}
		glutTimerFunc(5, Timer, 2);
	}
	if (id == 3)
	{
		for (int i = 1; i < 6; i++)
			for (int j = 1; j < 6; j++)
				if (arr[i][10][j] == 1)
				{
					//
					GameOver = true;

					

					glutPostRedisplay();
				}
		glutTimerFunc(100, Timer, 3);
	}
}
void MakeWall()
{
	//

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 7; j++) //
			arr[0][i][j] = 1;

		for (int j = 0; j < 7; j++) //
			arr[6][i][j] = 1;
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 11; j++) //
			arr[i][j][0] = 1;

		for (int j = 0; j < 11; j++) //
			arr[i][j][6] = 1;

		for (int j = 0; j < 7; j++) //
			arr[i][0][j] = 1;
	}

	//
}

void InitGL()
{
	///єыјіБ¤
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat light0_position[] = { -1, 1, 1, 0 }; //
	GLfloat light0_ambient[] = { 0.7, 0.7, 0.7 }; //
	GLfloat light0_diffuse[] = { 0.7, 0.7, 0.7 }; //
	GLfloat light0_specular[] = { 0.7, 0.7, 0.7 }; //
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

}

bool CollisionCube(int x, int y, int z) //
{

	if (arr[x][y][z] == 1)
	{
		return true;
	}
	else return false;
}

bool CollisionBlock(int x, int y, int z)//
//
{
	if (CollisionCube(x, y, z) == true || CollisionCube(x + x_1, y + y_1, z + z_1) == true || CollisionCube(x + x_2, y + y_2, z + z_2) == true || CollisionCube(x + x_3, y + y_3, z + z_3) == true)
	{
		//
		return true;
	}
	else return false;
}


void Render() //
{
	float i;
	char scr[20];
	char scre[20];

	strcpy_s(scr, "S C O R E : ");
	sprintf_s(scre, "%d", score);
	strcat_s(scr, scre);
	// 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	SetupViewVolume();

	//
	SetupViewTransform();

	glMatrixMode(GL_MODELVIEW);

	printTxt(1.0f, 1.0f, scr);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, Frame_diffuse);
	RenderInterFace();

	if (GameOver)
	{
		text(5.0, 10.0, 5.0, "GAME OVER");
		text(5.0, 13.0, 5.0, "Press F2 to restart");
		
		
		//text(5.0, 8.0, 5.0, "SCORE:");
		//text_1(5.0, 16.0, 7.0, &score1[0]);
	}



	//
	float mat_ambient[] = { 0.3, 0.3, 0.3 };
	float mat_specular[] = { 0.9, 0.9, 0.9 };
	float mat_shininess = 120;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shininess);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, Frame_diffuse);//
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Frame_ambient); //

	RenderFrame(); //

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Floor_Block_diffuse);//

	RenderCubes();//

	if (first == 1) //
	{
		Next_choose = rand() % 5;
		resetBlock();
		first = 0;
	}
	if (!GameOver)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Block_diffuse); //
	RenderBlock(); //
	RenderNextB();


	//
	glutSwapBuffers();
	glFlush();
}
void RenderInterFace()
{
	glBegin(GL_LINES); //
	glLineWidth(10);
	glVertex3f(-7, 7, 0);
	glVertex3f(-4.5, 7, 0);



	glVertex3f(-4.5, 7, 0);
	glVertex3f(-4.8, 7, -0.3);

	glVertex3f(-4.5, 7, 0);
	glVertex3f(-4.8, 7, 0.3);
	glEnd();
	text(-5, 7.3, 0, "1");

	glBegin(GL_LINES); //
	glLineWidth(10);
	glVertex3f(-7, 7, 0);
	glVertex3f(-7, 9.5f, 0);

	glVertex3f(-7, 9.5, 0);
	glVertex3f(-6.7, 9.2, 0);

	glVertex3f(-7, 9.5, 0);
	glVertex3f(-7.3, 9.2, 0);
	glEnd();
	text(-7, 9, 0.5, "2");

	glBegin(GL_LINES); //
	glLineWidth(10);
	glVertex3f(-7, 7, 0);
	glVertex3f(-7, 7, 2.5);

	glVertex3f(-7, 7, 2.5);
	glVertex3f(-6.7, 7, 2.2);

	glVertex3f(-7, 7, 2.5);
	glVertex3f(-7.3, 7, 2.2);
	glEnd();
	text(-7, 7.5, 2.5, "3");


	text(-7, 5, 2.5, "F:DOWN");
	text(-7, 12, 0.5, "F1:EXIT");

	//text(-7, 3, 2.5, "SCORE:");
	

}
void RenderFrame() //
{
	for (float x = -2.5; x <= 2.5; x += 1.0) //
	{
		glBegin(GL_LINES);
		glVertex3f(x, 0.0, -2.5f);
		glVertex3f(x, 0.0, 2.5f);
		glVertex3f(x, 10.0, -2.5f);
		glVertex3f(x, 10.0, 2.5f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-2.5f, 0.0, x);
		glVertex3f(2.5f, 0.0, x);
		glVertex3f(-2.5f, 10.0, x);
		glVertex3f(2.5f, 10.0, x);
		glEnd();
	}

	for (float x = 0.0; x <= 10.0; x += 1.0) //
	{
		glBegin(GL_LINES);
		glVertex3f(-2.5, x, -2.5f);
		glVertex3f(-2.5, x, 2.5f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-2.5f, x, -2.5);
		glVertex3f(2.5f, x, -2.5);
		glEnd();
	}
	for (float x = -2.5; x <= 2.5; x += 1.0) //
	{
		glBegin(GL_LINES);
		glVertex3f(-2.5, 0.0, x);
		glVertex3f(-2.5, 10.0, x);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(x, 0.0, -2.5);
		glVertex3f(x, 10.0, -2.5);
		glEnd();
	}
}

void RenderCubes() //
{
	for (int y = 1; y < 11; y++)
	{
		for (int x = 1; x < 6; x++)
		{
			for (int z = 1; z < 6; z++)
			{
				if (arr[x][y][z] == 1)
					drawCube(x, y, z);
			}
		}
	}
}

void drawCube(float x, float y, float z) //
{
	glPushMatrix(); {
		glTranslatef(-3.0 + x, -0.5 + y, -3.0 + z);
		glutSolidCube(1.0); //

		glBegin(GL_LINE_STRIP); //
		glVertex3f(-0.5, -0.5, -0.5);
		glVertex3f(-0.5, -0.5, 0.5);
		glVertex3f(0.5, -0.5, 0.5);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(-0.5, -0.5, -0.5);

		glVertex3f(-0.5, 0.5, -0.5);
		glVertex3f(-0.5, 0.5, 0.5);
		glVertex3f(0.5, 0.5, 0.5);
		glVertex3f(0.5, 0.5, -0.5);
		glVertex3f(-0.5, 0.5, -0.5);
		glEnd();

		glBegin(GL_LINES); //
		glVertex3f(-0.5, -0.5, 0.5);
		glVertex3f(-0.5, 0.5, 0.5);
		glVertex3f(0.5, -0.5, 0.5);
		glVertex3f(0.5, 0.5, 0.5);
		glVertex3f(0.5, -0.5, -0.5);
		glVertex3f(0.5, 0.5, -0.5);
		glEnd();

	}
	glPopMatrix();
}


void floorDelete(int y) //
{
	if (y > 10) return; //
	int x, z;
	for (x = 1; x < 6; x++)
	{
		for (z = 1; z < 6; z++)
		{
			if (arr[x][y][z] == 0)
			{
				floorDelete(y + 1); //
				return;
			}
		}
	}

	if (x == 6) //
	{
		for (x = 1; x < 6; x++) //
		{
			for (z = 1; z < 6; z++)
			{
				arr[x][y][z] = 0;
			}
		}
		//

		for (int i = y + 1; i < 11; i++)  //
		{
			int b = 0;
			for (x = 1; x < 6; x++)
			{
				for (z = 1; z < 6; z++)
				{
					arr[x][i - 1][z] = arr[x][i][z];
					arr[x][i][z] = 0;

				}
			}
			
		}
		glutPostRedisplay();

		floorDelete(y); //
	}
}

void resetBlock() //
{
	B_height = 10;
	B_x = 3, B_z = 3;

	switch (Next_choose) //
	{
	case 0: //
		x_1 = -1, y_1 = 0, z_1 = 0, x_2 = 0, y_2 = 0, z_2 = -1, x_3 = -1, y_3 = 0, z_3 = -1;
		Block_diffuse[0] = 1;
		Block_diffuse[1] = 0;
		Block_diffuse[2] = 0;
		break;
	case 1: //
		x_1 = -1, y_1 = 0, z_1 = 0, x_2 = 0, y_2 = 0, z_2 = -1, x_3 = 1, y_3 = 0, z_3 = -1;
		Block_diffuse[0] = 1;
		Block_diffuse[1] = 0.36f;
		Block_diffuse[2] = 0;
		break;
	case 2: //
		x_1 = 0, y_1 = 0, z_1 = 1, x_2 = 0, y_2 = 0, z_2 = -1, x_3 = 1, y_3 = 0, z_3 = -1;
		Block_diffuse[0] = 1;
		Block_diffuse[1] = 1;
		Block_diffuse[2] = 0;
		break;
	case 3: //
		x_1 = 0, y_1 = 0, z_1 = 1, x_2 = 0, y_2 = 0, z_2 = -1, x_3 = 1, y_3 = 0, z_3 = 0;
		Block_diffuse[0] = 0;
		Block_diffuse[1] = 1;
		Block_diffuse[2] = 0;
		break;
	case 4://
		x_1 = 0, y_1 = 0, z_1 = 1, x_2 = 0, y_2 = 0, z_2 = -1, x_3 = 0, y_3 = 0, z_3 = 2;
		Block_diffuse[0] = 0;
		Block_diffuse[1] = 0;
		Block_diffuse[2] = 1;
		break;
	}

	Next_choose = rand() % 5;
	glutPostRedisplay();
}

void RenderBlock() //
{
	drawBlock(B_x, B_height, B_z);
}

void drawBlock(int x, int y, int z) // 
{
	drawCube(x, y, z);
	drawCube(x + x_1, y + y_1, z + z_1);
	drawCube(x + x_2, y + y_2, z + z_2);
	drawCube(x + x_3, y + y_3, z + z_3);
}

void RenderNextB() //
{
	int nx_1, ny_1, nz_1, nx_2, ny_2, nz_2, nx_3, ny_3, nz_3;

	switch (Next_choose) //
	{
	case 0:
		nx_1 = -1, ny_1 = 0, nz_1 = 0, nx_2 = 0, ny_2 = 0, nz_2 = -1, nx_3 = -1, ny_3 = 0, nz_3 = -1; //
		Next_Block_diffuse[0] = 1;
		Next_Block_diffuse[1] = 0;
		Next_Block_diffuse[2] = 0;
		break;
	case 1:
		nx_1 = -1, ny_1 = 0, nz_1 = 0, nx_2 = 0, ny_2 = 0, nz_2 = -1, nx_3 = 1, ny_3 = 0, nz_3 = -1; //
		Next_Block_diffuse[0] = 1;
		Next_Block_diffuse[1] = 0.36f;
		Next_Block_diffuse[2] = 0;
		break;
	case 2:
		nx_1 = 0, ny_1 = 0, nz_1 = 1, nx_2 = 0, ny_2 = 0, nz_2 = -1, nx_3 = 1, ny_3 = 0, nz_3 = -1; //
		Next_Block_diffuse[0] = 1;
		Next_Block_diffuse[1] = 1;
		Next_Block_diffuse[2] = 0;
		break;
	case 3:
		nx_1 = 0, ny_1 = 0, nz_1 = 1, nx_2 = 0, ny_2 = 0, nz_2 = -1, nx_3 = 1, ny_3 = 0, nz_3 = 0; //
		Next_Block_diffuse[0] = 0;
		Next_Block_diffuse[1] = 1;
		Next_Block_diffuse[2] = 0;
		break;
	case 4:
		nx_1 = 0, ny_1 = 0, nz_1 = 1, nx_2 = 0, ny_2 = 0, nz_2 = -1, nx_3 = 0, ny_3 = 0, nz_3 = 2;//
		Next_Block_diffuse[0] = 0;
		Next_Block_diffuse[1] = 0;
		Next_Block_diffuse[2] = 1;
		break;
	}

	int x = -2, y = 10, z = 1;

	glMaterialfv(GL_FRONT, GL_DIFFUSE, Next_Block_diffuse);
	drawCube(x, y, z);
	drawCube(x + nx_1, y + ny_1, z + nz_1);
	drawCube(x + nx_2, y + ny_2, z + nz_2);
	drawCube(x + nx_3, y + ny_3, z + nz_3);
}

void Keyboard(unsigned char key, int x, int y) // 
{

	int tmp;
	switch (key)
	{
	case 'f': //
		FastDown = true;
		break;
		//
	case 'a':
		if (!CollisionBlock(B_x - 1, B_height, B_z))
			B_x -= 1;
		break;
	case 'd':
		if (!CollisionBlock(B_x + 1, B_height, B_z))
			B_x += 1;
		break;
	case 'w':
		if (!CollisionBlock(B_x, B_height, B_z - 1))
			B_z -= 1;
		break;
	case 's':
		if (!CollisionBlock(B_x, B_height, B_z + 1))
			B_z += 1;
		break;

		//
	case '1':

		if (!CollisionCube(B_x, B_height + z_1, B_z - y_1) && !CollisionCube(B_x, B_height + z_2, B_z - y_2) && !CollisionCube(B_x, B_height + z_3, B_z - y_3))
		{
			tmp = y_1; y_1 = z_1; z_1 = -1 * tmp;//
			tmp = y_2; y_2 = z_2; z_2 = -1 * tmp;
			tmp = y_3; y_3 = z_3; z_3 = -1 * tmp;
		}
		break;
	case '2':
		if (!CollisionCube(B_x - z_1, B_height, B_z + x_1) && !CollisionCube(B_x - z_2, B_height, B_z + x_2) && !CollisionCube(B_x - z_3, B_height, B_z + x_3))
		{
			tmp = x_1; x_1 = -1 * z_1; z_1 = tmp;//
			tmp = x_2; x_2 = -1 * z_2; z_2 = tmp;
			tmp = x_3; x_3 = -1 * z_3; z_3 = tmp;
		}
		break;
	case '3':
		if (!CollisionCube(B_x + y_1, B_height - x_1, B_z) && !CollisionCube(B_x + y_2, B_height - x_2, B_z) && !CollisionCube(B_x + y_3, B_height - x_3, B_z))
		{
			tmp = x_1; x_1 = y_1; y_1 = -1 * tmp;//
			tmp = x_2; x_2 = y_2; y_2 = -1 * tmp;
			tmp = x_3; x_3 = y_3; y_3 = -1 * tmp;
		}
		break;
		
	}
	glutPostRedisplay();
}
void res(int key, int x, int y)
{
	int i = 1;
	switch (key)
	{
	case GLUT_KEY_F2:
	
		myfile << "(score)--->" << score << endl;
		for (int x = 0; x < 7; x++)
		{
			for (int y = 0; y < 12; y++)
			{
				for (int z = 0; z < 7; z++)
				{
					arr[x][y][z] = 0;
				}
			}

		}

		/*for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < 7; j++) //
				arr[0][i][j] = 1;

			for (int j = 0; j < 7; j++) //
				arr[6][i][j] = 1;
		}

		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 11; j++) //
				arr[i][j][0] = 1;

			for (int j = 0; j < 11; j++) //
				arr[i][j][6] = 1;

			for (int j = 0; j < 7; j++) //
				arr[i][0][j] = 1;
		}*/

		MakeWall();
		RenderCubes();


		GameOver = false;//

		score = 0;

		glutTimerFunc(1000, Timer, 1); //
		glutTimerFunc(5, Timer, 2); //
		glutTimerFunc(100, Timer, 3);//

		break;
	
		case GLUT_KEY_F1:
		
			exit(0);
			break;
		
	}
	
}

void Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		StartPt[0] = x; StartPt[1] = y;
		if (button == GLUT_LEFT_BUTTON)
			ManipulateMode = 1;   // 

		if (button == GLUT_RIGHT_BUTTON)
			ManipulateMode = 2;   // 
	}
	if (state == GLUT_UP)
	{
		ManipulateMode = 0;   // 
		StartPt[0] = StartPt[1] = 0;
		Angle = 0.0;
	}
}

void MouseMove(int x, int y)
{
	if (ManipulateMode == 1) {   // 
		float px, py, pz, qx, qy, qz;
		GetSphereCoord(StartPt[0], StartPt[1], &px, &py, &pz);
		GetSphereCoord(x, y, &qx, &qy, &qz);

		//
		Axis[0] = py * qz - pz * qy; Axis[1] = pz * qx - px * qz; Axis[2] = px * qy - py * qx;
		Angle = 0.0;

		float len = Axis[0] * Axis[0] + Axis[1] * Axis[1] + Axis[2] * Axis[2];
		if (len > 0.0001) //
			Angle = acos(px * qx + py * qy + pz * qz) * 180.0f / 3.141592f;
	}
	if (ManipulateMode == 2) {   // 
		double dy = (StartPt[1]-y) * 0.1;
		Zoom += dy;
	}

	StartPt[0] = x;   // 
	StartPt[1] = y;
	glutPostRedisplay();
}

void GetSphereCoord(int x, int y, float* px, float* py, float* pz)
{
	*px = (2.0 * x - Width) / Width;
	*py = (-2.0 * y + Height) / Height;

	float r = (*px) * (*px) + (*py) * (*py);
	if (r >= 1.0)
	{
		*px = *px / sqrt(r);
		*py = *py / sqrt(r);
		*pz = 0.0;
	}
	else
		*pz = sqrt(1.0 - r);
}

void Reshape(int w, int h)
{
	//
	glViewport(0, 0, w, h);
	Width = w;
	Height = h;
}

void SetupViewTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 
	glTranslatef(0.0, 0.0, Zoom);

	// 
	glRotatef(Angle, Axis[0], Axis[1], Axis[2]);
	glMultMatrixf(RotMat);

	// 
	glGetFloatv(GL_MODELVIEW_MATRIX, RotMat);
	RotMat[12] = RotMat[13] = RotMat[14] = 0.0;
}

void SetupViewVolume()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)Width / (double)Height, 1.0, 10000.0);
}
