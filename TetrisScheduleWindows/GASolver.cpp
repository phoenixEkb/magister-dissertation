#include <fstream>
#include<iostream>

#include "GASolver.h"

//We create genetical algorithm with fixed way of genetics and mutation, based on previous realisation by me.

//GASolver::GASolver()
//{
//	
//}

GASolver::GASolver(std::string figuresFile, std::string restrictionsFile, int confAm, double mutationPercentage, int bestConfigsAmount)
{
	std::ifstream restrFile(restrictionsFile, std::ifstream::in);
	restrFile >> gridWidth>>gridHeight;
	std::string line = "";
	std::getline(restrFile, line);
	MultiPoint2D restrPoints;
	bg::read_wkt(line, restrPoints);

	std::ifstream inFile(figuresFile, std::ifstream::in);
	while (inFile.eof())
	{
		std::getline(inFile, line);
		MultiPoint2D p;
		bg::read_wkt(line, p);
		PolyominoGroup group(p,restrPoints,gridWidth,gridHeight);
	//	figures.push_back(group); Problem 4
	}

	this->bestConfigsAmount = bestConfigsAmount;//10 by default
	this->mutationPercentage = mutationPercentage;
	this->configsInPoolAmount = confAm;
	
	//Config2D emptyConfig(static_cast<unsigned int>(figures.size()),&restrPoints);//TODO:rewrite. Problem 5: wrong constructor
	//In Config2D realised conception of multiple int config, whenever in p-group config is assumed as structure of multiple fields. 
	//Probably, it would be better to create bit array(bitmask or sth?) as universal interface for configs

	startCycling();
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
	configsPool[0] = FirstApproachGenerate();

	int active = 0, passive = 0;
	for (int i = 0; i < figures.size(); i++)
	{
		if (configsPool[0].valueAt(i)!=0)//returns true if polyomino persists in config.
			active++;
		else passive++;
	}

	if (active == figures.size() || passive == figures.size())//all polyominio placed or no placements found.
		return;


	for (int i = 1; i < configsInPoolAmount; i++)
	{
		configsPool[i] = SinglePointMutation(configsPool[0], rand);//or use active mutation.
	}
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
