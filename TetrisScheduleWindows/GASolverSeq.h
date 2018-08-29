#pragma once
#include "ConfigSequential.h"
#include <random>

class GASolverSeq
{
	int gridWidth, gridHeight;
	int configsInPoolAmount;
	std::vector<ConfigSequential> configsPool;
	const unsigned int leastEmptyCellsPossible = 0;//very interesting parameter.
	std::mt19937 rand;
	double mutationPercentage;
public:
	GASolverSeq();
	~GASolverSeq();
};

