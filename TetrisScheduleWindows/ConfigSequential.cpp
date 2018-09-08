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

void ConfigSequential::swapValue(int position)
{
	if (position >= 0 && position < elementsAmount)
	{
		currentConfiguration[position].flip();
	}
}

void ConfigSequential::setValueToActive(int position)
{
	if (position >= 0 && position < elementsAmount)
	{
		currentConfiguration[position]=1;
	}
}

void ConfigSequential::setValueToPassive(int position)
{
	if (position >= 0 && position < elementsAmount)
	{
		currentConfiguration[position] = 0;
	}
}

void ConfigSequential::swapPositionsInOrder(int pos1, int pos2)//maybe rewrite with find?
{
	if(pos1>=0&&pos2>=0&&pos1<figuresAmount&&pos2<figuresAmount)
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
	}
}

ConfigSequential::ConfigSequential()
{

}

ConfigSequential::ConfigSequential(std::string figuresFile, std::string restrictionsFile)
{
	QPPs = QPPSeq(figuresFile, restrictionsFile);
	figuresAmount = QPPs.getFiguresAmount();
	elementsAmount = figuresAmount * 3;
	currentConfiguration = boost::dynamic_bitset<>(elementsAmount);
	this->currentOrder = QPPs.getFiguresOrder();
	auto states = QPPs.getFiguresStates();
	for (int i = 0; i < states.size(); i++)
	{
		setBitsForConfig(i, states[i]);
	}
}

ConfigSequential::~ConfigSequential()
{
}

std::vector<stateSeq> ConfigSequential::generateStatesByConfig()
{
	std::vector<stateSeq> states(figuresAmount);
	for (int i = 0; i < figuresAmount; i++)
	{
		states[i].mirrored = currentConfiguration[3 * i];
		if (currentConfiguration[3 * i + 1])
		{
			if (currentConfiguration[3 * i + 2])
				states[i].rot = top;
			else
				states[i].rot = left;
		}
		else
		{
			if (currentConfiguration[3 * i + 2])
				states[i].rot = bottom;
			else
				states[i].rot = right;
		}
	}
	return states;
}

int ConfigSequential::getFreeCellsAmount()
{
	return QPPs.getFreeArea();
}

int ConfigSequential::getFreeCellsPercentage()
{
	return QPPs.getFreeArea()/(QPPs.getGridArea()-QPPs.getRestrictionsArea());
}

void ConfigSequential::setBitsForConfig(int number, stateSeq st)
{
	if (st.mirrored)
	{
		currentConfiguration[number * 3] = 1;
	}
	else
		currentConfiguration[number * 3] = 0;
	switch (st.rot)
	{
	case right:
	{
		currentConfiguration[number * 3 + 1] = 0;
		currentConfiguration[number * 3 + 2] = 0;
		break;
	}
	case bottom:
	{
		currentConfiguration[number * 3 + 1] = 0;
		currentConfiguration[number * 3 + 2] = 1;
		break;
	}
	case left:
	{
		currentConfiguration[number * 3 + 1] = 1;
		currentConfiguration[number * 3 + 2] = 0;
		break;
	}
	case top:
	{
		currentConfiguration[number * 3 + 1] = 1;
		currentConfiguration[number * 3 + 2] = 1;
		break;
	}
	}

}

void ConfigSequential::updateQPPs()
{
	auto states = generateStatesByConfig();
	QPPs.packFigures(states, currentOrder);
}

int ConfigSequential::length()
{
	return currentConfiguration.size();
}

//For debugging purposes
void ConfigSequential::showMatrix()
{
	this->QPPs.showMatrix();
}
