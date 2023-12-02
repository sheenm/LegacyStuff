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
				   if (branches[j].NodeNum1==MainNode && branches[j].NodeNum2==CurrNode || branches[j].NodeNum2==MainNode && branches[j].NodeNum1==CurrNode )
				   {	 //Изменение коэфициента в матрице
						if (branches[j].NodeNum1==MainNode)
							Matrix[CurrEqual][j] = -branches[j].R;
						else	 Matrix[CurrEqual][j] = branches[j].R;

						//Проверка на повтор ( не было ли таких уравнений уже?)
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

NormalSearch:	//Обычный алгоритм поиска пути.
				for (int j = 0; j < branches.size(); j++) {
					if (branches[j].NodeNum1==CurrNode || branches[j].NodeNum2==CurrNode)
					{
					   if (Identification==i) {
							//добавление сопротивлений
							if  (N->number=branches[j].NodeNum1)
								Matrix[CurrEqual][j]=-branches[j].R;
							else
								Matrix[CurrEqual][j]=branches[j].R;
							//переназначение текукщего узла
							if (branches[j].NodeNum1==CurrNode)
								CurrNode=branches[j].NodeNum2;
							else
								CurrNode=branches[j].NodeNum1;
							//если мы нашли ветвь, то выходимм из цикла ведь?
							break;
					   }
					   else Identification++; // здесь кое-что потом сделать..Спроси).
					}
				}
				//Завершился ли обход контура? Предпринимательство необходимых мер в случае положительного ответа
				if (CurrNode==MainNode)
				{
					if (CurrEqual!=nodes-1) {
						for (int m = nodes-1; m < CurrEqual; m++)
						{
							int id = 0, minusid = 0;
							//Проверка на повтор уравнения.
							for (int n = 0; n <= branches.size(); i++) {             // ????????????????????????????????????????
								if (Matrix[CurrEqual][n]==Matrix[m][n])
								   id++;
								if (Matrix[CurrEqual][n]==-Matrix[m][n])
									minusid++;
							}
							// удаляем полностью кравнение, если совпадает
							if (id==branches.size()+1 || minusid==branches.size()+1)
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


	if (CurrEqual<branches.size())
		Form2->Result->Lines->Add("Уравнений недостаточно.");
EndSearch:
	Form2->Result->Lines->Add("Уравнения по второму закону Кирхгофа:");
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