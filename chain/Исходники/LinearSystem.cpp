// Эта функция поможет найти нам определитель матрицы (Рекурсия, о да:):))
float SolveDeterminant(float **Matrix, int size) {
	float det = 0; //Искомое число
	int i,j,k; // Индексы для исходной матрицы в циклах.
	int ni,nj; // Индексы для уменьшенной матрицы.
	float **NewMatrix; //Соответствнно новая матрица.
	
	//Определитель матрицы размерности 2 мы считаем "вручную"
	if (size == 2) {
		det =  Matrix[0][0]*Matrix[1][1] - Matrix[0][1]*Matrix[1][0];
		return det;
	}
	// Судя по всему, матрица у нас больше, чем 2 на 2, поэтому выделяем память для новой матрицы:
	NewMatrix = new *float[size-1];
	if(NewMatrix == NULL) {
		///////////////////////// Здесь нужно сделать завершение программы с ошибкой ( память не может быть выделена).
	}
	for (ni = 0; ni < size-1; ni++) {
		NewMatrix[ni] =  new float[size-1];
		if(NewMatrix[ni] == NULL) {
			//Очищение всей той памяти, которую уже успели выделить. И нужно снова сделать завершение программы  с той же самой ошибкой.
			for(nj = 0; nj < ni; nj++)
				delete [] NewMatrix[nj];
		}
	}
	//Память выделена. Можно двигаться дальше.
	//Любую другую матрицу мы уменьшаем на 1 размерность и ищем новые определителЬ:).
	for ( i = 0; i < size; i++) {
		//Сначала мы заполним новую матрицу, а потом посчитаем для нее определитель.
		for(j = 1, ni = 0; j <size; j++) {
			for(k = 0, nj = 0; k < size; k++) {
				if(k == i) continue;
				NewMatrix[ni][nj] = Matrix[j][k];
				nj++;
			}
			ni++;
		}
		//В случае четного i знак у слагаемого будет положительный (раскрытие идет по первой строке). Иначе минус
		det += pow(-1,i)*SolveDeterminant(NewMatrix,size-1) * Matrix[0][i];
	}
	//После того как мы нашли определитель (А мы его непременно нашли), надо очистить всю память, которую выделяли
	for(ni = 0; ni < size-1; ni++)
		delete [] NewMatrix[ni];
	delete [] NewMatrix;
	
	return det;
}
//Эта функция посчитает матрицу алгебраических дополнений.
void AlgAddMatrix(float **Matrix, int size, float **ResultMatrix) {
	float **TempMatrix; //Временная матрица (дополнение для текущего элемента).
	int i,j,a,b; // Индексы для исходной матрицы. 
	int ni,nj; // Индексы для уменьшенной матрицы.
	
	//Выделение памяти для матрицы
	TempMatrix = new *float[size-1];
	if(TempMatrix == NULL) {
		///////////////////////// Здесь нужно сделать завершение программы с ошибкой ( память не может быть выделена).
	}
	for (ni = 0; ni < size-1; ni++) {
		TempMatrix[ni] =  new float[size-1];
		if(TempMatrix[ni] == NULL) {
			//Очищение всей той памяти, которую уже успели выделить. И нужно снова сделать завершение программы  с той же самой ошибкой.
			for(nj = 0; nj < ni; nj++)
				delete [] TempMatrix[nj];
		}
	}
	
	// Что ж, приступим искать алгебраическое дополнение к каждому элементу.
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
		
			// Теперь нам надо найти искомую матрицу TempMatrix и посчитать ее определитель. В принципе похоже на само нахождение определителя.	
			for(a = 0, ni = 0; a <size; a++) {
				if(a==i) continue;
				for(b = 0, nj = 0; b < size; b++) {
					if(b == j) continue;
					TempMatrix[ni][nj] = Matrix[a][b];
					nj++;
				}
				ni++;
			}
			//Получаем элемент искомой матрицы:
			ResultMatrix[i][j] = pow(-1,i+j) * SolveDeterminant(TempMatrix,size-1);
		}
	}
	
	//Очистка памяти (Ура, товарищи) для матриц с дополнениями. Ну а что, она больше не нужна нам:).
	for(ni = 0; ni < size-1; ni++)
		delete [] TempMatrix[ni];
	delete [] TempMatrix;
}
void Transposition (float **Matrix, size) {
	float tmp; // Помогающее обмену число:)
	int i, j; //Индексы :)
	//Начинаем менять местами строки и столбцы:).
	for(i = 0; i < size; i++) {
		for (j = i ; j < size; j++) {
			tmp = Matrix[i][j];
			Matrix[i][j] = Matrix[j][i];
			Matrix[j][i] = tmp;
		}
	}
}


	//Что написать в решении системы:
	float Det = SolveDeterminant(Matrix, branches.size());
	float **InvMatrix;  //Обратная матрица к Matrix.
	
	//Снова копипаст выделения памяти.
	InvMatrix = new *float[branches.size()];
	if(InvMatrix == NULL) {
		///////////////////////// Здесь нужно сделать завершение программы с ошибкой ( память не может быть выделена).
	}
	for (int ni = 0; ni < branches.size(); ni++) {
		InvMatrix[ni] =  new float[branches.size()];
		if(InvMatrix[ni] == NULL) {
			//Очищение всей той памяти, которую уже успели выделить. И нужно снова сделать завершение программы  с той же самой ошибкой.
			for(int nj = 0; nj < ni; nj++)
				delete [] InvMatrix[nj];
		}
	}
	//Вычисление обратной матрицы.
	AlgAddMatrix(Matrix, branches.size(), InvMatrix);
	Transposition(InvMatrix, branches.size());
	for (int i = 0; i < branches.size(); i++)
		for(int j = 0; j < branches.size(); j++) 
			InvMatrix[i][j]/=Det;
	
	//Найдем искомые токи.
	float *Amperages = new float[branches.size()]; // вектор токов (решения системы)/
	for (int i = 0; i< branches.size(); i++) {
		for (Amperages[i] = 0, int j = 0; j < branches.size(); j++) {
			Amperages[i] += InvMatrix[i][j] * Matrix[j][branches.size];
		}
	}
	// Теперь можно выдать значения токов как ответ:). В принципе можно сделать проверку, но мне пока лень:).
	