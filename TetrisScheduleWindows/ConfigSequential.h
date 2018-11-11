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
	//TODO: вы€снить/про€снить св€зь currentConfiguration и currentOrder
	QPPSeq QPPs;
public:
	void swapValue(int position);//mindless
	void setValueToActive(int position);
	void setValueToPassive(int position);
	void setValue(int position, bool value);
	void swapPositionsInOrder(int pos1, int pos2);
	ConfigSequential();
	ConfigSequential(std::string figuresFile,std::string restrictionsFile);
	~ConfigSequential();
	std::vector<stateSeq> generateStatesByConfig();
	int getFreeCellsAmount();
	//returns result from 0 to 1.
	double getFreeCellsPercentage();
	void setBitsForConfig(int number, stateSeq state);
	void updateQPPs();//Have to do it manually right now; TODO: fix.
	int length();
	void showMatrix();
	void printMatrix(std::string resFile);
	//bool Equals(ConfigSequential lhs, ConfigSequential rhs) if == does not work
	bool valueAt(int position);
};

