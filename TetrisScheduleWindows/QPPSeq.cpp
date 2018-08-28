#include "stdafx.h"

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
	figuresOrder = std::vector<int>(figures.size());
	for (int i = 0; i < figures.size(); i++)
		figuresOrder[i] = i;
	std::cout << "finished constructor work, starting packing" << std::endl;
	showMatrix();
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


//Packing occurs without spaces, we don't skip figures.
void QPPSeq::packFigures(std::vector<stateSeq> newStates, std::vector<int> newOrder)
{
	if (newStates.size() != this->figures.size() || newOrder.size() != this->figures.size())
		return;
	Point2D currentPosition(0, 0), oldPosition(0, 0);
	for (int i : newOrder)
	{
		MultiPoint2D currentFigure = getFigureByState(i, newStates[i]);
		int currentFigureWidth = figuresWidth[i], currentFigureHeigth = figuresHeight[i];
		if (newStates[i].rot == top || newStates[i].rot == bottom)
		{
			std::swap(currentFigureWidth, currentFigureHeigth);
		}

		bool foundPlacement = false;

		while (!foundPlacement)
		{
			if (gridHeight - 1 < currentFigureHeigth + currentPosition.get<1>())//figure does not fit to the top
			{
				break;
			}
			else if (gridWidth - 1 < currentFigureWidth + currentPosition.get<0>())//figure does not fit to the right
			{
				oldPosition = currentPosition;
				currentPosition = findFreeStartPoint(currentPosition, currentFigureWidth, currentFigureHeigth);
				if (currentPosition.get<0>() == -1)
				{
					break;
				}
			}
			trans::translate_transformer<int, 2, 2> move(currentPosition.get<0>() - oldPosition.get<0>(), currentPosition.get<1>() - oldPosition.get<1>());
			bg::transform(currentFigure, currentFigure, move);//assumed that figure does not leave the grid.
			bool positionAcceptible = true;
			for (int j = 0; j < currentFigure.size(); j++)
			{
				if (currentStateMatrix(currentFigure[j].get<0>(), currentFigure[j].get<1>()) != -1)
				{
					positionAcceptible = false;
					oldPosition = currentPosition;
					currentPosition = findFreeStartPoint(currentPosition, currentFigureWidth, currentFigureHeigth);
					break;
				}
			}
			if (positionAcceptible)
				foundPlacement = true;
			std::cout << "finished while iteration for figure "<<i << std::endl;
		}

		if (!foundPlacement)
		{
			this->placedFiguresAmount = i;
			return;
		}
		for (int j = 0; j < currentFigure.size(); j++)
		{
			currentStateMatrix(currentFigure[j].get<0>(), currentFigure[j].get<1>()) = i;
			//Find starting point for next figure. 
		}
	}
	this->placedFiguresAmount = figures.size();
}

MultiPoint2D QPPSeq::getFigureByState(int number, stateSeq newState)
{
	MultiPoint2D newFigure = figures[number];
	if (newState.mirrored)
	{
		trans::scale_transformer<int, 2, 2>xMirror(-1, 1);
		bg::transform(newFigure, newFigure, xMirror);
	}
	if (newState.rot != right)
	{
		trans::rotate_transformer<bg::degree, int, 2, 2> rotate(newState.rot);//TODO:check if works correctly
		bg::transform(newFigure, newFigure, rotate);
	}
	return newFigure;
}

Point2D QPPSeq::findFreeStartPoint(Point2D oldStartPoint, int figureWidth, int figureHeight)
{
	//find new point,satisfying this condition and pack again
	for (int k = oldStartPoint.get<1>() + 1; k <= gridHeight - figureHeight; k++)
	{
		for (int l = 0; l <= gridWidth - figureWidth; l++)
		{
			if (currentStateMatrix(l, k) == -1)
			{
				return Point2D(l, k);
			}
		}
	}
	return Point2D(-1, -1);
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
	MultiPoint2D newFigure;
	trans::translate_transformer<double, 2, 2> translate(-xMin, -yMin);
	bg::transform(figure, newFigure, translate);
	if (figuresWidth[number] < figuresHeight[number])//VERY IMPORTANT, rotatig figures for 90 degrees clockwise 
	{
		std::swap(figuresWidth[number], figuresHeight[number]);
		trans::rotate_transformer<bg::degree, int, 2, 2> rotate(90);//TODO:check if works correctly
		bg::transform(newFigure, newFigure, rotate);
	}
	return newFigure;
}
