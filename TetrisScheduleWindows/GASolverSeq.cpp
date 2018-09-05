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
	restrFile >> gridWidth>>gridHeight;
	restrFile.close();
	rand=std::mt19937();
	this->configsInPoolAmount = confAm;
	this->configsPool = std::vector<ConfigSequential>(configsInPoolAmount);
	for (int i = 0; i < configsInPoolAmount; ++i)
	{
		ConfigSequential conf(figuresFile, restrictionsFile);
		configsPool[i] = conf;
	}
	//startCycling();
}
