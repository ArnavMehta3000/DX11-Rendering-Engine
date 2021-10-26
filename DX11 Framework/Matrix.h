#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <Windows.h>

template <typename T> class Matrix
{
private:
	std::vector<std::vector<T>> mat;
	unsigned rows, cols;

public:
	Matrix(unsigned _rows, unsigned _cols, const T& _initial);
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
};

#include "Matrix.cpp"
#endif // !MATRIX_H