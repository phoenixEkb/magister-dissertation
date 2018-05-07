#pragma once
#include <vector>
#include <string>
using namespace std;

//This realisation of config is not quite informative. It is possible to make configuration in more dynamic way: 
//Each object is described as struct of 2 ints: xCoord,yCoord for left top angle of object, and 3 bools: rotation(from 0 to 3), mirrored 
class Config2D
{
private: vector<unsigned int> config;
		 vector<unsigned int> restrictions;
	
public:
	Config2D(unsigned int elementsAmount, vector<unsigned int>& restrictions);
	//Config2D(const vector<unsigned int>&);
	~Config2D();
	void nullPosition(unsigned int position);
	const unsigned int valueAt(unsigned int position) const;
	void setValue(unsigned int value, unsigned int position);
	 int size() const;
	friend bool operator==(const Config2D& left, const Config2D& right);
	friend bool operator<(const Config2D& left, const Config2D& right);
	
private:
	Config2D();
};