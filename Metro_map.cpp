#include <iostream>
#include "GL/glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h> //библиотека для работы со строками
#include <locale.h>
#include <dirent.h>
#include <GL/glaux.h>
AUX_RGBImageRec *pImage;

//Координаты первой точки
float x1_ = -20.;
float y1_ = 0.;
//Координаты второй точки
float x2_ = 20.;
float y2_ = 0.;

GLint Width = 1350 , Height = 700; // Ширина и высота

//Радиус круга-станции
float r=2.5; //было 2

//Количество станций
int n=67;

//Структура "Станция"
struct station{ int number;    //порядковый номер станции
                char name[30];   //имя станции
                float x;       //координата x точки
                float y;       //координата y точки
                float color[3];//цвет линии: 1 - фиолетовый, 2- зеленый, 3 - желтая, 4 - голубая, 5 - красная
                float sign;      //поле отвечает за то, печатать название станции справа или слева от точки
                int length;
              };

//Массив структур-станций
station massiv[67];

//Матрица смежности для станций
int *A;

//Структура "Лист графа"
struct node{node * prev;
            int data;
            node * next;
            node * neighbour;};

//Структура "Результирующий путь"
struct _rezult {int lengthmin;
                int nummin;
                int nummas;}
    typedef rezult;

struct _start{int num;    //Номер начальной станции
             int active; //Активность
            };

struct _finish{int num;    //Номер конечной станции
              int active; //Активность
             };

_start start;
_finish finish;
rezult out; //Результирующий путь
int find_way=0; //Искать путь или нет: 0 - не искать, 1 - искать

//Структура "Дерево путей"
struct tree{node * head;
            int ways; //количество возможных путей
            node **lasts; //количество последних элементов
            void init()
            {
                head=NULL;
                ways=0;
                lasts=NULL;
            };
            void add_head(int start)
            {
                node * n;
                n=(node *)malloc(sizeof(node));
                n->data=start;
                n->prev=NULL;
                n->next=NULL;
                n->neighbour=NULL;
                head=n;
             //   printf("Head done\n");
            }
            void add_next(int data, node * p)
            {
                node * t;
                t=(node *)malloc(sizeof(node));
                t->prev=p;
                t->data=data;
                t->neighbour=NULL;
                t->next=NULL;
                p->next=t;
            }
            void add_neighbour(int data, node * p)
            {
                node * t;
                t=(node *)malloc(sizeof(node));
                t->prev=p->prev;
                t->data=data;
                t->next=NULL;
                t->neighbour=NULL;
                p->neighbour=t;
            }
            int let (node * d, int n)
            {
                int numways=0;
                int numvis=0;
                int i;
                node * h;
                for (i=0; i<n; i++)
                {
                    if (A[(d->data)+i*n]>0) numways++;
                }
                h=d;
                while (h->prev!=NULL)
                {
                    h=h->prev;
                    if (A[(d->data)+(h->data)*n]>0) numvis++;
                }
                if (numvis>=numways) return 0; else return 1;
            }
            void mainfunc(node * now, int finish, int n)
            {
                //printf("%d %d\n", finish, n);
                int make=0;
                int i;
                int m=0;
                node * t;
                int repeat=1;
                for (i=0; i<n; i++)
                {
                    if (A[(now->data)+i*n]>0)
                    {
                       // printf("%d\n", A[(now->data)+i*n]);
                        m=1;
                        //printf("yea\n");
                        t=now;
                        repeat=0;
                        while (t->prev!=NULL)
                        {
                            if ((t->prev->data)==i) repeat=1;
                            t=t->prev;
                        }
                      //  printf("%d %d %d\n", now->data, i, repeat);
                        if (repeat==0)
                        {
                            make=1;
                            if (now->next==NULL)
                            {
                                add_next(i, now);
                               // printf("NEXT DONE:%d\n",now->next->data);
                            }
                            else
                            {
                                t=now->next;
                                while (t->neighbour!=NULL) t=t->neighbour;
                                add_neighbour(i, t);
                                //printf("NEIGHBOUR DONE:%d\n",t->neighbour->data);
                            }
                        }
                    }
                }
               // printf("NEXT %d\n",now->next->data);
                //printf("Finish %d\n", finish);
              //  printf("make=%d\n",make);      printf("mainfunc(next)\n");                     (now->next->data!=4)
               // printf("%d\n", let(now->next,n));
                if ((now->next->data!=finish) && (now->next!=NULL) && (let(now->next,n)==1)) {mainfunc(now->next, finish, n); };
                if ((now->next->data==finish) && (m==1))
                {
                    ways++;
                  //  printf("%d\n", ways);
                    lasts=(node **)realloc(lasts,ways*sizeof(node));
                    lasts[ways-1]=now->next;
                  //  printf("%d\n",lasts[ways-1]->data);
                }
                t=now->next->neighbour;
                while (t!=NULL)
                {
                    if ((t->data!=finish) && (let(t,n)==1)) mainfunc(t, finish, n);
                    if (t->data==finish)
                    {
                        ways++;
                        lasts=(node **)realloc(lasts,ways*sizeof(node));
                        lasts[ways-1]=t;
                     //   printf("%d\n",lasts[ways-1]->data);
                    }
                    t=t->neighbour;
                }
            }
            int * output; //указатель на массив, который следует вывести
            int makemassiv(int start) //получение массива на вывод
            {
                int i,j;
                int k;
                int number=0;
                node * t;
                for (i=0; i<ways; i++)
                {
                    k=1;
                    t=lasts[i];
                    while (t->prev!=NULL)
                    {
                        number++;
                        output=(int *)realloc(output,number*(sizeof(int)));
                        output[number-1]=t->data;
                        t=t->prev;
                        k++;
                    }
                    output=(int *)realloc(output,(number+1)*(sizeof(int)));
                    output[number]=start;
                    number++;
                //    printf("%d %d %d\n", k, number, k/2);
                    //меняем местами элементы массива
                    int m=number-k;;
                    for (j=number-1; j>=number-k/2; j--)
                    {
                        int x;
                        x=output[j]; output[j]=output[m]; output[m]=x;
                        m++;
                    }
                }
                return number;
            }
            void printways()
            {
                int i;
                node * t;
                for (i=0; i<ways; i++)
                {
                    t=lasts[i];
                    while (t->prev!=NULL)
                    {
                        printf("%d",t->data);
                        t=t->prev;
                    }
                    printf("\n");
                }
            }
           } tree1;

