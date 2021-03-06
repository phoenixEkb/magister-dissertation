// TetrisScheduleWindows.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TetrisScheduleWindows.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

//Problem 6 - it isn't allowed to call Geometyr directly.
#include <boost/geometry.hpp>
//
//#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
//#include <boost/geometry/io/io.hpp>

//#include <boost/qvm/all.hpp>
#include <boost/math/constants/constants.hpp>

namespace qvm = boost::qvm;
namespace constants = boost::math::double_constants;
namespace ublas= boost::numeric::ublas;
//namespace trans = boost::geometry::strategy::transform;

//Configuration cyclicShiftRight(Configuration &in)
//{
//	bool lastVal = in.pop_back();
//	in.push_front(lastVal);
//	return in;
//}

//int main0()
//{
//	std::vector<Configuration> configs(3);
//	std::vector<int> a = { 1,3,2 }, b = { 1,1,1 }, c = { 1 };
//	configs[0] = Configuration(a);
//	configs[1] = Configuration(b);
//	configs[2] = Configuration(c);
//	std::vector<void*> functions;
//	functions.push_back(cyclicShiftRight);
//	Problem p(configs,functions);
//    return 0;
//}
//
//template<typename T,int Rows,int Cols>
//void print_qvm_matrix(qvm::mat<T,Rows,Cols> m)//TODO: rewrite as cout extension for bMatrix
//{
//	for (size_t i = 0; i < Rows; i++)
//	{
//		for (size_t j = 0; j < Cols; j++)
//		{
//			std::cout << m.a[i][j]<<" ";
//		}
//		std::cout << std::endl;
//	}
//}
//template<typename T, int Rows>
//void print_qvm_vector(qvm::vec<T, Rows> v)
//{
//	for (size_t i = 0; i < Rows; i++)
//	{
//		std::cout << v.a[i]<<" ";
//	}
//	std::cout << std::endl;
//}
//
//template<typename T, int Rows, int Cols, int i, int j>
//qvm::mat<T, i, j>grid_matrix(int i, int j, qvm::mat<T,Rows,Cols> figure)
//{
//	qvm::mat<T, i, j> m=qvm::zero_mat<int,i,j>;
//
//}


//int main1()//qvm bMatrix manipulations, require #include <boost/qvm/all.hpp>
//{
//	boost::qvm::vec<int, 2> v = { 1,3 };
//	boost::qvm::mat<int, 2, 2> m = { 0,1,1,0 };
//	boost::qvm::mat<int, 3, 3> t = qvm::translation_mat(v);
//	boost::qvm::mat<int, 3, 3> r = qvm::rotx_mat<3>(constants::half_pi);
//	boost::qvm::mat<int, 3, 3> rf = { -1,0,0,
//									  0,1,0,
//									  0,0,1 };
//	boost::qvm::mat<int, 3, 4 >gamma_figure= {1,1,1,2,
//		1,2,3,3,
//		1,1,1,1};
//	auto res =  r*gamma_figure;
//	boost::qvm::mat<int, 2, 3> to_nonhomogen_mat = { 1,0,0, 0,1,0};
//	boost::qvm::mat<int, 2, 4 > homogen_to_cartesian_figure = qvm::del_row<2>(gamma_figure);
//	boost::qvm::mat<int, 3, 4> cartesian_to_homogen_figure;
//	for (int i = 0; i < 2; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			cartesian_to_homogen_figure.a[i][j] = homogen_to_cartesian_figure.a[i][j];
//		}
//	}
//
//	for (int i = 0; i < 4; i++)
//	{
//		cartesian_to_homogen_figure.a[2][i] = 1;
//	}
//
//
//	print_qvm_matrix(gamma_figure);
//	std::cout << std::endl;
//	print_qvm_matrix(homogen_to_cartesian_figure);
//	std::cout << std::endl;
//	print_qvm_matrix(cartesian_to_homogen_figure);
//	
//	//what if?
//
//	
//	qvm::vec<int, 2> grid_sizes = { 10,10 };
//	//transforming to grid form?
//	return 0;
//}


