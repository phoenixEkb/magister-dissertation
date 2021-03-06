#include "stdafx.h"
#include "Config2D.h"


std::vector<unsigned int> Config2D::getFiguresStatesAmount() const
{
	std::vector<unsigned int> cpyRestrictions(maxValues);
	return cpyRestrictions;
}

Config2D::Config2D()
{
	
}

Config2D::Config2D(unsigned int statesAmount, std::vector<unsigned int>& maxValues)
{
	config = std::vector<unsigned int>(statesAmount);
	this->maxValues = maxValues;
}

Config2D::~Config2D()
{
}

//Removes element from configuration
void Config2D::nullPosition(unsigned int position)
{
	if (position < config.size())
		config[position] = 0;
}

const unsigned int Config2D::valueAt(unsigned int position) const
{
	return config[position];
}

bool Config2D::setStateBinaryValue(unsigned int value, unsigned int position)
{
	if (maxValues[position]<value)
	{
		config[position] = value;
		return true;
	}
	return false;//TODO: Throw exception?
}

int  Config2D::size() const
{
	return config.size();
}

//string Config2D::toString()
//{
//	string 
//}

bool operator==(const Config2D & left, const Config2D & right)
{
	if (left.size() != right.size())
		return false;

	for (int i = 0; i < left.size(); i++)
	{
		if (left.valueAt(i) != right.valueAt(i)) return false;
	}
	return true;
}

bool operator<(const Config2D & left, const Config2D & right)
{

	for (int i = 0; i < left.size(); i++)
	{
		if (left.valueAt(i) > right.valueAt(i)) return false;
		else if (left.valueAt(i) < right.valueAt(i)) return true;
	}
	return false;
}
