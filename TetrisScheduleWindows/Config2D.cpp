#include "Config2D.h"


Config2D::Config2D()
{
	
}

Config2D::Config2D(unsigned int elementsAmount,vector<unsigned int>& restrictions)
{
	config = vector<unsigned int>(elementsAmount);
	this->restrictions = restrictions;
}

//Config2D::Config2D(const vector<unsigned int>&)
//{
//	this->config
//}

Config2D::~Config2D()
{
}
//nulled element == no element in config.
void Config2D::nullPosition(unsigned int position)
{
	if (position < config.size())
		config[position] = 0;
}

const unsigned int Config2D::valueAt(unsigned int position) const
{
	return config[position];
}

void Config2D::setValue(unsigned int value, unsigned int position)
{
	config[position]=value;
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
	for (unsigned int i = 0; i < left.size(); i++)
	{
		if (left.valueAt(i) != right.valueAt(i)) return false;
	}
	return true;
}

bool operator<(const Config2D & left, const Config2D & right)
{
	for (unsigned int i = 0; i < left.size(); i++)
	{
		if (left.valueAt(i) > right.valueAt(i)) return false;
		else if (left.valueAt(i) < right.valueAt(i)) return true;
	}
	return false;
}
