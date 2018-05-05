#pragma once

#include <array>

#include "Configuration.h"

class Problem
{
	std::vector<Configuration> configs;
	std::vector<bool> currentStates;//current binary "genotype"
	std::vector<void*> transforms;
	std::vector<int> configsOrder;
	std::vector<int> configsPositions;

public:
	Problem();
	~Problem();
	Problem(std::vector<Configuration> &newConfigs, std::vector<void*> &newTransforms);
	bool checkComposition(std::vector<int>& comp);
	bool tryToPlaceConfigInPosition(unsigned int configurationNumber, unsigned int possiblePosition);
	int solveLinear();
	int solveRandom();
	int solveGreedy();
};

