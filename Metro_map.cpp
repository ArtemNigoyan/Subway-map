#include <iostream>
#include "GL/glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h> //библиотека для работы со строками

//Координаты первой точки
float x1_ = -20.;
float y1_ = 0.;
//Координаты второй точки
float x2_ = 20.;
float y2_ = 0.;

GLint Width = 1350 , Height = 700; // Ширина и высота

struct station{ int number;    //порядковый номер станции
                char name[30];   //имя станции
                float x;       //координата x точки
                float y;       //координата y точки
                float color[3];//цвет линии: 1 - фиолетовый, 2- зеленый, 3 - желтая, 4 - голубая, 5 - красная
                float sign;      //поле отвечает за то, печатать название станции справа или слева от точки
                int length;
              };

//Количество станций
int n=67;

//Массив структур-станций
station massiv[67];

//Матрица смежности для станций
int *A;

std::string str; //объявление переменной на вывод

std::string to_string(int val) {
 char buff[32];
 sprintf(buff,"%d",val);
 return std::string(buff);
}

std::string sText()
{
	std::string sx1,sx2,sy1,sy2,sa,sb,sc;
	sx1 = to_string(x1_);
	sx2 = to_string(x2_);
	sy1 = to_string(y1_);
	sy2 = to_string(y2_);
	sa  = 'a';
	sb  = 'b';
	sc  = 'c';
	str = str.empty();
	str += sx1;
	str += ",";
	str += sx2;
	str += ",";
	str += sy1;
	str += ",";
	str += sy2;
	str += ";";
	str += sa;
	str += sb;
	str += sc;
	return str;
}

