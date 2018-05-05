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

unsigned int Config2D::valueAt(unsigned int position)
{
	return 0;
}

unsigned int Config2D::setValue(unsigned int value, unsigned int position)
{
	return 0;
}

int Config2D::size()
{
	return 0;
}

string Config2D::toString()
{
	return string();
}

bool operator==(const Config2D & left, const Config2D & right)
{
	return false;
}
