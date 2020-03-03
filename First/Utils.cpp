#include "Utils.h"

Point2D& Utils::FindMaximumInMatrix(Node maze[MSZ_UTILS][MSZ_UTILS])
{
	Point2D* maxValueLocation = new Point2D();
	int rowIndex, colIndex;
	double curValue, maxValue;

	maxValueLocation->setRow(0);
	maxValueLocation->setCol(0);
	maxValue = maze[0][0].GetValue();

	for (rowIndex = 0; rowIndex < MSZ_UTILS; rowIndex++)
	{
		for (colIndex = 0; colIndex < MSZ_UTILS; colIndex++)
		{
			curValue = maze[rowIndex][colIndex].GetValue();
			if (maxValue < curValue)
			{
				maxValueLocation->setRow(rowIndex);
				maxValueLocation->setCol(colIndex);
				maxValue = maze[rowIndex][colIndex].GetValue();
			}
		}
	}

	return *maxValueLocation;
}

Point2D& Utils::FindMinimumInMatrix(Node maze[MSZ_UTILS][MSZ_UTILS])
{
	Point2D* minValueLocation = new Point2D();
	int rowIndex, colIndex;
	double curValue , minValue;

	minValueLocation->setRow(0);
	minValueLocation->setCol(0);
	minValue = maze[0][0].GetValue();

	for (rowIndex = 0; rowIndex < MSZ_UTILS; rowIndex++)
	{
		for (colIndex = 0; colIndex < MSZ_UTILS; colIndex++)
		{
			curValue = maze[rowIndex][colIndex].GetValue();
			if (minValue > curValue)
			{
				minValueLocation->setRow(rowIndex);
				minValueLocation->setCol(colIndex);
				minValue = maze[rowIndex][colIndex].GetValue();
			}
		}
	}

	return *minValueLocation;
}