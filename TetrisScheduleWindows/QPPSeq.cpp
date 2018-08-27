#include "QPPSeq.h"



QPPSeq::QPPSeq(std::string restrictionsFile, std::string figuresFile)
{
	std::ifstream restrFile(restrictionsFile, std::ifstream::in);
	restrFile >> gridWidth >> gridHeight;
	std::string line;
	restrFile.get();//read \n to get next line;
	std::getline(restrFile, line);
	if (line != "")
		bg::read_wkt(line, restrictions);
	else
		restrictions = MultiPoint2D();//TODO:Check if works
	std::ifstream inFile(figuresFile, std::ifstream::in);
	std::getline(inFile, line);
	while (!inFile.eof())
	{
		MultiPoint2D p;
		bg::read_wkt(line, p);
		figures.push_back(p);
		std::getline(inFile, line);
	}
	figuresWidth = std::vector<int>(figures.size());
	figuresHeight = std::vector<int>(figures.size());

	for (int i = 0; i < figures.size(); i++)
	{
		figures[i] = normaliseFigure(figures[i], i);
	}
	currentStateMatrix = boost::numeric::ublas::matrix<int>(gridWidth, gridHeight);
	for (size_t i = 0; i < gridWidth; i++)
	{
		for (size_t j = 0; j < gridHeight; j++)
		{
			currentStateMatrix(i, j) = -1;
		}
	}
	for (size_t i = 0; i < bg::num_points(restrictions); i++)
	{
		currentStateMatrix(restrictions[i].get<0>(), restrictions[i].get<1>()) = -2;
	}
	figuresStates = std::vector<stateSeq>(figures.size());
	figuresOrder= std::vector<int>(figures.size());
	for (int i = 0; i < figures.size(); i++)
		figuresOrder[i] = i;
	packFigures(figuresStates, figuresOrder);
}


QPPSeq::~QPPSeq()
{
}

void QPPSeq::showMatrix()
{
	for (int i = currentStateMatrix.size1() - 1; i >= 0; i--)
	{
		for (size_t j = 0; j < currentStateMatrix.size2(); j++)
		{
			std::cout << std::setw(3) << std::setfill(' ');
			switch (currentStateMatrix(i, j))
			{//TODO: write some wraparound for numbers.
			case -1:std::cout << '_'; break;//no figure
			case -2:std::cout << '#'; break;//restriction
			case -3:std::cout << '*'; break;//error
			default:std::cout << currentStateMatrix(i, j);
			};
			std::cout << " ";//TODO: calculate perfect range for setw(i.e. 2 for <10 figures, 3 for 10-99 etc.)
		}
		std::cout << std::endl;
	}
}

MultiPoint2D QPPSeq::normaliseFigure(MultiPoint2D figure, int number)
{
	int xMin = INT_MAX, yMin = INT_MAX, xMax = INT_MIN, yMax = INT_MIN;
	for (int j = 0; j < bg::num_points(figure); j++)
	{
		xMax = std::max(figure[j].get<0>(), xMax);
		yMax = std::max(figure[j].get<1>(), yMax);
		xMin = std::min(figure[j].get<0>(), xMin);
		yMin = std::min(figure[j].get<1>(), yMin);
	}
	figuresWidth[number] = 1 + xMax - xMin;
	figuresHeight[number] = 1 + yMax - yMin;

	trans::translate_transformer<double, 2, 2> translate(-xMin, -yMin);
	MultiPoint2D newFigure;
	bg::transform(figure, newFigure, translate);
	return newFigure;
}
