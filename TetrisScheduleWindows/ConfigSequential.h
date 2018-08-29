#pragma once
#include "stdafx.h"
#include <vector>
#include "boost\dynamic_bitset.hpp" 
#include "QPPSeq.h"

class ConfigSequential
{
	int figuresAmount,elementsAmount;
	boost::dynamic_bitset<> currentConfiguration;//for states,length= elAm*3?
	std::vector<int> currentOrder;//permutation
	QPPSeq QPPs;
public:
	void swapValue(int position);//mindless
	void setValueToActive(int position);
	void setValueToPassive(int position);
	void swapPositionsInOrder(int pos1, int pos2);
	ConfigSequential();
	ConfigSequential(std::string figuresFile,std::string restrictionsFile);
	~ConfigSequential();
	std::vector<stateSeq> generateStatesByConfig();
	int getFreeCellsAmount();
	int getFreeCellsPercentage();
	//bool Equals(ConfigSequential lhs, ConfigSequential rhs) if == does not work
	void setBitsForConfig(int number, stateSeq state);
	void updateQPPs();//Have to do it manually right now; TODO: fix.
};

