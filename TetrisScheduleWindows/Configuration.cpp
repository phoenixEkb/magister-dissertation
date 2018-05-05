#include "stdafx.h"
#include "Configuration.h"



Configuration::Configuration()
{
	config = std::deque<bool>();
}

Configuration::Configuration(std::deque<bool> &newConf)
{
	config = newConf;//TODO ofc check this one.
}

Configuration::Configuration(std::vector<int> &newConf)
{
	config = std::deque<bool>();
	bool onesNow = true;
	int i = 0;
	for (int c : newConf)
	{
		for (int j = i; j < c; j++)
			config.push_back(onesNow);
		onesNow = !onesNow;
	}
}

int Configuration::size()
{
	return config.size();
}

Configuration::~Configuration()
{
}

bool Configuration::operator[](int val)
{
	return config[val];//TODO:check if correct?
}

bool Configuration::pop_front()
{
	bool val = config.front();
	config.pop_front();
	return val;
}

bool Configuration::pop_back()
{
	bool val = config.back();
	config.pop_back();
	return val;
}

void Configuration::push_front(bool val)
{
	config.push_front(val);
}

void Configuration::push_back(bool val)
{
	config.push_back(val);
}

void Configuration::trimLeft()
{
	while (config[0] == false)
	{
		config.pop_front();
	}
}

void Configuration::trimRight()
{
	while (config.back() == false)
	{
		config.pop_back();
	}
}