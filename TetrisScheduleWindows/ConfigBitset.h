#pragma once
#include "stdafx.h"

#include <vector>
#include "boost\dynamic_bitset.hpp" 
#include "QuasiPolyomino.h"

class ConfigBitset
{
	int elementsAmount;
	boost::dynamic_bitset<> currentConfiguration;	
	static boost::dynamic_bitset<> possiblePolyominoesPositions;
	//Saves information about possible states in binary form.
	//If position is enabled, it is possible to switch values in currentConfigurations
	//TODO: Separate this structure into individual object. 
public:
	~ConfigBitset();
	ConfigBitset(std::vector<QuasiPolyomino> polyominoes, uint32_t gridWidth, uint32_t gridHeight);

	//ConfigBitset(vector<int> &initConfig);//
	//ConfigBitset(ConfigBitset &conf); //copy constructor
	//Two constructors above are taken from old GA and were never used
	
	//What if methods doesnt't work if element is passive?
	void setValueToActive(int position);
	void setValueToPassive(int position);
	void swapValue(int position);
	bool isValueActive(int position);
	bool valueAt(int position) const;
	int size() const;
	//methods for sorting purposes, required for std::sort
	friend bool operator==(const ConfigBitset& left, const ConfigBitset& right);
	friend bool operator<(const ConfigBitset& left, const ConfigBitset& right);
	
	bool isElementActive(int number);
	void switchElementState(int number);
//private:
	ConfigBitset();
};

