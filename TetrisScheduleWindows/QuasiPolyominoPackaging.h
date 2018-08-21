#pragma once
#include "stdafx.h"
#include <vector>
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
	std::vector<int> figuresWidth, figuresHeight;//in default state;
	std::vector<state> figuresStates;
	bMatrix currentStateMatrix;

	bool hasConflicts;
	std::vector<Point2D> conflictingPoints;
	std::pair<int,int> conflictFiguresNumbers;//2 figures only;
	boost::container::vector<MultiPoint2D> tempFigures;//if figure added via updateFigures without intersections, it is added into that vector. 
	std::vector<state> tempStates;
	//While all the figures are proceeded - this vector is used for updating stateMatrix
	//It is possible to check on whichfigure we have stopped - it is second argument of confFigNum
public:
	QuasiPolyominoPackaging(std::string restrictionsFile, std::string figuresFile);
	~QuasiPolyominoPackaging();
	MultiPoint2D normaliseFigure(MultiPoint2D figure, int number);
	//may be useful to make this 2 functions boolean to check, if they led to intersection
	
	//void AddFigure(int number, state newState);//
	bool changeFigure(int number, state newState);
	MultiPoint2D generateFigureByState(state newState, int figureNumber);
	//returns false if state didn't changed
	void removeFigure(int number);
	void showMatrix();
	int returnFigureNumber(Point2D coords);

	bool updateFigures(std::vector<state> newStates);//returns false if there are conflicts between figures. Presumably used for crossingover.
	std::pair<int, int> getConflictFiguresNumbers();
	void resolveLastConflictWithRemove(int figureToRemoveNumber);
	//UpdateMatrix?
	bool Equals(state& lhs, state& rhs);//TODO: move with structs into utility.

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

