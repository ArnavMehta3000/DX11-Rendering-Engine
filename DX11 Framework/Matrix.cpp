#ifndef MATRIX_CPP
#define MATRIX_CPP
#include "Matrix.h"

#pragma region Constructors
template<typename T>
Matrix<T>::Matrix(unsigned int _rows, unsigned int _cols, const T& _initial)
{
	mat.resize(_rows);

	for (int i = 0; i < _rows; i++)
	{
		//std::vector<T> v = std::vector<T>();
		 //Push back the entire row

		for (int j = 0; j < _cols; j++)
		{
			mat[i].push_back(_initial);  // Push back each element (col) in that row
		}
	}

	rows = _rows;
	cols = _cols;
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
{
	mat = rhs.mat;
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

	mat = rhs.mat;
	rows = rhs.rows;
	cols = rhs.cols;

	return *this;
}

template<typename T>
const T& Matrix<T>::operator()(const unsigned& row, const unsigned& col) const {

	return this->mat[row][col];

}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs)
{
	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			result.mat[i][j] = mat[i][j] + rhs.mat[i][j];
		}

	return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			mat[i][j] = mat[i][j] + rhs.mat[i][j];

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

	OutputDebugStringA(matrix.str().c_str());
}

template<typename T>
void Matrix<T>::Debug(unsigned row, unsigned col, std::string msg)
{
	std::ostringstream os;
	if (msg == "")
		os << "Matrix[" << row << "," << col << "]: " << mat[row][col] << "\n";
	else
		os << "(" << msg << ") Matrix[" << row << "," << col << "]: " << mat[row][col] << "\n";

	OutputDebugStringA(os.str().c_str());
}

template<typename T>
Matrix<float> Matrix<T>::Float4x4ToMatrix(const DirectX::XMFLOAT4X4& other)
{
	Matrix output(4, 4, 0.f);

	output[0][0] = other._11;
	output[0][1] = other._12;
	output[0][2] = other._13;
	output[0][3] = other._14;

	output[1][0] = other._21;
	output[1][1] = other._22;
	output[1][2] = other._23;
	output[1][3] = other._24;

	output[2][0] = other._31;
	output[2][1] = other._32;
	output[2][2] = other._33;
	output[2][3] = other._34;

	output[3][0] = other._41;
	output[3][1] = other._42;
	output[3][2] = other._43;
	output[3][3] = other._44;

	return output;
}

template<typename T>
DirectX::XMFLOAT4X4 Matrix<T>::MatrixToFloat4x4(const Matrix<float>& mat)
{
	DirectX::XMFLOAT4X4 output(
		mat[0][1], mat[0][1], mat[0][2], mat[0][3],
		mat[1][1], mat[1][1], mat[1][2], mat[1][3],
		mat[2][1], mat[2][1], mat[2][2], mat[2][3],
		mat[3][1], mat[3][1], mat[3][2], mat[3][3],
	);

	

	return output;
}

template<typename T>
DirectX::XMMATRIX Matrix<T>::MatrixToXMMATRIX(const Matrix<float>& mat)
{
	return DirectX::XMLoadFloat4x4(MatrixToFloat4x4(mat));;
}

template<typename T>
Matrix<float> Matrix<T>::XMMATRIXToMatrix(const DirectX::XMMATRIX& mat)
{
	DirectX::XMFLOAT4X4 f(DirectX::XMStoreFloat4x4(mat));
	return Float4x4ToMatrix(f);
}


#pragma endregion
#endif // !MATRIX_CPP