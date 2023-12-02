//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <vector>
#include <fstream>

#include "Unit1.h"
#include "File1.h"
#include "PAintList.h"
#include "cMath.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int i,j;//�������� ��� �����
int gx2=650, gy2=350, xs=50, ys=50;  //��������� ���������� �����
int gx, gy; //
int id; // ������������� ������� ������
int vv = 0; //��������� ��� �����������
int cx, cy; // ��� ���������.
Round coord; //�����
float I, U, R;
int ses = 1, resis = 1, conds = 1; // ��� ���������
int or = 0; //��� ���������� ��������

Nodes *NCurr, *NHead;
bool test (int x, int y)
{                                                        //�������� ����� �������
	if (Form1->Canvas->Pixels[x][y]==clBlack)
		return true;
	else return false;
};
/*
int testnodes (int x, int y )
{
	int t = 0;
	if ( test(1+50*x, 50*y) )  t++;
	if ( test(50*x-1, 50*y) )  t++;
	if ( test(50*x, 1+50*y) )  t++;
	if ( test(50*x, 50*y-1) )  t++;
	return t;
}
*/

void AddNode (int x, int y, int num)     {
	Nodes *temp = new Nodes;
	temp->next = NULL;
	temp->xCoord = x;
	temp->yCoord = y;
	temp->number = num;
	NCurr->next=temp;
	NCurr=temp;
}
//�������� ��������
void  DelElement(int x, int y, int o)
{
	int t = 0;
	PaintList *pl = PLHead;
	PaintList *PL = PLHead->next;
	while (PL!=NULL){
		if (PL->xCoord==x && PL->yCoord==y && PL->or==o) {
			if ( PL->next == NULL) PLCurr = pl;
			pl->next = PL->next;
			delete PL;
			Form1->Objects->Items->Delete(t);
			break;
		}
		PL=PL->next;
		pl=pl->next;
		t++;
	}

}

//��������� ������ ��������� ������ �������� - ��������� ��� ������
int testnodes (int x, int y, int& napr) {   
	//��� ������� ������� ����������:) ���� napr = ����� ������:)
		//�������� ������������ ������:) ������������ ObrNapr, � napr �������� ����� �����������, ���� t<3 ����� 1
	int t = 0;
	if ( test(1+50*x, 50*y) ) t++;
	if ( test(50*x-1, 50*y) ) t++;
	if ( test(50*x, 1+50*y) ) t++;
	if ( test(50*x, 50*y-1) ) t++;
	if (napr!=-1) {
		int ObrNapr;
		if (napr%2==1) ObrNapr = napr+1; // ������� �������� �����������
		else ObrNapr = napr-1;
		if (t<=3)	{
			if (test(1+x, y) && ObrNapr!=1)
				napr = 1;
			if ( test(x-1, y) && ObrNapr!=2)
				napr = 2;
			if ( test(x, 1+y) && ObrNapr!=3)
				napr = 3;
			if (test(x, y-1)&& ObrNapr!=4)
				napr = 4;
		}
	}
	return t;


	// if (napr !=-1) - ��� ���:)? ��� ������ ����� �����������
	// � ����� ��� ���� ���� � �� �� ���������). ��� � ������ ������� ���� ��������� �����) � � �� ��� ������� ObrNapr �������
	// � ������, ������ � ������ ���� + 50*x � � �, � � ������ 1+x? � ������ ���� ���� ��������� :).
	// � ����������, ��� �� �������� ����� ��������: ��������, ����� �� ��������� � �����-�� �������. ���� ����� � ��� �� ObrNapr, �� napr = ��� �����������
	// � ��� �� ���� ������� ����. ���� ����������� ����� ���, �� ������ t = 2 (���� �� ������������) �  napr = ������ ��� �����������
	// ����  t >2 (������� ������ �� �����), �� napr ����� ��������� �� ����������� ���� �����������, � ������� ����� ���������, �� ��� ��� �������:). 
	// ����� t>2 ��� ������� �������� napr, ������ ��� �� ������� �� ����� � ���������� �����:).
	}

