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
	clearMatrix();
	for (size_t i = 0; i < bg::num_points(restrictions); i++)
	{
		currentStateMatrix(restrictions[i].get<0>(), restrictions[i].get<1>()) = -2;
	}
	figuresStates = std::vector<stateSeq>(figures.size());
	figuresOrder = std::vector<int>(figures.size());
	for (int i = 0; i < figures.size(); i++)
		figuresOrder[i] = i;
	packFigures(figuresStates, figuresOrder);
}

void QPPSeq::clearMatrix()
{
	for (size_t i = 0; i < gridWidth; i++)
	{
		for (size_t j = 0; j < gridHeight; j++)
		{
			if (currentStateMatrix(i, j) != -2)
				currentStateMatrix(i, j) = -1;
		}
	}
}

QPPSeq::~QPPSeq()
{
}

void QPPSeq::swapPositionsInCurrentOrder(int firstPos, int secondPos)
{
	if (firstPos >= 0 && secondPos >= 0 && firstPos < figuresOrder.size() && secondPos < figuresOrder.size())
	{
		std::swap(this->figuresOrder[firstPos], this->figuresOrder[secondPos]);
		this->packFigures(figuresStates, figuresOrder);
	}
}



void QPPSeq::showMatrix()
{
	/*for (int i = currentStateMatrix.size1() - 1; i >= 0; i--)
	{
		for (size_t j = 0; j < currentStateMatrix.size2(); j++)
		{*/
	for (int j = currentStateMatrix.size2() - 1; j >= 0; j--)
	{
		for (int i = 0; i < currentStateMatrix.size1(); i++)
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
	std::cout << std::endl;
}
//returns -1 for emply positions and -2 for restricted positions
int QPPSeq::returnFigureNumber(Point2D coords)
{
	return this->currentStateMatrix(coords.get<0>(), coords.get<1>());
}


//Packing occurs without spaces, we don't skip figures.
void QPPSeq::packFigures(std::vector<stateSeq> newStates, std::vector<int> newOrder)
{
	if (newStates.size() != this->figures.size() || newOrder.size() != this->figures.size())
		return;
	Point2D currentPosition(0, 0), oldPosition;
	placedFiguresAmount = 0;
	clearMatrix();
	for (int i : newOrder)
	{
		/*if (!newStates[i].isIncluded)
			continue;*/
		MultiPoint2D currentFigure = getFigureByState(i, newStates[i]);
		int currentFigureWidth = figuresWidth[i], currentFigureHeigth = figuresHeight[i];
		if (newStates[i].rot == top || newStates[i].rot == bottom)
		{
			std::swap(currentFigureWidth, currentFigureHeigth);
		}
		//Also possible to use 0.0 as 1st argument if want more clumsy packaging.
		currentPosition = findFreeStartPoint(currentPosition, currentFigureWidth, currentFigureHeigth);
		if (currentPosition.get<0>() == -1)
		{
			return;
		}
		oldPosition = Point2D(0, 0);
		bool foundPlacement = false;

		while (!foundPlacement)
		{
			if (gridHeight < currentFigureHeigth + currentPosition.get<1>())//figure does not fit to the top
			{
				break;
			}
			else if (gridWidth < currentFigureWidth + currentPosition.get<0>())//figure does not fit to the right
			{
				oldPosition = currentPosition;
				currentPosition = findFreeStartPoint(currentPosition, currentFigureWidth, currentFigureHeigth);
				if (currentPosition.get<0>() == -1)//проверить эти брейки
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
			//std::cout << "finished while iteration for figure "<<i << std::endl;
		}

		if (!foundPlacement)
		{
			//this->placedFiguresAmount = i;
			return;
		}
		for (int j = 0; j < currentFigure.size(); j++)
		{
			currentStateMatrix(currentFigure[j].get<0>(), currentFigure[j].get<1>()) = i;
		}
		placedFiguresAmount++;
	}
	this->placedFiguresAmount = figures.size();
}

std::vector<int> QPPSeq::getFiguresOrder()
{

	return this->figuresOrder;
}

std::vector<stateSeq> QPPSeq::getFiguresStates()
{
	return this->figuresStates;
}

int QPPSeq::getPlacedFiguresAmount()
{
	return this->placedFiguresAmount;
}

int QPPSeq::getFreeArea()
{
	int sum = 0;
	for (int i = 0; i < this->placedFiguresAmount; i++)
		sum += figures[figuresOrder[i]].size();
	return gridWidth * gridHeight - sum - this->restrictions.size();
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
	for (int l = oldStartPoint.get<0>() + 1; l <= gridWidth - figureWidth; l++)
	{
		if (currentStateMatrix(l, oldStartPoint.get<1>()) == -1)
		{
			return Point2D(l, oldStartPoint.get<1>());
		}
	}
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
	if (figuresWidth[number] < figuresHeight[number])//VERY IMPORTANT, rotatig figures for 90 degrees clockwise AND VERY bugged.
	{
		std::swap(figuresWidth[number], figuresHeight[number]);
		trans::translate_transformer<double, 2, 2> t2(1, 1), t3(-1, 1);//TODO:check if works correctly, check numbers for t3. t2 is chosen to move avay from axis lines.
		trans::rotate_transformer<bg::degree, double, 2, 2> rotate(90.0);
		bg::transform(newFigure, newFigure, t2);
		bg::transform(newFigure, newFigure, rotate);
		bg::transform(newFigure, newFigure, t3);
	}
	return newFigure;
}

MultiPoint2D QPPSeq::rotateSavingLeftCorner(MultiPoint2D figure, rotation rot, int xLeftCorner, int yLeftCorner, int width, int height)
{
	if (rot == rotation::right)
		return figure;
	MultiPoint2D newFigure;
	if (!xLeftCorner)
	{
		if (!yLeftCorner)
		{
			trans::translate_transformer<double, 2, 2> t1(1, 1);
			bg::transform(figure, newFigure, t1);
		}
		else
		{
			trans::translate_transformer<double, 2, 2> t1(1, 0);
			bg::transform(figure, newFigure, t1);
		}
	}
	else
	{
		if (!yLeftCorner)
		{
			trans::translate_transformer<double, 2, 2> t1(0, 1);
			bg::transform(figure, newFigure, t1);
		}
	}
	//trans::rotate_transformer<bg::degree, double, 2, 2> rotate(rot);
	//bg::transform(newFigure, newFigure, rotate);
	newFigure = rotateTransform(newFigure, rot);
	switch (rot)
	{
	case rotation::bottom:
	{
		trans::translate_transformer<double, 2, 2> t2(-1, width);
		bg::transform(newFigure, newFigure, t2);
		break;
	}
	case rotation::left:
	{
		trans::translate_transformer<double, 2, 2> t2(width, height);
		bg::transform(newFigure, newFigure, t2);
		break;
	}
	case rotation::top:
	{
		trans::translate_transformer<double, 2, 2> t2(height, -1);
		bg::transform(newFigure, newFigure, t2);
		break;
	}
	}
	return newFigure;
}

MultiPoint2D QPPSeq::rotateTransform(MultiPoint2D figure, rotation angle)
{
	bMatrix rotMatrix = boost::numeric::ublas::matrix<int>(2, 2);
	bMatrix tmpFigure = boost::numeric::ublas::matrix<int>(figure.size(),2 );
	for(int i=0;i<figure.size();i++)
	{
		tmpFigure(i,0) = figure[i].get<0>();
		tmpFigure(i,1) = figure[i].get<1>();
	}
	double rAngle = angle * constants::pi / 180;
	rotMatrix(0, 0) = cos(rAngle);
	rotMatrix(1, 0) = sin(rAngle);
	rotMatrix(0, 1) = -sin(rAngle);
	rotMatrix(1, 1) = cos(rAngle);
	bMatrix newFigureMatrix = boost::numeric::ublas::prod(tmpFigure,rotMatrix);
	MultiPoint2D newFigure;
	for (int i = 0; i < figure.size(); i++)
	{
		newFigure.push_back(Point2D(newFigureMatrix( i,0), newFigureMatrix(i,1))) ;
	}
	return newFigure;
}