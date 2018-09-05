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
	std::string resultFile;
public:
	GASolverSeq();
	~GASolverSeq();
	GASolverSeq(std::string figuresFile, std::string restrictionsFile, std::string resultFile, int confAm, double mutationPercentage, int bestConfigsAmount);
	
	void MakeIteration();
	//void RestartAlgorithm(double flushPercent);

	double GetNormalizedMaximalKnapsackCost();
	double GetNormaizedAveragePoolCost();
	double GetAbsoluteMaximalKnapsackCost();
	double GetAbsoluteAverageKnapsackCost();
private:
	void startCycling();
	//void emptyBestConfigs(std::vector<ConfigSequential> &targetConfig);
	void updateConfigs(std::vector<ConfigSequential> & currentPool, double tuningCoeff);
	ConfigSequential FirstApproachGenerate();
	ConfigSequential SinglePointCrossover(ConfigSequential sack1, ConfigSequential sack2, bool isLeft);
	ConfigSequential SinglePointMutation(ConfigSequential sack, std::mt19937 rand);
	bool IsValid(ConfigSequential config);
	double GetKnapsackCost(ConfigSequential sack);
	ConfigSequential MakeValid(ConfigSequential sack);
	std::vector<double> GetBestConfigsCosts();
};

