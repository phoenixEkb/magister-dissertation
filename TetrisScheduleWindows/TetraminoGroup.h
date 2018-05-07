#pragma once
#include <boost/qvm/all.hpp>
#include <vector>
namespace qvm = boost::qvm;
template <int pointsAmount, int restrictionsAmount, int tGridWidth, int tGridHeight, int tWidth, int tHeight>
class TetraminoGroup
{
	int groupWidth, groupHeight, gridWidth, gridHeight, placementsAmount;
	qvm::mat<int, 2, pointsAmount> points;
	std::vector<qvm::mat<int, tGridWidth, tGridHeight>> placementsMatrixes;
public:
	TetraminoGroup(const qvm::mat<int, 2, pointsAmount>& points);
	unsigned int getPlacementsAmount(const qvm::mat<int, 2, pointsAmount>& restrictions, int gridWidth, int gridHeight);
	qvm::mat<int, tGridWidth, tGridHeight> getMatrix(int number);
	~TetraminoGroup();
private:
	qvm::mat<int, tGridWidth, tGridHeight> getMatrix(int xCoord, int yCoord, int rotation, bool mirrored);//TODO make in future, if current approach fails.
	TetraminoGroup();

};

