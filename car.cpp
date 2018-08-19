// Это приложение рисует в окне консоли "автомобиль" по нажатию клавиши вверх запускается анимация
#include <Windows.h>
#include <conio.h>
#include <cmath>
#define PI 3.1415
using namespace std;

int  main () {
	
	int cX,cY, x, y;
	long i;
	double r, alpha, mn, xx, yy; // xx, yy просто так. точнее yy для задержки. а xx вообще нигде.
	char t;
	RECT rc;
	HWND hw = GetConsoleWindow();
	HDC hd = GetDC(hw);
	
	// Набор кистей
	HPEN pn = CreatePen(PS_SOLID,1,RGB(128,64,0)); //коричневый
	HPEN bpn = CreatePen(PS_SOLID,1,RGB(0,0,0)); //черный
	//заполнения цвет.
	HBRUSH hb = CreateSolidBrush(RGB(0,255,0));
	SelectObject(hd,hb);
	// узнаем размеры окна и координаты центра
	GetClientRect(hw,&rc);
	cX=rc.right/2;
	cY=rc.bottom/2;	

	//Трапеция:
	SelectObject(hd,pn);
	MoveToEx(hd, cX-10, cY-50,NULL);
	LineTo(hd, cX-100,cY+50);
	LineTo(hd, cX+180, cY+50);
	LineTo(hd, cX+90, cY-50);
	LineTo(hd, cX-10, cY-50);

	//Прямоугольник
	DeleteObject(pn);
	pn = CreatePen(PS_SOLID,1,RGB(0,255,0));
	SelectObject(hd,pn);
	Rectangle(hd,cX-100,cY+50, cX+180, cY+100);

	// Колёса 
	DeleteObject(pn);
	pn = CreatePen(PS_SOLID,1,RGB(192,192,192));
	SelectObject(hd,pn);
	DeleteObject(hb);
	hb = CreateSolidBrush(RGB(192,192,192));
	SelectObject(hd,hb);

	Rectangle(hd, cX-80, cY+100,
		cX-40, cY+140);
	Rectangle(hd, cX+120, cY+100,
		cX+160, cY+140);

	//фары
	DeleteObject(pn);
	pn = CreatePen(PS_SOLID,1,RGB(255,255,255));
	SelectObject(hd,pn);
	DeleteObject(hb);
	hb = CreateSolidBrush(RGB(255,255,255));
	SelectObject(hd,hb);

	Ellipse(hd, cX-65, cY+65,
		cX-45, cY+85);
	Ellipse(hd, cX+125, cY+65,
		cX+145, cY+85);
	//движущаяся часть
	r= 80;
	alpha = PI/12;
	x=r*cos(alpha);
	y=r*sin(alpha);
	//+40 +50
	cX+=40;  cY+=50;
	MoveToEx(hd,cX,cY,NULL);
	LineTo(hd,cX+x,cY-y);
	while(1) {
	      t = (char) getch();
	      if(t==27) break;
	      if(t==72) {  // ­ ¦ в  Є­®ЇЄ  ўўҐpе
			  while(1) {
				if(alpha <= PI/12)
					mn=+PI/24;
				if(alpha >= 11*PI/12)
					mn=-PI/24;

				  for(i =0,yy=0; i<1000000;i++) yy+=sin((double)i)/cos(double(i+3));

				  if(kbhit())  t=getch();
				  if(!t) t=getch();
				  if(t==27) {
					  DeleteObject(hb);
					  DeleteObject(pn);
			          DeleteObject(bpn);
					  return 0;
				  }
				  if(t==80) break;

				  MoveToEx(hd,cX,cY,NULL);
				  SelectObject(hd,bpn);
				  LineTo(hd,cX+x,cY-y);
				  SelectObject(hd,pn);
				  alpha+=mn;
				  x=r*cos(alpha);
				  y=r*sin(alpha);
				  MoveToEx(hd,cX,cY,NULL);
				  LineTo(hd,cX+x,cY-y);
		      }
	      }
	}
	DeleteObject(hb);
	DeleteObject(pn);
	DeleteObject(bpn);
	return 0;
}