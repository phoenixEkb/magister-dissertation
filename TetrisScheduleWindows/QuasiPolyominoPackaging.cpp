#include "stdafx.h"

#include "QuasiPolyominoPackaging.h"


QuasiPolyominoPackaging::QuasiPolyominoPackaging(std::string restrictionsFile,std::string figuresFile)
{
		std::ifstream restrFile(restrictionsFile, std::ifstream::in);
		restrFile >> gridWidth >> gridHeight;
		std::string line;
		restrFile.get();//read \n to get next line;
		std::getline(restrFile, line);

		MultiPoint2D restrPoints;
		bg::read_wkt(line, restrPoints);

		boost::container::vector<MultiPoint2D> figuresPoints;
		std::ifstream inFile(figuresFile, std::ifstream::in);
		while (inFile.eof())
		{
			std::getline(inFile, line);
			MultiPoint2D p;
			bg::read_wkt(line, p);
			figuresPoints.push_back(p);
		}
		this->gridWidth = gridWidth;
		this->gridHeight = gridHeight;
		this->restrictions = restrPoints;
		this->figures = figuresPoints;
		for (int i =0; i<this->figures.size();i++)
		{
			int xMin = INT_MAX, yMin = INT_MAX, xMax = INT_MIN, yMax = INT_MIN;
				for (int j = 0; j < bg::num_points(figures[i]); i++)
				{
					xMax = std::max(figures[i][j].get<0>(),xMax);
					yMax = std::max(figures[i][j].get<1>(), yMax);
					xMin = std::min(figures[i][j].get<0>(), xMin);
					yMin = std::min(figures[i][j].get<1>(), yMin);
				}
			figuresWidth[i] = 1 + xMax - xMin;
			figuresHeight[i] = 1 + yMax - yMin;
			figures[i] = normaliseFigure(figures[i],i);
		}
		currentStateMatrix = boost::numeric::ublas::zero_matrix<int>(gridWidth, gridHeight);
		for (size_t i = 0; i < bg::num_points(restrictions); i++)
		{
			currentStateMatrix(restrictions[i].get<0>(), restrictions[i].get<1>()) = -1;
		}
		showMatrix();
}

MultiPoint2D QuasiPolyominoPackaging::normaliseFigure(MultiPoint2D figure,int number)
{
	MultiPoint2D newFigure;

	return newFigure;
}

QuasiPolyominoPackaging::~QuasiPolyominoPackaging()
{

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

void QuasiPolyominoPackaging::showMatrix()
{
	
	for (size_t i = 0; i < currentStateMatrix.size1(); i++)
	{
		for (size_t j = 0; j < currentStateMatrix.size2(); j++)
		{
			std::cout << std::setw(4) << std::setfill(' ')<<currentStateMatrix(i, j)<<" ";//TODO: add aligning
		}
		std::cout << std::endl;
	}
}
