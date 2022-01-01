#include "Grid.h"

Grid::Grid() {}
Grid::~Grid() {}

GridData Grid::GenerateGrid(int rows, int cols, float cellSpacing)
{
	unsigned int vertexCount = rows * cols;
	unsigned int indexCount = rows * cols * 6;  // Plane size * 3 verts * 2 tris per quad

	// Calculate cell spacing and half size
	cellSpacing *= 2;
	float dx = cellSpacing / (rows - 1);
	float dz = cellSpacing / (cols - 1);
	float half = 0.5f * cellSpacing;

	//
	// Calculate Vertices
	//

	std::vector<Vector3> vertices(vertexCount);
	for (int i = 0, z = 0; z < rows; z++)
	{
		for (int x = 0; x < cols; x++)
		{
			vertices[i] = Vector3(-half + (x * dx), 0.0f, half - (z * dz));
			i++;
		}
	}


	//
	//Calculate Indices
	//
	std::vector<Vector3> indices(indexCount);
	int vert = 0, tris = 0;

	for (int z = 0; z < rows; z++)
	{
		for (int x = 0; x < cols; x++)
		{
			indices[tris + 0] = vert + 0;
			indices[tris + 1] = vert + rows + 1;
			indices[tris + 2] = vert + 1;
			indices[tris + 3] = vert + 1;
			indices[tris + 4] = vert + rows + 1;
			indices[tris + 5] = vert + rows + 2;

			vert++;
			tris += 6;
		}
		vert++;
	}

	GridData gridData(vertices, indices);
	return gridData;
}