//Функция по очистке дерева путей
void clean_memory (node * head)
{
    node * t;
    t=head;
    if (t->neighbour!=NULL) clean_memory(t->neighbour);
    if (t->next!=NULL) clean_memory(t->next);
}

//Функция выбора лучшего пути
rezult best_way(struct tree * tree1, int start, int finish, int n)
{
   tree1->init();
   tree1->add_head(start);
   tree1->mainfunc(tree1->head,finish,n);
   //tree1->printways();
   int num; //количество элементов в массиве, содержащем все пути
   num=tree1->makemassiv(start);
   rezult output; //переменная для вывода
   int i;
   int numstart=0; //переменная для печати номера текущего (рассматриваемого) пути
   int length=0; //переменная для печати длины текущего массива
   int lengthmin; //минимальное расстояние
   int nummin=0; //номер начала минимальной комбинации
   int x; //номер начального элемента в текущей комбинации
/*   printf("All the ways:"); */
   for (i=0; i<num; i++)
   {
       if (tree1->output[i]==start)
       {
           x=i;
         //  printf("\n");
           numstart++;
         //  printf("Way %d: ",numstart);
           length=0;
       }
       if (tree1->output[i]!=finish)
       {
        //   printf("%d -> ", tree1->output[i]);
           if (tree1->output[i]!=start)
           length+=A[tree1->output[i-1]+n*tree1->output[i]];
       }
       else
       {
           length+=A[tree1->output[i-1]+n*tree1->output[i]];
           if (numstart==1)
           {
               lengthmin=length;
               nummin=0;
           }
           else if (lengthmin>length)
           {
               nummin=x;
               lengthmin=length;
           }
         //  printf("%d", tree1->output[i]);
         //  printf(" Length=%d",length);
       }
   }
   printf("\n\nThe length of the shortest path is %d",lengthmin);
   rezult out;
   int j=nummin;
   int k=1;
   while (tree1->output[j]!=finish)
   {
       j++; k++;
   }
   out.lengthmin=lengthmin;
   out.nummin=nummin;
   out.nummas=k;
   clean_memory(tree1->head);
   return out;
};

