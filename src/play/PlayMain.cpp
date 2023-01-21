#include "GeneralRegression.h"
#include <cmath>
#include <iostream>
#include <ctype.h>



int** convert_board(std::string fen)
{	
	int** ret = new int*[2];
	int sz = 0;
	while (fen[sz] != ' ')
	{
		sz += 1;
	}

	std::string sub_fen = fen.substr(0, sz);

	int* L_top = new int;
	int* L_bottom = new int;

	for (int j = 0; j < 8; j ++)
	{
		L_top[j] = 8;
		L_bottom[j] = -1;	
	}
	
	int counter = 0;
	int row = 0;

	for (int j = 0; j < sz; j ++)
	{
		char chess_piece = fen[j];
		

		if (chess_piece == '/')
		{
			counter = 0;
			row += 1;
		}

		else if (isdigit(chess_piece))
			{
				counter += (int)chess_piece - '0';
			}
		
		else
		{
			if (chess_piece == 'p')
			{
				L_bottom[counter] = row;
			}

			if ((chess_piece == 'P') && (L_top[counter] == 8))
			{
				L_top[counter] = row;
			}

			counter += 1;
		}
			
		
	}
	
	ret[0] = L_top;
	ret[1] = L_bottom;

	return ret;


}




float closeness_evf(std::string fen)
{
	Func *basis = SqrtDifBasis::GenerateBasis();
	int** board = convert_board(fen);
	Eigen::VectorXd theta = EvaluationFunction::getTheta();
	
	int dimension = 23;
	return EvaluationFunction::Evaluate(basis, theta, board[0],
							 board[1], dimension);


}

int main()
{
	std::cout << closeness_evf("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP3K1/1P6/RNBQ1B1R w - - 0 1");
	return 0;
}