Branch SearchBranch(int napr, int number, int x, int y)	{
	// �������� ������
	Branch b;
	b.R = 0;
	b.E = 0;
	b.NodeNum1=number;

	Nodes *NL = NHead->next;
	PaintList *PL = PLHead->next;

	//����� ������ ��������:) ������ ��� � ��� ����� � ������ �������� �������������� � ����� do while. ����� �������� ������ �������� �� ��, ���
	//����� ������ � ���� ����������� ����.

	if (napr < 3 && test(x-pow(-1.0,napr), y))
	   x-=pow(-1.0,napr)*50;
	else if (napr >2 && test(x, y-pow(-1.0,napr)))
	   y-=pow(-1.0,napr)*50;
	else {
		b.NodeNum2=number;
		return b;
	}
	
	do
	{
		while (PL!=NULL) {
			// ���� �������� �������� �� ���������,�����������, ���� ������ �� ���������� � ����� ����:)
			// �� � ���������� � R � � E. �����-���-���).
			if ((PL->xCoord==x) && (PL->yCoord==y) && ((napr <=2 && PL->type==1) || ( napr>=3 && PL->type==4) ))
			{
					b.R+=PL->R;
					Application->MessageBox("������� ����� ��������:) ","������",MB_OK);
			}
			if ((PL->xCoord==x) && (PL->yCoord==y) &&  ((napr <=2 && PL->type==2) || ( napr>=3 && PL->type==5) )) {
					b.E+=PL->U;
					Application->MessageBox("������� ����� ���:) ","������",MB_OK);
			}
			PL=PL->next;
		}
		
		//����� �������� x, y �� ����� (��������� ������������)
		if (napr < 3)
		   x-=pow(-1.0,napr)*50;
		else if (napr >2)
		   y-=pow(-1.0,napr)*50;
	
	}
	while (testnodes(x,y, napr) < 3) ; // ����� ������ �����������

	while (NL!=NULL)  {
		if (NL->xCoord==x && NL->yCoord==y)
		{
			b.NodeNum2=NL->number;
			break;
		}
		NL = NL->next;
	}
		return b;
}

/*
Branch SearchBranch(int napr, int number, int x, int y)
{

	Branch b;
	b.R = 0;
	b.E = 0;
	b.NodeNum2 = 0;
	b.NodeNum1=number;
	Nodes *n = NHead->next;
	PaintList *PL = PLHead->next;
	while (PL!=NULL) {
		if ((PL->xCoord==x) && (PL->yCoord==y) && ((napr <=2 && PL->type==1) || ( napr>=3 && PL->type==4) ))
		  {
				b.R+=PL->R;
				Application->MessageBox("������� ����� ��������:) ","������",MB_OK);
			 }
		if ((PL->xCoord==x) && (PL->yCoord==y) && ((napr <=2 && PL->type==2) || ( napr>=3 && PL->type==5) )) {
		   {
			if (PL->or==1)
				b.E+=PL->U;
			else b.E-=PL->U;
					Application->MessageBox("������� ����� ���:) ","������",MB_OK);
		   }
		}
		PL=PL->next;
	}
	if (napr < 3 && test(x-pow(-1.0,napr), y))
	   x-=pow(-1.0,napr)*50;
	else if (napr >2 && test(x, y-pow(-1.0,napr)))
	   y-=pow(-1.0,napr)*50;
	else {
		b.NodeNum2=number;
		return b;
	}

		Application->MessageBox("������. ���� ������:) ","������",MB_OK);
	int WrongNapr; //��� ����, ����� �� ����� �������� �����������. � ��� ���������� � ���, ��
	if (napr%2==1) WrongNapr = napr+1;
	else WrongNapr = napr-1;
	int NewNapr = 0;
	do
	{
		int t = testnodes(x,y); //�������
		NewNapr = 0;
		if (t<3) {
			if (test(1+x, y) && WrongNapr!=1)
				NewNapr = 1;
			if ( test(x-1, y) && WrongNapr!=2)
				NewNapr = 2;
			if ( test(x, 1+y) && WrongNapr!=3)
				NewNapr = 3;
			if (test(x, y-1)&& WrongNapr!=4)
				NewNapr = 4;
		}
		else {
				NewNapr=-1;
				while (n!=NULL)  {
					if (n->xCoord==x && n->yCoord==y)
					   {
						b.NodeNum2=n->number;
						break;
					   }
					n = n->next;
				}
		}
			if ( test(1+x, y) && t == 0 && on!=1)  { t++; m = 1; }
			else m=-1;
			if ( test(x-1, y) && t == 0 && on!=2)  { t++; m = 2; }
			else m=-1;
			if ( test(x, 1+y) && t == 0 && on!=3)  { t++; m = 3; }
			else m=-1;
			if ( test(x, y-1) && t == 0 && on!=4)  { t++; m = 4;}
			else m=-1;


			while (PL!=NULL) {
				if (PL->xCoord==x && PL->yCoord==y && ((napr < 3 && PL->type==1) || ( napr>2 && PL->type==4) ))
					b.R+=PL->R;
				if (PL->xCoord==x && PL->yCoord==y && ((napr < 3 && PL->type==2) || ( napr>2 && PL->type==5) )) {
					if (PL->or==1)
						b.E+=PL->U;
					else b.E-=PL->U;
				}
			PL=PL->next;
			}

			switch (NewNapr) {
			case 1:                                        //��������� �����������
				x+=50;                                     // � ������ ����������
			break;                                         //����� �������
			case 2:
				x-=50;
			break;
			case 3:
				y+=50;
			break;
			case 4:
				y-=50;
			break;
			default:
			break;
			}
			}
		while (NewNapr!=-1);
		Application->MessageBox("������ ��������� � ������:) ","������",MB_OK);

	return b;

}
*/
void ClearPaintList()
{
		PaintList *PL = PLHead->next;
		PaintList *pl = PLHead;
		int t = 0;
			while (PL!=NULL){
				pl->next = PL->next;
				delete PL;
				Form1->Objects->Items->Delete(t);
				PL=pl->next;
				t++;
			}
			PLCurr = PLHead;

}

	void MouseL()
	{
		Form1->Add->Visible=true;
		Form1->Add->Enabled=true;
		Form1->Del1->Enabled=true;
		Form1->Del2->Enabled=true;
		Form1->Del1->Checked = false;
		Form1->Del2->Checked = false;
		Form1->DelPar->Visible=false;
		Form1->AddPar->Visible=true;
		Form1->Delete->Visible=false;

	}
	void Standing()                         //���������� ��� �������� �����
	{
		Form1->DelPar->Visible=false;
		Form1->AddPar->Visible=true;
		Form1->Rr->Text="";
		Form1->Ii->Text="";
		Form1->Uu->Text="";
		Form1->Rlabel->Visible=false;
		Form1->Ulabel->Visible=false;
		Form1->Ilabel->Visible=false;
		Form1->Rr->Visible=false;
		Form1->Uu->Visible=false;
		Form1->Ii->Visible=false;
		Form1->Route->Visible=false;
		Form1->Add->Enabled=false;
		Form1->Rb1->Checked=false;
		Form1->Rb2->Checked=false;
		Form1->Add->Enabled=false;
		Form1->DelPar->Visible=false;
	}