//Функция печати лучшего пути в консоль
void printbestway(tree * tree1, rezult out)
{
    int i;
    printf("\nBest way: ");
    for (i=out.nummin; i<out.nummin+out.nummas-1; i++)
        printf("%d -> ",tree1->output[i]);
    printf("%d", tree1->output[out.nummin+out.nummas-1]);
}

//Переменная для вывода на экран
std::string str;

//Функция преобразования в строку целого значения
std::string to_string(int val) {
 char buff[32];
 sprintf(buff,"%d",val);
 return std::string(buff);
}

/*
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
*/

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

int t=0;

//Соединение станций, расположенных вертикально
void draw_vertical (int i,int j)
{
        glBegin(GL_POLYGON);
        glVertex2f(massiv[i].x, massiv[i].y);
        glVertex2f(massiv[i].x-1, massiv[i].y);
        glVertex2f(massiv[j].x-1, massiv[j].y);
        glVertex2f(massiv[j].x, massiv[j].y);
        glEnd();
}

//Соединение станций, расположенных горизонтально
void draw_horizontal (int i, int j)
{
        glBegin(GL_POLYGON);
        glVertex2f(massiv[i].x, massiv[i].y);
        glVertex2f(massiv[i].x, massiv[i].y-1);
        glVertex2f(massiv[j].x, massiv[j].y-1);
        glVertex2f(massiv[j].x, massiv[j].y);
        glEnd();
}

void connect_two(int i, int j)
{
        // glColor3ub(255.0, 20.0, 147.0); //розовый
        glColor3ub(0.0, 0.0, 0.0);
        float ellipse=3.0;
        float homot=0.6;
        if (i==6)
        {
            ellipse*=1.62;
            homot=0.35;
        }
        float angle=0.0;
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(massiv[i].x-massiv[i].sign*0, (massiv[i].y+massiv[j].y)/2, 0);
        glBegin(GL_LINE_STRIP);
        while (angle<2*3.1416)
        {
            glVertex3f(homot*ellipse*cos(angle)-0.5, 2.8*ellipse*sin(angle), 0);
            angle+=0.1;
        }
        glPopMatrix();
        glEnd();
}

void draw_way(int j)
{
    if (j<out.nummin+out.nummas-1)
    {
        int f,g;
        f=tree1.output[j];
        g=tree1.output[j+1];
        if ((massiv[f].color[0]==massiv[g].color[0]) && (massiv[f].color[1]==massiv[g].color[1])  && (massiv[f].color[2]==massiv[g].color[2]))
            //Если станции принадлежат одной ветке, то
        {
            if (((f==13) && (g==14)) || ((g==13) && (f==14)))
            {
                if (g<f) f=g;
                glBegin(GL_POLYGON);
                glColor3ub(144, 238, 144);
                glVertex2f(massiv[f].x, massiv[f].y);
                glVertex2f(massiv[f].x-1, massiv[f].y);
                glVertex2f(55.0-1.0, 250.0);
                glVertex2f(55.0, 250.0);
                glEnd();
                glBegin(GL_POLYGON);
                glVertex2f(54.0, 250.0);
                glVertex2f(54.0, 250.0-1);
                glVertex2f(massiv[f+1].x, massiv[f+1].y-1);
                glVertex2f(massiv[f+1].x, massiv[f+1].y);
                glEnd();
            }
            else
            if (((f==15) && (g==16)) || ((f==16) && (g==15)))
            {
                if (g<f) f=g;
                glBegin(GL_POLYGON);
                glVertex2f(230.0, 250.0);
                glVertex2f(230.0, 250.0-1);
                glVertex2f(massiv[f].x, massiv[f].y-1);
                glVertex2f(massiv[f].x, massiv[f].y);
                glEnd();
                glBegin(GL_POLYGON);
                glVertex2f(massiv[f+1].x, massiv[f+1].y);
                glVertex2f(massiv[f+1].x-1, massiv[f+1].y);
                glVertex2f(230.0-1.0, 250.0);
                glVertex2f(230.0, 250.0);
                glEnd();
            }
            else
            if (((f==24) && (g==25)) || ((f==25) && (g==24)))
            {
                if (g<f) f=g;
                glBegin(GL_POLYGON);
                glVertex2f(massiv[f].x, massiv[f].y);
                glVertex2f(massiv[f].x, massiv[f].y-1);
                glVertex2f(220.0, 220.0-1);
                glVertex2f(220.0, 220.0);
                glEnd();
                f++;
                glBegin(GL_POLYGON);
                glVertex2f(220.0, 220.0);
                glVertex2f(220.0, 220.0-1);
                glVertex2f(massiv[f].x, massiv[f].y-1);
                glVertex2f(massiv[f].x, massiv[f].y);
                glEnd();
            }
            else
            if (massiv[f].x==massiv[g].x)
                draw_vertical(f,g);
            else
                draw_horizontal(f,g);
        }
        else //Если станции не принадлежат одной ветке, то
        {
            connect_two(g,f);
        }
    }
}

