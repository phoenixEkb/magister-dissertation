#pragma once
#include "stdafx.h"
#include <vector>
#include "boost\dynamic_bitset.hpp" 
#include "QPPSeq.h"

class ConfigSequential
{
	int figuresAmount,elementsAmount;
	boost::dynamic_bitset<> currentConfiguration;//for states,length= elAm*3?
	std::vector<int> currentOrder;//permutation, length=elAm
	bool QPPsUpdated;
	//TODO:MAJOR ��������/��������� ����� currentConfiguration � currentOrder
	QPPSeq QPPs;
public:
	ConfigSequential();
	ConfigSequential(std::string figuresFile, std::string restrictionsFile);
	~ConfigSequential();

	void swapStateBinaryValue(int position);//mindless
	void setStateBinaryValueToActive(int position);
	void setStateBinaryValueToPassive(int position);
	void setStateBinaryValue(int position, bool value);
	void swapPositionsInOrder(int pos1, int pos2);
	int getFreeCellsAmount();
	//returns result from 0 to 1.
	double getFreeCellsPercentage();
	void updateQPPs();//Have to do it manually right now; TODO: fix.
	int length();
	void showMatrix();
	void printMatrix(std::string resFile);
	//bool Equals(ConfigSequential lhs, ConfigSequential rhs) if == does not work
	bool valueAt(int position);
private:
	std::vector<stateSeq> generateStatesByConfig();
	void setBitsForConfig(int number, stateSeq state);

};

