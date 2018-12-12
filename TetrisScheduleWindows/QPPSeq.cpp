#include "stdafx.h"

#include "QPPSeq.h"



QPPSeq::QPPSeq()
{
}

QPPSeq::QPPSeq(std::string figuresFile, std::string restrictionsFile)
{
	std::ifstream restrFile(restrictionsFile, std::ifstream::in);
	restrFile >> gridWidth >> gridHeight;
	std::string line;
	restrFile.get();//read \n to get next line;
	std::getline(restrFile, line);
	if (line != "")
		bg::read_wkt(line, restrictions);
	else
		restrictions = MultiPoint2D();//TODO:MINOR:Check if adding empty restriction works
	restrFile.close();
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
	inFile.close();
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

bool QPPSeq::changeFigureByPosition(int number, stateSeq newState)//Presumably works
{
	//work with figures[i]
	if (!Equals(figuresStates[number], newState))
	{
		figuresStates[number] = newState;
		this->packFigures(figuresStates, figuresOrder);
		return true;
	}
	return false;
}

bool QPPSeq::Equals(stateSeq& lhs, stateSeq& rhs)//comparator
{
	return (lhs.mirrored == rhs.mirrored&&
		lhs.rot == rhs.rot);
}

bool QPPSeq::changeFigureByNumber(int number, stateSeq newState)//Presumably works
{
	int index = distance(figuresOrder.begin(), std::find(figuresOrder.begin(), figuresOrder.end(), number));
	if (!Equals(figuresStates[index], newState))
	{
		figuresStates[index] = newState;
		this->packFigures(figuresStates, figuresOrder);
		return true;
	}
	return false;
}
;

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
			{//TODO:REFACTOR write some wraparound for numbers.
			case -1:std::cout << '_'; break;//no figure
			case -2:std::cout << '#'; break;//restriction
			case -3:std::cout << '*'; break;//error
			default:std::cout << currentStateMatrix(i, j);
			};
			std::cout << " ";//TODO:MINOR: calculate perfect range for drawing with setw(i.e. 2 for <10 figures, 3 for 10-99 etc.)
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
//returns -1 for emply positions and -2 for restricted positions

void QPPSeq::printMatrix(std::string resFile)
{
	std::ofstream restrFile(resFile, std::ifstream::out);
	for (int j = currentStateMatrix.size2() - 1; j >= 0; j--)
	{
		for (int i = 0; i < currentStateMatrix.size1(); i++)
		{

			restrFile << std::setw(3) << std::setfill(' ');
			switch (currentStateMatrix(i, j))
			{//TODO:REFACTOR:Stop reusing this code.
			case -1:restrFile << '_'; break;//no figure
			case -2:restrFile << '#'; break;//restriction
			case -3:restrFile << '*'; break;//error
			default:restrFile << currentStateMatrix(i, j);
			};
			restrFile << " ";//TODO: REFACTOR:Stop reusing this code
		}
		restrFile << std::endl;
	}
	restrFile << std::endl;
}


int QPPSeq::returnFigureNumber(Point2D coords)
{
	return this->currentStateMatrix(coords.get<0>(), coords.get<1>());
}


//Packing occurs without spaces, we don't skip figures.
//TODO:REFACTOR THIS!
void QPPSeq::packFigures(std::vector<stateSeq> newStates, std::vector<int> newOrder)//TODO: maybe, add starting number
{
	if (newStates.size() != this->figures.size() || newOrder.size() != this->figures.size())
		return;
	Point2D currentPosition(0, 0), oldPosition;
	placedFiguresAmount = 0;
	clearMatrix();
	oldPosition = Point2D(0, 0);

	for (int i : newOrder)
	{
		/*if (!newStates[i].isIncluded)
			continue;*/
		MultiPoint2D currentFigure = createFigureByState(i, newStates[i]);
		trans::translate_transformer<int, 2, 2> move(currentPosition.get<0>(), currentPosition.get<1>());
		bg::transform(currentFigure, currentFigure, move);//assumed that figure does not leave the grid.

		int currentFigureWidth = figuresWidth[i], currentFigureHeigth = figuresHeight[i];
		if (newStates[i].rot == top || newStates[i].rot == bottom)
		{
			std::swap(currentFigureWidth, currentFigureHeigth);
		}
		//std::cout << "Figure sizes are " << currentFigureWidth<<" " << currentFigureHeigth<<", rotation is "<<newStates[i].rot<<std::endl;

		//TODO:Also possible to use 0.0 as 1st argument if want more clumsy packaging. Or not.
		if (currentPosition.get<0>() == -1)
		{
			return;
		}
		bool foundPlacement = false;

		while (!foundPlacement)
		{
			if (currentPosition.get<0>() == -1)//TODO:MINOR:Check this if;
			{
				break;
			}

			//std::cout << "Position is " << currentPosition.get<0>() << " " << currentPosition.get<1>() << std::endl;

			if (!checkAvailableHeight(currentPosition, currentFigureHeigth))//figure does not fit to the top
			{
				break;
			}
			else if (!checkAvailableWidth(currentPosition, currentFigureWidth+1))//figure does not fit to the right
			{
				if (checkAvailableHeight(currentPosition, currentFigureHeigth + 1))
					currentPosition = Point2D(0, currentPosition.get<1>()+1);
			}
			else
			{
				currentPosition.set<0>(currentPosition.get<0>()+1);
			}
			if (currentPosition.get<0>() == oldPosition.get<0>() && currentPosition.get<1>() == oldPosition.get<1>())
				break;
			trans::translate_transformer<int, 2, 2> move(currentPosition.get<0>() - oldPosition.get<0>(), currentPosition.get<1>() - oldPosition.get<1>());
			bg::transform(currentFigure, currentFigure, move);//assumed that figure does not leave the grid.
			bool positionAcceptible = true;
			for (int j = 0; j < currentFigure.size(); j++)
			{
				if (currentStateMatrix(currentFigure[j].get<0>(), currentFigure[j].get<1>()) != -1)
				{
					positionAcceptible = false;
					//TODO:MAJOR:Add translation to tright fix for case when old adn new points are equal;
					//Maybe, we should make a grid-width check, if width check fails, we move up, if top check falis we finish, if both checks pass - move right 1 point.
					break;
				}
			}
			oldPosition = currentPosition;
			if (positionAcceptible)
				foundPlacement = true;
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
		//std::cout << "Figure "<< i <<" placed" << std::endl;
		//showMatrix();

		//clumsy packing trick, can be disabled
		currentPosition = Point2D(0, 0);
		oldPosition = currentPosition;
		//
	}
	//showMatrix();
	//this->placedFiguresAmount = figures.size();
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

MultiPoint2D QPPSeq::createFigureByState(int number, stateSeq newState)
{
	MultiPoint2D newFigure = figures[number];
	if (newState.mirrored)
	{
		trans::scale_transformer<int, 2, 2>xMirror(-1, 1);
		bg::transform(newFigure, newFigure, xMirror);
		trans::translate_transformer<int, 2, 2>xMove(figuresWidth[number] - 1, 0);
		bg::transform(newFigure, newFigure, xMove);
	}
	if (newState.rot != right)
	{
		newFigure = this->rotateSavingLeftCorner(newFigure, newState.rot, 0, 0, figuresWidth[number], figuresHeight[number]);
		//TODO:DEBUG: Print newFigure in some way.
	}
	return newFigure;
}

Point2D QPPSeq::findFreeStartPoint(Point2D oldStartPoint, int figureWidth, int figureHeight)
{
	////find new point,satisfying this condition and pack again
	//for (int l = oldStartPoint.get<0>() + 1; l <= gridWidth - figureWidth; l++)
	//{
	//	if (currentStateMatrix(l, oldStartPoint.get<1>()) == -1)
	//	{
	//		return Point2D(l, oldStartPoint.get<1>());
	//	}
	//}
	//for (int k = oldStartPoint.get<1>() + 1; k <= gridHeight - figureHeight; k++)
	//{
	//	for (int l = 0; l <= gridWidth - figureWidth; l++)
	//	{
	//		if (currentStateMatrix(l, k) == -1)
	//		{
	//			return Point2D(l, k);
	//		}
	//	}
	//}
	if (checkAvailableWidth(oldStartPoint, figureWidth))
	{
		if (checkAvailableHeight(oldStartPoint, figureHeight))
			return oldStartPoint;
	}
	else
	{
		if (checkAvailableHeight(oldStartPoint, figureHeight - 1))
			return Point2D(0, oldStartPoint.get<1>() + 1);
	}
	return Point2D(-1, -1);
}

bool QPPSeq::checkAvailableWidth(Point2D startPoint, int figureWidth)
{
	return (startPoint.get<0>() + figureWidth <= gridWidth);
}

bool QPPSeq::checkAvailableHeight(Point2D startPoint, int figureHeight)
{
	return (startPoint.get<1>() + figureHeight <= gridHeight);
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
		//trans::translate_transformer<double, 2, 2> t2(1, 1), t3(-1, 1);//TODO:check if works correctly, check numbers for t3. t2 is chosen to move avay from axis lines.
		//trans::rotate_transformer<bg::degree, double, 2, 2> rotate(90.0);
		//bg::transform(newFigure, newFigure, t2);
		//bg::transform(newFigure, newFigure, rotate);
		//bg::transform(newFigure, newFigure, t3);
		newFigure = rotateSavingLeftCorner(newFigure, rotation::bottom, 0, 0, figuresWidth[number], figuresHeight[number]);
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
	int xMin = INT_MAX, yMin = INT_MAX;
	for (int j = 0; j < bg::num_points(figure); j++)
	{
		xMin = std::min(newFigure[j].get<0>(), xMin);
		yMin = std::min(newFigure[j].get<1>(), yMin);
	}
	trans::translate_transformer<double, 2, 2> translate(-xMin, -yMin);
	bg::transform(newFigure, newFigure, translate);
	/*switch (rot)
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
	}*/
	return newFigure;
}

MultiPoint2D QPPSeq::rotateTransform(MultiPoint2D figure, rotation angle)
{
	bMatrix rotMatrix = boost::numeric::ublas::matrix<int>(2, 2);
	bMatrix tmpFigure = boost::numeric::ublas::matrix<int>(figure.size(), 2);
	for (int i = 0; i < figure.size(); i++)
	{
		tmpFigure(i, 0) = figure[i].get<0>();
		tmpFigure(i, 1) = figure[i].get<1>();
	}
	double rAngle = angle * constants::pi / 180;
	rotMatrix(0, 0) = cos(rAngle);
	rotMatrix(1, 0) = sin(rAngle);
	rotMatrix(0, 1) = -sin(rAngle);
	rotMatrix(1, 1) = cos(rAngle);
	bMatrix newFigureMatrix = boost::numeric::ublas::prod(tmpFigure, rotMatrix);
	MultiPoint2D newFigure;
	for (int i = 0; i < figure.size(); i++)
	{
		newFigure.push_back(Point2D(newFigureMatrix(i, 0), newFigureMatrix(i, 1)));
	}
	return newFigure;
}

int QPPSeq::getFiguresAmount()
{
	return figures.size();
}

int QPPSeq::getWidth()
{
	return gridWidth;
}

int QPPSeq::getHeight()
{
	return gridHeight;
}

int QPPSeq::getGridArea()
{
	return gridWidth * gridHeight;
}

int QPPSeq::getRestrictionsArea()
{
	return restrictions.size();
}