void draw_station(int f)
{
        float angle1=0.0;
        glPushMatrix();
        glLoadIdentity();
        glColor3ub(massiv[f].color[0], massiv[f].color[1], massiv[f].color[2]);
        glTranslatef(massiv[f].x-massiv[f].sign*0, massiv[f].y, 0);
        glBegin(GL_POLYGON);
        while (angle1<2*3.1416)
        {
            glVertex3f((r-0.9)*cos(angle1)-0.5, r*sin(angle1), 0);
            angle1+=0.001;
        }
        glPopMatrix();
        glEnd();
        if (massiv[f].sign==1.0)
        {
            if ((f==23) || (f==24) || (f==14) || (f==15)) drawText(massiv[f].name,strlen(massiv[f].name),massiv[f].x+1,massiv[f].y-8);
            else if (f==22) drawText(massiv[f].name,strlen(massiv[f].name),massiv[f].x+2,massiv[f].y+3);
            else
            drawText(massiv[f].name,strlen(massiv[f].name),massiv[f].x+3*massiv[f].sign,massiv[f].y);
        }
        else drawText(massiv[f].name,strlen(massiv[f].name),massiv[f].x+13*massiv[f].sign*(massiv[f].length)/5,massiv[f].y);
    }

//Прорисовка кадров
void display()
{
  //Устанавливаем цвет фона (белый)
  glClearColor(1.0, 1.0, 1.0, 1);
//  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3ub(0,0,0);
  //Выводим начальную и конечную станции на экран
  drawText("FROM:", 5, 320.0, 370.0);
  drawText("TO:", 3, 324.8, 360.0);
  if (start.active==1)
      {
        glColor3ub(massiv[start.num].color[0],massiv[start.num].color[1], massiv[start.num].color[2]);
        drawText(massiv[start.num].name, strlen(massiv[start.num].name), 332.0, 370.0);
      }
  if (finish.active==1)
      {
        glColor3ub(massiv[finish.num].color[0],massiv[finish.num].color[1], massiv[finish.num].color[2]);
        drawText(massiv[finish.num].name, strlen(massiv[finish.num].name), 332.0, 360.0);
      }
  if ((start.active==1) && (finish.active==1) && (start.num!=finish.num))
  {
      glColor3ub(0,0,0);
      drawText("Best way is done for You!", 25, 300.0, 340.0);
  }

  //Запуск алгоритма по поиску наиличшего пути
  if (!((start.num==0) && (finish.num==0)) && (finish.active==1) && (start.num!=finish.num))
  {
     find_way=0;
     out=best_way(&tree1, start.num, finish.num, n);
     printbestway(&tree1, out); //печать лучшего пути
  }
  int j;
  int f;
  int g;
//Вывод пути на экран

/*
  for (j=out.nummin; j<out.nummin+out.nummas; j++)
  {
    f=tree1.output[j];
    glColor3ub(massiv[f].color[0],massiv[f].color[1],massiv[f].color[2]);
    drawText(massiv[f].name, strlen(massiv[f].name), 300.0,330.0-10.0*j);
  }
*/
  glColor3ub(0,0,0);
  if ((start.num!=finish.num) && (finish.active==1))
  {
    drawText("Time of your drive: ", 20, 300.0, 320.0);
    str=str.empty();
    std::string sx1;
    sx1=to_string(out.lengthmin);
    str+=sx1;
    drawText((char*)str.data(), str.size(), 345.0, 320.0);
    drawText("minutes", 7, 352.0, 320.0);
  }
  int i;

  //Запуск прорисовки всей карты при отсутствии точек начала и конца
  if (!((start.active==1) && (finish.active==1)) || (finish.num==start.num))
{
//Прорисовка всей карты метро
{
    //Фиолетовая ветка
  for (i=0; i<11; i++)
  {
     glColor3ub(128, 0, 128);
     draw_vertical(i,i+1);
  }
    //Зеленая ветка
  for (i=12; i<13; i++)
  {
    glColor3ub(144, 238, 144);
    draw_vertical(i,i+1);
  }
  glBegin(GL_POLYGON);
        glColor3ub(144, 238, 144);
        glVertex2f(massiv[i-1].x, massiv[i-1].y);
        glVertex2f(massiv[i-1].x-1, massiv[i-1].y);
        glVertex2f(55.0-1.0, 250.0);
        glVertex2f(55.0, 250.0);
  glEnd();
  glBegin(GL_POLYGON);
        glVertex2f(54.0, 250.0);
        glVertex2f(54.0, 250.0-1);
        glVertex2f(massiv[i+1].x, massiv[i+1].y-1);
        glVertex2f(massiv[i+1].x, massiv[i+1].y);
  glEnd();
  for (i=14; i<15; i++)
  {
        glColor3ub(144, 238, 144);
        draw_horizontal(i,i+1);
  }
  glBegin(GL_POLYGON);
        glVertex2f(230.0, 250.0);
        glVertex2f(230.0, 250.0-1);
        glVertex2f(massiv[i-1].x, massiv[i-1].y-1);
        glVertex2f(massiv[i-1].x, massiv[i-1].y);
  glEnd();
  glBegin(GL_POLYGON);
        glVertex2f(massiv[i+1].x, massiv[i+1].y);
        glVertex2f(massiv[i+1].x-1, massiv[i+1].y);
        glVertex2f(230.0-1.0, 250.0);
        glVertex2f(230.0, 250.0);
  glEnd();
  for (i=16; i<21; i++)
  {
        glColor3ub(144, 238, 144);
        draw_vertical(i,i+1);
  }
//Желтая ветка
  for (i=22; i<24; i++)
{
    glColor3ub(255, 193, 37);
    draw_horizontal(i,i+1);
}
    glBegin(GL_POLYGON);
        glVertex2f(massiv[i].x, massiv[i].y);
        glVertex2f(massiv[i].x, massiv[i].y-1);
        glVertex2f(220.0, 220.0-1);
        glVertex2f(220.0, 220.0);
    glEnd();
    i++;
    glBegin(GL_POLYGON);
        glVertex2f(220.0, 220.0);
        glVertex2f(220.0, 220.0-1);
        glVertex2f(massiv[i].x, massiv[i].y-1);
        glVertex2f(massiv[i].x, massiv[i].y);
    glEnd();
  for (i=25; i<29; i++)
{
    glColor3ub(255, 193, 37);
    draw_vertical(i,i+1);
}
//Голубая ветка
  for (i=30; i<47; i++)
  {
    glColor3ub(145, 44, 238);
    draw_vertical(i,i+1);
  }
//Красная ветка
  for (i=48; i<66; i++)
  {
    glColor3ub(238, 0, 0);
    draw_vertical(i,i+1);
  }
}
//Радиус
//  float r=2.5; //было 2
//Рисование кругов-станций
  for (i=0; i<n; i++)
  {
    glColor3ub(massiv[i].color[0],massiv[i].color[1],massiv[i].color[2]);
    float angle=0.0;
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(massiv[i].x-massiv[i].sign*0, massiv[i].y, 0);
    glBegin(GL_POLYGON);
    while (angle<2*3.1416)
    {
        glVertex3f((r-0.9)*cos(angle)-0.5, r*sin(angle), 0);
        angle+=0.001;
    }
    glPopMatrix();
    glEnd();

    connect_two(14,38); //Невский+Гостиный
    connect_two(15,57); //Восстания+Маяковская
    connect_two(40,60); //Техно+Техно
    connect_two(23,58); //Владимирская+Достоевская
    connect_two(16,25); //Невского+Невского
    connect_two(59, 7); //Пушкинская+Звенигородская
    connect_two(6,39); //Соединяем тройку Сенная+Садовая+Спасская

    glColor3ub(massiv[i].color[0],massiv[i].color[1],massiv[i].color[2]);
    if (massiv[i].sign==1.0)
    {
        if ((i==23) || (i==24) || (i==14) || (i==15)) drawText(massiv[i].name,strlen(massiv[i].name),massiv[i].x+1,massiv[i].y-8);
        else if (i==22) drawText(massiv[i].name,strlen(massiv[i].name),massiv[i].x+2,massiv[i].y+3);
        else
        drawText(massiv[i].name,strlen(massiv[i].name),massiv[i].x+3*massiv[i].sign,massiv[i].y);
    }
    else drawText(massiv[i].name,strlen(massiv[i].name),massiv[i].x+13*massiv[i].sign*(massiv[i].length)/5,massiv[i].y);
  }
}

//Учитываем, что нужно теперь рисовать на экране только те станции, которые входят в путь
  else
  {
      int change=0;
      glColor3ub(massiv[start.num].color[0],massiv[start.num].color[1],massiv[start.num].color[2]);
      drawText(massiv[start.num].name, strlen(massiv[start.num].name), 330.0,300.0-25.0*change);
      change++;
      for (j=out.nummin; j<out.nummin+out.nummas; j++)
      {
            f=tree1.output[j];
            //Печать пути справа - Здесь ошибка
            if (j<out.nummin+out.nummas-1)
            {
                g=tree1.output[j+1];
                if (!((massiv[f].color[0]==massiv[g].color[0]) && (massiv[f].color[1]==massiv[g].color[1]) && (massiv[f].color[2]==massiv[g].color[2])))
            {
                glColor3ub(massiv[f].color[0],massiv[f].color[1],massiv[f].color[2]);
                drawText(massiv[f].name, strlen(massiv[f].name), 330.0,300.0-25.0*change);
                change++;
                glColor3ub(massiv[g].color[0],massiv[g].color[1],massiv[g].color[2]);
                drawText(massiv[g].name, strlen(massiv[g].name), 330.0,300.0-25.0*change);
                change++;
            }
       }
       //Рисование кругов-станций
       draw_station(f);
       //Соеднинение станций
       draw_way(j);
       }
            glColor3ub(massiv[finish.num].color[0],massiv[finish.num].color[1],massiv[finish.num].color[2]);
            drawText(massiv[finish.num].name, strlen(massiv[finish.num].name), 330.0,300.0-25.0*change);
            change++;
       }
  glutSwapBuffers();
}

