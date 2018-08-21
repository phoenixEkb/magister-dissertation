#include "stdafx.h"

#include "QuasiPolyominoPackaging.h"


QuasiPolyominoPackaging::QuasiPolyominoPackaging(std::string restrictionsFile, std::string figuresFile)
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
	figuresStates = std::vector<state>(figures.size());
	hasConflicts = false;
	conflictingPoints = std::vector<Point2D>();
	conflictFiguresNumbers = std::pair<int,int>();
}

MultiPoint2D QuasiPolyominoPackaging::normaliseFigure(MultiPoint2D figure, int number)
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

bool QuasiPolyominoPackaging::changeFigure(int number, state newState)
{
	if (newState.xCoord < 0 ||
		newState.xCoord >= gridWidth ||
		newState.yCoord < 0 ||
		newState.yCoord >= gridHeight ||
		newState.rot < 0 ||
		newState.rot >= 4)//TODO: rewrite coordinates condition for rotated fugure
	{
		std::cout << "error state obtained" << std::endl;
		return false;
	}
	if (Equals(figuresStates[number], newState))
		return false;
	if (newState.isIncluded == false)
	{
		removeFigure(number);
		return true;
	}
	MultiPoint2D newFigure=generateFigureByState(newState,number);
	
	for (int i = 0; i < bg::num_points(newFigure); i++)
	{
		if (currentStateMatrix(newFigure[i].get<0>(), newFigure[i].get<1>() != -1))
			return false;
	}
	for (int i = 0; i < bg::num_points(newFigure); i++)
	{
		currentStateMatrix(newFigure[i].get<0>(), newFigure[i].get<1>()) = number;
	}
	figuresStates[number] = newState;
	return true;
}

MultiPoint2D QuasiPolyominoPackaging::generateFigureByState(state newState, int figureNumber)
{
	MultiPoint2D newFigure(figures[figureNumber]);
	//first we generate figure, then create bMatrix;
	if (newState.mirrored)
	{
		trans::scale_transformer<int, 2, 2>xMirror(-1, 1);
		bg::transform(newFigure, newFigure, xMirror);
	}
	if (newState.rot != rotation::right)
	{
		trans::rotate_transformer<bg::degree, int, 2, 2> rotate(newState.rot);//TODO:check if works correctly
		bg::transform(newFigure, newFigure, rotate);
	}
	if (newState.xCoord != 0 || newState.yCoord != 0)
	{
		trans::translate_transformer<int, 2, 2> move(newState.xCoord, newState.yCoord);
		bg::transform(newFigure, newFigure, move);
	}
	return newFigure;
}

bool QuasiPolyominoPackaging::Equals(state& lhs, state& rhs)//comparator
{
	return (lhs.isIncluded == rhs.isIncluded &&
		lhs.mirrored == rhs.mirrored&&
		lhs.rot == rhs.rot&&
		lhs.xCoord == rhs.xCoord&&
		lhs.yCoord == rhs.yCoord);
};

void QuasiPolyominoPackaging::removeFigure(int number)
{
	figuresStates[number] = state();
	for (int i = currentStateMatrix.size1() - 1; i >= 0; i--)
	{
		for (size_t j = 0; j < currentStateMatrix.size2(); j++)
		{
			if (currentStateMatrix(i, j) == number )
				currentStateMatrix(i, j) = -1;
		}
	}
}


QuasiPolyominoPackaging::~QuasiPolyominoPackaging()
{

}

void QuasiPolyominoPackaging::showMatrix()
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
				std::cout<< " ";//TODO: calculate perfect range for setw(i.e. 2 for <10 figures, 3 for 10-99 etc.)
		}
		std::cout << std::endl;
	}
}

int QuasiPolyominoPackaging::returnFigureNumber(Point2D coords)//returns -1 if no figure found.
{
	return this->currentStateMatrix(coords.get<0>(), coords.get<1>())>-1?this->currentStateMatrix(coords.get<0>(), coords.get<1>()):-1;
}

bool QuasiPolyominoPackaging::updateFigures(std::vector<state> newStates)
{
	if (newStates.size() != this->figuresStates.size())
	{
		std::cout << "Wrong states vector" << std::endl;
		return false;
	}
	this->tempStates = newStates;
	for (int i = 0; i < newStates.size(); i++)
	{
		
	}
	return false;
}

std::pair<int, int> QuasiPolyominoPackaging::getConflictFiguresNumbers()
{
	return conflictFiguresNumbers;
}















//deprecated

