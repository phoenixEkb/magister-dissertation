#pragma once
#include "ConfigSequential.h"
#include <vector>
#include <random>
#include <algorithm>

class GASolverSeq
{
	int gridWidth, gridHeight;
	int configsInPoolAmount;
	std::vector<ConfigSequential> configsPool;
	const unsigned int leastEmptyCellsPossible = 0;//very interesting parameter.
	std::mt19937 rand;
	std::uniform_int_distribution<int> configLengthDistribution,configAmountsDistribution;
	double mutationPercentage;
	std::string resultFile;

	int iterations;
public:
	GASolverSeq();
	~GASolverSeq();
	GASolverSeq(std::string figuresFile, std::string restrictionsFile, std::string resultFile, int confAm, double mutationPercentage, int bestConfigsAmount);
	
	void makeIteration();
	//void RestartAlgorithm(double flushPercent);

	/*double GetNormalizedMaximalKnapsackCost();
	double GetNormaizedAveragePoolCost();
	double GetAbsoluteMaximalKnapsackCost();
	double GetAbsoluteAverageKnapsackCost();*/
private:
	void startCycling();
	ConfigSequential SinglePointMutation(ConfigSequential sack);
	//void emptyBestConfigs(std::vector<ConfigSequential> &targetConfig);
	ConfigSequential BitByBitCrossover(ConfigSequential sack1, ConfigSequential sack2, bool isLeft);
	void saveResults(int poolPosition);
};

