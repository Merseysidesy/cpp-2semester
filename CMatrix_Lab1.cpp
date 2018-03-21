#include <iostream>
#include <fstream>

using namespace std;

class CMatrix
{
	private:
		int rows_count;
		int column_count;
		double **data;
	public:
		CMatrix(int rows = 0, int column = 0)
		: rows_count(rows),
		column_count(column)
		{
			try
			{
				if (rows < 0 || column < 0)
				{
					throw "negative number of rows or columns! Returned empty CMatrix!";
				}
				data = new double* [rows_count];
				for (int i = 0; i < rows_count; ++i)
				{
					data[i] = new double [column_count];
				}

				for (int i = 0; i < rows_count; ++i)		//default null matrix
					for (int j = 0; j < column_count; ++j)
					{
						data[i][j] = 0;
					}
			}
			catch(const char* str)
			{
				cout << str << endl;
				rows_count = 0;
				column_count = 0;
			}
		}
		~CMatrix()
		{
			for(int i = 0; i < rows_count; i++)
				delete[] data[i];
			delete[] data;
		}
//Ñopy constructor
		CMatrix(CMatrix const &otherMatrix)
		: rows_count(otherMatrix.rows_count),
		column_count(otherMatrix.column_count)
		{
			data = new double* [rows_count];
			for (int i = 0; i < rows_count; ++i)
			{
				data[i] = new double [column_count];
			}
			
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					data[i][j] = otherMatrix.data[i][j];
				}
		}
//Overloanding operator=
		CMatrix operator= (CMatrix const &otherMatrix)
		{
			if (&otherMatrix != this)
			{
				if (rows_count != 0 || column_count != 0)
				{
					for(int i = 0; i < rows_count; i++)
						delete[] data[i];
					delete[] data;
				}
				
				rows_count = otherMatrix.rows_count;
				column_count = otherMatrix.column_count;
				
				data = new double* [rows_count];
				for (int i = 0; i < rows_count; ++i)
				{
					data[i] = new double [column_count];
				}
				
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						this->data[i][j] = otherMatrix.data[i][j];
					}
			}
			return *this;
		}
//File input
		friend
		inline ifstream & operator>> (ifstream & fin, CMatrix &thisMatrix)
		{
			int rows, column;
			fin >> rows >> column;
			if (thisMatrix.rows_count != rows || thisMatrix.column_count != column)
			{
				for(int i = 0; i < thisMatrix.rows_count; i++)
				{
					delete[] thisMatrix.data[i];
				}
				delete[] thisMatrix.data;
				
				thisMatrix.rows_count = rows;
				thisMatrix.column_count = column;
				
				thisMatrix.data = new double* [thisMatrix.rows_count];
				for (int i = 0; i < thisMatrix.rows_count; ++i)
				{
					thisMatrix.data[i] = new double [thisMatrix.column_count];
				}
			}
			
			for (int i = 0; i < thisMatrix.rows_count; ++i)
				for (int j = 0; j < thisMatrix.column_count; ++j)
				{
					fin >> thisMatrix.data[i][j];
				}
			return fin;
		}
//File output
		friend
		inline ofstream & operator<< (ofstream & fout, CMatrix const &thisMatrix)
		{
			
			for (int i = 0; i < thisMatrix.rows_count; ++i)
			{
				for (int j = 0; j < thisMatrix.column_count; ++j)
				{
					fout << thisMatrix.data[i][j] << " ";
				}
				fout << endl;
			}
			return fout;
		}
	
		double & operator() (const int i, const int j)
		{
			try
			{
				if (rows_count - 1 < i || column_count - 1 < j)
					throw "non-existent index of CMatrix!";
			
				return this->data[i][j];
			}
			catch(const char* str)
			{
				cout << str << endl;
				exit(1);
			}
		}
//Transpose
		void tpos ()
		{
			if (rows_count == column_count)
			{
				for (int i = 0; i < rows_count - 1; ++i)
					for (int j = i + 1; j < rows_count; ++j)
					{
						swap(data[i][j], data[j][i]);
					}
			}
		else
			{
				double **result = new double* [column_count];
				for (int i = 0; i < column_count; ++i)
				{
					result[i] = new double [rows_count];
					for (int j = 0; j < rows_count; ++j)
					{
						result[i][j] = data[j][i];
					}
				}
				
				for(int i = 0; i < rows_count; ++i)
				{
					delete[] data[i];
				}
				delete[] data;
				
				swap(rows_count, column_count);
				data = new double* [rows_count];
				for (int i = 0; i < rows_count; ++i)
				{
					data[i] = new double [column_count];
				}
				
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						data[i][j] = result[i][j];
					}
			}
			return;
		}
