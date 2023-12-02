	int CurrEqual = nodes-1; //показывает, когда уравнений достаточно
	N = NHead->next;
	while (N != NULL)
	{
		int MainNode=N->number; //узел, от которого программа начинает обходить контур.
		//Поиск контура,начинающегося от MainNode
	for (int i = 1; i < 5; i++)
		{

			int CurrNode = MainNode;// в начале CurrNode=MainNode  - узел, на котором программа  в текущий момент находится.
			 //Проверка, достаточно ли сейчас уравнений.
			 if (CurrEqual==branches.size())
				goto EndSearch;

			 while (1)
			{
				int Identification = 1;   //Чтоб далее из удовлетворяющих условию ветвей выбрать нужную.
				//проход по ветвям
				for (int j=0; j < branches.size(); j++)
				{
				//Проверка, можно ли завершить контур сейчас. ( С помощью данной j ветви).

				   if ((branches[j].NodeNum1==MainNode && branches[j].NodeNum2==CurrNode || branches[j].NodeNum2==MainNode && branches[j].NodeNum1==CurrNode)&& Matrix[CurrEqual][j]==0)
				   {
						 //Изменение коэфициента в матрице
						if (branches[j].NodeNum1==CurrNode )
							Matrix[CurrEqual][j] = branches[j].R;
						else	 Matrix[CurrEqual][j] = - branches[j].R;
						if (CurrNode!=branches[j].ENodeNum)
							Matrix[CurrEqual][branches.size()]-=branches[j].E;
						else
							Matrix[CurrEqual][branches.size()]+=branches[j].E;
						//Проверка на повтор ( не было ли таких уравнений уже?)
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

NormalSearch:	//Обычный алгоритм поиска пути.
				int FormerCurrNode = CurrNode;
				for (int j = 0; j < branches.size(); j++) {
					if ((branches[j].NodeNum1==CurrNode || branches[j].NodeNum2==CurrNode) && Matrix[CurrEqual][j]==0)
					{
						if (Identification==i && Matrix[CurrEqual][j]==0) {
							//добавление сопротивлений
							if  (CurrNode==branches[j].NodeNum1 && branches[j].R!=0)
								Matrix[CurrEqual][j]=branches[j].R;
							else
								Matrix[CurrEqual][j]=-branches[j].R;

							if (CurrNode!=branches[j].ENodeNum)
								Matrix[CurrEqual][branches.size()]-=branches[j].E;
							else
								Matrix[CurrEqual][branches.size()]+=branches[j].E;
							//переназначение текукщего узла
							if (branches[j].NodeNum1==CurrNode)
								CurrNode=branches[j].NodeNum2;
							else
								CurrNode=branches[j].NodeNum1;
							//если мы нашли ветвь, то выходимм из цикла ведь?
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

					//Завершился ли обход контура? Предпринимательство необходимых мер в случае положительного обхода.
				if (CurrNode==MainNode)
				{
					if (CurrEqual!=nodes-1) {
						for (int m = nodes-1; m < CurrEqual; m++)
						{
							int id = 0;
							//Проверка на повтор уравнения.
							for (int n = 0; n <= branches.size(); n++) {
								if (fabs(Matrix[CurrEqual][n])==fabs(Matrix[m][n]))
								   id++;
							}
							// удаляем полностью кравнение, если совпадает
							if (id==branches.size()+1)
							{
								 for (int n = 0; n <=branches.size(); n++) {
									  Matrix[CurrEqual][n] = 0;
								 }
								goto SearchNewEqual;  //На поиск нового уравнения.
							}
							//если не нашлось повторов, то оставляем всё как есть и идём
							if (m==CurrEqual-1) {
								CurrEqual++;
								goto SearchNewEqual;  //На поиск нового уравнения.
							}
						}
					}
					else {
						CurrEqual++;
						goto SearchNewEqual;  //На поиск нового уравнения
					}
				}

			}
			SearchNewEqual:
		}
		N=N->next;
	}
