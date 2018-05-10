#include "PolyominoGroup.h"
#include <algorithm>
namespace bg = boost::geometry;
namespace trans = bg::strategy::transform;

PolyominoGroup::PolyominoGroup(const MultiPoint2D& points, const MultiPoint2D& restrictions, int gridWidth, int gridHeight)
{
	this->restrictions=restrictions;//?Not sure;
	createMatrixByMultipoint(restrictions, restrictMatrix, gridWidth, groupHeight);
	//calculating figure sizes and normalisation
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

unsigned int PolyominoGroup::getPlacementsAmount()
{
	if (placementsMatrixes.empty())//
	{
		//generate matrixes, assuming we're using 3 types of basic transformations
		for (int i = 0; i < gridWidth-groupWidth; ++i)
		{
			for (int j = 0; j <gridHeight-groupHeight ; ++j)//TODO: check condition on j and i
			{
				for (int rot = 0; rot < 4; rot++)
				{
					for (bool mir : { false, true })
					{
						matrix t = generateMatrix(i, j, rot, mir);
						if (isMatrixCorrect(t))
							placementsMatrixes.push_back(t);
					}
				}
			}
		}
	}
	else return placementsMatrixes.size();
	return 0;
}

matrix PolyominoGroup::getMatrix(unsigned int number)
{
	return placementsMatrixes[number];
}

PolyominoGroup::~PolyominoGroup()
{
}

PolyominoGroup::PolyominoGroup()
{
}

//returns false if matrix has crosses w/ restrictions matrix;
bool PolyominoGroup::isMatrixCorrect(const matrix & m)
{
	return false;
}

matrix PolyominoGroup::generateMatrix(int xCoord, int yCoord, int rotation, bool mirrored)//todo: rewrite rotation w/ angle, rewrite as void function;
{
	if (xCoord<0||xCoord>=gridWidth-groupWidth||yCoord<0||yCoord>=gridHeight-groupHeight||rotation < 0 || rotation >=4 );
	{
		//first we generate figure, then create matrix;
		MultiPoint2D t(points);
		if (mirrored)
		{

		}
		if (rotation != 0)
		{

		}
		if (xCoord != 0 || yCoord != 0)
		{

		}
	}
	std::cout << "error matrix" << std::endl;
}

void PolyominoGroup::createMatrixByMultipoint(const MultiPoint2D& figure, matrix &m, unsigned int width, unsigned int height)
{
	m = zeroMatrix(width, height);
	for (auto p:figure)
	{
		m(p.get<0>(), p.get<1>()) = 1;
	}
}


