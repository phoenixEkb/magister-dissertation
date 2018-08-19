#include "stdafx.h"
#include "ConfigBitset.h"



bool ConfigBitset::isElementActive(int number)
{
	if (number<0||number >elementsAmount)
		return false;//TODO exceptions
	return currentConfiguration[number];
}

void ConfigBitset::switchElementState(int number)
{
	if (number<0 || number >elementsAmount)
		return;//TODO exceptions

}

ConfigBitset::ConfigBitset()
{
}


ConfigBitset::~ConfigBitset()
{
}

int fastBinaryDigitsCount(uint32_t v)
{
	static const int MultiplyDeBruijnBitPosition[32] =
	{
		0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
		8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
	};

	v |= v >> 1; // first round down to one less than a power of 2 
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;

	return MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
}

ConfigBitset::ConfigBitset(std::vector<QuasiPolyominoPackaging> polyominoes, uint32_t gridWidth, uint32_t gridHeight)
{
	auto gridMaxDim = std::max(gridWidth, gridHeight);
	elementsAmount = polyominoes.size();	auto gridMaxDimDigitsAmount = fastBinaryDigitsCount(gridMaxDim);

	int rotationsBitsAmount = 2, reflectionsBitsAmount = 1, includeBitsAmount=1,
		bitsForOneConfig= includeBitsAmount+ 2*gridMaxDimDigitsAmount + rotationsBitsAmount + reflectionsBitsAmount;

	//problem 5 here 
	//config = boost::dynamic_bitset<>(polyominoes.size()*bitsForOneConfig);
	//availablePositions = boost::dynamic_bitset<>(polyominoes.size()*bitsForOneConfig);
	//availablePositions.flip();
//	
//	for (auto i=0;i<polyominoes.size();++i)
//	{
//		polyminoStates states=polyominoes[i].getAvailableStates(gridWidth, gridHeight);
//		if (states.maxDimension < gridMaxDim)
//			for (auto j = fastBinaryDigitsCount(states.maxDimension); j < gridMaxDimDigitsAmount; ++j)
//			{
//				this->availablePositions[i*bitsForOneConfig+ 1 + j] = false;
//				this->availablePositions[i*bitsForOneConfig+ 1 + gridMaxDimDigitsAmount+ j] = false;
//			}
//		if (states.possibleRotationsNumber == 1)
//		{
//			this->availablePositions[i*bitsForOneConfig + 1 + 2 * gridMaxDimDigitsAmount + 1] = false;
//			this->availablePositions[i*bitsForOneConfig + 1 + 2 * gridMaxDimDigitsAmount + 2] = false;
//		}
//		else if (states.possibleRotationsNumber==2)
//			this->availablePositions[i*bitsForOneConfig + 1 + 2 * gridMaxDimDigitsAmount + 2] = false;
//		if(!states.mirrorable)
//			this->availablePositions[i*bitsForOneConfig + 1 + 2 * gridMaxDimDigitsAmount + 2+1] = false;
//	}//maybe that kind of pre-gen would be better with smallmethods a-la changeParam(param, item).
}

void ConfigBitset::setValueToActive(int position)
{
	currentConfiguration[position] = true;
}

void ConfigBitset::setValueToPassive(int position)
{
	currentConfiguration[position] = false;
}

void ConfigBitset::swapValue(int position)
{
	currentConfiguration[position].flip();
}

bool ConfigBitset::isValueActive(int position)
{
	return currentConfiguration[position];
}

bool ConfigBitset::valueAt(int position) const
{
	return currentConfiguration[position];
}

int ConfigBitset::size() const
{
	return currentConfiguration.size();
}

//bool operator==(const ConfigBitset & left, const ConfigBitset & right) //Problem 6 - sth wrong with function signature.
//{
//	if (left.size() != right.size())
//		return false;
//	for (unsigned int i = 0; i < left.size(); i++)
//	{
//		if (left.valueAt(i) != right.valueAt(i)) return false;
//	}
//	return true;
//}

//bool operator<(const ConfigBitset & left, const ConfigBitset & right)
//{
//	for (unsigned int i = 0; i < left.size(); i++)
//	{
//		if (left.valueAt(i) > right.valueAt(i)) return false;
//		else if (left.valueAt(i) < right.valueAt(i)) return true;
//	}
//	return false;
//}
