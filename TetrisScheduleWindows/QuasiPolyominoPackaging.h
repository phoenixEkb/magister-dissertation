#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <fstream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <algorithm>
#include <boost/container/vector.hpp>
#include <boost/geometry/index/rtree.hpp>

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

typedef struct polyminoState //contains info about possible transformations of QP. Currently not in use.
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
	//possible to make them static
	int gridWidth, gridHeight;
	MultiPoint2D restrictions;
	boost::container::vector<MultiPoint2D> figures;
	//figures sizes in default state, may be unnecessary as class params.
	std::vector<int> figuresWidth, figuresHeight;
	std::vector<state> figuresStates;
	bMatrix currentStateMatrix;

	bool hasConflicts;
	std::map<int,std::vector<Point2D>> conflictingPoints;
	std::set<int> conflictFiguresNumbers;
	//Number of figure, conflicting with others through the process of updateMatrix.
	int stopStep;
	//if figure added via updateFigures without intersections, it is added into that vector. Or not.
	//boost::container::vector<MultiPoint2D> tempFigures;

	//While the figures are proceeded - this vector is used for updating stateMatrix.
	//However, we can dynamically add and remove figures from stateMatrix.

	std::vector<state> tempStates;
	
public:
	QuasiPolyominoPackaging(std::string restrictionsFile, std::string figuresFile);
	~QuasiPolyominoPackaging();
	MultiPoint2D normaliseFigure(MultiPoint2D figure, int number);

	bool changeFigure(int number, state newState); //returns false if state didn't changed
	MultiPoint2D generateFigureByState(state newState, int figureNumber);
	
	void removeFigure(int number);
	void showMatrix();
	int returnFigureNumber(Point2D coords);

	bool updateFigures(std::vector<state> newStates, int startPositon=0);//returns false if there are conflicts between figures. Presumably used for crossingover.
	std::vector<int> getConflictFiguresNumbers();
	void resolveLastConflictWithRemove(int figureToRemoveNumber);
	bool Equals(state& lhs, state& rhs);//TODO: move with structs into utility.

	//

//Deprecated
public:
	//QuasiPolyominoPackaging(const MultiPoint2D points, const MultiPoint2D restrictions, int gridWidth, int gridHeight);
	/*unsigned int getPlacementsAmount();
	bMatrix getMatrix(unsigned int number);
	polyminoState getAvailableStates(int gridWidth, int gridHeight);*///TODO: Maybe move ConfigBitset::possiblePolyominoesPositions here.
	//f, can't recall which one is correct
private:
	//bMatrix generateMatrix(state s);
	//void createMatrixByMultipoint(const MultiPoint2D & figure, bMatrix & m, unsigned int width, unsigned int height);
	//bool isMatrixCorrect(const bMatrix& m);
};

