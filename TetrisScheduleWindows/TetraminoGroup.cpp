#include "TetraminoGroup.h"

template <int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
TetraminoGroup<pointsAmount, restrictionsAmount, tGridWidth, tGridHeight, tWidth, tHeight>
::TetraminoGroup(const qvm::mat<int, 2, pointsAmount>& points)
{
	this->points = points;//?Not sure;
	this->groupWidth = 0;
	this->groupHeight = 0;
	//normalisation
	int xMin = INT_MAX, y = INT_MAX, xMax=INT_MIN,yMax=INT_MIN;
	
	for (int i = 0; i < pointsAmount; i++)
	{
		groupWidth = max(groupWidth, this->points.a[0][i]);
		groupHeight =max(groupHeight, this->points.a[1][i]);

	}
}

template<int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
unsigned int TetraminoGroup<pointsAmount, restrictionsAmount, tGridWidth, tGridHeight, tWidth, tHeight>
::getPlacementsAmount(const qvm::mat<int, 2, pointsAmount>& restrictions, int gridWidth, int gridHeight)
{
	 
	if (!placementsMatrixes)//?
	{
		//generate matrixes
	}
	else return placementsAmount;
	return 0;
}

template<int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
qvm::mat<int, tGridWidth, tGridHeight> TetraminoGroup<pointsAmount, restrictionsAmount, tGridWidth, tGridHeight, tWidth, tHeight>
::getMatrix(int number)
{
	return qvm::mat<int, width, height>();
}

template<int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
TetraminoGroup<pointsAmount, restrictionsAmount, tGridWidth, tGridHeight, tWidth, tHeight>
::~TetraminoGroup()
{
}

template<int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
TetraminoGroup<pointsAmount, restrictionsAmount, tGridWidth, tGridHeight, tWidth, tHeight>
::TetraminoGroup()
{
}

template<int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
qvm::mat<int, tGridWidth, tGridHeight> TetraminoGroup<pointsAmount, restrictionsAmount, tGridWidth, tGridHeight, tWidth, tHeight>
::getMatrix(int xCoord, int yCoord, int rotation, bool mirrored)
{
	return qvm::mat<int, width, height>();
}


