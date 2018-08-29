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
