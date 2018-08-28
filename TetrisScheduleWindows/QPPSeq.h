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
enum rotation {
	right = 0,
	top = 90,
	left = 180,
	bottom = 270
};
typedef struct
{
	bool isIncluded = false;
	rotation rot = rotation::right;
	bool mirrored = false;
}stateSeq;



class QPPSeq
{
	int gridWidth, gridHeight;
	MultiPoint2D restrictions;
	boost::container::vector<MultiPoint2D> figures;
	//figures sizes in default state, may be unnecessary as class params.
	std::vector<int> figuresWidth, figuresHeight;
	std::vector<stateSeq> figuresStates;
	std::vector<int> figuresOrder;
	bMatrix currentStateMatrix;
	int placedFiguresAmount;
public:
	QPPSeq(std::string restrictionsFile, std::string figuresFile);
	~QPPSeq();
	bool changeFigure(int number, stateSeq newState);
	//returns false if state didn't changed
	void swapFigures(int firstPos, int secondPos);
	void showMatrix();
	int returnFigureNumber(Point2D coords);
	//We divide these 2 structures here and connect them in ConfigSeq
	void packFigures(std::vector<stateSeq> newStates,std::vector<int> newOrder);
	std::vector<int> getFiguresOrder();
	int getPlacedFiguresAmount();
private:
	MultiPoint2D normaliseFigure(MultiPoint2D figure, int number);
};

