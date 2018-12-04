#include "ConfigSequential.h"
#include "stdafx.h"



//ConfigSequential::ConfigSequential()
//{
//}
//
//
//ConfigSequential::~ConfigSequential()
//{
//}

void ConfigSequential::swapStateBinaryValue(int position)
{
	if (position >= 0 && position < statesAmount)
	{
		currentStatesConfiguration[position].flip();
		QPPsUpdated = false;
	}
}

void ConfigSequential::setStateBinaryValueToActive(int position)
{
	if (position >= 0 && position < statesAmount)
	{
		currentStatesConfiguration[position] = true;
		QPPsUpdated = false;
	}
}

void ConfigSequential::setStateBinaryValueToPassive(int position)
{
	if (position >= 0 && position < statesAmount)
	{
		currentStatesConfiguration[position] = false;
		QPPsUpdated = false;
	}
}
//This method replaces methods above, can refactor
void ConfigSequential::setStateBinaryValue(int position, bool value)
{
	if (position >= 0 && position < statesAmount)
	{
		currentStatesConfiguration[position] = value;
		QPPsUpdated = false;
	}
}

void ConfigSequential::swapPositionsInOrder(int pos1, int pos2)//TODO:MINOR:maybe rewrite with find?
{
	if (pos1 >= 0 && pos2 >= 0 && pos1 < figuresAmount&&pos2 < figuresAmount)
	{
		int loc1 = -1, loc2 = -1;
		for (int i = 0; i < figuresAmount; ++i)
		{
			if (currentOrder[i] == pos1)
				loc1 = i;
			if (currentOrder[i] == pos2)
				loc2 = i;
			if (loc1 != -1 && loc2 != -1)
			{
				std::swap(currentOrder[loc1], currentOrder[loc2]);
			}
		}
		QPPsUpdated = false;
	}

}

ConfigSequential::ConfigSequential()
{

}

ConfigSequential::ConfigSequential(std::string figuresFile, std::string restrictionsFile)
{
	QPPs = QPPSeq(figuresFile, restrictionsFile);
	figuresAmount = QPPs.getFiguresAmount();
	statesAmount = figuresAmount * 3;
	currentStatesConfiguration = boost::dynamic_bitset<>(statesAmount);
	this->currentOrder = QPPs.getFiguresOrder();
	auto states = QPPs.getFiguresStates();
	for (int i = 0; i < states.size(); i++)
	{
		setBitsForConfig(i, states[i]);
	}
	QPPsUpdated = false;
}

ConfigSequential::~ConfigSequential()
{
}

std::vector<stateSeq> ConfigSequential::generateStatesByConfig()
{
	std::vector<stateSeq> states(figuresAmount);
	for (int i = 0; i < figuresAmount; i++)
	{
		states[i].mirrored = currentStatesConfiguration[3 * i];
		if (currentStatesConfiguration[3 * i + 1])
		{
			if (currentStatesConfiguration[3 * i + 2])
				states[i].rot = top;
			else
				states[i].rot = left;
		}
		else
		{
			if (currentStatesConfiguration[3 * i + 2])
				states[i].rot = bottom;
			else
				states[i].rot = right;
		}
	}
	return states;
}

int ConfigSequential::getFreeCellsAmount()
{
	if (!QPPsUpdated)
		updateQPPs();
	return QPPs.getFreeArea();
}

double ConfigSequential::getFreeCellsPercentage()
{
	if (!QPPsUpdated)
		updateQPPs();
	return (double)QPPs.getFreeArea() / (double)(QPPs.getGridArea() - QPPs.getRestrictionsArea());
}

void ConfigSequential::setBitsForConfig(int number, stateSeq st)
{
	if (st.mirrored)
	{
		currentStatesConfiguration[number * 3] = 1;
	}
	else
		currentStatesConfiguration[number * 3] = 0;
	switch (st.rot)
	{
	case right:
	{
		currentStatesConfiguration[number * 3 + 1] = 0;
		currentStatesConfiguration[number * 3 + 2] = 0;
		break;
	}
	case bottom:
	{
		currentStatesConfiguration[number * 3 + 1] = 0;
		currentStatesConfiguration[number * 3 + 2] = 1;
		break;
	}
	case left:
	{
		currentStatesConfiguration[number * 3 + 1] = 1;
		currentStatesConfiguration[number * 3 + 2] = 0;
		break;
	}
	case top:
	{
		currentStatesConfiguration[number * 3 + 1] = 1;
		currentStatesConfiguration[number * 3 + 2] = 1;
		break;
	}
	}

}

void ConfigSequential::updateQPPs()
{
	if (QPPsUpdated) return;
	auto states = generateStatesByConfig();
	QPPs.packFigures(states, currentOrder);
	QPPsUpdated = true;
}

int ConfigSequential::getStatesAmount()//TODO:check where is used, probably can return CCsize+COsize
{
	return currentStatesConfiguration.size();
}

int ConfigSequential::getFiguresAmount()
{
	return this->figuresAmount;
}

//For debugging purposes
void ConfigSequential::showMatrix()
{
	if (!QPPsUpdated)
		updateQPPs();
	this->QPPs.showMatrix();
}

void ConfigSequential::printMatrix(std::string resFile)
{
	if (!QPPsUpdated)
		updateQPPs();
	this->QPPs.printMatrix(resFile);
}

bool ConfigSequential::valueAt(int position)
{
	return currentStatesConfiguration[position];
}
