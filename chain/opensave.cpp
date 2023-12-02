void __fastcall TForm1::SaveClick(TObject *Sender)
{
	Form1->SaveDialog1->Filter = "Файлы Electrical Network|*.enf";
	Form1->SaveDialog1->DefaultExt=".enf";

	if (Form1->SaveDialog1->Execute())  {

		ofstream fout (Form1->SaveDialog1->FileName.c_str());
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

	OpenDialog1->Filter = "Файлы Electrical Network|*.enf";
	if (OpenDialog1->Execute())  {
		ifstream fin (OpenDialog1->FileName.c_str());
		fin >> ses  >> resis >> conds ;
			for(int  i=0; i< ses+resis+conds;i++) {
				PL = new PaintList;
				PL->next=NULL;
				fin >> PL->xCoord ;                        //считываем данные из файла
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
					Form1->Objects->Items->Add("Резистор "+IntToStr(PL->number));
				   break;
				case 2:
				case 5:
					Form1->Objects->Items->Add("Источник ЭДС "+IntToStr(PL->number));
					break;
				}
				PLCurr->next = PL;
				PLCurr = PL;
			}
	fin.close();
	Form1->Refresh();
	}
	}