int main2()
{
	//Basic work w/ vectors 
	ublas::vector<int> v(2);
	v(0) = 1;
	v(1) = 3;
	//Basic work w/ matrixes
	ublas::matrix<int> m1(2, 2), m2(2, 2);
	for (size_t i = 0; i < m1.size1(); i++)
	{
		for (size_t j = 0; j < m1.size2(); j++)
		{
			m1(i, j) = (i + j) * 2 + 2;
		}
	}

	////adding aliases for basic types
	//namespace bg = boost::geometry;
	//namespace trans = bg::strategy::transform;
	//using bg::dsv;
	//typedef bg::model::point<int, 2, boost::geometry::cs::cartesian> point;
	//typedef bg::model::multi_point<point> mpoint_t;
	////Basic work w/ boost/geometry
	//mpoint_t p1{ { {1,1} ,{2,1},{3,1},{1,2}} }, p2, p3, p4;
	////reading multipoints from file
	//std::ifstream inFile("figures.txt", std::ifstream::in);
	//std::string line="";
	//std::getline(inFile, line);
	//bg::read_wkt(line, p1);
	////also possible to use bg::append()
	//p1.push_back({ 2,2 });
	//bg::append(p1, point(4, 2));
	//std::cout << bg::num_points(p1) << std::endl;
	//std::cout << p1[0].get<1>() << std::endl;
	////Three types of transformation: translation, inversion, mirroring;
	//trans::translate_transformer<int, 2, 2> myTranslation(0, +2);
	//trans::rotate_transformer<bg::degree, double,2,2> myRotation(180.0);
	//trans::scale_transformer<int, 2, 2> myScale(-1, 1);
	//bg::transform(p1, p2, myTranslation);
	//bg::transform(p1, p3, myRotation);
	//bg::transform(p1, p4, myScale);
	//std::cout << dsv(p1) << std::endl;
	//std::cout << dsv(p2) << std::endl;
	//std::cout << dsv(p3) << std::endl;
	//std::cout << dsv(p4) << std::endl;
	//std::cout << bg::num_points(p1) <<std::endl;
	//std::cout << p1[0].get<1>() << std::endl;
	////transforming multipoint to matrix
	//matrix<int>m3=zero_matrix<int>(5,5);
	//for (auto const &p: p1)
	//{
	//	m3(p.get<0>(), p.get<1>()) = 1;
	//}
	//std::cout << m3 << std::endl;
	return 0;
}

void test1()
{
	std::string restrictionsFile = "restrictions.txt", figuresFile = "figures.txt";
	QPPSeq testQPP(restrictionsFile, figuresFile);
	testQPP.showMatrix();
	std::vector<int> t = testQPP.getFiguresOrder();
	std::cout << "Current order is ";
	for (int x : t)
		std::cout << x << ", ";
	std::cout << std::endl << "free space is: " << testQPP.getFreeArea() << " cells, " << testQPP.getPlacedFiguresAmount() << " figures are placed." << std::endl << std::endl;
	MultiPoint2D testRotationFigure, r1, r2, r3;
	boost::geometry::read_wkt("MULTIPOINT(0 0, 1 0, 0 1, 2 1, 0 2, 1 2, 1 3)", testRotationFigure);
	r1 = testQPP.rotateSavingLeftCorner(testRotationFigure, rotation::bottom, 0, 0, 3, 4);
	r2 = testQPP.rotateSavingLeftCorner(testRotationFigure, rotation::left, 0, 0, 3, 4);
	r3 = testQPP.rotateSavingLeftCorner(testRotationFigure, rotation::top, 0, 0, 3, 4);


	testQPP.swapPositionsInCurrentOrder(0, 1);
	testQPP.showMatrix();
	t = testQPP.getFiguresOrder();
	std::cout << "Current order is ";
	for (int x : t)
		std::cout << x << ", ";
	std::cout << std::endl << "free space is: " << testQPP.getFreeArea() << " cells, " << testQPP.getPlacedFiguresAmount() << " figures are placed." << std::endl << std::endl;

	std::reverse(t.begin(), t.end());
	auto oldStates = testQPP.getFiguresStates();
	testQPP.packFigures(oldStates, t);
	testQPP.showMatrix();
	std::cout << "Current order is ";
	for (int x : t)
		std::cout << x << ", ";
	std::cout << std::endl << "free space is: " << testQPP.getFreeArea() << " cells, " << testQPP.getPlacedFiguresAmount() << " figures are placed." << std::endl << std::endl;
}

int main()
{
	GASolverSeq ga("Problems/2/figures.txt", "Problems/2/restrictions.txt", "Problems/2/result.txt", 16, 0.5, 10);
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		ga.makeIteration();
		if (i % 1000 == 0)
		{
			std::cout << ga.GetNormalizedMaximalConfigCost() << " " << ga.GetNormaizedConfigPoolCost() << std::endl;	
		}
		if (ga.GetNormalizedMaximalConfigCost() == 0)
		{
			std::cout << i << " iterations" << std::endl;
			break;
		}
	}
	std::cout << ga.GetNormalizedMaximalConfigCost() << std::endl;
	ga.showMatrix(0);
	ga.saveResults(0);
	auto end = std::chrono::system_clock::now();
	auto elapsed = end - start;
	std::cout << "seconds: " << std::chrono::duration_cast<std::chrono::seconds>( elapsed).count() << std::endl;
	ga.saveResults(0);
	//testQPP.changeFigure(0, s1);
	//GASolver ga("figures.txt", "restrictions.txt", "result.txt", 16, 0.5, 10);
	return 0;
}