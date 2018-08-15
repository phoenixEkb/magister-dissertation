#include "PolyominoGroup.h"
#include <algorithm>
namespace bg = boost::geometry;
namespace trans = bg::strategy::transform;


//Problem 2, appears in these 3 functions

PolyominoGroup::PolyominoGroup(const MultiPoint2D points, const MultiPoint2D restrictions, int gridWidth, int gridHeight)
{
	this->restrictions=restrictions;//?Not sure;
	//createMatrixByMultipoint(restrictions, restrictMatrix, gridWidth, groupHeight);  //Problem string
	////calculating figure sizes and normalisation
	int xMin = INT_MAX, yMin = INT_MAX, xMax=INT_MIN,yMax=INT_MIN;
	for (int i = 0; i < bg::num_points(points); i++)
	{
		xMax = std::max(points[i].get<0>(),xMax);
		yMax = std::max(points[i].get<1>(), yMax);
		xMin = std::min(points[i].get<0>(), xMin);
		yMin = std::min(points[i].get<1>(), yMin);
	}
	groupWidth = 1 + xMax - xMin;
	groupHeight = 1 + yMax - yMin;
	trans::translate_transformer<int, 2, 2> normTranslation(-xMin , -yMin);
	bg::transform(points, this->points, normTranslation);
}

//
unsigned int PolyominoGroup::getPlacementsAmount()
{
	if (placementsMatrixes.empty())//
	{
		//generate matrixes, assuming we're using 3 types of basic transformations
		for (int i = 0; i < gridWidth - groupWidth; ++i)
		{
			for (int j = 0; j < gridHeight - groupHeight; ++j)//TODO: check condition on j and i
			{
				for (int rot = rotation::right; rot < 360; rot += 90)//That's dirty
				{
					for (bool mir : { false, true })
					{
						state s = { i, j, static_cast<rotation>(rot), mir };
						//matrix t = generateMatrix(s); problem: can't use this function
						//	if (isMatrixCorrect(t))//problem in this 2 strings
											//placementsMatrixes.push_back(t);					
					}
				}
			}
		}
	}
	else return placementsMatrixes.size();
	return 0;
}

//matrix PolyominoGroup::getMatrix(unsigned int number)
//{
//	return placementsMatrixes[number];
//}

PolyominoGroup::~PolyominoGroup()
{
}

PolyominoGroup::PolyominoGroup()
{
}

//returns false if matrix has crosses w/ restrictions matrix;
bool PolyominoGroup::isMatrixCorrect(const matrix  m)
{
	for (int i = 0; i < gridWidth; i++)
	{
		for (int j = 0; j < gridHeight; j++)
		{
			if (m(i, j) && restrictMatrix(i, j) == 1) return false;
		}
	}
	return true;
}


//Problem 3: usage of bg::transform stops building.
//Problem 4: can't return matrix
matrix PolyominoGroup::generateMatrix(state s)//todo: rewrite rotation w/ angle, rewrite as void function;
{
	if (s.xCoord<0||s.xCoord>=gridWidth-groupWidth
		||s.yCoord<0||s.yCoord>=gridHeight-groupHeight
		||s.rot < 0 || s.rot >=4 )//TODO: rewrite coordinates condition for rotated fugure
	{
		std::cout << "error matrix" << std::endl;
	}
	else
	{
		//first we generate figure, then create matrix;
		MultiPoint2D t(points);
		if (s.mirrored)
		{
			trans::scale_transformer<int,2,2> xMirror(-1, 1);
			//bg::transform(t, xMirror);Problem 3
		}
		if (s.rot!= rotation::right)
		{
			trans::rotate_transformer<bg::degree, int, 2, 2> rotate(s.rot);//TODO:check if works correctly
			//bg::transform(t, rotate);Problem 3
		}
		if (s.xCoord != 0 || s.yCoord != 0)
		{
			trans::translate_transformer<int, 2, 2> move(s.xCoord, s.yCoord);
			//bg::transform(t, move);Problem 3
		}
		matrix m = zeroMatrix(gridWidth, gridHeight);
		for (auto &p: points)
		{
			m(p.get<0>(), p.get<1>())= 1;
		}
		//return m;//Problem 4
	}
	
	return matrix();
}

//Problem 1.

//void PolyominoGroup::createMatrixByMultipoint(const MultiPoint2D figure, matrix m, unsigned int width, unsigned int height)
//{
//	m = zeroMatrix(width, height);//Problem string
//	
//	for (auto p:figure)
//	{
//		m(p.get<0>(), p.get<1>()) = 1;
//	}
//}


