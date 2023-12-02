//---------------------------------------------------------------------------

#pragma hdrstop
#include "Math.h"
#include "iostream.h"
// ��� ������� ������� ����� ��� ������������ ������� (��������, � ��:):))
float SolveDeterminant(float **Matrix, int size) {
	float det = 0; //������� �����
	int i,j,k; // ������� ��� �������� ������� � ������.
	int ni,nj; // ������� ��� ����������� �������.
	float **NewMatrix; //������������� ����� �������.

	//������������ ������� ����������� 2 �� ������� "�������"
	if (size == 2) {
		det =  Matrix[0][0]*Matrix[1][1] - Matrix[0][1]*Matrix[1][2];
		return det;
	}
	// ���� �� �����, ������� � ��� ������, ��� 2 �� 2, ������� �������� ������ ��� ����� �������:
	NewMatrix = new float* [size-1];
	if(NewMatrix == NULL) {
			//Application->MessageBox("������ �� ����� ���� ��������","������",MB_OK);
			//return;
		///////////////////////// ����� ����� ������� ���������� ��������� � ������� ( ������ �� ����� ���� ��������).

	}
	for (ni = 0; ni < size-1; ni++) {
		NewMatrix[ni] =  new float[size-1];
		if(NewMatrix[ni] == NULL) {
			//�������� ���� ��� ������, ������� ��� ������ ��������. � ����� ����� ������� ���������� ���������  � ��� �� ����� �������.
			for(nj = 0; nj < ni; nj++)
				delete [] NewMatrix[nj];
		}
	}
	//������ ��������. ����� ��������� ������.
	//����� ������ ������� �� ��������� �� 1 ����������� � ���� ����� ������������:).
	for ( i = 0; i < size; i++) {
		//������� �� �������� ����� �������, � ����� ��������� ��� ��� ������������.
		for(j = 1, ni = 0; j <size; j++) {
			for(k = 0, nj = 0; k < size; k++) {
				if(k == i) continue;
				NewMatrix[ni][nj] = Matrix[j][k];
				nj++;
			}
			ni++;
		}
		//� ������ ������� i ���� � ���������� ����� ������������� (��������� ���� �� ������ ������). ����� �����
		det += pow(-1.0,i)*SolveDeterminant(NewMatrix,size-1) * Matrix[0][i];
	}
	//����� ���� ��� �� ����� ������������ (� �� ��� ���������� �����), ���� �������� ��� ������, ������� ��������
	for(ni = 0; ni < size-1; ni++)
		delete [] NewMatrix[ni];
	delete [] NewMatrix;

	return det;
}
//��� ������� ��������� ������� �������������� ����������.
void AlgAddMatrix(float **Matrix, int size, float **ResultMatrix) {
	float **TempMatrix; //��������� ������� (���������� ��� �������� ��������).
	int i,j,a,b; // ������� ��� �������� �������.
	int ni,nj; // ������� ��� ����������� �������.

	//��������� ������ ��� �������
	TempMatrix = new float*[size-1];
	if(TempMatrix == NULL) {
		///////////////////////// ����� ����� ������� ���������� ��������� � ������� ( ������ �� ����� ���� ��������).
		//Application->MessageBox("������ �� ����� ���� ��������","������",MB_OK);
		return;
	}
	for (ni = 0; ni < size-1; ni++) {
		TempMatrix[ni] =  new float[size-1];
		if(TempMatrix[ni] == NULL) {
			//�������� ���� ��� ������, ������� ��� ������ ��������. � ����� ����� ������� ���������� ���������  � ��� �� ����� �������.
			for(nj = 0; nj < ni; nj++)
				delete [] TempMatrix[nj];
				//Application->MessageBox("������ �� ����� ���� ��������","������",MB_OK);
				return;
		}
	}

	// ��� �, ��������� ������ �������������� ���������� � ������� ��������.
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {

			// ������ ��� ���� ����� ������� ������� TempMatrix � ��������� �� ������������. � �������� ������ �� ���� ���������� ������������.
			for(a = 0, ni = 0; a <size; a++) {
				if(a==i) continue;
				for(b = 0, nj = 0; b < size; b++) {
					if(b == j) continue;
					TempMatrix[ni][nj] = Matrix[a][b];
					nj++;
				}
				ni++;
			}
			//�������� ������� ������� �������:
			ResultMatrix[i][j] = pow(-1.0,i+j) * SolveDeterminant(TempMatrix,size-1);
		}
	}

	//������� ������ (���, ��������) ��� ������ � ������������. �� � ���, ��� ������ �� ����� ���:).
	for(ni = 0; ni < size-1; ni++)
		delete [] TempMatrix[ni];
	delete [] TempMatrix;
}
void Transposition (float **Matrix, int size) {
	float tmp; // ���������� ������ �����:)
	int i, j; //������� :)
	//�������� ������ ������� ������ � �������:).
	for(i = 0; i < size; i++) {
		for (j = i ; j < size; j++) {
			tmp = Matrix[i][j];
			Matrix[i][j] = Matrix[j][i];
			Matrix[j][i] = tmp;
		}
	}
}


//---------------------------------------------------------------------------

#pragma argsused
int main(int argc, char* argv[])
{

	float **Matrix;
	Matrix = new float* [3];
	for (int i = 0; i < 3; i++) {
		Matrix[i] = new float [3];
	}

	Matrix[0][0]=1;
	Matrix[0][1]=2;
	Matrix[0][2]=1;
	Matrix[1][0]=4;
	Matrix[1][1]=3;
	Matrix[1][2]=2;
	Matrix[2][0]=1;
	Matrix[2][1]=2;
	Matrix[2][2]=5;

	for (int i = 0; i < 3; i++) {
		 for (int j = 0; j < 3; j++) {
			 cout<<Matrix[i][j]<<" ";
		 }
		 cout<<endl;
	}
	cout<< SolveDeterminant(Matrix, 3);

	getchar();



	return 0;
}
//---------------------------------------------------------------------------