//Функция для печати текста на экран
void drawText(char *text, int length, float x, float y)
{
	glMatrixMode(GL_PROJECTION);
//	double *matrix = new double[16];
//	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0,400,0,400,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i=0; i<length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,(int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

//Прорисовка кадров
void display()
{
  //Устанавливаем цвет фона (белый)
  glClearColor(1.0, 1.0, 1.0, 1);
//  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//  glColor3f(1.0,0.0,1.0);
  //Установка размера точки
/*  glPointSize(10);
  glBegin(GL_POINTS);
  glColor3f(1.0,0.0,1.0);
  glVertex2f(x1_,y1_);
  glVertex2f(x2_,y2_);
  glEnd();
  str = sText(); */
  int i;
//Фиолетовая ветка
  glPointSize(10);
  glBegin(GL_LINE_STRIP);
  glColor3ub(128, 0, 128);
  for (i=0; i<12; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  for (i=11; i>=0; i--)
        glVertex2f(massiv[i].x+1, massiv[i].y);
  glVertex2f(massiv[0].x, massiv[0].y);
  glEnd();
//Зеленая ветка
  glPointSize(10);
  glBegin(GL_LINE_STRIP);
  glColor3ub(144, 238, 144);
  for (i=12; i<14; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  glVertex2f(55.0, 250.0);
  for (i=14; i<16; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  glVertex2f(230.0, 250.0);
  for (i=16; i<22; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  for (i=21; i>=16; i--)
        glVertex2f(massiv[i].x-1, massiv[i].y);
  glVertex2f(230.0-1.0, 250.0-1.0);
  for (i=15; i>=14; i--)
        glVertex2f(massiv[i].x, massiv[i].y-1);
  glVertex2f(55.0-1.0, 250.0-1.0);
  for (i=13; i>11; i--)
        glVertex2f(massiv[i].x-1, massiv[i].y);
  glVertex2f(massiv[12].x, massiv[12].y);
  glEnd();
//Желтая ветка
  glPointSize(10);
  glBegin(GL_LINE_STRIP);
  glColor3ub(255, 255, 0);
  for (i=22; i<25; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  glVertex2f(220.0, 220.0);
  for (i=25; i<30; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  for (i=29; i>24; i--)
        glVertex2f(massiv[i].x-1, massiv[i].y);
  glVertex2f(220.0, 220.0-1.0);
  for (i=24; i>21; i--)
        glVertex2f(massiv[i].x, massiv[i].y-1);
  glVertex2f(massiv[22].x, massiv[22].y);
  glEnd();
//Голубая ветка
  glPointSize(10);
  glBegin(GL_POLYGON);
  glColor3ub(141, 238, 238);
  for (i=30; i<48; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  for (i=47; i>29; i--)
        glVertex2f(massiv[i].x+1, massiv[i].y);
  glVertex2f(massiv[30].x, massiv[30].y);
  glEnd();
//Красная ветка
  glPointSize(10);
  glBegin(GL_LINE_STRIP);
  glColor3ub(238, 0, 0);
  for (i=48; i<67; i++)
        glVertex2f(massiv[i].x, massiv[i].y);
  for (i=66; i>47; i--)
        glVertex2f(massiv[i].x-1, massiv[i].y);
  glVertex2f(massiv[48].x, massiv[48].y);
  glEnd();

  for (i=0; i<n; i++)
  {
 //   printf("%s %f %f %d\n",massiv[i].name, massiv[i].x, massiv[i].y, massiv[i].number);
    glColor3ub(massiv[i].color[0],massiv[i].color[1],massiv[i].color[2]);
 //   glColor3f(0.0,0.0,0.0);
    printf("%d\n", massiv[i].length);
    if (massiv[i].sign==1.0) drawText(massiv[i].name,strlen(massiv[i].name),massiv[i].x+3*massiv[i].sign,massiv[i].y+2);
    else drawText(massiv[i].name,strlen(massiv[i].name),massiv[i].x+13*massiv[i].sign*(massiv[i].length)/5,massiv[i].y+2);
  }
//  drawText(str.data(),str.size(),100, 100);
  glutSwapBuffers();
}

void keyEvent(int key, int, int)
{
  switch (key)
  {
  case GLUT_KEY_LEFT:
    x1_ -= 5.;
	x2_ -= 5.;
    break;
  case GLUT_KEY_UP:
   y1_ += 5;
   y2_ += 5;
    break;
  case GLUT_KEY_RIGHT:
    x1_ += 5.;
	x2_ += 5.;
    break;
  case GLUT_KEY_DOWN:
   y1_ -= 5;
   y2_ -= 5;
   break;
}
}

void timer(int = 0)
{
  display();
  glutTimerFunc(10000000, timer, 0);
}

void Reshape(GLint w, GLint h) //меняем размеры окна
{
    Width = w;
    Height = h;
    glViewport(0, 0, w, h); //устанавливаем размеры области отображения
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //замена текущей матрицы на единичную
    glOrtho(-w/2, w/2, -h/2, h/2, -1.0, 1.0); //задает кубик в мировых координатах, на который мы смортим сверху
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Keyboard(unsigned char key, int cx, int cy)
{
    #define ESCAPE '\033'
    if (key == ESCAPE)
        exit(0);
    glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
    //if ((state==GLUT_DOWN) && (x>100)) exit(0);
}

//Чтение данных из файлов
void InitMap()
{
    int i=0;
    //Чтение матрицы смежности из файла input.txt
    FILE * fin1=fopen("input.txt", "r");
    A=(int *)malloc(n*n*sizeof(int));
    for (i=0; i<n*n; i++) fscanf(fin1,"%d",&A[i]);
    fclose(fin1);
    free(A);
    //Чтение всех станций из файла Metro+.txt
    FILE * fin2=fopen("Metro+.txt", "r");
    char c[30];
    for (i=0; i<n; i++)
    {
        fscanf(fin2,"%s",&c);
        int j;
        for (j=0; j<strlen(c); j++)
            massiv[i].name[j]=c[j];
        fscanf(fin2,"%f",&massiv[i].x);
        fscanf(fin2,"%f",&massiv[i].y);
        fscanf(fin2,"%d",&massiv[i].number);
//        printf("%s %f %f %d\n",massiv[i].name, massiv[i].x, massiv[i].y, massiv[i].number);
        if (i<12) //Фиолетовая ветка
        {
            massiv[i].color[0]=128;
            massiv[i].color[1]=0;
            massiv[i].color[2]=128;
        }
        else
        if (i<22) //Зеленая ветка
        {
            massiv[i].color[0]=144;
            massiv[i].color[1]=238;
            massiv[i].color[2]=144;
        }
        else
        if (i<30) //Желтая ветка
        {
            massiv[i].color[0]=255;
            massiv[i].color[1]=255;
            massiv[i].color[2]=0;
        }
        else
        if (i<48) //Голубая ветка
        {
            massiv[i].color[0]=141;
            massiv[i].color[1]=238;
            massiv[i].color[2]=238;
        }
        else
        if (i<67) //Красная ветка
        {
            massiv[i].color[0]=238;
            massiv[i].color[1]=0;
            massiv[i].color[2]=0;
        }


        if (((i>=0) && (i<7)) || (i==12) || (i==13) || ((i>=17) && (i<=21)) || (i==40) || ((i>=60) && (i<67)))
            massiv[i].sign=-1.0;
        else massiv[i].sign=1.0;
        massiv[i].length=strlen(massiv[i].name);
    }
    fclose(fin2);
}


int main(int argc, char **argv)
{
  InitMap();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(Width, Height);
  glutCreateWindow("Subway Map");
  glClearColor(0, 0, 0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-400, 400, -400, 400, -400, 400);
  glutDisplayFunc(display);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(keyEvent);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
 // timer();
  glutMainLoop();
}
