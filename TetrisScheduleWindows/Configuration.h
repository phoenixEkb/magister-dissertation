#pragma once
#include <vector>
#include <list>
#include<deque>
class Configuration
{
	std::deque<bool> config;
public:
	Configuration();
	Configuration(std::deque<bool> &newConf);
	Configuration(std::vector<int> &newConf);
	int size();
	~Configuration();

	bool operator[](int val);
	bool pop_front();
	bool pop_back();
	void push_front(bool val);
	void push_back(bool val);
	void trimLeft();
	void trimRight();
};

