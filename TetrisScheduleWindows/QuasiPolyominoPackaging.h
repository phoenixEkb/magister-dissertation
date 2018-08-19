#pragma once
#include "stdafx.h"
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/strategies/strategies.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <algorithm>

namespace bg = boost::geometry;
namespace trans = bg::strategy::transform;

typedef bg::model::point<int, 2, boost::geometry::cs::cartesian> Point2D;
typedef bg::model::multi_point<Point2D> MultiPoint2D;

typedef boost::numeric::ublas::matrix<int> bMatrix;
typedef boost::numeric::ublas::matrix<int> zeroMatrix;

//using namespace std;
enum rotation {
	right = 0,
	top = 90,
	left = 180,
	bottom = 270
};
typedef
struct polyminoState
{
	unsigned int maxDimension;//(max of xwidth and ywidth)
	int possibleRotationsNumber;//1,2,4 - depends of symmetries amount
	bool mirrorable;//if has <2 symmetry
} polyminoState;


 typedef struct 
{
	int xCoord = 0;
	int yCoord = 0;
	rotation rot=rotation::right;
	bool mirrored = false;

} state;

 //TODO:Rename to Quasipolyomino
class QuasiPolyominoPackaging
{
	int groupWidth, groupHeight, gridWidth, gridHeight;
	MultiPoint2D points, restrictions;
	std::vector<bMatrix> placementsMatrixes;
	bMatrix restrictMatrix;
public:

	QuasiPolyominoPackaging(const MultiPoint2D points, const MultiPoint2D restrictions, int gridWidth, int gridHeight);
	unsigned int getPlacementsAmount();

	bMatrix getMatrix(unsigned int number);
	~QuasiPolyominoPackaging();

	polyminoState getAvailableStates(int gridWidth, int gridHeight);//TODO: Maybe move ConfigBitset::possiblePolyominoesPositions here.
//f, can't recall which one is correct
private:
	bMatrix generateMatrix(state s);
	void createMatrixByMultipoint(const MultiPoint2D & figure, bMatrix & m, unsigned int width, unsigned int height);
	QuasiPolyominoPackaging();

	bool isMatrixCorrect(const bMatrix& m);
};