//������,���� ���������, �������.
void PaintChainEl(int x, int y, int id, int o, int num)
{
	Form1->Canvas->Pen->Color=clBlack;
	Form1->Canvas->Brush->Color=clWhite;
	Form1->Canvas->MoveTo(x,y);
	Form1->Canvas->Font->Size=6;

	switch (id) {
	//������ ����������
	case 0:	Form1->Canvas->LineTo(x+50,y); break;
	case 3: Form1->Canvas->LineTo(x,y+50); break;
	//������ ���������
	case 1:
			Form1->Canvas->LineTo(x+50, y);               			//������
			Form1->Canvas->Rectangle(x+12,y-10, x+38,y+10);
			Form1->Canvas->TextOutA(x+20, y-5, "R"+IntToStr(num));
	break;
	case 4:
			Form1->Canvas->LineTo(x, y+50);
			Form1->Canvas->Rectangle(x-10,y+12,x+10,y+38);
			Form1->Canvas->TextOutA(x-5, y+20, "R"+IntToStr(num));
	break;
		//�����v ���
	case 2:
			Form1->Canvas->LineTo(x+50, y);               			//������
			Form1->Canvas->Ellipse(x+10,y-15, x+40,y+15);
			if (vv==1) {
				Form1->Canvas->MoveTo(x+30, y-10);
				Form1->Canvas->LineTo(x+20, y);
				Form1->Canvas->LineTo(x+30, y+10);
				Form1->Canvas->TextOutA(x+25, y-5, "E"+IntToStr(num));
			}
			else {
				Form1->Canvas->MoveTo(x+20, y-10);
				Form1->Canvas->LineTo(x+30, y);
				Form1->Canvas->LineTo(x+20, y+10);
				Form1->Canvas->TextOutA(x+15, y-5, "E"+IntToStr(num));
			}
	break;
	case 5:
			Form1->Canvas->LineTo(x, y+50);
			Form1->Canvas->Ellipse(x-15,y+10, x+15,y+40);
			if (vv==1) {
				Form1->Canvas->MoveTo(x-10, y+30);
				Form1->Canvas->LineTo(x, y+20);
				Form1->Canvas->LineTo(x+10, y+30);
				Form1->Canvas->TextOutA(x-5, y+25, "E"+IntToStr(num));
			}
			else {
				Form1->Canvas->MoveTo(x-10, y+20);
				Form1->Canvas->LineTo(x, y+30);
				Form1->Canvas->LineTo(x+10, y+20);
				Form1->Canvas->TextOutA(x-5, y+15, "E"+IntToStr(num));
			}
	break;
	}
	}

