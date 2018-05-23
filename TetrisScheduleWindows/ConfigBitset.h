#pragma once
#include <vector>
#include "boost\dynamic_bitset.hpp" 
#include "PolyominoGroup.h"
using namespace std;

class ConfigBitset
{
	int elementsAmount;
	boost::dynamic_bitset<> config;	
	boost::dynamic_bitset<> availablePositions;
public:
	~ConfigBitset();
	ConfigBitset(vector<PolyominoGroup> polyominoes, uint32_t gridWidth, uint32_t gridHeight);
	//ConfigBitset(vector<int> &initConfig);//probably isn't requierd
	//ConfigBitset(ConfigBitset &conf); copy constructor, not sure if required
	//taken from old GA
	//What if methods doesnt't work if element is passive?
	void setValueToActive(int position);
	void setValueToPassive(int position);
	void swapValue(int position);
	bool isValueActive(int position);
	bool valueAt(int position) const;
	int size() const;
	//methods for sorting purposes
	friend bool operator==(const ConfigBitset& left, const ConfigBitset& right);
	friend bool operator<(const ConfigBitset& left, const ConfigBitset& right);
	
	bool isElementActive(int number);
	void switchElementState(int number);
private:
	ConfigBitset();
};

