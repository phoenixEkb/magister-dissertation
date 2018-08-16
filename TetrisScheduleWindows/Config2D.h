#pragma once
#include "stdafx.h"

#include <vector>
#include <string>


//using namespace std;


//This configuration class saves information about current configuration in linear vector.
//Not included in program.
class Config2D
{
private: std::vector<unsigned int> config;
		 std::vector<unsigned int> maxValues;
		 //bit
	
public:

	Config2D(unsigned int elementsAmount, std::vector<unsigned int>& maxValues);
	//Config2D(const vector<unsigned int>&);
	~Config2D();
	void nullPosition(unsigned int position);
	const unsigned int valueAt(unsigned int position) const;
	bool setValue(unsigned int value, unsigned int position);
	int size() const;
	friend bool operator==(const Config2D& left, const Config2D& right);
	friend bool operator<(const Config2D& left, const Config2D& right);
	
	std::vector<unsigned int> getFiguresStatesAmount() const;

	Config2D();
};