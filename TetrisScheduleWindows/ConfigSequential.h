#pragma once
#include "stdafx.h"
#include <vector>
#include "boost\dynamic_bitset.hpp" 
#include "QPPSeq.h"

class ConfigSequential
{
	int figuresAmount,statesAmount;
	boost::dynamic_bitset<> currentStatesConfiguration;//for states,getStatesAmount= elAm*3 - 2 bits for rotation, 1 bit for mirror;
	std::vector<int> currentOrder;//permutation, getStatesAmount=elAm
	bool QPPsUpdated;
	//TODO:MAJOR вы€снить/про€снить св€зь currentStatesConfiguration и currentOrder
	QPPSeq QPPs;
public:
	ConfigSequential();
	ConfigSequential(std::string figuresFile, std::string restrictionsFile);
	~ConfigSequential();
	void swapStateBinaryValue(int position);//mindless
	void setStateBinaryValueToActive(int position);
	void setStateBinaryValueToPassive(int position);
	void setStateBinaryValue(int position, bool value);
	void setOrder(std::vector<int> newOrder);
	void swapPositionsInOrder(int pos1, int pos2);
	int getFreeCellsAmount();
	//returns result from 0 to 1.
	double getFreeCellsPercentage();
	void updateQPPs();//Have to do it manually right now; TODO:MINOR: check fix.
	int getStatesAmount();//TODO:REFACTOR check getters in C++
	int getFiguresAmount();
	std::vector<int> getCurrentOrder();
	void showMatrix();
	void printMatrix(std::string resFile);
	//bool Equals(ConfigSequential lhs, ConfigSequential rhs) if == does not work
	bool valueAt(int position);
private:
	std::vector<stateSeq> generateStatesByConfig();
	void setBitsForConfig(int number, stateSeq state);

};

