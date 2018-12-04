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
	//DEBUG
	std::cout << "Naive algorithm starting placement" << std::endl;
	configsPool[0] = ConfigSequential(figuresFile, restrictionsFile);

	rand = std::mt19937(42);//DEBUG
	configStatesLengthDistr = std::uniform_int_distribution<int>(0, configsPool[0].getStatesAmount() - 1);
	configsAmountDistr = std::uniform_int_distribution<int>(0, configsInPoolAmount);
	configOrderLengthDistr = std::uniform_int_distribution<int>(0, configsPool[0].getFiguresAmount() - 1);
	uniformDistr = std::uniform_real_distribution<double>(0, 1);

	iteration = 0;
	startCycling();
}

void GASolverSeq::makeIteration()
{
	iteration++;
	if (configsPool[0].getFreeCellsPercentage() == 0) return;
	std::vector<int> positions;

	while (positions.size() < mutationPercentage * configsInPoolAmount)
	{
		positions.push_back(configsAmountDistr(rand));
		auto last = std::unique(positions.begin(), positions.end());
		positions.erase(last, positions.end());
	}
	for (int i = 0; i < configsInPoolAmount; i++)
	{
		configsPool[i] = SinglePointMutation(configsPool[i]);
	}
	std::vector<ConfigSequential> CrossoverPool(configsInPoolAmount * 2 - 2);//not very well, if i want to customize Crossover ,but works
	for (int j = 0; j < configsInPoolAmount - 1; j++)
	{
		CrossoverPool[j] = this->BitByBitCrossover(configsPool[j], configsPool[j + 1], true);
		CrossoverPool[(CrossoverPool.size() - 1) - j] = this->BitByBitCrossover(configsPool[j], configsPool[j + 1], false);
	}
	std::sort(CrossoverPool.begin(), CrossoverPool.end(), [](ConfigSequential l, ConfigSequential r) {return l.getFreeCellsPercentage() < r.getFreeCellsPercentage(); });
	auto last = std::unique(positions.begin(), positions.end());
	positions.erase(last, positions.end());

	if (CrossoverPool.size() > configsInPoolAmount)
		for (int i = 0; i < CrossoverPool.size() - configsInPoolAmount; i++);
	CrossoverPool.pop_back();
	configsPool = CrossoverPool;
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
		configsPool[i] = SinglePointMutation(configsPool[0]);//or use active mutation.
	}
}

ConfigSequential GASolverSeq::SinglePointMutation(ConfigSequential conf)
{
	ConfigSequential mutatedSack = ConfigSequential(conf);//copy constructor
	if (this->uniformDistr(rand)<=0.33)
	{
		int mutationPosition = configStatesLengthDistr(rand);//TODO: add order-based things. Maybe -- work with elAm*4. Somehow.
		conf.swapStateBinaryValue(mutationPosition);
	}
	else
	{
		int first = configsAmountDistr(rand), second = configsAmountDistr(rand);
		while (first == second)
		{
			second = configsAmountDistr(rand);
		}
		conf.swapPositionsInOrder(first, second);
	}
	//Debug
	conf.showMatrix();
	//
	std::cout << "Mutation on " << this->iteration << " iteration" << std::endl;
	return conf;
}

ConfigSequential GASolverSeq::BitByBitCrossover(ConfigSequential conf1, ConfigSequential conf2, bool isLeft)//TODO: add order-based things
{
	ConfigSequential newSack;
	if (isLeft)
	{
		for (int i = 0; i < conf1.getStatesAmount(); i++)
		{
			if (i % 2 == 0)
				newSack.setStateBinaryValue(i, conf2.valueAt(i));
			else
				newSack.setStateBinaryValue(i, conf1.valueAt(i));
		}
	}
	else
	{
		for (int i = 0; i < conf1.getStatesAmount(); i++)
		{
			if (i % 2 == 0)
				newSack.setStateBinaryValue(i, conf1.valueAt(i));
			else
				newSack.setStateBinaryValue(i, conf2.valueAt(i));
		}
	}
	return newSack;
}

void GASolverSeq::saveResults(int poolPosition)
{
	configsPool[poolPosition].printMatrix(this->resultFile);
}