//Dot
		CMatrix dot (const CMatrix &otherMatrix)
		{
			try
			{
				if (column_count != otherMatrix.rows_count)
					throw "the number of columns (of first CMatrix) isn't equal to the number of rows (of second CMatrix)! Returned CMatrix without values!";
				
				CMatrix result(rows_count, otherMatrix.column_count);
				for (int i = 0; i < result.rows_count; ++i)
					for (int j = 0; j < result.column_count; ++j)
						for(int k = 0; k < column_count; ++k)
						{
							result.data[i][j] += this->data[i][k] * otherMatrix.data[k][j];
						}
				return result;
			}
			catch(const char* th)
			{
				cout << th << endl;
				CMatrix drop;
				return drop;
			}
		}

		CMatrix operator+ (const CMatrix &otherMatrix)
		{
			try
			{
				if (rows_count != otherMatrix.rows_count || column_count != otherMatrix.column_count)
					throw "different sizes of matrices! Returned CMatrix without values!";
				
				CMatrix result(rows_count, column_count);
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						result.data[i][j] = this->data[i][j] + otherMatrix.data[i][j];
					}
				return result;
			}
			catch(const char* th)
			{
				cout << th << endl;
				CMatrix drop;
				return drop;
			}
		}
		CMatrix operator+ (const double value)
		{
			CMatrix result(rows_count, column_count);
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					result.data[i][j] = this->data[i][j] + value;
				}
			return result;
		}
	
		CMatrix operator- (const CMatrix &otherMatrix)
		{
			try
			{
				if (rows_count != otherMatrix.rows_count || column_count != otherMatrix.column_count)
					throw "different sizes of matrices! Returned CMatrix without values!";
				
				CMatrix result(rows_count, column_count);
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						result.data[i][j] = this->data[i][j] - otherMatrix.data[i][j];
					}
				return result;
			}
			catch(const char* th)
			{
				cout << th << endl;
				CMatrix drop;
				return drop;
			}
		}
		CMatrix operator- (const double value)
		{
			CMatrix result(rows_count, column_count);
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					result.data[i][j] = this->data[i][j] - value;
				}
			return result;
		}

		CMatrix operator* (const CMatrix &otherMatrix)
		{
			try
			{
				if (rows_count != otherMatrix.rows_count || column_count != otherMatrix.column_count)
					throw "different sizes of matrices! Returned CMatrix without values!";
				
				CMatrix result(rows_count, column_count);
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						result.data[i][j] = this->data[i][j] * otherMatrix.data[i][j];
					}
				return result;
			}
			catch(const char* th)
			{
				cout << th << endl;
				CMatrix drop;
				return drop;
			}
		}
		CMatrix operator* (const double value)
		{
			CMatrix result(rows_count, column_count);
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					result.data[i][j] = this->data[i][j] * value;
				}
			return result;
		}

		CMatrix operator+= (const CMatrix &otherMatrix)
		{
			try
			{
				if (rows_count != otherMatrix.rows_count || column_count != otherMatrix.column_count)
					throw "different sizes of matrices! Returned CMatrix without values!";
				
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						this->data[i][j] += otherMatrix.data[i][j];
					}
				return *this;
			}
			catch(const char* th)
			{
				cout << th << endl;
				return *this;
			}
		}
		CMatrix operator+= (const int value)
		{
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					this->data[i][j] += value;
				}
			return *this;
		}

		CMatrix operator-= (const CMatrix &otherMatrix)
		{
			try
			{
				if (rows_count != otherMatrix.rows_count || column_count != otherMatrix.column_count)
					throw "different sizes of matrices! Returned CMatrix without values!";
				
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						this->data[i][j] -= otherMatrix.data[i][j];
					}
				return *this;
			}
			catch(const char* th)
			{
				cout << th << endl;
				return *this;
			}
		}
		CMatrix operator-= (const int value)
		{
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					this->data[i][j] -= value;
				}
			return *this;
		}

		CMatrix operator*= (const CMatrix &otherMatrix)
		{
			try
			{
				if (rows_count != otherMatrix.rows_count || column_count != otherMatrix.column_count)
					throw "different sizes of matrices! Returned CMatrix without values!";
				
				for (int i = 0; i < rows_count; ++i)
					for (int j = 0; j < column_count; ++j)
					{
						this->data[i][j] *= otherMatrix.data[i][j];
					}
				return *this;
			}
			catch(const char* th)
			{
				cout << th << endl;
				return *this;
			}
		}
		CMatrix operator*= (const int value)
		{
			for (int i = 0; i < rows_count; ++i)
				for (int j = 0; j < column_count; ++j)
				{
					this->data[i][j] *= value;
				}
			return *this;
		}

};

int main()
{
	return 0;
}
