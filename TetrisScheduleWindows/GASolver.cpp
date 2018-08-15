#include "GASolver.h"

//We create genetical algorithm with fixed way of genetics and mutation, based on previous realisation by me.
GASolver::GASolver()
{
	
}

GASolver::GASolver(std::string figuresFile, std::string restrictionsFile, int confAm, double mutationPercentage)
{
}




GASolver::~GASolver()
{
}

void GASolver::MakeIteration()
{
}

void GASolver::RestartAlgorithm(double flushPercent)
{
}

double GASolver::GetNormalizedMaximalKnapsackCost()
{
	return 0.0;
}

double GASolver::GetNormaizedAveragePoolCost()
{
	return 0.0;
}

double GASolver::GetAbsoluteMaximalKnapsackCost()
{
	return 0.0;
}

double GASolver::GetAbsoluteAverageKnapsackCost()
{
	return 0.0;
}

void GASolver::startCycling()
{
}

void GASolver::emptyBestConfigs(std::vector<Config2D>& targetConfig)
{
}

void GASolver::updateConfigs(std::vector<Config2D>& currentPool, double tuningCoeff)
{
}

Config2D GASolver::FirstApproachGenerate()
{
	return Config2D();
}

Config2D GASolver::SinglePointCrossover(Config2D sack1, Config2D sack2, bool isLeft)
{
	return Config2D();
}

Config2D GASolver::SinglePointMutation(Config2D sack, std::mt19937 rand)
{
	return Config2D();
}

bool GASolver::IsValid(Config2D config)
{
	return false;
}

double GASolver::GetKnapsackCost(Config2D sack)
{
	return 0.0;
}

Config2D GASolver::MakeValid(Config2D sack)
{
	return Config2D();
}

vector<double> GASolver::GetBestConfigsCosts()
{
	return vector<double>();
}