//Problem 2, appears in these 3 functions
//
//QuasiPolyominoPackaging::QuasiPolyominoPackaging(const MultiPoint2D points, const MultiPoint2D restrictions, int gridWidth, int gridHeight)
//{
//	this->restrictions=restrictions;//?Not sure;
//	//createMatrixByMultipoint(restrictions, restrictMatrix, gridWidth, groupHeight);  //Problem string
//	////calculating figure sizes and normalisation
//	int xMin = INT_MAX, yMin = INT_MAX, xMax=INT_MIN,yMax=INT_MIN;
//	for (int i = 0; i < bg::num_points(points); i++)
//	{
//		xMax = std::max(points[i].get<0>(),xMax);
//		yMax = std::max(points[i].get<1>(), yMax);
//		xMin = std::min(points[i].get<0>(), xMin);
//		yMin = std::min(points[i].get<1>(), yMin);
//	}
//	groupWidth = 1 + xMax - xMin;
//	groupHeight = 1 + yMax - yMin;
//	trans::translate_transformer<int, 2, 2> normTranslation(-xMin , -yMin);
//	bg::transform(points, this->points, normTranslation);
//}
//
////
//unsigned int QuasiPolyominoPackaging::getPlacementsAmount()
//{
//	if (placementsMatrixes.empty())//
//	{
//		//generate matrixes, assuming we're using 3 types of basic transformations
//		for (int i = 0; i < gridWidth - groupWidth; ++i)
//		{
//			for (int j = 0; j < gridHeight - groupHeight; ++j)//TODO: check condition on j and i
//			{
//				for (int rot = rotation::right; rot < 360; rot += 90)//That's dirty
//				{
//					for (bool mir : { false, true })
//					{
//						state s = { i, j, static_cast<rotation>(rot), mir };
//						//matrix t = generateMatrix(s); problem: can't use this function
//						//	if (isMatrixCorrect(t))//problem in this 2 strings
//											//placementsMatrixes.push_back(t);					
//					}
//				}
//			}
//		}
//	}
//	else return placementsMatrixes.size();
//	return 0;
//}
//
////bMatrix QuasiPolyominoPackaging::getMatrix(unsigned int number)
////{
////	return placementsMatrixes[number];
////}
//
//QuasiPolyominoPackaging::~QuasiPolyominoPackaging()
//{
//}
//
//polyminoState QuasiPolyominoPackaging::getAvailableStates(int gridWidth, int gridHeight)
//{
//	return polyminoState();
//}
//
//QuasiPolyominoPackaging::QuasiPolyominoPackaging()
//{
//}
//
////returns false if bMatrix has crosses w/ restrictions bMatrix;
//bool QuasiPolyominoPackaging::isMatrixCorrect(const bMatrix & m)
//{
//	for (int i = 0; i < gridWidth; i++)
//	{
//		for (int j = 0; j < gridHeight; j++)
//		{
//			if ((m(i, j))&&(restrictMatrix(i, j)) == 1) return false;
//		}
//	}
//	return true;
//}
//
//
////Problem 3: usage of bg::transform stops building.
////Problem 4: can't return matrix
//bMatrix QuasiPolyominoPackaging::generateMatrix(state s)//todo: rewrite rotation w/ angle, rewrite as void function;
//{
//	if (s.xCoord<0||s.xCoord>=gridWidth-groupWidth
//		||s.yCoord<0||s.yCoord>=gridHeight-groupHeight
//		||s.rot < 0 || s.rot >=4 )//TODO: rewrite coordinates condition for rotated fugure
//	{
//		std::cout << "error matrix" << std::endl;
//		return zeroMatrix(gridWidth, gridHeight);
//	}
//	else
//	{
//		//first we generate figure, then create bMatrix;
//		MultiPoint2D t(points);
//		if (s.mirrored)
//		{
//			trans::scale_transformer<int,2,2>xMirror(-1, 1);
//			//bg::transform(t, xMirror);Problem 3
//		}
//		if (s.rot!= rotation::right)
//		{
//			trans::rotate_transformer<bg::degree, int, 2, 2> rotate(s.rot);//TODO:check if works correctly
//			//bg::transform(t, rotate);Problem 3
//		}
//		if (s.xCoord != 0 || s.yCoord != 0)
//		{
//			trans::translate_transformer<int, 2, 2> move(s.xCoord, s.yCoord);
//			//bg::transform(t, move);Problem 3
//		}
//		bMatrix m = zeroMatrix(gridWidth, gridHeight);
//		for (auto &p: points)
//		{
//			m(p.get<0>(), p.get<1>())= 1;
//		}
//		//return m;//Problem 4
//	}
//	
//	return bMatrix();
//}

//Problem 1.

//void QuasiPolyominoPackaging::createMatrixByMultipoint(const MultiPoint2D figure, matrix m, unsigned int width, unsigned int height)
//{
//	m = zeroMatrix(width, height);//Problem string
//	
//	for (auto p:figure)
//	{
//		m(p.get<0>(), p.get<1>()) = 1;
//	}
//}

