#pragma once
#include <string>
#include <vector>
#include <random>
#include "ISolver.h"
#include "PolyominoGroup.h"
#include "ConfigBitset.h"

class GASolver :
	public ISolver
{
private:
	std::vector<PolyominoGroup> figures;
	int gridWidth, gridHeight;
	//private static int itemsAmount, dimensions;
	//private static double[, ] itemsSet;//amount of items*their dimensions
	//private static double[] restrictions;
	//private double[] itemsCosts;

	int configsInPoolAmount;
	int bestConfigsAmount;
	std::vector<ConfigBitset> configsPool;
	std::vector<ConfigBitset>  currentBestConfigs;//however, this pool is resetted over and over again. We should create a permanent pool for practical use. Possibliy
	std::vector<ConfigBitset>  bestConfigsAllTime;
	const unsigned int leastEmptyCellsPossible=0;//AKA maximalKnapsackCost 

	//private Crossover activeCrossover;
	//private Mutation activeMutation;
	std::mt19937 rand;
	 double mutationPercentage;
public:
	GASolver();
	GASolver(std::string figuresFile, std::string restrictionsFile, std::string resultFile,int confAm, /*Crossover myCrs, Mutation myMt,*/ double mutationPercentage, int bestConfigsAmount);
	~GASolver();
	void MakeIteration();
	void RestartAlgorithm(double flushPercent);

	double GetNormalizedMaximalKnapsackCost();
	double GetNormaizedAveragePoolCost();
	double GetAbsoluteMaximalKnapsackCost();
	double GetAbsoluteAverageKnapsackCost();
private:
	void startCycling();
	void emptyBestConfigs(std::vector<ConfigBitset> &targetConfig);
	void updateConfigs(std::vector<ConfigBitset> & currentPool, double tuningCoeff);
	ConfigBitset FirstApproachGenerate();
	ConfigBitset SinglePointCrossover(ConfigBitset sack1, ConfigBitset sack2, bool isLeft);
	ConfigBitset SinglePointMutation(ConfigBitset sack, std::mt19937 rand);
	bool IsValid(ConfigBitset config);
	double GetKnapsackCost(ConfigBitset sack);
	ConfigBitset MakeValid(ConfigBitset sack);
	std::vector<double> GetBestConfigsCosts();
};

