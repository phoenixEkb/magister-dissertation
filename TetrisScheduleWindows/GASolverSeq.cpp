#include "GASolverSeq.h"



GASolverSeq::GASolverSeq()
{
}


GASolverSeq::~GASolverSeq()
{
}

GASolverSeq::GASolverSeq(std::string figuresFile, std::string restrictionsFile, std::string resFile, int confAm, double mutationPercentage, int bestConfigsAmount)
{
	this->resultFile = resFile;
	this->mutationPercentage = mutationPercentage;
	std::ifstream restrFile(restrictionsFile, std::ifstream::in);
	restrFile >> gridWidth >> gridHeight;
	restrFile.close();
	this->configsInPoolAmount = confAm;
	this->configsPool = std::vector<ConfigSequential>(configsInPoolAmount);
	configsPool[0] = ConfigSequential(figuresFile, restrictionsFile);
	rand = std::mt19937(42);
	distribution = std::uniform_int_distribution<int>(0, configsPool[0].length() - 1);
	startCycling();
}

void GASolverSeq::startCycling()
{
	//UNDONE: add some checks in here for the case of accidental solving of the problem with default config.
	
	if (configsPool[0].getFreeCellsPercentage() == 0)
	{
		std::cout << "Optimal configuration found before GA started" << std::endl;
		saveResults(0);
		return;
	}
	for (int i = 1; i < configsInPoolAmount; i++)
	{
		configsPool[i] = SinglePointMutation(configsPool[0], rand);//or use active mutation.
	}
}

ConfigSequential GASolverSeq::SinglePointMutation(ConfigSequential sack, std::mt19937 rand)
{

	ConfigSequential mutatedSack = ConfigSequential(sack);//copy constructor
	int mutationPosition = distribution(rand);
	sack.swapValue(mutationPosition);
	sack.updateQPPs();
	sack.showMatrix();
	//var count = 0;
	//while (mutatedSack.Equals(sack) && count < 1000000)//TODO - not mutate empty sack
	//{
	//	mutatedSack.swapValue(mutationPosition);
	//	if (!IsValid(mutatedSack))
	//	{
	//		mutatedSack.swapValue(mutationPosition);
	//		mutationPosition = rand.Next(itemsAmount);
	//		count++;
	//	}
	//}
	//if (count == 1000000)
	//{
	//	return MakeValid(mutatedSack);
	//}
	return sack;
}

void GASolverSeq::saveResults(int poolPosition)
{
	//UNDONE
}
