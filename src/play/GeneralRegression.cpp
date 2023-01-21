#include "GeneralRegression.h"

/*
 *@brief This namespace contains the functions that do the linear regression on
 *a basis of functions using the Eigen library
 *
 */

namespace TheRegression {

/*
 *@brief This functions sets up the Eigen matrix Q of type Xd (dense matrix
 *because the data isn't sparse) such that:
 *@brief (Q)_(i, j) = (phi_j(x_i)) where phi_j denotes the j-th function of the
 *basis and x_i the i-th chessboard
 *@param basis:  a given basis of functions chosen from the different bases
 *below
 *@param X: the chessboard (we only care for the positions of the pons so for
 *each i: X[2 * i] is the position of the i-th chessboard top pons) and X[2 * i
 *+ 1] the i-th chessboards bottom pons
 *@param dimension: dimension of the function basis (probably always 23)
 *@param num_data_points: how manys chessboards we are given
 *@return Q: an eigen matrix as described above.
 */

Eigen::MatrixXd setUpQ(Func *basis, int **X, int dimension,
											 int num_data_points) {
	Eigen::MatrixXd Q(num_data_points, dimension);

	for (int j = 0; j < dimension; j++) {
		for (int i = 0; i < num_data_points; i++) {
			Q(i, j) = basis[j].Eval(X[2 * i], X[2 * i + 1]);
		}
	}

	return Q;
}

/*
 *@brief This function creates the vector containing the closness (on a interval
 *from 0 to 1) of each give chessboard in order
 *@param data_outputs: the data outputs for closness given by the data team
 *@param num_data_points: how manys chessboards we are given
 *@return Y: the vector described above.
 */

Eigen::VectorXd setUpYVect(double *data_outputs, int num_data_points) {
	Eigen::VectorXd Y(num_data_points);
	for (int i = 0; i < num_data_points; i++) {
		Y(i) = data_outputs[i];
	}

	return Y;
}

/*
 *@brief This function uses the two function below to create  Q and Y. Then,
 *using a statistical method  it determines the best linear
 *@brief combination of the basis of functions to best approximate the outputs
 *in Y. This regression is based on least square method.
 *@brief note that at some point we ad a matrix mu. The reason for this is that
 *we need to make sure trans_Q * Q is invertible, by adding
 *@brief and identity matrix times a small value, the new obtained matrix isn't
 *invertible iff minus the small value is an eigenvalue which isn't probable.
 *@param basis:  a given basis of functions chosen from the different bases
 *below
 *@param X: the chessboard (we only care for the positions of the pons so for
 *each i: X[2 * i] is the position of the i-th chessboard top pons) and X[2 * i
 *+ 1] the i-th chessboards bottom pons
 *@param dimension: dimension of the function basis (probably always 23)
 *@param num_data_points: how manys chessboards we are given
 *@return theta: of length dimension, gives the best linear combination of the
 *basis of functions to best approximate the data.
 */

Eigen::VectorXd bestFitF(Func *basis, int **X, double *data_outputs,
												 int dimension, int num_data_points) {

	Eigen::MatrixXd Q = setUpQ(basis, X, dimension, num_data_points);
	Eigen::VectorXd Y = setUpYVect(data_outputs, num_data_points);

	Eigen::MatrixXd trans_Q = Q.transpose();
	Eigen::MatrixXd mu(23, 23);
	mu.setIdentity();
	Eigen::MatrixXd helper_M = (trans_Q * Q + mu * 0.001).inverse();

	return (helper_M * trans_Q) * Y;
}

} // namespace TheRegression

/*
 *@brief This first namespace enables us to Genearate a basis based on the
 *square distance function solely.
 *
 */

