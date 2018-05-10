#pragma once
#include <vector>
#include <boost/geometry.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace bg = boost::geometry;
typedef bg::model::point<int, 2, boost::geometry::cs::cartesian> Point2D;
typedef bg::model::multi_point<Point2D> MultiPoint2D;
typedef boost::numeric::ublas::matrix<int> matrix;
class PolyominoGroup
{
	int groupWidth, groupHeight, gridWidth, gridHeight, placementsAmount;
	MultiPoint2D points;
	std::vector<matrix> placementsMatrixes;
public:
	PolyominoGroup(MultiPoint2D points);
	unsigned int getPlacementsAmount(const MultiPoint2D& restrictions, int gridWidth, int gridHeight);
	matrix getMatrix(int number);
	~PolyominoGroup();
private:
	matrix getMatrix(int xCoord, int yCoord, int rotation, bool mirrored);//TODO make in future, if current approach fails.
	PolyominoGroup();
};

