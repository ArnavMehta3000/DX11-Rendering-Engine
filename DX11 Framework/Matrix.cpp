#ifndef MATRIX_CPP
#define MATRIX_CPP
#include "Matrix.h"

#pragma region Constructors
template<typename T>
Matrix<T>::Matrix(unsigned _rows, unsigned _cols, const T& _initial)
{
	mat.resize(_rows);

	std::vector<T> v;
	v.resize(_cols);

	// Populate the rows
	for (int i = 0; i < v.size(); i++)
		v.push_back(_initial);

	// Populate the cols
	for (int i = 0; i < mat.size(); i++)
		mat.push_back(v);

	v.clear();


	rows = _rows;
	cols = _cols;
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
	mat = rhs;
	rows = rhs.rows;
	cols = rhs.cols;
}

template<typename T>
Matrix<T>::~Matrix() {}
#pragma endregion



#pragma region Operators
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs)
{
	if (&rhs == this)
		return this;
	
	mat = rhs;
	rows = rhs.rows; 
	cols = rhs.cols;

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs)
{
	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result(i, j) = mat(i, j) + rhs(i, j);

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			mat(i, j) = mat(i, j) + rhs(i, j);

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs)
{
	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result(i, j) = mat(i, j) - rhs(i, j);

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			mat(i, j) = mat(i, j) - rhs(i, j);

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs)
{
	// Matrix X Matrix
	Matrix result(rows, cols, 0.0);
	for (i = 0; i < rows; ++i)
		for (j = 0; j < rhs.cols; ++j)
			for (k = 0; k < cols; ++k)
				result[i][j] += mat(i, k) * rhs(k, j);

	return result;
}

template<typename T>
std::vector<T> Matrix<T>::operator*(const std::vector<T>& rhs)
{
	// Matrix X Vector
	std::vector<T> result(rhs.size(), 0.0);
	for (i = 0; i < rows; ++i)
		for (j = 0; j < rhs.size(); ++j)
			for (k = 0; k < cols; ++k)
				result[i * j] = mat(i, k) * rhs(k, j);

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs)
{
	// Matrix X Matrix
	Matrix result(rows, cols, 0.0);
	for (i = 0; i < rows; ++i)
		for (j = 0; j < rhs.cols; ++j)
			for (k = 0; k < cols; ++k)
				result[i][j] += mat(i, k) * rhs(k, j);

	mat = result;

	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T& rhs)
{
	Matrix result(rows, cols, 0.0);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result(i, j) = mat(i, j) + rhs;

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const T& rhs)
{
	Matrix result(rows, cols, 0.0);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result(i, j) = mat(i, j) - rhs;

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T& rhs)
{
	Matrix result(rows, cols, 0.0);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result(i, j) = mat(i, j) * rhs;

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const T& rhs)
{
	Matrix result(rows, cols, 0.0);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			result(i, j) = mat(i, j) / rhs;

	return result;
}

template<typename T>
Matrix<T> Matrix<T>::Transpose()
{
	Matrix transpose(rows, cols, 0.0);
	
	// Get transpose
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			transpose(j, i) = mat(i, j);

	// Apply transpose to current matrix
	mat = transpose;
	transpose.clear();

	return *this;
}

template<typename T>
std::vector<T> Matrix<T>::DiagonalVec()
{
	std::vector<T> diag;

	int i, j;
	while (i < rows && j < cols)
	{
		diag.push_back(math(i, j));
		i++; j++;
	}

	return diag;
}

template<typename T>
const T& Matrix<T>::operator()(const unsigned& row, const unsigned& col) const {

	return this->mat[row][col];

}

template<typename T>
unsigned Matrix<T>::GetRowSize() const
{
	return this->rows;
}

template<typename T>
unsigned Matrix<T>::GetColSize() const
{
	return this->cols;
}

template<typename T>
void Matrix<T>::Debug(std::string msg)
{
	std::ostringstream os;
	std::ostringstream matrix;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			matrix << mat(i, j);
		}
		matrix << std::endl;
	}


	/*if (msg == "")
		os << "Matrix\n" << matrix;
	else
		os << "[" << msg << "]Matrix\n" << matrix;*/

	OutputDebugStringA(matrix.str().c_str());
}


#pragma endregion
#endif // !MATRIX_CPP