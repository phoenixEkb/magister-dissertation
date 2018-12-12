#pragma once
#include "stdafx.h"
#include "ConfigSequential.h"
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <execution>


class GASolverSeq
{
	int gridWidth, gridHeight;
	int configsInPoolAmount;
	std::vector<ConfigSequential> configsPool;
	const unsigned int leastEmptyCellsPossible = 0;//very interesting parameter.
	std::mt19937 rand;
	std::uniform_int_distribution<int> configStatesLengthDistr,configOrderLengthDistr,configsAmountDistr;
	std::uniform_real_distribution<double> uniformDistr;
	double mutationPercentage;
	std::string resultFile;

	int iteration;
public:
	GASolverSeq();
	~GASolverSeq();
	GASolverSeq(std::string figuresFile, std::string restrictionsFile, std::string resultFile, int confAm, double mutationPercentage, int bestConfigsAmount);
	
	void makeIteration();
	//void RestartAlgorithm(double flushPercent);

	double GetNormalizedMaximalConfigCost();
	double GetNormaizedConfigPoolCost();
	/*double GetAbsoluteMaximalKnapsackCost();
	double GetAbsoluteAverageKnapsackCost();
	*/void saveResults(int poolPosition);
	void showMatrix(int position);
private:
	void startCycling();
	ConfigSequential SinglePointMutation(ConfigSequential sack);
	//void emptyBestConfigs(std::vector<ConfigSequential> &targetConfig);
	ConfigSequential BitByBitCrossover(ConfigSequential sack1, ConfigSequential sack2, bool isLeft);

};

