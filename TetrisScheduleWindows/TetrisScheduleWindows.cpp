// TetrisScheduleWindows.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Configuration.h"
#include "Problem.h"

#include <boost/qvm/all.hpp>
#include <boost/math/constants/constants.hpp>
#include <iostream>

namespace qvm = boost::qvm;
namespace constants = boost::math::double_constants;

Configuration cyclicShiftRight(Configuration &in)
{
	bool lastVal = in.pop_back();
	in.push_front(lastVal);
	return in;
}

int main0()
{
	std::vector<Configuration> configs(3);
	std::vector<int> a = { 1,3,2 }, b = { 1,1,1 }, c = { 1 };
	configs[0] = Configuration(a);
	configs[1] = Configuration(b);
	configs[2] = Configuration(c);
	std::vector<void*> functions;
	functions.push_back(cyclicShiftRight);
	Problem p(configs,functions);
    return 0;
}

template<typename T,int Rows,int Cols>
void print_qvm_matrix(qvm::mat<T,Rows,Cols> m)//TODO: rewrite as cout extension for matrix
{
	for (size_t i = 0; i < Rows; i++)
	{
		for (size_t j = 0; j < Cols; j++)
		{
			std::cout << m.a[i][j]<<" ";
		}
		std::cout << std::endl;
	}
}
template<typename T, int Rows>
void print_qvm_vector(qvm::vec<T, Rows> v)
{
	for (size_t i = 0; i < Rows; i++)
	{
		std::cout << v.a[i]<<" ";
	}
	std::cout << std::endl;
}

template<typename T, int Rows, int Cols, int i, int j>
qvm::mat<T, i, j>grid_matrix(int i, int j, qvm::mat<T,Rows,Cols> figure)
{
	qvm::mat<T, i, j> m=qvm::zero_mat<int,i,j>;

}

int main()
{
	boost::qvm::vec<int, 2> v = { 1,3 };
	boost::qvm::mat<int, 2, 2> m = { 0,1,1,0 };
	boost::qvm::mat<int, 3, 3> t = qvm::translation_mat(v);
	boost::qvm::mat<int, 3, 3> r = qvm::rotx_mat<3>(constants::half_pi);
	boost::qvm::mat<int, 3, 3> rf = { -1,0,0,
									  0,1,0,
									  0,0,1 };
	boost::qvm::mat<int, 3, 4 >gamma_figure= {1,1,1,2,
		1,2,3,3,
		1,1,1,1};
	auto res =  r*gamma_figure;
	boost::qvm::mat<int, 2, 3> to_nonhomogen_mat = { 1,0,0, 0,1,0};
	boost::qvm::mat<int, 2, 4 > homogen_to_cartesian_figure = qvm::del_row<2>(gamma_figure);
	boost::qvm::mat<int, 3, 4> cartesian_to_homogen_figure;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cartesian_to_homogen_figure.a[i][j] = homogen_to_cartesian_figure.a[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		cartesian_to_homogen_figure.a[2][i] = 1;
	}


	print_qvm_matrix(gamma_figure);
	std::cout << std::endl;
	print_qvm_matrix(homogen_to_cartesian_figure);
	std::cout << std::endl;
	print_qvm_matrix(cartesian_to_homogen_figure);
	
	qvm::vec<int, 2> grid_sizes = { 10,10 };
	//transforming to grid form?

}