//��� ���������� �������, ���� ���� ��� ���������� � �����
	void FinalPaintChainEl(int x, int y, int id, int o, int num)
{
	Form1->Canv->Canvas->Pen->Color=clBlack;
	Form1->Canv->Canvas->Brush->Color=clWhite;
	Form1->Canv->Canvas->MoveTo(x,y);
	Form1->Canv->Canvas->Font->Size=6;

	switch (id) {
	//������ ����������
	case 0:	Form1->Canv->Canvas->LineTo(x+50,y); break;
	case 3: Form1->Canv->Canvas->LineTo(x,y+50); break;
	//������ ���������
	case 1:
			Form1->Canv->Canvas->LineTo(x+50, y);               			//������
			Form1->Canv->Canvas->Rectangle(x+12,y-10, x+38,y+10);
			Form1->Canv->Canvas->TextOutA(x+20, y-5, "R"+IntToStr(num));
	break;
	case 4:
			Form1->Canv->Canvas->LineTo(x, y+50);
			Form1->Canv->Canvas->Rectangle(x-10,y+12,x+10,y+38);
			Form1->Canv->Canvas->TextOutA(x-5, y+20, "R"+IntToStr(num));
	break;
		//�����v ���
	case 2:
			Form1->Canv->Canvas->LineTo(x+50, y);               			//������
			Form1->Canv->Canvas->Ellipse(x+10,y-15, x+40,y+15);
			if (vv==1) {
				Form1->Canv->Canvas->MoveTo(x+30, y-10);
				Form1->Canv->Canvas->LineTo(x+20, y);
				Form1->Canv->Canvas->LineTo(x+30, y+10);
				Form1->Canv->Canvas->TextOutA(x+25, y-5, "E"+IntToStr(num));
			}
			else {
				Form1->Canv->Canvas->MoveTo(x+20, y-10);
				Form1->Canv->Canvas->LineTo(x+30, y);
				Form1->Canv->Canvas->LineTo(x+20, y+10);
				Form1->Canv->Canvas->TextOutA(x+15, y-5, "E"+IntToStr(num));

			}
	break;
	case 5:
			Form1->Canv->Canvas->LineTo(x, y+50);
			Form1->Canv->Canvas->Ellipse(x-15,y+10, x+15,y+40);
			if (vv==1) {
				Form1->Canv->Canvas->MoveTo(x-10, y+30);
				Form1->Canv->Canvas->LineTo(x, y+20);
				Form1->Canv->Canvas->LineTo(x+10, y+30);
				Form1->Canv->Canvas->TextOutA(x-5, y+25, "E"+IntToStr(num));
			}
			else {
				Form1->Canv->Canvas->MoveTo(x-10, y+20);
				Form1->Canv->Canvas->LineTo(x, y+30);
				Form1->Canv->Canvas->LineTo(x+10, y+20);
				Form1->Canv->Canvas->TextOutA(x-5, y+15, "E"+IntToStr(num));
			}
	break;
			}
	}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

	for(int k = 0; k < 6; k++) {
			boxes [k] = new TRadioButton(Form1);
			boxes [k]->Parent = Form1;
			boxes [k]->Tag=k;
			boxes[k]->OnClick = boxesClick;
			if (k<3) {
				boxes[k]->Left = Form1->Width-195;
				boxes[k]->Top = 90+40*k;
			}
			else
			{
				boxes[k]->Left = Form1->Width-80;
				boxes[k]->Top = 90+40*(k-3);
			}
		}
	Standing(); //��� ������� � ������, �� �������������.
}
//--------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
	gx=(Form1->Width-350)/50;             //����� ����������
	gy=(Form1->Height-150)/50;            //gy,gx - ���-�� ����� �����
	gx2=gx*50;                            // gy2,gx2 - ����� ����
	gy2=gy*50;
	Canvas->Brush->Color=clWhite;
	Canvas->Pen->Color=cl3DLight;
	Canvas->Rectangle(50,50, gx2, gy2);
	for (i=1; i < gy; i++) {                //����� �������������� �����
		Canvas->MoveTo(50,50+ys*i);
		Canvas->LineTo(gx2,50+ys*i);
	};
	for (j = 1; j < gx; j++) {
		Canvas->MoveTo(50+xs*j,50);
		Canvas->LineTo(50+xs*j, gy2);
	}
	for(int k = 0; k < 6; k++) {                //�������� ������ ���� ����
		if (k<3) {
			boxes[k]->Left = gx2+110;
			boxes[k]->Top = 90+40*k;
		}
		else
		{
			boxes[k]->Left = gx2+230;
			boxes[k]->Top = 90+40*(k-3);
		}
	}
	Add->Left=gx2+90;
	Delete->Left=gx2+90;                           //������ ����������
	AddPar->Left=gx2+20;
	DelPar->Left=gx2+20;					   //���������� ��������
	ObjR->Top=gy2+20;                          //����� � ������ ����
	ObjU->Top=gy2+20;
	ObjI->Top=gy2+20;
	Rl->Top=gy2+20;
	Ul->Top=gy2+20;
	Il->Top=gy2+20;
	Objects->Top=gy2+20;
	Canv->Height=gy2-50;                        //� ����� �������
	Canv->Width=gx2-50;
	Go->Top=gy2+50;
	Clear->Top=gy2+50;                         //������ ������

	//���, ����� ������� - ��� ;-) ��������. ������ ��!)
	PaintList *PL = PLHead->next;
	while (PL!=NULL) {
		PaintChainEl(PL->xCoord,PL->yCoord,PL->type,PL->orient, PL->number);
		PL=PL->next;
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ObjectsCloseUp(TObject *Sender)
{
	PaintList *PL = PLHead->next;
	if (PL==NULL)
	Application->MessageBox("� �� �������� ������� ���-������:)","������",MB_OK);
	else
	{
		for (int t = 0; t < Objects->ItemIndex; t++) {
			PL=PL->next;
		}
		ObjR->Text=FloatToStr(PL->R);
		ObjU->Text=FloatToStr(PL->U);
		ObjI->Text=FloatToStr(PL->I);
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	//���������  3 ������ - ������������ ���������, ���������� ���� �������
   PLHead  =  new PaintList;
   PLHead->next=NULL;
   PLCurr=PLHead;

   for(int j = 0; j<6;j++) {
		boxes[j]->Enabled = false;
		boxes[j]->Checked = false;
	}


}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddClick(TObject *Sender)
{
	//��-����� ������������ ������� ����� �����.   ��, �����, ������ ���
	Canvas->Pen->Color=cl3DLight;
	if (!test(coord.Hx+1,coord.Hy)) {
		Canvas->MoveTo(coord.Hx, coord.Hy);
		Canvas->LineTo(coord.Hx+50, coord.Hy);
	}
	if (!test(coord.Vx,coord.Vy+1)) {
		Canvas->MoveTo(coord.Vx, coord.Vy);
		Canvas->LineTo(coord.Vx, coord.Vy+50);
	}
	//C��������� ������
	if (Ii->Text!="")
		I=StrToFloat(Ii->Text);
		else I=0;
	if (Uu->Text=="" && (id==2 || id==5)&& Uu->Visible==true) {
		if  ((Application->MessageBox("������� ����������!","������",MB_OK)))
			return;
	}
	else if (Uu->Text!="") {
		U=StrToFloat(Uu->Text);
	}
	else U = 0;
	if (Rr->Text=="" && Rr->Visible==true){
		if  ((Application->MessageBox("������� �������������!","������",MB_OK)))
			return;
	}
	else if (Rr->Text!=""){
		R=StrToFloat(Rr->Text);
	}
	else R=0;
	//��� ���. ��������������� ����, ���� �������� ���������
	if (Rb1->Checked)
		vv++;


	int num; //��� ������ ��������

	if (id<=2) {
		cx = coord.Hx;
		cy = coord.Hy;
		or=1;
	}
	else {
		cx = coord.Vx;
		cy = coord.Vy;
		or=0;
	}

	//���������� � �������.
	switch (id) {
	case 0:     //�������� ��� �����������
	case 3:
		Form1->Objects->Items->Add("��������� "+IntToStr(conds));
		num = conds;
		conds++;
		break;
	case 1:				 //���������
	case 4:
		Objects->Items->Add("�������� "+IntToStr(resis));
		num = resis;
		resis++;
		break;
	case 2:     //���
	case 5:
			Objects->Items->Add("�������� ��� "+IntToStr(ses));
		num = ses;
		ses++;
		break;
	}
	PaintChainEl(cx, cy, id, vv, num); //��������� ��������
	AddPaintE(cx, cy, id, vv, or, num,  R, U, I);	//���������� ��� � ������, ���� ����� ���� ��������������.

	//��������� ������
	vv=0;

	Standing();

	for (int j = 0; j <=5; j++) {
		boxes[j]->Visible=true;
		boxes[j]->Checked = false;
		boxes[j]->Enabled = false;

	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::boxesClick(TObject *Sender)
{
	TRadioButton *boxes;
	boxes = (TRadioButton*)Sender;
	// �������� Tag ������ �������� �� �������������
	id = boxes->Tag;

	switch(id)
	{
	case 0:
	case 3:
		Rlabel->Visible=false;
		Ulabel->Visible=false;
		Ilabel->Visible=false;
		Rr->Visible=false;
		Uu->Visible=false;
		Ii->Visible=false;
		Route->Visible=false;

	break;

	case 1:
	case 4:
		Rlabel->Visible=true;
		Ulabel->Visible=true;
		Ilabel->Visible=true;
		Rr->Visible=true;
		Uu->Visible=true;
		Ii->Visible=true;
		Route->Visible=false;
	break;

	case 2:
	case 5:
		Ulabel->Visible=true;
		Rlabel->Visible=false;
		Ilabel->Visible=false;
		Uu->Visible=true;
		Ii->Visible=false;
		Rr->Visible=false;
		Route->Visible=true;
		if (id==2) {
			Rb1->Caption="�����";
			Rb2->Caption="������";
		}
		else {
			Rb1->Caption="�����";
			Rb2->Caption="����";
		}
	break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GoClick(TObject *Sender)
{
	PaintList *PL = PLHead->next;
	if (PL==NULL) {
		Application->MessageBox("�� ������ �� ����������:) ","������",MB_OK);
		return;
	}
	   /*	if (ses==1) {
		Application->MessageBox("�������� �������� ���!:)","������",MB_OK);
			return;
	}*/

   NHead  =  new Nodes;     //������������� ������ �����
   NHead->next=NULL;
   NCurr=NHead;
   int napr = -1;

	int nodes = 0; //����� ������� ���������� �����
	int i, j; //�������� �������
	int t = 0;

  /*	for (i = 1; i < gy; i++) {
		for (j = 1; j < gx; j++) {

			if (testnodes(j,i)==1)  {
				Application->MessageBox("���� �� ��������!:)","������",MB_OK);
				return;
			}
	`		}
	}  */
	for (i = 1; i < gy; i++) {                      //��������, ��� �� ����
		for (j = 1; j < gx; j++) {
			if (testnodes(j,i, napr)>=3) {
			nodes++;
			AddNode(50*j, 50*i, nodes);
			}
		}
	}
	Nodes *N = NHead->next;
	ofstream out("test.txt");
	out<<"����"<<endl;
	out<<nodes<<" \n";                           //���-�� �����
	while (N != NULL) {
		out<<N->xCoord<<" "<<N->yCoord<<" "<<N->number<<endl;  //��������� �����
		N=N->next;
	}
	Application->MessageBox("���� ���������!:)","������",MB_OK);

	N = NHead->next;

	while (N!=NULL) {
		int x = N->xCoord;
		int y= N->yCoord;
		int num  = N->number;

		for(i=1;i<5;i++) {
			Branch bfin;
			bfin = SearchBranch(i,num, x, y);
			if (bfin.NodeNum1!=bfin.NodeNum2)
				branches.push_back(bfin);
		}

		N=N->next;
	}
	out<<"�����"<<endl;
	out<<branches.size()<<endl;					  //���-�� �����
	for (i = 0; i <= branches.size();  i++) {
		out<<branches[i].NodeNum1<<" "<<branches[i].NodeNum2<<" "<<branches[i].R<<" "<<branches[i].E<<endl;
	}
				  //���� ��� �����
	 out.close();



}
//---------------------------------------------------------------------------



void __fastcall TForm1::ObjRChange(TObject *Sender)
{
	PaintList *PL = PLHead->next;
	if (PL==NULL)
	Application->MessageBox("� �� �������� ������� ���-������:)","������",MB_OK);
	if (ObjR->Text!=""){
		for (int t = 0; t < Objects->ItemIndex; t++) {
			PL=PL->next;
		}
	}
	PL->R=StrToFloat(ObjR->Text);

}
//---------------------------------------------------------------------------


void __fastcall TForm1::ClearClick(TObject *Sender)
{
	PaintList *PL = PLHead->next;


		if (PL != NULL ) {
			ClearPaintList();
		}
		else Application->MessageBox("������ �������:)","������",MB_OK);

			Refresh();

			Standing();
			for (int j = 0; j <=5; j++) {
				boxes[j]->Visible=true;
				boxes[j]->Checked = false;
				boxes[j]->Enabled = false;

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ObjUChange(TObject *Sender)
{
		PaintList *PL = PLHead->next;
	if (PL==NULL)
	Application->MessageBox("� �� �������� ������� ���-������:)","������",MB_OK);
	if (ObjU->Text!=""){
		for (int t = 0; t < Objects->ItemIndex; t++) {
			PL=PL->next;
		}
	}
	PL->U=StrToFloat(ObjU->Text);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ObjIChange(TObject *Sender)
{
		PaintList *PL = PLHead->next;
	if (PL==NULL)
	Application->MessageBox("� �� �������� ������� ���-������:)","������",MB_OK);
	   if (ObjI->Text!=""){
	   for (int t = 0; t < Objects->ItemIndex; t++) {
			PL=PL->next;
		}

	PL->I=StrToFloat(ObjI->Text);

}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteClick(TObject *Sender)
{
		if (Del1->Checked==true)  {
		or=1;   //del1 - �������������� �������. 2 - ��������
		DelElement(coord.Hx, coord.Hy, or);
		}
		else  {
		or=2;
		DelElement(coord.Vx, coord.Vy, or);
		}
		Form1->Refresh();

		Standing();
		for (int j = 0; j <=5; j++) {
			boxes[j]->Visible=true;
			boxes[j]->Checked = false;
			boxes[j]->Enabled = false;

		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveClick(TObject *Sender)
{
	SaveDialog1->Filter = "����� Electrical Network|*.enf";
	SaveDialog1->DefaultExt=".enf";


	if (SaveDialog1->Execute())  {

		ofstream fout (SaveDialog1->FileName.c_str());
		PaintList *PL = PLHead->next;
		fout << ses-1 << " " <<resis-1 << " " << conds-1 << endl;

		while (PL != NULL) {
			fout << PL->xCoord << " ";
			fout << PL->yCoord << " ";
			fout << PL->type << " ";
			fout << PL->orient << " ";
			fout << PL->or << "  ";
			fout << PL->number << "  ";
			fout << PL->R << " ";
			fout << PL->U << " ";
			fout << PL->I << endl;
			PL = PL->next;
		}
		fout.close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenClick(TObject *Sender)
{
	/*PaintMemoryFree();
	PaintList  *PL;
	PLHead  =  new PaintList;
	PLHead->next=NULL;
	PLCurr=PLHead; */
		PaintList *PL = PLHead->next;
	PaintList *pl = PLHead;

		if (PL != NULL ) {
			int t = 0;
			while (PL!=NULL){
				pl->next = PL->next;
				delete PL;
				Form1->Objects->Items->Delete(t);
				PL=pl->next;
				t++;
			}
			PLCurr = PLHead;
		}

	OpenDialog1->Filter = "����� Electrical Network|*.enf";
	if (OpenDialog1->Execute())  {
		ifstream fin (OpenDialog1->FileName.c_str());
		fin >> ses  >> resis >> conds ;
			for(int  i=0; i< ses+resis+conds;i++) {
			PL = new PaintList;
			PL->next=NULL;
			fin >> PL->xCoord ;                        //��������� ������ �� �����
			fin >> PL->yCoord;
			fin >> PL->type;
			fin >> PL->orient;
			fin >> PL->or;
			fin >> PL->number;
			fin >> PL->R;
			fin >> PL->U;
			fin >> PL->I;
			switch (PL->type) {
			case 1:
			case 4:
				Form1->Objects->Items->Add("�������� "+IntToStr(PL->number));
			   break;
			case 2:
			case 5:
				Form1->Objects->Items->Add("�������� ��� "+IntToStr(PL->number));
				break;
			}
			PLCurr->next = PL;
			PLCurr = PL;
			}
	fin.close();
	Form1->Refresh();
	}
	}

//---------------------------------------------------------------------------


void __fastcall TForm1::TheoryMClick(TObject *Sender)
{

ShellExecute(0,"open","EN.chm",NULL,NULL,SW_NORMAL);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::CanvMouseDown(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{
Canvas->Pen->Color=cl3DLight;                     //������������ ��������
	if(!test(coord.Hx+1,coord.Hy)) {                  //��� ���������
		Canvas->MoveTo(coord.Hx, coord.Hy);           //�� ���� ��� �����
		Canvas->LineTo(coord.Hx+50, coord.Hy);
	}
	if(!test(coord.Vx,coord.Vy+1)) {
		Canvas->MoveTo(coord.Vx, coord.Vy);
		Canvas->LineTo(coord.Vx, coord.Vy+50);
	}
	vv=0;
	X+=50;
	Y+=50;
	coord.set(X,Y);
	int i  = 0;

if (Button == mbLeft) {
	MouseL();
	for (int i = 0; i < 6; i++) {
		boxes[i]->Visible=true;
		 boxes[i]->Enabled=true;
	}
	// ���� ������� ��� ��������. �������� = ������, ��
	// � ������ ����� ������ ��������.
	if(test(coord.Hx+1,coord.Hy)) {
		boxes[0]->Enabled = false;
		boxes[1]->Enabled = false;
		boxes[2]->Enabled = false;
		i++;
	}
	else {
	Canvas->Pen->Color=clFuchsia;
	Canvas->MoveTo(coord.Hx, coord.Hy);
	Canvas->LineTo(coord.Hx+50, coord.Hy);
	};
	if(test(coord.Vx,coord.Vy+1)) {
		boxes[3]->Enabled = false;
		boxes[4]->Enabled = false;
		boxes[5]->Enabled = false;
		i++;
	}
	else {
	Canvas->Pen->Color=clFuchsia;
	Canvas->MoveTo(coord.Vx, coord.Vy);
	Canvas->LineTo(coord.Vx, coord.Vy+50);
	};
	//������   ������
	if (i == 2)
		Application->MessageBox("���������� ������� � ������ �����!","������",MB_OK); //� ��� ���, � ��� ��� ����� ����

	Uu->Enabled = true;
	Rr->Enabled = true;
	Ii->Enabled = true;
	}
	if (Button == mbRight)
	{
		AddPar->Visible = false;
		DelPar->Visible = true;
		Del1->Checked=false;
		Del2->Checked=false;
		for(int j = 0; j<6;j++) {
			boxes[j]->Visible = false;
		}
		Add->Visible=false;
		Del1->Enabled=false;
		Del2->Enabled=false;
		Delete->Visible=true;
		Delete->Enabled=true;

	if(test(coord.Hx+1,coord.Hy)) {           //���� ��� � ������ ���������
		Del1->Enabled=true;
		i++;
	}
	if(test(coord.Vx,coord.Vy+1)) {
		Del2->Enabled=true;
		i++;
	}
	if (i==0)
			if (Application->MessageBox("������ �������!","������",MB_OK))
				{
					Standing();
					for (int i = 0; i < 6; i++) {
						boxes[i]->Visible=true;
					}
				}
	}
		i = 0;


}
//---------------------------------------------------------------------------



void __fastcall TForm1::SavePicClick(TObject *Sender)
{
//������,���� ���������, �������.

	SavePictureDialog1->DefaultExt=".jpg";

	if (SavePictureDialog1->Execute())
	{
		PaintList *PL = PLHead->next;
		while (PL!=NULL) {
			FinalPaintChainEl(PL->xCoord-50,PL->yCoord-50,PL->type,PL->orient, PL->number);
			PL=PL->next;
		}
		Canv->Picture->SaveToFile(SavePictureDialog1->FileName.c_str());
		Canv->Picture->Bitmap->FreeImage();
		Canv->Picture->Bitmap = NULL;
	}

}
//---------------------------------------------------------------------------

