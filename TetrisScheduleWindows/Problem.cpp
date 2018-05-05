#include "stdafx.h"
#include "Problem.h"
//disable debug for boost ublas

Problem::Problem()
{
}


Problem::~Problem()
{
}

Problem::Problem(std::vector<Configuration> &newConfigs, std::vector<void*> &newTransforms)
{
	configs = newConfigs;
	transforms = newTransforms;
	int maxLength = 0;
	for (auto conf : configs)//TODO:rewrite w/ normalisation/antinormalisation
		maxLength += conf.size();
	currentStates = std::vector<bool>(maxLength);
	configsOrder = std::vector<int>(configs.size());
}

bool Problem::checkComposition(std::vector<int> &comp)//what about states? comp.lengt
{
	//for (auto x: conf)

	return false;

}

bool Problem::tryToPlaceConfigInPosition(unsigned int configurationNumber, unsigned int possiblePosition)
{
	unsigned int innerPosition = 0;
	for (auto i = 0; i < possiblePosition; ++i)
		innerPosition += configs[configsOrder[i]].size();
	unsigned int endPosition;
	for (auto i = 0; i < configs[configurationNumber].size(); i++)
	{
		if (configs[configurationNumber][i] == true)
			if (currentStates[innerPosition + i] == true)
				//TODO: finish it!
				return false;
	}
	return false;
}

int Problem::solveLinear()//returns the amount of empty cells for current problem
{
	for (auto i = 0; i < configs.size(); i++)
		configsOrder[i] = i;
	return 0;
}