namespace SqrtDifBasis {

/*
 *@brief this method constructs the basis of functions described below:
 *@brief     - first we have the square distance between the heights of
 *consecutive pons of the top player
 *@brief     - same thing for the bottom player
 *@brief     - then the square difference between pons of distinct players that
 *share the same collumn
 *@brief     - finally the last function is the distance between the average
 *height of the players and 3.5 (middle of the board)
 *@return basis: the basis described above.
 */

Func *GenerateBasis() {
	Func *basis = new Func[23];

	for (int i = 0; i < 7; i++) {
		basis[i].InitFunc(1, i, 1, 2);		 // top pons consecutive difference
		basis[i + 7].InitFunc(1, i, 1, 1); // bottom pons consecutive difference
	}
	for (int j = 0; j < 8; j++) {
		basis[j + 14].InitFunc(1, j, 2, -1);
	}

	basis[22].InitFunc(0, -1, 2, -1);

	return basis;
}

} // namespace SqrtDifBasis
/*
 *@brief This first namespace enables us to Genearate a basis based on the
 *modified erf distance function solely.
 *
 */

namespace AbsErfBasis {
/*
 *@brief this method constructs the basis of functions described below:
 *@brief     - first we have the modified erf distance  between the heights of
 *consecutive pons of the top player
 *@brief     - same thing for the bottom player
 *@brief     - then the modified erf distance between pons of distinct players
 *that share the same collumn
 *@brief     - finally the last function is the distance between the average
 *height of the players and 3.5 (middle of the board)
 *@return basis: the basis described above.
 */

Func *GenerateBasis() {
	Func *basis = new Func[23];

	for (int i = 0; i < 7; i++) {
		basis[i].InitFunc(2, i, 1, 2);		 // top pons consecutive difference
		basis[i + 7].InitFunc(2, i, 1, 1); // bottom pons consecutive difference
	}
	for (int j = 0; j < 8; j++) {
		basis[j + 14].InitFunc(2, j, 2, -1);
	}

	basis[22].InitFunc(0, -1, 2, -1);

	return basis;
}

} // namespace AbsErfBasis

namespace AbsSqrtDifBasis {
Func *GenerateBasis() {
	Func *basis = new Func[23];

	for (int i = 0; i < 7; i++) {
		basis[i].InitFunc(2, i, 1, 2);		 // top pons consecutive difference
		basis[i + 7].InitFunc(2, i, 1, 1); // bottom pons consecutive difference
	}
	for (int j = 0; j < 8; j++) {
		basis[j + 14].InitFunc(1, j, 2, -1);
	}

	basis[22].InitFunc(0, -1, 2, -1);

	return basis;
}

} // namespace AbsSqrtDifBasis

namespace SqrtAbsDifBasis {
Func *GenerateBasis() {
	Func *basis = new Func[23];

	for (int i = 0; i < 7; i++) {
		basis[i].InitFunc(1, i, 1, 2);		 // top pons consecutive difference
		basis[i + 7].InitFunc(1, i, 1, 1); // bottom pons consecutive difference
	}
	for (int j = 0; j < 8; j++) {
		basis[j + 14].InitFunc(2, j, 2, -1);
	}

	basis[22].InitFunc(0, -1, 2, -1);

	return basis;
}
} // namespace SqrtAbsDifBasis

namespace EvaluationFunction {

float Evaluate(Func *basis, Eigen::VectorXd theta, int *l_top_pons,
							 int *l_bottom_pons, int dimension) {
	float output_val = 0;
	for (int i = 0; i < dimension; i++) {
		output_val += basis[i].Eval(l_top_pons, l_bottom_pons) * theta[i];
	}
	if (output_val >= 1) {
		return 1;
	}

	else if (output_val <= 0) {
		return 0;
	}

	return output_val;
}
/*
 *@brief this function enables us to test how well the ai performs on the data
 *that wasn't used for training it but for testing.
 *@return emp_risk: the empirical risk of the outputed data.
 */

float TestAi(Func *basis, Eigen::VectorXd theta, int **test_data_points,
						 double *outputs, int dimension, int num_data_points) {
	float emp_risk = 0;

	for (int i = 0; i < num_data_points; i++) {
		emp_risk +=
				pow(outputs[i] - Evaluate(basis, theta, test_data_points[2 * i],
																	test_data_points[2 * i + 1], dimension),
						2);
	}

	return emp_risk / (float)num_data_points;
}
} // namespace EvaluationFunction