	int CurrEqual = nodes-1; //����������, ����� ��������� ����������
	N = NHead->next;
	while (N != NULL)
	{
		int MainNode=N->number; //����, �� �������� ��������� �������� �������� ������.
		//����� �������,������������� �� MainNode
		for (int i = 1; i < 5; i++)
		{

			int CurrNode = MainNode;// � ������ CurrNode=MainNode  - ����, �� ������� ���������  � ������� ������ ���������.
			 //��������, ���������� �� ������ ���������.
			 if (CurrEqual==branches.size())
				goto EndSearch;

			 while (1)
			{
				int Identification = 1;   //���� ����� �� ��������������� ������� ������ ������� ������.
				//������ �� ������
				for (int j=0; j < branches.size(); j++)
				{
					//��������, ����� �� ��������� ������ ������. ( � ������� ������ j �����).
				   if (branches[j].NodeNum1==MainNode && branches[j].NodeNum2==CurrNode || branches[j].NodeNum2==MainNode && branches[j].NodeNum1==CurrNode )
				   {	 //��������� ����������� � �������
						if (branches[j].NodeNum1==MainNode)
							Matrix[CurrEqual][j] = -branches[j].R;
						else	 Matrix[CurrEqual][j] = branches[j].R;

						//�������� �� ������ ( �� ���� �� ����� ��������� ���?)
						if (CurrEqual!=nodes-1)  {                           
							for (int m = nodes-1; m < CurrEqual; m++) {
								int id = 0, minusid = 0;
								for (int n = 0; n <= branches.size(); n++) {
									if (Matrix[CurrEqual][n]==Matrix[m][n])
									   id++;
									if (Matrix[CurrEqual][n]==-Matrix[m][n])
										minusid++;
								}

								if (id==branches.size()+1 || minusid==branches.size()+1){
									 Matrix[CurrEqual][j] = 0;
									 goto NormalSearch;
								}
								if (m==CurrEqual-1) {
									CurrEqual++;
								  goto SearchNewEqual;
								}

							}
						}
						else  {
							CurrEqual++;
							goto SearchNewEqual;
						}
				   }

				}

NormalSearch:	//������� �������� ������ ����.
				for (int j = 0; j < branches.size(); j++) {
					if (branches[j].NodeNum1==CurrNode || branches[j].NodeNum2==CurrNode)
					{
					   if (Identification==i) {
							//���������� �������������
							if  (N->number=branches[j].NodeNum1)
								Matrix[CurrEqual][j]=-branches[j].R;
							else
								Matrix[CurrEqual][j]=branches[j].R;
							//�������������� ��������� ����
							if (branches[j].NodeNum1==CurrNode)
								CurrNode=branches[j].NodeNum2;
							else
								CurrNode=branches[j].NodeNum1;
							//���� �� ����� �����, �� �������� �� ����� ����?
							break;
					   }
					   else Identification++; // ����� ���-��� ����� �������..������).
					}
				}
				//���������� �� ����� �������? ������������������� ����������� ��� � ������ �������������� ������
				if (CurrNode==MainNode)
				{
					if (CurrEqual!=nodes-1) {
						for (int m = nodes-1; m < CurrEqual; m++)
						{
							int id = 0, minusid = 0;
							//�������� �� ������ ���������.
							for (int n = 0; n <= branches.size(); i++) {             // ????????????????????????????????????????
								if (Matrix[CurrEqual][n]==Matrix[m][n])
								   id++;
								if (Matrix[CurrEqual][n]==-Matrix[m][n])
									minusid++;
							}
							// ������� ��������� ���������, ���� ���������
							if (id==branches.size()+1 || minusid==branches.size()+1)
							{
								 for (int n = 0; n <=branches.size(); n++) {
									  Matrix[CurrEqual][n] = 0;
								 }
								goto SearchNewEqual;  //�� ����� ������ ���������.
							}
							//���� �� ������� ��������, �� ��������� �� ��� ���� � ���
							if (m==CurrEqual-1) {
								CurrEqual++;
								goto SearchNewEqual;  //�� ����� ������ ���������.
							}
						}
					}
					else {
						CurrEqual++;
						goto SearchNewEqual;  //�� ����� ������ ���������
					}
				}

			}
SearchNewEqual:
		}
		N=N->next;
	}


	if (CurrEqual<branches.size())
		Form2->Result->Lines->Add("��������� ������������.");
EndSearch:
	Form2->Result->Lines->Add("��������� �� ������� ������ ��������:");
	for (i = nodes-1; i < branches.size(); i++) {
			for (j = 0; j < branches.size(); j++) {
			if (Matrix[i][j]!=0) {
				if (Matrix[i][j]<0)
					Form2->Result->Lines->Text = Form2->Result->Lines->Text + FloatToStr(Matrix[i][j])+"*I"+IntToStr(j+1)+" ";
				else Form2->Result->Lines->Text = Form2->Result->Lines->Text + " + " + FloatToStr(Matrix[i][j])+"*I"+IntToStr(j+1)+" ";
				}
			}
		Form2->Result->Lines->Text = Form2->Result->Lines->Text + " = " + FloatToStr(Matrix[i][branches.size()+1]);
		Form2->Result->Lines->Add("");
	}