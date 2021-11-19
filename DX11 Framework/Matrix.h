#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXMath.h>
#include <math.h>
#include <string>
#include <sstream>
#include <Windows.h>

template <typename T> class Matrix
{
private:
	std::vector<std::vector<T>> mat;
	unsigned int rows, cols;

public:
	Matrix(unsigned int _rows, unsigned int _cols, const T& _initial);
	Matrix(const Matrix<T>& rhs);
	virtual ~Matrix();

	Matrix<T>& operator=(const Matrix<T>& rhs);
	Matrix<T> operator+(const Matrix<T>& rhs);
	Matrix<T>& operator+=(const Matrix<T>& rhs);
	Matrix<T> operator-(const Matrix<T>& rhs);
	Matrix<T>& operator-=(const Matrix<T>& rhs);
	Matrix<T> operator*(const Matrix<T>& rhs);
	std::vector<T> operator*(const std::vector<T>& rhs);
	Matrix<T>& operator*=(const Matrix<T>& rhs);

	Matrix<T> operator+(const T& rhs);
	Matrix<T> operator-(const T& rhs);
	Matrix<T> operator*(const T& rhs);
	Matrix<T> operator/(const T& rhs);


	Matrix<T> Transpose();
	std::vector<T> DiagonalVec();

	const T& operator()(const unsigned& row, const unsigned& col) const;  // Access operator

	unsigned GetRowSize() const;  // Get rows size
	unsigned GetColSize() const;  // Get cols size

	void Debug(std::string msg = "");
	void Debug(unsigned row, unsigned col, std::string msg = "");

	static Matrix<float> Float4x4ToMatrix(const DirectX::XMFLOAT4X4& other);
	static DirectX::XMFLOAT4X4 MatrixToFloat4x4(const Matrix<float>& mat);

	static DirectX::XMMATRIX MatrixToXMMATRIX(const Matrix<float>& mat);
	static Matrix<float> XMMATRIXToMatrix(const DirectX::XMMATRIX& mat);
};

#include "Matrix.cpp"
#endif // !MATRIX_H
