#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#define R 10
#define FOOD 1
#define PI 3.14
GLint winWidth=800;
GLint winHeight=600;
int id;
void init()
{
	glClearColor(1.0,1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,winWidth,0,winHeight);

}
void circle(float x, float y, float r) 
{ 
	float delta_theta = 0.01;
	glBegin(GL_POINTS);
		glVertex2i(24,33);
	glEnd();
	glBegin( GL_POLYGON ); // OR GL_LINE_LOOP

		for( float angle = 0; angle < 2*PI; angle += delta_theta )
		glVertex3f(x+ r*cos(angle), y+r*sin(angle), 0 );

	glEnd();
}
using namespace std;
class Color
{
	public :
		float r,g,b;
};
class Food
{
	public:
		Color color;
		int x,y;
		void Generate()
		{
			color.r=(float)((random()%4)/4.0);
			color.g=(float)((random()%4)/4.0);
			color.b=(float)((random()%4)/4.0);
			x=random()%winWidth;
			y=random()%winHeight;
		}
		Food()
		{
			color.r=(float)((random()%4)/4.0);
			color.g=(float)((random()%4)/4.0);
			color.b=(float)((random()%4)/4.0);
			x=random()%winWidth;
			y=random()%winHeight;
		}
}f[FOOD];
class Point
{
	public:
		int x,y;
		void set(int x,int y)
		{
			Point::x=x;
			Point::y=y;
		}
};
class Snake
{
	public:
		//char *name;
		char name[20];
		int score;
		int cords;  //length of snake
		Point list[20];
		Color color;
		int dir_x,dir_y;		
		void setCoordinates(int ,int);
		void Draw();
		void Update();
		friend ostream& operator<<(ostream &mycout,Snake s);
}snakes[5];
void Snake::setCoordinates(int x,int y)
{
	list[0].set(x,y);
	for(int i=0;i<10;i++)
	{
		x+=R*1.6*dir_x;
		y+=R*1.6*dir_y;
		list[i+1].set(x,y);
	}
	//dir_x=0;
	//dir_y=1;
	cords=11;
}
void Snake::Draw()
{
	for(int i=0;i<cords;i++)
		circle(list[i].x,list[i].y,R);
				
}
ostream &operator<<(ostream &mycout,Snake s)
{
	cout<<endl;
	cout<<"Name\t"<<s.name<<endl;
	cout<<"Score\t"<<s.score<<endl;
	cout<<"Cordinates\t"<<endl;
	for(int i=0;i<s.cords;i++)
		cout<<"("<<s.list[i].x<<","<<s.list[i].y<<"),";
	cout<<"Color\t"<<"("<<s.color.r<<","<<s.color.g<<","<<s.color.b<<")"<<endl;
	cout<<"Direction\t"<<s.dir_x<<","<<s.dir_y<<endl;
	return mycout;
}

void Snake::Update()
{
	for(int i=0;i<cords;i++)
	{
		list[i].x+=dir_x*R*1.6;
		list[i].y+=dir_y*R*1.6;
		if(list[i].x>=winWidth)
			list[i].x-=winWidth;
		else if(list[i].x<=0)
			list[i].x+=winWidth;
		if(list[i].y>=winHeight)
			list[i].y-=winHeight;
		else if(list[i].y<=0)
			list[i].y+=winHeight;
	}
}

int sockfd;
int len;
sockaddr_in address;
int result;
int count;

void getInitial()
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("127.0.0.1");
	address.sin_port=htons(53000);
	len=sizeof(address);

	result=connect(sockfd,(struct sockaddr *)&address,len);
	if(result==-1)
	{
		cout<<"Failed to connect"<<endl;
		exit(1);
	}
	read(sockfd,&id,sizeof(id));
	read(sockfd,&count,sizeof(int));
	cout<<"Number of clients = "<<count<<endl;
	for(int i=0;i<count;i++)
	{
		cout<<i<<endl;
		read(sockfd,snakes[i].name,sizeof(snakes[i].name));
		read(sockfd,&snakes[i].score,sizeof(snakes[i].score));
		read(sockfd,&snakes[i].color,sizeof(snakes[i].color));
		read(sockfd,&snakes[i].dir_x,sizeof(snakes[i].dir_x));
		read(sockfd,&snakes[i].dir_y,sizeof(snakes[i].dir_y));
		int c[2]; // initial coordinates
		read(sockfd,c,sizeof(c));
		snakes[i].setCoordinates(c[0],c[1]);
		cout<<snakes[i];
	}
	//close(sockfd);
}
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for(int i=0;i<count;i++)
	{
		glColor3f(snakes[i].color.r,snakes[i].color.g,snakes[i].color.b);
		snakes[i].Draw();
	}
	glColor3f(1,1,1);
	circle(10,10,10);
	glutSwapBuffers();
	glFlush();
}
void p1(GLint key,GLint x,GLint y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:	if(snakes[id].dir_x!=1)
						snakes[id].dir_x=-1;
					snakes[id].dir_y=0;
					break;
		case GLUT_KEY_RIGHT:	if(snakes[id].dir_x!=-1)
						snakes[id].dir_x=1;
					snakes[id].dir_y=0;
					break;
		case GLUT_KEY_UP:	if(snakes[id].dir_y!=-1)
						snakes[id].dir_y=1;
					snakes[id].dir_x=0;
					break;
		case GLUT_KEY_DOWN:	if(snakes[id].dir_y!=1)	
						snakes[id].dir_y=-1;
					snakes[id].dir_x=0;
	}
	int dir[2];
	dir[0]=snakes[id].dir_x;
	dir[1]=snakes[id].dir_y;
	cout<<dir[0]<<dir[1];
	write(sockfd,dir,sizeof(dir));
}
void repeat()
{
	int dir[2];
	for(int i=0;i<count;i++)
	{
		read(sockfd,dir,sizeof(dir));
		snakes[i].dir_x=dir[0];
		snakes[i].dir_y=dir[1];
	}
	for(int i=0;i<count;i++)
	{
		snakes[i].Update();
		
	}
	glColor3f(1,1,1);
	//display();
	//output(100,100,text);
	usleep(100000);
	draw();
}
void winReshapeFcn(GLint newWidth,GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	gluOrtho2D(0,winWidth,0,winHeight);
}
int main(int argc,char *argv[])
{
	glutInit(&argc,argv);	
	init();
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(winWidth,winHeight);

	
//	glutFullScreen();
	getInitial();//get snake details from server
	glutCreateWindow("Snake Game");
	glutDisplayFunc(draw);
	glutReshapeFunc(winReshapeFcn);
	glutSpecialFunc(p1);
	glutIdleFunc(repeat);	
	glutMainLoop();
}
