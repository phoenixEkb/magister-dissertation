#pragma once
#include "stdafx.h"
#include <vector>
#include <utility>
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
	bool isIncluded=false;
	int xCoord = 0;
	int yCoord = 0;
	rotation rot=rotation::right;
	bool mirrored = false;

} state;

class QuasiPolyominoPackaging
{
	static int gridWidth, gridHeight;
	static MultiPoint2D restrictions;
	static std::vector<MultiPoint2D> figures;
	static std::vector<int> figuresWidth, figuresHeight;//in default state;
	std::vector<state> figuresStates;

	bMatrix currentStateMatrix;
	bool hasConflicts;
	std::vector<Point2D> conflictingPoints;
	std::vector<int> conflictFiguresNumbers;//2 figures only;
public:
	QuasiPolyominoPackaging(int gridWidth, int gridHeight, MultiPoint2D restrictions, std::vector<MultiPoint2D> figures);
	MultiPoint2D normaliseFigures(MultiPoint2D figure);
	//may be useful to make this 2 functions boolean to check, if they led to intersection
	void AddFigure(int number, state newState);
	void ChangeFigure(int number, state newState);
	
	void removeFigure(int number);
	void showMatrix();
	int returnFigureNumber(Point2D coords);
	bool updateFigures(std::vector<state> newStates);
	std::pair<int, int> getConflictFiguresNumbers();



//Deprecated
#pragma region deprecated

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
#pragma endregion

};
