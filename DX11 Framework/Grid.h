#pragma once
#ifndef GRID_H
#define GRID_H
#include <vector>
#include "Vector3.h"

struct GridData
{
	GridData(std::vector<Vector3> vertices, std::vector<Vector3> indices)
	{
		Vertices = vertices;
		Indices = indices;
	}

	std::vector<Vector3> Vertices;
	std::vector<Vector3> Indices;
};

class Grid
{
public:
	Grid();
	~Grid();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="xSize">Number of rows</param>
	/// <param name="zSize">Number of columns</param>
	/// <param name="cellSpacing">The distance between two vertices</param>
	/// <param name="outData"></param>
	static GridData GenerateGrid(int xSize, int zSize, float cellSpacing);

private:
};

#endif // !GRID_H