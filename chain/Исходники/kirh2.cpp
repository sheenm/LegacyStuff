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

				   if ((branches[j].NodeNum1==MainNode && branches[j].NodeNum2==CurrNode || branches[j].NodeNum2==MainNode && branches[j].NodeNum1==CurrNode)&& Matrix[CurrEqual][j]==0)
				   {
						 //��������� ����������� � �������
						if (branches[j].NodeNum1==CurrNode )
							Matrix[CurrEqual][j] = branches[j].R;
						else	 Matrix[CurrEqual][j] = - branches[j].R;
						if (CurrNode!=branches[j].ENodeNum)
							Matrix[CurrEqual][branches.size()]-=branches[j].E;
						else
							Matrix[CurrEqual][branches.size()]+=branches[j].E;
						//�������� �� ������ ( �� ���� �� ����� ��������� ���?)
						if (CurrEqual!=nodes-1)  {
							for (int m = nodes-1; m < CurrEqual; m++) {
								int id = 0;
								for (int n = 0; n <= branches.size(); n++) {
									if (fabs(Matrix[CurrEqual][n])==fabs(Matrix[m][n]))
									   id++;
								}

								if (id==branches.size()+1){
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
				int FormerCurrNode = CurrNode;
				for (int j = 0; j < branches.size(); j++) {
					if ((branches[j].NodeNum1==CurrNode || branches[j].NodeNum2==CurrNode) && Matrix[CurrEqual][j]==0)
					{
						if (Identification==i && Matrix[CurrEqual][j]==0) {
							//���������� �������������
							if  (CurrNode==branches[j].NodeNum1 && branches[j].R!=0)
								Matrix[CurrEqual][j]=branches[j].R;
							else
								Matrix[CurrEqual][j]=-branches[j].R;

							if (CurrNode!=branches[j].ENodeNum)
								Matrix[CurrEqual][branches.size()]-=branches[j].E;
							else
								Matrix[CurrEqual][branches.size()]+=branches[j].E;
							//�������������� ��������� ����
							if (branches[j].NodeNum1==CurrNode)
								CurrNode=branches[j].NodeNum2;
							else
								CurrNode=branches[j].NodeNum1;
							//���� �� ����� �����, �� �������� �� ����� ����?
							break;
							}
					   else if(Identification != i)  Identification++;

					}
				}
				if (FormerCurrNode==CurrNode) {
					for (int n = 0; n <= branches.size(); n++) {
						 Matrix[CurrEqual][n] = 0;
					}
					goto SearchNewEqual;
				}

					//���������� �� ����� �������? ������������������� ����������� ��� � ������ �������������� ������.
				if (CurrNode==MainNode)
				{
					if (CurrEqual!=nodes-1) {
						for (int m = nodes-1; m < CurrEqual; m++)
						{
							int id = 0;
							//�������� �� ������ ���������.
							for (int n = 0; n <= branches.size(); n++) {
								if (fabs(Matrix[CurrEqual][n])==fabs(Matrix[m][n]))
								   id++;
							}
							// ������� ��������� ���������, ���� ���������
							if (id==branches.size()+1)
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
