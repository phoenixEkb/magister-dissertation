#pragma once
#include <vector>
#include <boost/geometry.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace bg = boost::geometry;
typedef bg::model::point<int, 2, boost::geometry::cs::cartesian> Point2D;
typedef bg::model::multi_point<Point2D> MultiPoint2D;
typedef boost::numeric::ublas::matrix<int> matrix;
typedef boost::numeric::ublas::matrix<int> zeroMatrix;
using namespace std;
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

 struct state
{
	int xCoord = 0;
	int yCoord = 0;
	rotation rot=rotation::right;
	bool mirrored = false;
} ;

 //TODO:Rename to Quasipolyomino
class PolyominoGroup
{
	int groupWidth, groupHeight, gridWidth, gridHeight;
	MultiPoint2D points, restrictions;
	std::vector<matrix> placementsMatrixes;
	matrix restrictMatrix;
public:

	PolyominoGroup(const MultiPoint2D& points, const MultiPoint2D& restrictions, int gridWidth, int gridHeight);
	unsigned int getPlacementsAmount();
	matrix getMatrix(unsigned int number);
	~PolyominoGroup();
	polyminoState getAvailableStates(int gridWidth, int gridHeight);//TODO: Maybe move ConfigBitset::availablePolyominoesPositions here.
private:
	matrix generateMatrix(state s);
	void createMatrixByMultipoint(const MultiPoint2D & figure, matrix & m, unsigned int width, unsigned int height);
	PolyominoGroup();
	bool isMatrixCorrect(const matrix& m);
};

