#pragma once
#include "C:\Users\Cassi\Downloads\eigen-3.4.0\eigen-3.4.0\Eigen\Dense"
//#include <Eigen/Dense>

/*
 *@brief This is the funtion class that will enables us to have an array of
 *functions without to many technical difficulties.
 *
 *The class contains the following methods:
 *
 *   - InitFunc: this will dermine what function of the class will be chosen.
 *The following are the possible functions:
 *
 *       - the square distance function of a distance
 *       - a modified erf function computed on the absolute value of a
 *difference
 *       - a modified erf function computed on the square of a difference
 *       - a modified arctan function computed on the absolute value of a
 *difference
 *       - a modified arctan function computed on the square of a difference
 *
 *   - Eval: Evaluates the function on a given chess board board. The
 *
 *
 */
class Func {
public:
	Func() {}

	/*
	 *@brief This method Inititialises the function we choose
	 *
	 *@param func_num: the function it will choose (possibilities are described
	 *above)
	 *@param  position_to_consider: all the function consider to specific pons out
	 *of the 16 provided (except the average function for which this parameter
	 *isn't used)
	 *@param difference_type: are we computing a difference on pons of the same
	 *color or different color
	 *@param height: in the case it is same color this tells us if we are
	 *considering the top or bottom pons
	 */

	void InitFunc(int func_num, int position_to_consider, int difference_type,
								int height) {
		this->func_num = func_num;
		placement = position_to_consider;
		this->difference_type = difference_type;
		this->height = height;
	}

	/*
	 *@brief Evaluate the function on a board (because we only consider the pons
	 *on the board here, l_top_pons and l_bottom_pons fully describe the board)
	 *@param l_top_pons: the pons of the player at the top of the board
	 *@param l_bottom_pons: the pons of the player at the bottom of the board
	 *@return the actual computated value of the function
	 */

	float Eval(int *l_top_pons, int *l_bottom_pons) {
		if (difference_type == 1) {
			if (height == 2) {
				return Eval_help(l_top_pons);
			} else {
				return Eval_help(l_bottom_pons);
			}
		} else {
			return Eval_help(l_top_pons, l_bottom_pons);
		}
	}

	/*
	 *@brief helper function for Evaluate in the case where we are considering
	 *difference between pons heights of pons of distinct color
	 *@param l_top_pons: the pons of the player at the top of the board
	 *@param l_bottom_pons: the pons of the player at the bottom of the board
	 *@return the actual computated value of the function
	 */

	float Eval_help(int *l_top_pons, int *l_bottom_pons) {
		int t = l_top_pons[placement];
		int b = l_bottom_pons[placement];

		if (func_num == 0) {
			return 0;
		}

		if (func_num == 1) {
			return SquareDistance(t, b);
		} else {
			return AbsoluteErfDistance(t, b);
		}
	}

	/*
	 *@brief helper function for Evaluate in the case where we are considering
	 *difference between pons heights of pons of same color
	 *@param l: the pons could be pons of top or bottom player depending on
	 *initialization
	 *@return the actual computated value of the function
	 */

	float Eval_help(int *l) {
		int pon1 = l[placement];
		int pon2 = l[placement + 1];

		if (func_num == 1) {
			return SquareDistance(pon1, pon2);
		} else {
			return AbsoluteErfDistance(pon1, pon2);
		}
	}

	/*
	 *@brief We consider here the square difference of two heights of two pons on
	 *the board
	 *@return the square difference of two heights of two pons on the board
	 */

	int SquareDistance(int x, int y) { return (x - y) * (x - y); }

	/*
	 *@brief Here we consider a modified erf function. We use erf because it is a
	 *function that varies slowly and then very rapidly.
	 *@brief I modified it after some calculation so that is even more quickly
	 *(that's why there is the times 10) and is very close to zero for each
	 *difference less or equal to 1.
	 *@return computation of the function
	 */

	float AbsoluteErfDistance(int x, int y) { return erf((x - y) * (x - y)); }

	/*
	 *@brief this is to take into account that if on average the pons are shifted
	 *towards a certain side, the board should be less closed
	 *@param l_top_pons: the pons of the player at the top of the board
	 *@param l_bottom_pons: the pons of the player at the bottom of the board
	 *@return the distance between the average position of all of the pons and the
	 *middle of the board (of height 3.5)
	 */

	float AveragePos(int *l_top_pons, int *l_bottom_pons) {
		int tot_sum = 0;
		double pon_count = 0;

		for (int i = 0; i < 8; i++) {

			int pos_1 = l_top_pons[i];
			int pos_2 = l_bottom_pons[i];
			if (pos_1 <= 7) {
				tot_sum += pos_1;
				pon_count += 1;
			}

			if (pos_2 >= 0) {
				tot_sum += pos_2;
				pon_count += 1;
			}
		}

		float ret = (float)tot_sum / (float)pon_count;

		if (ret > 3.5) {
			return ret - 3.5;
		}
		return 3.5 - ret;
	}

private:
	int func_num;
	int placement;
	int difference_type;
	int height;
};

namespace TheRegression {
Eigen::MatrixXd setUpQ(Func *basis, int **X, int dimension,
											 int num_data_points);
Eigen::VectorXd setUpYVect(double *data_outputs, int num_data_points);
Eigen::VectorXd bestFitF(Func *basis, int **X, double *data_outputs,
												 int dimension, int num_data_points);
} // namespace TheRegression
namespace SqrtDifBasis {
Func *GenerateBasis();
}
namespace AbsErfBasis {
Func *GenerateBasis();
}
namespace AbsSqrtDifBasis {
Func *GenerateBasis();
}
namespace SqrtAbsDifBasis {
Func *GenerateBasis();
}
namespace EvaluationFunction {
Eigen::VectorXd getTheta();
float Evaluate(Func *basis, Eigen::VectorXd theta, int *l_top_pons,
							 int *l_bottom_pons, int dimension);
float TestAi(Func *basis, Eigen::VectorXd theta, int **test_data_points,
						 double *outputs, int dimension, int num_data_points);
} // namespace EvaluationFunction