void timer(int = 0)
{
  display();
  glutTimerFunc(1, timer, 0);
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
    {
        free(A);
        exit(0);
    }
    glutPostRedisplay();
}

void no_activity()
{
    start.active=0;
    start.num=0;
    finish.active=0;
    finish.num=0;
}

void activation(int i)
{
     start.active=1; //было 0
     finish.active=1;
     find_way=1;
     finish.num=i;
}

void make_start(int i)
{
    start.num=i;
    start.active=1;
}

void Mouse(int button, int state, int x, int y)
{
    int a=0; //проверка нахождения точек начала и конца
    int i;
    if (state==GLUT_DOWN)
    {
        for (i=0; i<n; i++)
        {
            if (sqrt((x-3.404*massiv[i].x)*(x-3.404*massiv[i].x)+(y-(52+(370-massiv[i].y)*1.7666666666667))*(y-(52+(370-massiv[i].y)*1.7666666666667)))<=10)
            {
                a=1;
                if (start.active==0)
                    make_start(i);
                else activation(i);
            }
        }
        if (a==0)
            no_activity();
        printf("%d %d\n", start.num, finish.num);
    }
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
    //Чтение всех станций из файла Metro+.txt
    FILE * fin2=fopen("Metro+.txt", "r");
    char c[30];
    for (i=0; i<n; i++)
    {
        fscanf(fin2,"%s",&c);
        int j;
        for (j=0; j<strlen(c); j++)
        {
            if (c[j]=='_') c[j]=' ';
            massiv[i].name[j]=c[j];
        }
        fscanf(fin2,"%f",&massiv[i].x);
        fscanf(fin2,"%f",&massiv[i].y);
        fscanf(fin2,"%d",&massiv[i].number);
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
            massiv[i].color[1]=193;
            massiv[i].color[2]=37;
        }
        else
        if (i<48) //Голубая ветка
        {
            massiv[i].color[0]=145;
            massiv[i].color[1]=44;
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
    start.num=0;
    finish.num=0;
    fclose(fin2);
}

int main(int argc, char **argv)
{
  InitMap();
  glutInit(&argc, argv);
  setlocale(LC_ALL, "Rus"); //Не работает
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(Width, Height);
  glutCreateWindow("Subway Map");
  glClearColor(0, 0, 0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-400, 400, -400, 400, -400, 400);
  glutDisplayFunc(display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
  timer();
  glutMainLoop();
}
