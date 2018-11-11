#include "stdafx.h"
#include <fstream>
#include<iostream>
#include "GASolver.h"

//We create genetical algorithm with fixed way of genetics and mutation, based on previous realisation by me.
//
//GASolver::GASolver(std::string figuresFile, std::string restrictionsFile, std::string resultFile, int confAm, double mutationPercentage, int bestConfigsAmount)
//{
//	std::ifstream restrFile(restrictionsFile, std::ifstream::in);
//	restrFile >> gridWidth>>gridHeight;
//	std::string line = "";
//	std::getline(restrFile, line);
//	MultiPoint2D restrPoints;
//	bg::read_wkt(line, restrPoints);
//
//	std::ifstream inFile(figuresFile, std::ifstream::in);
//	while (inFile.eof())
//	{
//		std::getline(inFile, line);
//		MultiPoint2D p;
//		bg::read_wkt(line, p);
//		QuasiPolyominoPackaging group(p,restrPoints,gridWidth,gridHeight);
//	//	figures.push_back(group); Problem 4
//	}
//
//	this->bestConfigsAmount = bestConfigsAmount;//10 by default
//	this->mutationPercentage = mutationPercentage;
//	this->configsInPoolAmount = confAm;
//	
//	//Config2D emptyConfig(static_cast<unsigned int>(figures.size()),&restrPoints);//TODO:rewrite. Problem 5: wrong constructor
//	//In Config2D realised conception of multiple int config, whenever in p-group config is assumed as structure of multiple fields. 
//	//Probably, it would be better to create bit array(bitmask or sth?) as universal interface for configs
//
//	startCycling();
//}

GASolver::~GASolver()
{
}

void GASolver::makeIteration()
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
//
//void GASolver::startCycling()
//{
//	configsPool[0] = FirstApproachGenerate();
//
//	int active = 0, passive = 0;
//	for (int i = 0; i < figures.size(); i++)
//	{
//		if (configsPool[0].valueAt(i)!=0)//returns true if polyomino persists in config.
//			active++;
//		else passive++;
//	}
//
//	if (active == figures.size() || passive == figures.size())//all polyominio placed or no placements found.
//		return;
//
//
//	for (int i = 1; i < configsInPoolAmount; i++)
//	{
//		configsPool[i] = SinglePointMutation(configsPool[0], rand);//or use active mutation.
//	}
//}


void GASolver::emptyBestConfigs(std::vector<ConfigBitset>& targetConfig)
{
}


void GASolver::updateConfigs(std::vector<ConfigBitset>& currentPool, double tuningCoeff)
{
}


ConfigBitset GASolver::FirstApproachGenerate()
{

	return ConfigBitset();
}


ConfigBitset GASolver::SinglePointCrossover(ConfigBitset sack1, ConfigBitset sack2, bool isLeft)
{

	return ConfigBitset();
}


ConfigBitset GASolver::SinglePointMutation(ConfigBitset sack, std::mt19937 rand)
{

	return ConfigBitset();
}


bool GASolver::IsValid(ConfigBitset config)
{
	return false;
}


double GASolver::GetKnapsackCost(ConfigBitset sack)
{
	return 0.0;
}


ConfigBitset GASolver::MakeValid(ConfigBitset sack)
{

	return ConfigBitset();
}


std::vector<double> GASolver::GetBestConfigsCosts()
{

	return std::vector<double>();
}
