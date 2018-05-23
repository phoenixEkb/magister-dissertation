#pragma once
#include <string>
#include <vector>
#include <random>
#include "ISolver.h"
#include "PolyominoGroup.h"
#include "Config2D.h"

class GASolver :
	public ISolver
{
private:
	vector<PolyominoGroup> figures;
	int gridWidth, gridHeight;
	//private static int itemsAmount, dimensions;
	//private static double[, ] itemsSet;//amount of items*their dimensions
	//private static double[] restrictions;
	//private double[] itemsCosts;

	int configsInPoolAmount;
	int bestConfigsAmount;
	std::vector<Config2D> configsPool;
	std::vector<Config2D>  currentBestConfigs;//however, this pool is resetted over and over again. We should create a permanent pool for practical use. Possibliy
	std::vector<Config2D>  bestConfigsAllTime;
	const unsigned int leastEmptyCellsPossible=0;//AKA maximalKnapsackCost 

	//private Crossover activeCrossover;
	//private Mutation activeMutation;
	std::mt19937 rand;
	 double mutationPercentage;
public:
	GASolver();
	GASolver(std::string figuresFile, std::string restrictionsFile,int confAm, /*Crossover myCrs, Mutation myMt,*/ double mutationPercentage, int bestConfigsAmount);
	~GASolver();
	void MakeIteration();
	void RestartAlgorithm(double flushPercent);

	double GetNormalizedMaximalKnapsackCost();
	double GetNormaizedAveragePoolCost();
	double GetAbsoluteMaximalKnapsackCost();
	double GetAbsoluteAverageKnapsackCost();
private:
	void startCycling();
	void emptyBestConfigs(std::vector<Config2D> &targetConfig);
	void updateConfigs(std::vector<Config2D> & currentPool, double tuningCoeff);
	Config2D FirstApproachGenerate();
	Config2D SinglePointCrossover(Config2D sack1, Config2D sack2, bool isLeft);
	Config2D SinglePointMutation(Config2D sack, std::mt19937 rand);
	bool IsValid(Config2D config);
	double GetKnapsackCost(Config2D sack);
	Config2D MakeValid(Config2D sack);
	vector<double> GetBestConfigsCosts();
};

