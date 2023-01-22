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

Eigen::MatrixXd setUpQ(Func *basis, std::vector<std::vector<int>> X,
											 int dimension, int num_data_points) {
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

Eigen::VectorXd setUpYVect(std::vector<double> data_outputs,
													 int num_data_points) {
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

Eigen::VectorXd bestFitF(Func *basis, std::vector<std::vector<int>> X,
												 std::vector<double> data_outputs, int dimension,
												 int num_data_points) {

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

float Evaluate(std::vector<int> l_top_pons, std::vector<int> l_bottom_pons,
							 int dimension) {
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

float TestAi(std::vector<std::vector<int>> test_data_points, double *outputs,
						 int dimension, int num_data_points) {
	float emp_risk = 0;

	for (int i = 0; i < num_data_points; i++) {
		emp_risk +=
				pow(outputs[i] - Evaluate(test_data_points[2 * i],
																	test_data_points[2 * i + 1], dimension),
						2);
	}

	return emp_risk / (float)num_data_points;
}

void init() {
	basis = SqrtDifBasis::GenerateBasis();

	// training data set for ai:
	std::vector<double> data_outputs(500);
	std::vector<std::vector<int>> X(1000);

#pragma region declarations
	X[0] = std::vector<int>({3, 3, 4, 1, 1, 2, 2, 5});
	X[1] = std::vector<int>({4, 4, 5, 2, 2, 3, 3, 6});

	X[2] = std::vector<int>({4, 3, 3, 4, 3, 3, 5, 4});
	X[3] = std::vector<int>({5, 4, 4, 5, 4, 4, 6, 5});

	X[4] = std::vector<int>({5, 4, 2, 2, 2, 5, 1, 4});
	X[5] = std::vector<int>({6, 5, 3, 3, 3, 6, 2, 5});

	X[6] = std::vector<int>({5, 5, 3, 3, 2, 2, 3, 5});
	X[7] = std::vector<int>({6, 6, 4, 4, 3, 3, 4, 6});

	X[8] = std::vector<int>({4, 3, 4, 3, 3, 3, 5, 4});
	X[9] = std::vector<int>({5, 4, 5, 4, 4, 4, 6, 5});

	X[10] = std::vector<int>({3, 3, 5, 4, 3, 1, 4, 4});
	X[11] = std::vector<int>({4, 4, 6, 5, 4, 2, 5, 5});

	X[12] = std::vector<int>({4, 1, 4, 1, 1, 4, 5, 5});
	X[13] = std::vector<int>({5, 2, 5, 2, 2, 5, 6, 6});

	X[14] = std::vector<int>({5, 1, 5, 2, 4, 1, 2, 1});
	X[15] = std::vector<int>({6, 2, 6, 3, 5, 2, 3, 2});

	X[16] = std::vector<int>({4, 3, 2, 1, 2, 5, 4, 5});
	X[17] = std::vector<int>({5, 4, 3, 2, 3, 6, 5, 6});

	X[18] = std::vector<int>({2, 1, 2, 3, 1, 3, 1, 1});
	X[19] = std::vector<int>({3, 2, 3, 4, 2, 4, 2, 2});

	X[20] = std::vector<int>({4, 3, 4, 3, 5, 5, 2, 1});
	X[21] = std::vector<int>({5, 4, 5, 4, 6, 6, 3, 2});

	X[22] = std::vector<int>({3, 2, 2, 3, 3, 2, 1, 4});
	X[23] = std::vector<int>({4, 3, 3, 4, 4, 3, 2, 5});

	X[24] = std::vector<int>({5, 1, 5, 3, 4, 3, 1, 3});
	X[25] = std::vector<int>({6, 2, 6, 4, 5, 4, 2, 4});

	X[26] = std::vector<int>({5, 2, 1, 5, 1, 4, 3, 1});
	X[27] = std::vector<int>({6, 3, 2, 6, 2, 5, 4, 2});

	X[28] = std::vector<int>({3, 2, 5, 2, 1, 5, 4, 1});
	X[29] = std::vector<int>({4, 3, 6, 3, 2, 6, 5, 2});

	X[30] = std::vector<int>({3, 4, 5, 2, 3, 3, 5, 2});
	X[31] = std::vector<int>({4, 5, 6, 3, 4, 4, 6, 3});

	X[32] = std::vector<int>({3, 1, 2, 1, 4, 1, 5, 1});
	X[33] = std::vector<int>({4, 2, 3, 2, 5, 2, 6, 2});

	X[34] = std::vector<int>({5, 3, 3, 4, 1, 5, 2, 3});
	X[35] = std::vector<int>({6, 4, 4, 5, 2, 6, 3, 4});

	X[36] = std::vector<int>({4, 4, 3, 4, 5, 3, 5, 1});
	X[37] = std::vector<int>({5, 5, 4, 5, 6, 4, 6, 2});

	X[38] = std::vector<int>({4, 1, 3, 3, 1, 4, 2, 5});
	X[39] = std::vector<int>({5, 2, 4, 4, 2, 5, 3, 6});

	X[40] = std::vector<int>({4, 2, 3, 3, 4, 3, 2, 5});
	X[41] = std::vector<int>({5, 3, 4, 4, 5, 4, 3, 6});

	X[42] = std::vector<int>({3, 2, 3, 1, 3, 2, 4, 4});
	X[43] = std::vector<int>({4, 3, 4, 2, 4, 3, 5, 5});

	X[44] = std::vector<int>({4, 3, 4, 5, 4, 2, 2, 3});
	X[45] = std::vector<int>({5, 4, 5, 6, 5, 3, 3, 4});

	X[46] = std::vector<int>({2, 5, 3, 5, 5, 3, 1, 3});
	X[47] = std::vector<int>({3, 6, 4, 6, 6, 4, 2, 4});

	X[48] = std::vector<int>({2, 3, 2, 2, 5, 3, 5, 1});
	X[49] = std::vector<int>({3, 4, 3, 3, 6, 4, 6, 2});

	X[50] = std::vector<int>({3, 5, 5, 4, 5, 1, 1, 3});
	X[51] = std::vector<int>({4, 6, 6, 5, 6, 2, 2, 4});

	X[52] = std::vector<int>({2, 4, 4, 1, 1, 4, 3, 5});
	X[53] = std::vector<int>({3, 5, 5, 2, 2, 5, 4, 6});

	X[54] = std::vector<int>({2, 2, 3, 4, 5, 4, 5, 3});
	X[55] = std::vector<int>({3, 3, 4, 5, 6, 5, 6, 4});

	X[56] = std::vector<int>({5, 1, 3, 5, 3, 4, 3, 1});
	X[57] = std::vector<int>({6, 2, 4, 6, 4, 5, 4, 2});

	X[58] = std::vector<int>({3, 1, 3, 5, 1, 1, 3, 5});
	X[59] = std::vector<int>({4, 2, 4, 6, 2, 2, 4, 6});

	X[60] = std::vector<int>({2, 2, 3, 3, 5, 5, 1, 5});
	X[61] = std::vector<int>({3, 3, 4, 4, 6, 6, 2, 6});

	X[62] = std::vector<int>({3, 3, 3, 1, 5, 5, 4, 3});
	X[63] = std::vector<int>({4, 4, 4, 2, 6, 6, 5, 4});

	X[64] = std::vector<int>({4, 1, 2, 4, 1, 1, 3, 2});
	X[65] = std::vector<int>({5, 2, 3, 5, 2, 2, 4, 3});

	X[66] = std::vector<int>({2, 5, 2, 4, 4, 2, 1, 2});
	X[67] = std::vector<int>({3, 6, 3, 5, 5, 3, 2, 3});

	X[68] = std::vector<int>({5, 3, 3, 4, 3, 4, 5, 5});
	X[69] = std::vector<int>({6, 4, 4, 5, 4, 5, 6, 6});

	X[70] = std::vector<int>({3, 2, 3, 3, 3, 3, 3, 5});
	X[71] = std::vector<int>({4, 3, 4, 4, 4, 4, 4, 6});

	X[72] = std::vector<int>({3, 2, 2, 1, 1, 4, 1, 4});
	X[73] = std::vector<int>({4, 3, 3, 2, 2, 5, 2, 5});

	X[74] = std::vector<int>({5, 5, 3, 2, 2, 4, 2, 1});
	X[75] = std::vector<int>({6, 6, 4, 3, 3, 5, 3, 2});

	X[76] = std::vector<int>({3, 4, 4, 5, 5, 1, 2, 2});
	X[77] = std::vector<int>({4, 5, 5, 6, 6, 2, 3, 3});

	X[78] = std::vector<int>({4, 4, 5, 3, 2, 1, 1, 3});
	X[79] = std::vector<int>({5, 5, 6, 4, 3, 2, 2, 4});

	X[80] = std::vector<int>({3, 1, 1, 3, 2, 2, 3, 5});
	X[81] = std::vector<int>({4, 2, 2, 4, 3, 3, 4, 6});

	X[82] = std::vector<int>({5, 1, 1, 2, 2, 1, 1, 4});
	X[83] = std::vector<int>({6, 2, 2, 3, 3, 2, 2, 5});

	X[84] = std::vector<int>({4, 1, 2, 1, 4, 1, 4, 2});
	X[85] = std::vector<int>({5, 2, 3, 2, 5, 2, 5, 3});

	X[86] = std::vector<int>({3, 5, 3, 2, 2, 3, 3, 4});
	X[87] = std::vector<int>({4, 6, 4, 3, 3, 4, 4, 5});

	X[88] = std::vector<int>({3, 1, 3, 4, 2, 4, 2, 2});
	X[89] = std::vector<int>({4, 2, 4, 5, 3, 5, 3, 3});

	X[90] = std::vector<int>({4, 3, 5, 5, 4, 3, 5, 3});
	X[91] = std::vector<int>({5, 4, 6, 6, 5, 4, 6, 4});

	X[92] = std::vector<int>({2, 3, 5, 2, 2, 3, 1, 5});
	X[93] = std::vector<int>({3, 4, 6, 3, 3, 4, 2, 6});

	X[94] = std::vector<int>({3, 4, 2, 5, 2, 1, 2, 3});
	X[95] = std::vector<int>({4, 5, 3, 6, 3, 2, 3, 4});

	X[96] = std::vector<int>({5, 1, 3, 4, 5, 4, 2, 2});
	X[97] = std::vector<int>({6, 2, 4, 5, 6, 5, 3, 3});

	X[98] = std::vector<int>({2, 3, 1, 1, 2, 3, 3, 3});
	X[99] = std::vector<int>({3, 4, 2, 2, 3, 4, 4, 4});

	X[100] = std::vector<int>({3, 3, 5, 3, 4, 2, 1, 5});
	X[101] = std::vector<int>({4, 4, 6, 4, 5, 3, 2, 6});

	X[102] = std::vector<int>({5, 1, 3, 1, 1, 2, 4, 3});
	X[103] = std::vector<int>({6, 2, 4, 2, 2, 3, 5, 4});

	X[104] = std::vector<int>({2, 3, 5, 5, 3, 3, 3, 1});
	X[105] = std::vector<int>({3, 4, 6, 6, 4, 4, 4, 2});

	X[106] = std::vector<int>({4, 2, 2, 2, 3, 2, 5, 4});
	X[107] = std::vector<int>({5, 3, 3, 3, 4, 3, 6, 5});

	X[108] = std::vector<int>({2, 2, 2, 3, 4, 2, 2, 5});
	X[109] = std::vector<int>({3, 3, 3, 4, 5, 3, 3, 6});

	X[110] = std::vector<int>({5, 3, 4, 2, 1, 3, 2, 3});
	X[111] = std::vector<int>({6, 4, 5, 3, 2, 4, 3, 4});

	X[112] = std::vector<int>({2, 3, 3, 3, 2, 4, 2, 3});
	X[113] = std::vector<int>({3, 4, 4, 4, 3, 5, 3, 4});

	X[114] = std::vector<int>({3, 3, 3, 5, 4, 4, 5, 4});
	X[115] = std::vector<int>({4, 4, 4, 6, 5, 5, 6, 5});

	X[116] = std::vector<int>({4, 3, 3, 3, 2, 1, 1, 4});
	X[117] = std::vector<int>({5, 4, 4, 4, 3, 2, 2, 5});

	X[118] = std::vector<int>({3, 3, 2, 5, 1, 1, 4, 1});
	X[119] = std::vector<int>({4, 4, 3, 6, 2, 2, 5, 2});

	X[120] = std::vector<int>({3, 4, 3, 2, 2, 1, 3, 1});
	X[121] = std::vector<int>({4, 5, 4, 3, 3, 2, 4, 2});

	X[122] = std::vector<int>({3, 4, 1, 3, 5, 4, 3, 3});
	X[123] = std::vector<int>({4, 5, 2, 4, 6, 5, 4, 4});

	X[124] = std::vector<int>({4, 5, 2, 2, 2, 1, 4, 4});
	X[125] = std::vector<int>({5, 6, 3, 3, 3, 2, 5, 5});

	X[126] = std::vector<int>({4, 4, 4, 3, 2, 3, 3, 5});
	X[127] = std::vector<int>({5, 5, 5, 4, 3, 4, 4, 6});

	X[128] = std::vector<int>({2, 3, 2, 3, 5, 2, 4, 2});
	X[129] = std::vector<int>({3, 4, 3, 4, 6, 3, 5, 3});

	X[130] = std::vector<int>({5, 5, 2, 2, 5, 3, 5, 1});
	X[131] = std::vector<int>({6, 6, 3, 3, 6, 4, 6, 2});

	X[132] = std::vector<int>({5, 1, 4, 1, 3, 4, 2, 5});
	X[133] = std::vector<int>({6, 2, 5, 2, 4, 5, 3, 6});

	X[134] = std::vector<int>({5, 1, 3, 2, 4, 5, 5, 5});
	X[135] = std::vector<int>({6, 2, 4, 3, 5, 6, 6, 6});

	X[136] = std::vector<int>({3, 3, 1, 5, 5, 4, 5, 5});
	X[137] = std::vector<int>({4, 4, 2, 6, 6, 5, 6, 6});

	X[138] = std::vector<int>({4, 5, 4, 4, 1, 2, 1, 1});
	X[139] = std::vector<int>({5, 6, 5, 5, 2, 3, 2, 2});

	X[140] = std::vector<int>({3, 1, 5, 5, 5, 2, 1, 1});
	X[141] = std::vector<int>({4, 2, 6, 6, 6, 3, 2, 2});

	X[142] = std::vector<int>({5, 3, 2, 2, 5, 3, 5, 3});
	X[143] = std::vector<int>({6, 4, 3, 3, 6, 4, 6, 4});

	X[144] = std::vector<int>({3, 2, 5, 4, 3, 4, 1, 1});
	X[145] = std::vector<int>({4, 3, 6, 5, 4, 5, 2, 2});

	X[146] = std::vector<int>({5, 4, 1, 1, 1, 5, 5, 5});
	X[147] = std::vector<int>({6, 5, 2, 2, 2, 6, 6, 6});

	X[148] = std::vector<int>({3, 4, 2, 5, 2, 4, 4, 2});
	X[149] = std::vector<int>({4, 5, 3, 6, 3, 5, 5, 3});

	X[150] = std::vector<int>({4, 1, 3, 4, 4, 4, 2, 4});
	X[151] = std::vector<int>({5, 2, 4, 5, 5, 5, 3, 5});

	X[152] = std::vector<int>({4, 2, 3, 5, 4, 4, 2, 2});
	X[153] = std::vector<int>({5, 3, 4, 6, 5, 5, 3, 3});

	X[154] = std::vector<int>({4, 1, 3, 1, 5, 1, 4, 3});
	X[155] = std::vector<int>({5, 2, 4, 2, 6, 2, 5, 4});

	X[156] = std::vector<int>({2, 3, 4, 4, 1, 5, 3, 2});
	X[157] = std::vector<int>({3, 4, 5, 5, 2, 6, 4, 3});

	X[158] = std::vector<int>({3, 4, 5, 2, 2, 1, 1, 4});
	X[159] = std::vector<int>({4, 5, 6, 3, 3, 2, 2, 5});

	X[160] = std::vector<int>({2, 5, 4, 5, 2, 4, 2, 1});
	X[161] = std::vector<int>({3, 6, 5, 6, 3, 5, 3, 2});

	X[162] = std::vector<int>({4, 5, 1, 4, 2, 1, 4, 2});
	X[163] = std::vector<int>({5, 6, 2, 5, 3, 2, 5, 3});

	X[164] = std::vector<int>({5, 1, 2, 4, 1, 2, 2, 3});
	X[165] = std::vector<int>({6, 2, 3, 5, 2, 3, 3, 4});

	X[166] = std::vector<int>({5, 3, 3, 2, 1, 3, 1, 3});
	X[167] = std::vector<int>({6, 4, 4, 3, 2, 4, 2, 4});

	X[168] = std::vector<int>({3, 1, 3, 1, 5, 5, 1, 5});
	X[169] = std::vector<int>({4, 2, 4, 2, 6, 6, 2, 6});

	X[170] = std::vector<int>({3, 4, 5, 1, 1, 3, 4, 3});
	X[171] = std::vector<int>({4, 5, 6, 2, 2, 4, 5, 4});

	X[172] = std::vector<int>({4, 1, 1, 5, 4, 2, 5, 1});
	X[173] = std::vector<int>({5, 2, 2, 6, 5, 3, 6, 2});

	X[174] = std::vector<int>({4, 1, 5, 3, 5, 4, 4, 4});
	X[175] = std::vector<int>({5, 2, 6, 4, 6, 5, 5, 5});

	X[176] = std::vector<int>({5, 2, 5, 2, 1, 1, 5, 3});
	X[177] = std::vector<int>({6, 3, 6, 3, 2, 2, 6, 4});

	X[178] = std::vector<int>({5, 2, 3, 3, 2, 2, 4, 5});
	X[179] = std::vector<int>({6, 3, 4, 4, 3, 3, 5, 6});

	X[180] = std::vector<int>({3, 2, 3, 5, 5, 5, 4, 2});
	X[181] = std::vector<int>({4, 3, 4, 6, 6, 6, 5, 3});

	X[182] = std::vector<int>({4, 3, 5, 4, 2, 1, 2, 3});
	X[183] = std::vector<int>({5, 4, 6, 5, 3, 2, 3, 4});

	X[184] = std::vector<int>({3, 3, 4, 2, 3, 3, 1, 5});
	X[185] = std::vector<int>({4, 4, 5, 3, 4, 4, 2, 6});

	X[186] = std::vector<int>({4, 3, 1, 2, 3, 3, 4, 3});
	X[187] = std::vector<int>({5, 4, 2, 3, 4, 4, 5, 4});

	X[188] = std::vector<int>({5, 3, 2, 5, 5, 1, 3, 5});
	X[189] = std::vector<int>({6, 4, 3, 6, 6, 2, 4, 6});

	X[190] = std::vector<int>({2, 2, 2, 3, 5, 3, 2, 5});
	X[191] = std::vector<int>({3, 3, 3, 4, 6, 4, 3, 6});

	X[192] = std::vector<int>({2, 5, 5, 5, 2, 1, 4, 4});
	X[193] = std::vector<int>({3, 6, 6, 6, 3, 2, 5, 5});

	X[194] = std::vector<int>({3, 1, 1, 3, 4, 5, 2, 1});
	X[195] = std::vector<int>({4, 2, 2, 4, 5, 6, 3, 2});

	X[196] = std::vector<int>({5, 4, 5, 1, 1, 1, 1, 5});
	X[197] = std::vector<int>({6, 5, 6, 2, 2, 2, 2, 6});

	X[198] = std::vector<int>({3, 1, 2, 3, 5, 1, 3, 2});
	X[199] = std::vector<int>({4, 2, 3, 4, 6, 2, 4, 3});

	X[200] = std::vector<int>({2, 1, 1, 4, 5, 1, 4, 3});
	X[201] = std::vector<int>({3, 2, 2, 5, 6, 2, 5, 4});

	X[202] = std::vector<int>({3, 4, 3, 2, 3, 4, 5, 4});
	X[203] = std::vector<int>({4, 5, 4, 3, 4, 5, 6, 5});

	X[204] = std::vector<int>({4, 5, 5, 2, 5, 3, 5, 2});
	X[205] = std::vector<int>({5, 6, 6, 3, 6, 4, 6, 3});

	X[206] = std::vector<int>({2, 3, 5, 3, 5, 1, 3, 3});
	X[207] = std::vector<int>({3, 4, 6, 4, 6, 2, 4, 4});

	X[208] = std::vector<int>({2, 5, 2, 3, 4, 4, 4, 5});
	X[209] = std::vector<int>({3, 6, 3, 4, 5, 5, 5, 6});

	X[210] = std::vector<int>({2, 3, 2, 3, 3, 4, 3, 5});
	X[211] = std::vector<int>({3, 4, 3, 4, 4, 5, 4, 6});

	X[212] = std::vector<int>({4, 4, 2, 1, 1, 5, 5, 4});
	X[213] = std::vector<int>({5, 5, 3, 2, 2, 6, 6, 5});

	X[214] = std::vector<int>({3, 5, 2, 5, 5, 4, 4, 1});
	X[215] = std::vector<int>({4, 6, 3, 6, 6, 5, 5, 2});

	X[216] = std::vector<int>({5, 5, 3, 1, 5, 3, 2, 5});
	X[217] = std::vector<int>({6, 6, 4, 2, 6, 4, 3, 6});

	X[218] = std::vector<int>({3, 4, 1, 5, 3, 3, 4, 4});
	X[219] = std::vector<int>({4, 5, 2, 6, 4, 4, 5, 5});

	X[220] = std::vector<int>({5, 4, 3, 5, 2, 4, 2, 3});
	X[221] = std::vector<int>({6, 5, 4, 6, 3, 5, 3, 4});

	X[222] = std::vector<int>({2, 1, 2, 1, 1, 4, 5, 4});
	X[223] = std::vector<int>({3, 2, 3, 2, 2, 5, 6, 5});

	X[224] = std::vector<int>({2, 5, 5, 4, 1, 5, 3, 5});
	X[225] = std::vector<int>({3, 6, 6, 5, 2, 6, 4, 6});

	X[226] = std::vector<int>({2, 3, 1, 1, 1, 1, 3, 2});
	X[227] = std::vector<int>({3, 4, 2, 2, 2, 2, 4, 3});

	X[228] = std::vector<int>({4, 1, 1, 3, 1, 3, 2, 3});
	X[229] = std::vector<int>({5, 2, 2, 4, 2, 4, 3, 4});

	X[230] = std::vector<int>({4, 2, 5, 2, 5, 5, 3, 2});
	X[231] = std::vector<int>({5, 3, 6, 3, 6, 6, 4, 3});

	X[232] = std::vector<int>({5, 1, 1, 1, 3, 4, 4, 1});
	X[233] = std::vector<int>({6, 2, 2, 2, 4, 5, 5, 2});

	X[234] = std::vector<int>({4, 5, 5, 5, 5, 4, 5, 2});
	X[235] = std::vector<int>({5, 6, 6, 6, 6, 5, 6, 3});

	X[236] = std::vector<int>({2, 5, 5, 2, 3, 5, 4, 4});
	X[237] = std::vector<int>({3, 6, 6, 3, 4, 6, 5, 5});

	X[238] = std::vector<int>({5, 2, 1, 4, 5, 1, 3, 2});
	X[239] = std::vector<int>({6, 3, 2, 5, 6, 2, 4, 3});

	X[240] = std::vector<int>({2, 4, 4, 4, 2, 1, 2, 5});
	X[241] = std::vector<int>({3, 5, 5, 5, 3, 2, 3, 6});

	X[242] = std::vector<int>({3, 1, 3, 3, 5, 5, 2, 1});
	X[243] = std::vector<int>({4, 2, 4, 4, 6, 6, 3, 2});

	X[244] = std::vector<int>({3, 3, 5, 4, 5, 3, 1, 5});
	X[245] = std::vector<int>({4, 4, 6, 5, 6, 4, 2, 6});

	X[246] = std::vector<int>({5, 3, 4, 4, 5, 5, 1, 1});
	X[247] = std::vector<int>({6, 4, 5, 5, 6, 6, 2, 2});

	X[248] = std::vector<int>({3, 2, 2, 1, 2, 1, 4, 1});
	X[249] = std::vector<int>({4, 3, 3, 2, 3, 2, 5, 2});

	X[250] = std::vector<int>({4, 5, 1, 4, 5, 4, 2, 4});
	X[251] = std::vector<int>({5, 6, 2, 5, 6, 5, 3, 5});

	X[252] = std::vector<int>({4, 1, 1, 1, 4, 5, 1, 3});
	X[253] = std::vector<int>({5, 2, 2, 2, 5, 6, 2, 4});

	X[254] = std::vector<int>({4, 2, 5, 1, 3, 1, 1, 1});
	X[255] = std::vector<int>({5, 3, 6, 2, 4, 2, 2, 2});

	X[256] = std::vector<int>({3, 3, 2, 3, 2, 5, 1, 5});
	X[257] = std::vector<int>({4, 4, 3, 4, 3, 6, 2, 6});

	X[258] = std::vector<int>({4, 5, 3, 2, 1, 1, 1, 5});
	X[259] = std::vector<int>({5, 6, 4, 3, 2, 2, 2, 6});

	X[260] = std::vector<int>({2, 3, 3, 3, 5, 2, 2, 2});
	X[261] = std::vector<int>({3, 4, 4, 4, 6, 3, 3, 3});

	X[262] = std::vector<int>({4, 3, 1, 1, 2, 4, 2, 2});
	X[263] = std::vector<int>({5, 4, 2, 2, 3, 5, 3, 3});

	X[264] = std::vector<int>({3, 5, 4, 2, 1, 1, 2, 5});
	X[265] = std::vector<int>({4, 6, 5, 3, 2, 2, 3, 6});

	X[266] = std::vector<int>({5, 4, 3, 5, 4, 4, 2, 4});
	X[267] = std::vector<int>({6, 5, 4, 6, 5, 5, 3, 5});

	X[268] = std::vector<int>({2, 4, 4, 1, 2, 2, 2, 3});
	X[269] = std::vector<int>({3, 5, 5, 2, 3, 3, 3, 4});

	X[270] = std::vector<int>({5, 1, 1, 5, 1, 3, 1, 4});
	X[271] = std::vector<int>({6, 2, 2, 6, 2, 4, 2, 5});

	X[272] = std::vector<int>({5, 3, 3, 5, 5, 3, 5, 1});
	X[273] = std::vector<int>({6, 4, 4, 6, 6, 4, 6, 2});

	X[274] = std::vector<int>({5, 4, 5, 1, 1, 1, 1, 5});
	X[275] = std::vector<int>({6, 5, 6, 2, 2, 2, 2, 6});

	X[276] = std::vector<int>({5, 3, 5, 3, 5, 5, 5, 4});
	X[277] = std::vector<int>({6, 4, 6, 4, 6, 6, 6, 5});

	X[278] = std::vector<int>({4, 5, 3, 4, 3, 2, 2, 3});
	X[279] = std::vector<int>({5, 6, 4, 5, 4, 3, 3, 4});

	X[280] = std::vector<int>({2, 3, 1, 1, 2, 1, 3, 3});
	X[281] = std::vector<int>({3, 4, 2, 2, 3, 2, 4, 4});

	X[282] = std::vector<int>({3, 5, 3, 4, 1, 2, 3, 4});
	X[283] = std::vector<int>({4, 6, 4, 5, 2, 3, 4, 5});

	X[284] = std::vector<int>({2, 1, 4, 3, 1, 1, 2, 5});
	X[285] = std::vector<int>({3, 2, 5, 4, 2, 2, 3, 6});

	X[286] = std::vector<int>({4, 3, 3, 1, 3, 4, 2, 3});
	X[287] = std::vector<int>({5, 4, 4, 2, 4, 5, 3, 4});

	X[288] = std::vector<int>({5, 1, 5, 3, 5, 1, 4, 3});
	X[289] = std::vector<int>({6, 2, 6, 4, 6, 2, 5, 4});

	X[290] = std::vector<int>({5, 5, 2, 4, 3, 5, 2, 2});
	X[291] = std::vector<int>({6, 6, 3, 5, 4, 6, 3, 3});

	X[292] = std::vector<int>({2, 4, 2, 3, 3, 3, 5, 1});
	X[293] = std::vector<int>({3, 5, 3, 4, 4, 4, 6, 2});

	X[294] = std::vector<int>({3, 4, 3, 5, 4, 1, 2, 2});
	X[295] = std::vector<int>({4, 5, 4, 6, 5, 2, 3, 3});

	X[296] = std::vector<int>({5, 2, 2, 5, 4, 4, 4, 1});
	X[297] = std::vector<int>({6, 3, 3, 6, 5, 5, 5, 2});

	X[298] = std::vector<int>({4, 1, 2, 5, 2, 5, 3, 5});
	X[299] = std::vector<int>({5, 2, 3, 6, 3, 6, 4, 6});

	X[300] = std::vector<int>({3, 3, 4, 2, 1, 1, 4, 1});
	X[301] = std::vector<int>({4, 4, 5, 3, 2, 2, 5, 2});

	X[302] = std::vector<int>({3, 1, 2, 2, 1, 5, 3, 3});
	X[303] = std::vector<int>({4, 2, 3, 3, 2, 6, 4, 4});

	X[304] = std::vector<int>({2, 3, 4, 5, 1, 2, 3, 4});
	X[305] = std::vector<int>({3, 4, 5, 6, 2, 3, 4, 5});

	X[306] = std::vector<int>({5, 3, 4, 2, 2, 1, 2, 1});
	X[307] = std::vector<int>({6, 4, 5, 3, 3, 2, 3, 2});

	X[308] = std::vector<int>({3, 4, 3, 1, 4, 1, 2, 4});
	X[309] = std::vector<int>({4, 5, 4, 2, 5, 2, 3, 5});

	X[310] = std::vector<int>({5, 1, 3, 4, 4, 2, 4, 1});
	X[311] = std::vector<int>({6, 2, 4, 5, 5, 3, 5, 2});

	X[312] = std::vector<int>({4, 2, 1, 4, 2, 3, 4, 2});
	X[313] = std::vector<int>({5, 3, 2, 5, 3, 4, 5, 3});

	X[314] = std::vector<int>({3, 1, 3, 4, 5, 4, 3, 5});
	X[315] = std::vector<int>({4, 2, 4, 5, 6, 5, 4, 6});

	X[316] = std::vector<int>({3, 3, 2, 4, 5, 4, 4, 2});
	X[317] = std::vector<int>({4, 4, 3, 5, 6, 5, 5, 3});

	X[318] = std::vector<int>({2, 3, 2, 5, 2, 5, 5, 1});
	X[319] = std::vector<int>({3, 4, 3, 6, 3, 6, 6, 2});

	X[320] = std::vector<int>({3, 1, 3, 2, 3, 4, 4, 3});
	X[321] = std::vector<int>({4, 2, 4, 3, 4, 5, 5, 4});

	X[322] = std::vector<int>({3, 2, 5, 3, 1, 3, 3, 1});
	X[323] = std::vector<int>({4, 3, 6, 4, 2, 4, 4, 2});

	X[324] = std::vector<int>({2, 1, 1, 5, 1, 1, 3, 3});
	X[325] = std::vector<int>({3, 2, 2, 6, 2, 2, 4, 4});

	X[326] = std::vector<int>({5, 2, 1, 4, 5, 1, 1, 4});
	X[327] = std::vector<int>({6, 3, 2, 5, 6, 2, 2, 5});

	X[328] = std::vector<int>({3, 1, 2, 5, 5, 5, 5, 2});
	X[329] = std::vector<int>({4, 2, 3, 6, 6, 6, 6, 3});

	X[330] = std::vector<int>({2, 1, 1, 2, 4, 3, 3, 4});
	X[331] = std::vector<int>({3, 2, 2, 3, 5, 4, 4, 5});

	X[332] = std::vector<int>({2, 4, 3, 3, 2, 5, 4, 2});
	X[333] = std::vector<int>({3, 5, 4, 4, 3, 6, 5, 3});

	X[334] = std::vector<int>({4, 1, 3, 1, 1, 1, 4, 1});
	X[335] = std::vector<int>({5, 2, 4, 2, 2, 2, 5, 2});

	X[336] = std::vector<int>({5, 1, 4, 4, 3, 2, 2, 3});
	X[337] = std::vector<int>({6, 2, 5, 5, 4, 3, 3, 4});

	X[338] = std::vector<int>({3, 2, 1, 2, 4, 2, 4, 2});
	X[339] = std::vector<int>({4, 3, 2, 3, 5, 3, 5, 3});

	X[340] = std::vector<int>({4, 1, 4, 1, 3, 3, 4, 4});
	X[341] = std::vector<int>({5, 2, 5, 2, 4, 4, 5, 5});

	X[342] = std::vector<int>({2, 5, 1, 1, 1, 2, 5, 2});
	X[343] = std::vector<int>({3, 6, 2, 2, 2, 3, 6, 3});

	X[344] = std::vector<int>({2, 5, 1, 5, 5, 5, 3, 4});
	X[345] = std::vector<int>({3, 6, 2, 6, 6, 6, 4, 5});

	X[346] = std::vector<int>({2, 4, 5, 4, 3, 4, 1, 1});
	X[347] = std::vector<int>({3, 5, 6, 5, 4, 5, 2, 2});

	X[348] = std::vector<int>({3, 4, 2, 5, 5, 1, 5, 2});
	X[349] = std::vector<int>({4, 5, 3, 6, 6, 2, 6, 3});

	X[350] = std::vector<int>({5, 5, 4, 1, 4, 2, 4, 5});
	X[351] = std::vector<int>({6, 6, 5, 2, 5, 3, 5, 6});

	X[352] = std::vector<int>({2, 5, 3, 3, 2, 3, 5, 4});
	X[353] = std::vector<int>({3, 6, 4, 4, 3, 4, 6, 5});

	X[354] = std::vector<int>({3, 4, 4, 4, 2, 4, 3, 2});
	X[355] = std::vector<int>({4, 5, 5, 5, 3, 5, 4, 3});

	X[356] = std::vector<int>({5, 3, 1, 3, 5, 3, 1, 5});
	X[357] = std::vector<int>({6, 4, 2, 4, 6, 4, 2, 6});

	X[358] = std::vector<int>({3, 1, 1, 2, 2, 3, 2, 2});
	X[359] = std::vector<int>({4, 2, 2, 3, 3, 4, 3, 3});

	X[360] = std::vector<int>({3, 3, 5, 3, 2, 2, 1, 4});
	X[361] = std::vector<int>({4, 4, 6, 4, 3, 3, 2, 5});

	X[362] = std::vector<int>({5, 1, 4, 1, 4, 3, 2, 3});
	X[363] = std::vector<int>({6, 2, 5, 2, 5, 4, 3, 4});

	X[364] = std::vector<int>({3, 1, 4, 2, 5, 3, 3, 1});
	X[365] = std::vector<int>({4, 2, 5, 3, 6, 4, 4, 2});

	X[366] = std::vector<int>({3, 4, 3, 5, 5, 5, 1, 3});
	X[367] = std::vector<int>({4, 5, 4, 6, 6, 6, 2, 4});

	X[368] = std::vector<int>({5, 1, 5, 1, 1, 4, 1, 3});
	X[369] = std::vector<int>({6, 2, 6, 2, 2, 5, 2, 4});

	X[370] = std::vector<int>({4, 1, 5, 1, 5, 3, 1, 3});
	X[371] = std::vector<int>({5, 2, 6, 2, 6, 4, 2, 4});

	X[372] = std::vector<int>({4, 3, 4, 1, 1, 1, 3, 3});
	X[373] = std::vector<int>({5, 4, 5, 2, 2, 2, 4, 4});

	X[374] = std::vector<int>({2, 3, 1, 3, 4, 3, 5, 5});
	X[375] = std::vector<int>({3, 4, 2, 4, 5, 4, 6, 6});

	X[376] = std::vector<int>({4, 2, 2, 5, 3, 2, 1, 4});
	X[377] = std::vector<int>({5, 3, 3, 6, 4, 3, 2, 5});

	X[378] = std::vector<int>({5, 3, 2, 3, 3, 1, 1, 5});
	X[379] = std::vector<int>({6, 4, 3, 4, 4, 2, 2, 6});

	X[380] = std::vector<int>({3, 3, 4, 2, 4, 1, 4, 1});
	X[381] = std::vector<int>({4, 4, 5, 3, 5, 2, 5, 2});

	X[382] = std::vector<int>({2, 4, 4, 3, 1, 4, 5, 5});
	X[383] = std::vector<int>({3, 5, 5, 4, 2, 5, 6, 6});

	X[384] = std::vector<int>({5, 1, 2, 2, 2, 5, 1, 2});
	X[385] = std::vector<int>({6, 2, 3, 3, 3, 6, 2, 3});

	X[386] = std::vector<int>({3, 3, 2, 1, 5, 2, 4, 4});
	X[387] = std::vector<int>({4, 4, 3, 2, 6, 3, 5, 5});

	X[388] = std::vector<int>({2, 2, 4, 2, 4, 4, 2, 3});
	X[389] = std::vector<int>({3, 3, 5, 3, 5, 5, 3, 4});

	X[390] = std::vector<int>({4, 4, 1, 3, 1, 3, 5, 3});
	X[391] = std::vector<int>({5, 5, 2, 4, 2, 4, 6, 4});

	X[392] = std::vector<int>({2, 4, 1, 3, 3, 2, 1, 5});
	X[393] = std::vector<int>({3, 5, 2, 4, 4, 3, 2, 6});

	X[394] = std::vector<int>({2, 3, 2, 2, 4, 4, 2, 5});
	X[395] = std::vector<int>({3, 4, 3, 3, 5, 5, 3, 6});

	X[396] = std::vector<int>({3, 2, 1, 5, 3, 3, 3, 4});
	X[397] = std::vector<int>({4, 3, 2, 6, 4, 4, 4, 5});

	X[398] = std::vector<int>({5, 5, 1, 5, 2, 4, 5, 4});
	X[399] = std::vector<int>({6, 6, 2, 6, 3, 5, 6, 5});

	X[400] = std::vector<int>({2, 2, 1, 4, 4, 3, 3, 4});
	X[401] = std::vector<int>({3, 3, 2, 5, 5, 4, 4, 5});

	X[402] = std::vector<int>({2, 2, 3, 4, 3, 1, 4, 5});
	X[403] = std::vector<int>({3, 3, 4, 5, 4, 2, 5, 6});

	X[404] = std::vector<int>({3, 4, 4, 1, 5, 5, 4, 4});
	X[405] = std::vector<int>({4, 5, 5, 2, 6, 6, 5, 5});

	X[406] = std::vector<int>({3, 3, 5, 2, 2, 2, 1, 5});
	X[407] = std::vector<int>({4, 4, 6, 3, 3, 3, 2, 6});

	X[408] = std::vector<int>({5, 4, 4, 3, 1, 4, 5, 3});
	X[409] = std::vector<int>({6, 5, 5, 4, 2, 5, 6, 4});

	X[410] = std::vector<int>({4, 4, 4, 5, 5, 3, 4, 4});
	X[411] = std::vector<int>({5, 5, 5, 6, 6, 4, 5, 5});

	X[412] = std::vector<int>({5, 2, 1, 1, 3, 1, 4, 1});
	X[413] = std::vector<int>({6, 3, 2, 2, 4, 2, 5, 2});

	X[414] = std::vector<int>({2, 3, 4, 4, 2, 1, 5, 5});
	X[415] = std::vector<int>({3, 4, 5, 5, 3, 2, 6, 6});

	X[416] = std::vector<int>({3, 4, 2, 5, 1, 5, 4, 4});
	X[417] = std::vector<int>({4, 5, 3, 6, 2, 6, 5, 5});

	X[418] = std::vector<int>({4, 4, 1, 1, 1, 1, 5, 5});
	X[419] = std::vector<int>({5, 5, 2, 2, 2, 2, 6, 6});

	X[420] = std::vector<int>({2, 3, 3, 2, 5, 2, 2, 3});
	X[421] = std::vector<int>({3, 4, 4, 3, 6, 3, 3, 4});

	X[422] = std::vector<int>({5, 3, 3, 1, 4, 3, 2, 5});
	X[423] = std::vector<int>({6, 4, 4, 2, 5, 4, 3, 6});

	X[424] = std::vector<int>({2, 2, 1, 5, 3, 1, 3, 4});
	X[425] = std::vector<int>({3, 3, 2, 6, 4, 2, 4, 5});

	X[426] = std::vector<int>({2, 5, 5, 3, 5, 1, 3, 3});
	X[427] = std::vector<int>({3, 6, 6, 4, 6, 2, 4, 4});

	X[428] = std::vector<int>({3, 5, 4, 3, 4, 5, 3, 1});
	X[429] = std::vector<int>({4, 6, 5, 4, 5, 6, 4, 2});

	X[430] = std::vector<int>({3, 3, 4, 1, 5, 2, 3, 3});
	X[431] = std::vector<int>({4, 4, 5, 2, 6, 3, 4, 4});

	X[432] = std::vector<int>({4, 4, 3, 4, 4, 2, 4, 1});
	X[433] = std::vector<int>({5, 5, 4, 5, 5, 3, 5, 2});

	X[434] = std::vector<int>({3, 2, 1, 4, 5, 3, 1, 4});
	X[435] = std::vector<int>({4, 3, 2, 5, 6, 4, 2, 5});

	X[436] = std::vector<int>({2, 2, 1, 4, 2, 1, 2, 3});
	X[437] = std::vector<int>({3, 3, 2, 5, 3, 2, 3, 4});

	X[438] = std::vector<int>({2, 1, 5, 1, 4, 5, 1, 2});
	X[439] = std::vector<int>({3, 2, 6, 2, 5, 6, 2, 3});

	X[440] = std::vector<int>({4, 2, 3, 2, 5, 4, 5, 4});
	X[441] = std::vector<int>({5, 3, 4, 3, 6, 5, 6, 5});

	X[442] = std::vector<int>({5, 1, 4, 4, 5, 2, 4, 1});
	X[443] = std::vector<int>({6, 2, 5, 5, 6, 3, 5, 2});

	X[444] = std::vector<int>({2, 1, 4, 3, 2, 4, 5, 3});
	X[445] = std::vector<int>({3, 2, 5, 4, 3, 5, 6, 4});

	X[446] = std::vector<int>({2, 3, 1, 5, 4, 2, 3, 5});
	X[447] = std::vector<int>({3, 4, 2, 6, 5, 3, 4, 6});

	X[448] = std::vector<int>({2, 1, 5, 3, 3, 2, 1, 4});
	X[449] = std::vector<int>({3, 2, 6, 4, 4, 3, 2, 5});

	X[450] = std::vector<int>({2, 1, 5, 4, 1, 3, 5, 3});
	X[451] = std::vector<int>({3, 2, 6, 5, 2, 4, 6, 4});

	X[452] = std::vector<int>({3, 3, 5, 5, 1, 5, 4, 2});
	X[453] = std::vector<int>({4, 4, 6, 6, 2, 6, 5, 3});

	X[454] = std::vector<int>({4, 3, 1, 4, 1, 4, 4, 4});
	X[455] = std::vector<int>({5, 4, 2, 5, 2, 5, 5, 5});

	X[456] = std::vector<int>({2, 5, 5, 5, 4, 4, 5, 5});
	X[457] = std::vector<int>({3, 6, 6, 6, 5, 5, 6, 6});

	X[458] = std::vector<int>({4, 2, 4, 2, 4, 5, 5, 2});
	X[459] = std::vector<int>({5, 3, 5, 3, 5, 6, 6, 3});

	X[460] = std::vector<int>({4, 3, 3, 5, 1, 1, 2, 4});
	X[461] = std::vector<int>({5, 4, 4, 6, 2, 2, 3, 5});

	X[462] = std::vector<int>({4, 1, 3, 2, 1, 4, 3, 3});
	X[463] = std::vector<int>({5, 2, 4, 3, 2, 5, 4, 4});

	X[464] = std::vector<int>({5, 5, 1, 4, 5, 5, 3, 3});
	X[465] = std::vector<int>({6, 6, 2, 5, 6, 6, 4, 4});

	X[466] = std::vector<int>({2, 1, 1, 3, 2, 1, 5, 4});
	X[467] = std::vector<int>({3, 2, 2, 4, 3, 2, 6, 5});

	X[468] = std::vector<int>({5, 1, 2, 1, 5, 5, 4, 5});
	X[469] = std::vector<int>({6, 2, 3, 2, 6, 6, 5, 6});

	X[470] = std::vector<int>({5, 4, 4, 2, 3, 4, 5, 5});
	X[471] = std::vector<int>({6, 5, 5, 3, 4, 5, 6, 6});

	X[472] = std::vector<int>({2, 3, 3, 5, 4, 2, 5, 2});
	X[473] = std::vector<int>({3, 4, 4, 6, 5, 3, 6, 3});

	X[474] = std::vector<int>({2, 3, 5, 5, 3, 3, 2, 4});
	X[475] = std::vector<int>({3, 4, 6, 6, 4, 4, 3, 5});

	X[476] = std::vector<int>({3, 2, 3, 5, 2, 3, 2, 5});
	X[477] = std::vector<int>({4, 3, 4, 6, 3, 4, 3, 6});

	X[478] = std::vector<int>({4, 4, 1, 3, 1, 5, 3, 4});
	X[479] = std::vector<int>({5, 5, 2, 4, 2, 6, 4, 5});

	X[480] = std::vector<int>({2, 3, 3, 4, 3, 4, 4, 4});
	X[481] = std::vector<int>({3, 4, 4, 5, 4, 5, 5, 5});

	X[482] = std::vector<int>({2, 1, 1, 3, 3, 3, 4, 2});
	X[483] = std::vector<int>({3, 2, 2, 4, 4, 4, 5, 3});

	X[484] = std::vector<int>({4, 2, 2, 5, 2, 1, 3, 4});
	X[485] = std::vector<int>({5, 3, 3, 6, 3, 2, 4, 5});

	X[486] = std::vector<int>({5, 4, 4, 5, 5, 1, 4, 1});
	X[487] = std::vector<int>({6, 5, 5, 6, 6, 2, 5, 2});

	X[488] = std::vector<int>({2, 4, 4, 2, 4, 4, 3, 5});
	X[489] = std::vector<int>({3, 5, 5, 3, 5, 5, 4, 6});

	X[490] = std::vector<int>({5, 1, 5, 1, 1, 4, 4, 4});
	X[491] = std::vector<int>({6, 2, 6, 2, 2, 5, 5, 5});

	X[492] = std::vector<int>({2, 5, 1, 2, 4, 2, 4, 3});
	X[493] = std::vector<int>({3, 6, 2, 3, 5, 3, 5, 4});

	X[494] = std::vector<int>({4, 5, 1, 2, 1, 1, 3, 3});
	X[495] = std::vector<int>({5, 6, 2, 3, 2, 2, 4, 4});

	X[496] = std::vector<int>({2, 4, 4, 2, 3, 1, 2, 4});
	X[497] = std::vector<int>({3, 5, 5, 3, 4, 2, 3, 5});

	X[498] = std::vector<int>({3, 2, 3, 4, 1, 5, 2, 4});
	X[499] = std::vector<int>({4, 3, 4, 5, 2, 6, 3, 5});

	X[500] = std::vector<int>({8, 2, 1, 4, 4, 2, 2, 8});
	X[501] = std::vector<int>({2, 3, 3, -1, -1, 3, 4, 3});

	X[502] = std::vector<int>({3, 2, 3, 3, 2, 2, 3, 4});
	X[503] = std::vector<int>({5, 3, 5, 5, 4, 3, 5, -1});

	X[504] = std::vector<int>({3, 4, 8, 8, 5, 8, 1, 1});
	X[505] = std::vector<int>({4, -1, 3, 3, -1, 2, 3, 3});

	X[506] = std::vector<int>({8, 5, 1, 8, 1, 2, 8, 1});
	X[507] = std::vector<int>({2, -1, 3, 1, 3, 4, 2, 2});

	X[508] = std::vector<int>({5, 8, 8, 8, 8, 3, 1, 4});
	X[509] = std::vector<int>({-1, 3, 2, 1, 2, 5, 3, -1});

	X[510] = std::vector<int>({5, 3, 4, 8, 8, 4, 3, 4});
	X[511] = std::vector<int>({-1, 5, -1, 3, 3, -1, 5, -1});

	X[512] = std::vector<int>({8, 2, 8, 4, 8, 5, 8, 4});
	X[513] = std::vector<int>({3, 3, 1, -1, 1, -1, 1, -1});

	X[514] = std::vector<int>({5, 8, 5, 8, 3, 3, 1, 2});
	X[515] = std::vector<int>({-1, 1, -1, 1, 5, 5, 2, 3});

	X[516] = std::vector<int>({4, 2, 5, 5, 4, 5, 4, 5});
	X[517] = std::vector<int>({-1, 3, -1, -1, -1, -1, -1, -1});

	X[518] = std::vector<int>({4, 5, 5, 5, 2, 1, 2, 3});
	X[519] = std::vector<int>({-1, -1, -1, -1, 4, 3, 4, 5});

	X[520] = std::vector<int>({5, 8, 4, 4, 3, 4, 5, 5});
	X[521] = std::vector<int>({-1, 3, -1, -1, 5, -1, -1, -1});

	X[522] = std::vector<int>({3, 3, 3, 8, 4, 3, 8, 8});
	X[523] = std::vector<int>({5, 5, 5, 3, -1, 4, 2, 2});

	X[524] = std::vector<int>({4, 3, 5, 8, 3, 1, 8, 5});
	X[525] = std::vector<int>({-1, 4, -1, 2, 5, 2, 2, -1});

	X[526] = std::vector<int>({3, 5, 2, 5, 5, 4, 5, 1});
	X[527] = std::vector<int>({5, -1, 3, -1, -1, -1, -1, 2});

	X[528] = std::vector<int>({5, 2, 3, 8, 8, 4, 3, 2});
	X[529] = std::vector<int>({-1, 4, 4, 2, 2, -1, 5, 3});

	X[530] = std::vector<int>({8, 2, 3, 1, 2, 8, 1, 5});
	X[531] = std::vector<int>({3, 3, 4, 3, 3, 2, 3, -1});

	X[532] = std::vector<int>({3, 8, 1, 5, 3, 4, 2, 5});
	X[533] = std::vector<int>({4, 2, 3, -1, 5, -1, 3, -1});

	X[534] = std::vector<int>({5, 5, 8, 5, 8, 8, 5, 3});
	X[535] = std::vector<int>({-1, -1, 2, -1, 2, 1, -1, 4});

	X[536] = std::vector<int>({2, 3, 4, 4, 3, 8, 2, 3});
	X[537] = std::vector<int>({3, 4, -1, -1, 5, 3, 3, 5});

	X[538] = std::vector<int>({3, 1, 5, 3, 3, 1, 4, 2});
	X[539] = std::vector<int>({4, 3, -1, 4, 5, 2, -1, 3});

	X[540] = std::vector<int>({8, 4, 3, 3, 2, 4, 3, 1});
	X[541] = std::vector<int>({3, -1, 5, 5, 4, -1, 4, 3});

	X[542] = std::vector<int>({3, 4, 2, 4, 8, 2, 8, 3});
	X[543] = std::vector<int>({5, -1, 4, -1, 2, 3, 2, 4});

	X[544] = std::vector<int>({3, 8, 1, 4, 4, 8, 2, 4});
	X[545] = std::vector<int>({5, 2, 2, -1, -1, 3, 3, -1});

	X[546] = std::vector<int>({2, 8, 2, 8, 5, 4, 8, 1});
	X[547] = std::vector<int>({4, 1, 4, 3, -1, -1, 1, 3});

	X[548] = std::vector<int>({4, 2, 5, 4, 1, 3, 3, 8});
	X[549] = std::vector<int>({-1, 3, -1, -1, 2, 4, 5, 2});

	X[550] = std::vector<int>({2, 5, 1, 8, 8, 1, 1, 8});
	X[551] = std::vector<int>({3, -1, 2, 3, 1, 2, 3, 1});

	X[552] = std::vector<int>({2, 8, 2, 2, 5, 2, 8, 1});
	X[553] = std::vector<int>({4, 1, 3, 3, -1, 4, 1, 3});

	X[554] = std::vector<int>({3, 5, 4, 1, 8, 8, 3, 8});
	X[555] = std::vector<int>({4, -1, -1, 3, 3, 3, 5, 2});

	X[556] = std::vector<int>({2, 5, 4, 4, 3, 1, 1, 8});
	X[557] = std::vector<int>({4, -1, -1, -1, 4, 3, 3, 1});

	X[558] = std::vector<int>({2, 3, 8, 8, 4, 2, 8, 4});
	X[559] = std::vector<int>({4, 5, 2, 3, -1, 4, 3, -1});

	X[560] = std::vector<int>({4, 5, 2, 1, 8, 2, 8, 3});
	X[561] = std::vector<int>({-1, -1, 4, 3, 1, 3, 3, 4});

	X[562] = std::vector<int>({3, 8, 8, 2, 8, 1, 8, 1});
	X[563] = std::vector<int>({4, 2, 1, 4, 3, 3, 1, 2});

	X[564] = std::vector<int>({3, 2, 8, 8, 5, 1, 1, 5});
	X[565] = std::vector<int>({4, 3, 2, 2, -1, 3, 2, -1});

	X[566] = std::vector<int>({3, 8, 3, 8, 5, 5, 5, 4});
	X[567] = std::vector<int>({5, 1, 4, 2, -1, -1, -1, -1});

	X[568] = std::vector<int>({8, 3, 2, 2, 4, 8, 5, 1});
	X[569] = std::vector<int>({3, 4, 3, 4, -1, 1, -1, 3});

	X[570] = std::vector<int>({3, 5, 8, 3, 3, 2, 8, 8});
	X[571] = std::vector<int>({4, -1, 3, 4, 5, 4, 3, 3});

	X[572] = std::vector<int>({3, 8, 2, 5, 5, 8, 3, 4});
	X[573] = std::vector<int>({5, 1, 3, -1, -1, 3, 5, -1});

	X[574] = std::vector<int>({8, 4, 2, 5, 5, 3, 2, 2});
	X[575] = std::vector<int>({3, -1, 4, -1, -1, 5, 3, 4});

	X[576] = std::vector<int>({2, 8, 5, 3, 1, 3, 8, 8});
	X[577] = std::vector<int>({4, 2, -1, 4, 2, 4, 1, 2});

	X[578] = std::vector<int>({2, 2, 4, 2, 1, 3, 3, 1});
	X[579] = std::vector<int>({3, 4, -1, 4, 2, 5, 4, 2});

	X[580] = std::vector<int>({3, 8, 2, 4, 4, 2, 8, 5});
	X[581] = std::vector<int>({5, 3, 3, -1, -1, 3, 3, -1});

	X[582] = std::vector<int>({2, 5, 3, 1, 2, 8, 2, 2});
	X[583] = std::vector<int>({3, -1, 4, 3, 3, 2, 3, 4});

	X[584] = std::vector<int>({8, 8, 3, 4, 3, 2, 8, 3});
	X[585] = std::vector<int>({3, 1, 4, -1, 5, 4, 3, 4});

	X[586] = std::vector<int>({2, 3, 1, 1, 3, 3, 8, 8});
	X[587] = std::vector<int>({3, 4, 3, 3, 5, 4, 1, 1});

	X[588] = std::vector<int>({4, 8, 1, 8, 1, 3, 8, 5});
	X[589] = std::vector<int>({-1, 2, 3, 1, 2, 5, 3, -1});

	X[590] = std::vector<int>({4, 4, 2, 4, 5, 2, 5, 5});
	X[591] = std::vector<int>({-1, -1, 3, -1, -1, 4, -1, -1});

	X[592] = std::vector<int>({4, 1, 2, 3, 1, 3, 1, 1});
	X[593] = std::vector<int>({-1, 2, 3, 4, 2, 5, 2, 2});

	X[594] = std::vector<int>({2, 4, 1, 4, 4, 5, 2, 5});
	X[595] = std::vector<int>({4, -1, 2, -1, -1, -1, 3, -1});

	X[596] = std::vector<int>({3, 2, 4, 5, 5, 8, 2, 2});
	X[597] = std::vector<int>({5, 4, -1, -1, -1, 2, 4, 4});

	X[598] = std::vector<int>({4, 5, 5, 1, 5, 5, 8, 1});
	X[599] = std::vector<int>({-1, -1, -1, 3, -1, -1, 3, 3});

	X[600] = std::vector<int>({8, 8, 3, 3, 4, 1, 5, 4});
	X[601] = std::vector<int>({2, 2, 4, 4, -1, 2, -1, -1});

	X[602] = std::vector<int>({5, 5, 4, 5, 4, 3, 8, 3});
	X[603] = std::vector<int>({-1, -1, -1, -1, -1, 5, 2, 4});

	X[604] = std::vector<int>({2, 5, 8, 8, 4, 2, 2, 5});
	X[605] = std::vector<int>({4, -1, 2, 3, -1, 3, 3, -1});

	X[606] = std::vector<int>({4, 4, 1, 8, 5, 8, 4, 4});
	X[607] = std::vector<int>({-1, -1, 3, 3, -1, 3, -1, -1});

	X[608] = std::vector<int>({3, 5, 4, 1, 3, 5, 8, 8});
	X[609] = std::vector<int>({4, -1, -1, 3, 4, -1, 3, 1});

	X[610] = std::vector<int>({2, 5, 5, 5, 8, 4, 1, 5});
	X[611] = std::vector<int>({3, -1, -1, -1, 3, -1, 2, -1});

	X[612] = std::vector<int>({4, 1, 8, 5, 8, 5, 1, 4});
	X[613] = std::vector<int>({-1, 3, 1, -1, 2, -1, 3, -1});

	X[614] = std::vector<int>({5, 2, 8, 4, 8, 2, 2, 4});
	X[615] = std::vector<int>({-1, 4, 2, -1, 1, 4, 4, -1});

	X[616] = std::vector<int>({4, 5, 8, 1, 2, 1, 2, 1});
	X[617] = std::vector<int>({-1, -1, 3, 3, 3, 2, 4, 3});

	X[618] = std::vector<int>({2, 4, 4, 3, 5, 4, 8, 2});
	X[619] = std::vector<int>({4, -1, -1, 4, -1, -1, 3, 3});

	X[620] = std::vector<int>({8, 3, 4, 2, 4, 2, 3, 1});
	X[621] = std::vector<int>({2, 5, -1, 4, -1, 3, 4, 3});

	X[622] = std::vector<int>({5, 2, 2, 1, 8, 2, 5, 8});
	X[623] = std::vector<int>({-1, 4, 3, 2, 2, 3, -1, 1});

	X[624] = std::vector<int>({5, 1, 4, 3, 4, 1, 2, 1});
	X[625] = std::vector<int>({-1, 3, -1, 5, -1, 2, 4, 3});

	X[626] = std::vector<int>({4, 1, 1, 5, 4, 5, 4, 2});
	X[627] = std::vector<int>({-1, 3, 2, -1, -1, -1, -1, 4});

	X[628] = std::vector<int>({2, 1, 3, 8, 5, 8, 8, 3});
	X[629] = std::vector<int>({3, 2, 5, 3, -1, 2, 1, 5});

	X[630] = std::vector<int>({3, 4, 5, 4, 5, 3, 5, 4});
	X[631] = std::vector<int>({4, -1, -1, -1, -1, 5, -1, -1});

	X[632] = std::vector<int>({4, 5, 8, 1, 2, 1, 5, 5});
	X[633] = std::vector<int>({-1, -1, 2, 2, 3, 2, -1, -1});

	X[634] = std::vector<int>({4, 8, 1, 2, 5, 8, 1, 1});
	X[635] = std::vector<int>({-1, 1, 3, 4, -1, 2, 2, 2});

	X[636] = std::vector<int>({5, 1, 2, 3, 8, 8, 1, 5});
	X[637] = std::vector<int>({-1, 3, 3, 4, 1, 2, 3, -1});

	X[638] = std::vector<int>({5, 4, 2, 8, 5, 2, 5, 3});
	X[639] = std::vector<int>({-1, -1, 4, 3, -1, 4, -1, 5});

	X[640] = std::vector<int>({8, 5, 1, 2, 4, 3, 5, 5});
	X[641] = std::vector<int>({2, -1, 2, 3, -1, 4, -1, -1});

	X[642] = std::vector<int>({8, 8, 8, 4, 4, 4, 8, 5});
	X[643] = std::vector<int>({2, 3, 1, -1, -1, -1, 3, -1});

	X[644] = std::vector<int>({2, 8, 5, 8, 5, 2, 8, 4});
	X[645] = std::vector<int>({3, 1, -1, 3, -1, 4, 2, -1});

	X[646] = std::vector<int>({3, 3, 5, 1, 4, 1, 4, 4});
	X[647] = std::vector<int>({4, 4, -1, 3, -1, 3, -1, -1});

	X[648] = std::vector<int>({8, 4, 5, 3, 1, 2, 5, 2});
	X[649] = std::vector<int>({2, -1, -1, 5, 3, 3, -1, 3});

	X[650] = std::vector<int>({3, 5, 4, 2, 4, 8, 8, 3});
	X[651] = std::vector<int>({5, -1, -1, 4, -1, 3, 2, 4});

	X[652] = std::vector<int>({4, 1, 2, 2, 5, 1, 2, 8});
	X[653] = std::vector<int>({-1, 2, 3, 4, -1, 3, 4, 1});

	X[654] = std::vector<int>({3, 5, 4, 4, 1, 4, 5, 8});
	X[655] = std::vector<int>({5, -1, -1, -1, 2, -1, -1, 3});

	X[656] = std::vector<int>({2, 5, 1, 1, 3, 3, 2, 5});
	X[657] = std::vector<int>({4, -1, 2, 2, 4, 4, 4, -1});

	X[658] = std::vector<int>({5, 3, 1, 5, 2, 3, 8, 1});
	X[659] = std::vector<int>({-1, 4, 2, -1, 4, 5, 1, 3});

	X[660] = std::vector<int>({5, 1, 1, 2, 5, 3, 4, 5});
	X[661] = std::vector<int>({-1, 2, 2, 3, -1, 5, -1, -1});

	X[662] = std::vector<int>({4, 5, 2, 2, 4, 5, 1, 2});
	X[663] = std::vector<int>({-1, -1, 4, 3, -1, -1, 2, 3});

	X[664] = std::vector<int>({4, 4, 4, 5, 8, 4, 8, 3});
	X[665] = std::vector<int>({-1, -1, -1, -1, 2, -1, 2, 5});

	X[666] = std::vector<int>({4, 2, 2, 3, 2, 1, 8, 3});
	X[667] = std::vector<int>({-1, 4, 4, 4, 3, 3, 3, 4});

	X[668] = std::vector<int>({3, 1, 8, 5, 8, 8, 4, 8});
	X[669] = std::vector<int>({4, 2, 2, -1, 1, 3, -1, 2});

	X[670] = std::vector<int>({4, 4, 1, 8, 3, 3, 3, 8});
	X[671] = std::vector<int>({-1, -1, 2, 3, 4, 5, 5, 2});

	X[672] = std::vector<int>({3, 8, 5, 4, 8, 3, 2, 5});
	X[673] = std::vector<int>({4, 1, -1, -1, 1, 4, 4, -1});

	X[674] = std::vector<int>({4, 3, 2, 8, 2, 8, 8, 8});
	X[675] = std::vector<int>({-1, 5, 4, 2, 3, 3, 2, 2});

	X[676] = std::vector<int>({5, 5, 3, 8, 4, 5, 5, 5});
	X[677] = std::vector<int>({-1, -1, 5, 1, -1, -1, -1, -1});

	X[678] = std::vector<int>({8, 2, 8, 1, 4, 2, 2, 4});
	X[679] = std::vector<int>({3, 3, 3, 2, -1, 3, 3, -1});

	X[680] = std::vector<int>({3, 1, 1, 1, 4, 3, 8, 3});
	X[681] = std::vector<int>({5, 3, 3, 2, -1, 4, 1, 4});

	X[682] = std::vector<int>({2, 1, 2, 3, 4, 2, 2, 4});
	X[683] = std::vector<int>({4, 2, 4, 5, -1, 3, 3, -1});

	X[684] = std::vector<int>({4, 2, 1, 8, 4, 2, 3, 5});
	X[685] = std::vector<int>({-1, 3, 3, 2, -1, 4, 4, -1});

	X[686] = std::vector<int>({4, 4, 1, 5, 4, 8, 4, 1});
	X[687] = std::vector<int>({-1, -1, 2, -1, -1, 3, -1, 2});

	X[688] = std::vector<int>({4, 3, 5, 5, 5, 3, 2, 8});
	X[689] = std::vector<int>({-1, 5, -1, -1, -1, 4, 3, 3});

	X[690] = std::vector<int>({5, 5, 8, 2, 8, 2, 5, 3});
	X[691] = std::vector<int>({-1, -1, 3, 4, 3, 3, -1, 4});

	X[692] = std::vector<int>({3, 1, 4, 8, 8, 4, 1, 2});
	X[693] = std::vector<int>({4, 2, -1, 1, 1, -1, 2, 3});

	X[694] = std::vector<int>({5, 5, 5, 3, 3, 1, 8, 4});
	X[695] = std::vector<int>({-1, -1, -1, 4, 4, 3, 3, -1});

	X[696] = std::vector<int>({5, 4, 5, 4, 5, 3, 4, 8});
	X[697] = std::vector<int>({-1, -1, -1, -1, -1, 5, -1, 1});

	X[698] = std::vector<int>({8, 2, 4, 4, 4, 2, 3, 1});
	X[699] = std::vector<int>({2, 3, -1, -1, -1, 3, 5, 3});

	X[700] = std::vector<int>({5, 5, 1, 2, 3, 8, 4, 8});
	X[701] = std::vector<int>({-1, -1, 3, 3, 4, 1, -1, 3});

	X[702] = std::vector<int>({3, 1, 3, 4, 5, 1, 1, 1});
	X[703] = std::vector<int>({5, 2, 5, -1, -1, 3, 2, 3});

	X[704] = std::vector<int>({2, 4, 2, 3, 1, 1, 3, 5});
	X[705] = std::vector<int>({4, -1, 4, 5, 2, 2, 5, -1});

	X[706] = std::vector<int>({8, 5, 5, 1, 1, 1, 2, 8});
	X[707] = std::vector<int>({2, -1, -1, 3, 3, 3, 4, 2});

	X[708] = std::vector<int>({5, 2, 3, 5, 1, 1, 1, 8});
	X[709] = std::vector<int>({-1, 3, 4, -1, 3, 3, 3, 2});

	X[710] = std::vector<int>({4, 4, 4, 8, 5, 3, 8, 5});
	X[711] = std::vector<int>({-1, -1, -1, 1, -1, 5, 3, -1});

	X[712] = std::vector<int>({2, 2, 1, 8, 2, 3, 3, 1});
	X[713] = std::vector<int>({4, 4, 2, 3, 3, 4, 4, 3});

	X[714] = std::vector<int>({4, 2, 4, 3, 4, 3, 8, 5});
	X[715] = std::vector<int>({-1, 4, -1, 5, -1, 4, 3, -1});

	X[716] = std::vector<int>({4, 1, 8, 3, 1, 5, 8, 4});
	X[717] = std::vector<int>({-1, 3, 1, 4, 2, -1, 1, -1});

	X[718] = std::vector<int>({3, 3, 1, 3, 3, 5, 3, 3});
	X[719] = std::vector<int>({4, 5, 2, 5, 4, -1, 4, 4});

	X[720] = std::vector<int>({5, 8, 5, 5, 4, 4, 5, 5});
	X[721] = std::vector<int>({-1, 2, -1, -1, -1, -1, -1, -1});

	X[722] = std::vector<int>({2, 1, 5, 4, 3, 2, 4, 1});
	X[723] = std::vector<int>({3, 3, -1, -1, 5, 3, -1, 2});

	X[724] = std::vector<int>({8, 4, 5, 5, 2, 4, 2, 3});
	X[725] = std::vector<int>({2, -1, -1, -1, 3, -1, 4, 5});

	X[726] = std::vector<int>({5, 1, 1, 3, 2, 8, 4, 4});
	X[727] = std::vector<int>({-1, 3, 3, 4, 4, 3, -1, -1});

	X[728] = std::vector<int>({4, 2, 3, 4, 8, 3, 1, 4});
	X[729] = std::vector<int>({-1, 4, 5, -1, 3, 5, 3, -1});

	X[730] = std::vector<int>({4, 8, 4, 5, 5, 4, 5, 5});
	X[731] = std::vector<int>({-1, 2, -1, -1, -1, -1, -1, -1});

	X[732] = std::vector<int>({4, 4, 3, 8, 8, 5, 1, 1});
	X[733] = std::vector<int>({-1, -1, 4, 1, 1, -1, 2, 2});

	X[734] = std::vector<int>({3, 4, 4, 5, 2, 5, 5, 1});
	X[735] = std::vector<int>({5, -1, -1, -1, 4, -1, -1, 3});

	X[736] = std::vector<int>({4, 3, 4, 8, 5, 4, 1, 4});
	X[737] = std::vector<int>({-1, 5, -1, 2, -1, -1, 2, -1});

	X[738] = std::vector<int>({8, 1, 8, 1, 3, 5, 4, 8});
	X[739] = std::vector<int>({3, 2, 3, 3, 5, -1, -1, 2});

	X[740] = std::vector<int>({4, 4, 4, 5, 4, 2, 8, 4});
	X[741] = std::vector<int>({-1, -1, -1, -1, -1, 3, 1, -1});

	X[742] = std::vector<int>({2, 5, 5, 4, 3, 4, 8, 5});
	X[743] = std::vector<int>({4, -1, -1, -1, 4, -1, 2, -1});

	X[744] = std::vector<int>({5, 8, 4, 8, 1, 8, 5, 5});
	X[745] = std::vector<int>({-1, 3, -1, 3, 3, 2, -1, -1});

	X[746] = std::vector<int>({4, 4, 5, 2, 4, 8, 2, 5});
	X[747] = std::vector<int>({-1, -1, -1, 4, -1, 3, 3, -1});

	X[748] = std::vector<int>({2, 3, 5, 4, 4, 5, 4, 1});
	X[749] = std::vector<int>({4, 4, -1, -1, -1, -1, -1, 3});

	X[750] = std::vector<int>({5, 3, 5, 2, 8, 8, 4, 2});
	X[751] = std::vector<int>({-1, 5, -1, 3, 3, 3, -1, 3});

	X[752] = std::vector<int>({5, 1, 8, 4, 5, 8, 3, 4});
	X[753] = std::vector<int>({-1, 3, 3, -1, -1, 2, 4, -1});

	X[754] = std::vector<int>({2, 1, 5, 5, 4, 8, 8, 4});
	X[755] = std::vector<int>({4, 2, -1, -1, -1, 2, 1, -1});

	X[756] = std::vector<int>({4, 4, 5, 3, 3, 5, 1, 2});
	X[757] = std::vector<int>({-1, -1, -1, 4, 4, -1, 3, 3});

	X[758] = std::vector<int>({5, 8, 1, 5, 2, 3, 5, 4});
	X[759] = std::vector<int>({-1, 1, 3, -1, 3, 5, -1, -1});

	X[760] = std::vector<int>({2, 3, 3, 4, 5, 5, 3, 5});
	X[761] = std::vector<int>({3, 4, 4, -1, -1, -1, 4, -1});

	X[762] = std::vector<int>({2, 1, 4, 5, 8, 2, 1, 2});
	X[763] = std::vector<int>({3, 3, -1, -1, 3, 4, 2, 4});

	X[764] = std::vector<int>({3, 5, 2, 4, 3, 8, 2, 5});
	X[765] = std::vector<int>({4, -1, 3, -1, 4, 1, 4, -1});

	X[766] = std::vector<int>({5, 3, 3, 5, 2, 5, 1, 8});
	X[767] = std::vector<int>({-1, 4, 4, -1, 4, -1, 3, 1});

	X[768] = std::vector<int>({4, 5, 8, 2, 8, 3, 3, 8});
	X[769] = std::vector<int>({-1, -1, 2, 3, 2, 4, 4, 1});

	X[770] = std::vector<int>({4, 4, 2, 5, 8, 4, 5, 2});
	X[771] = std::vector<int>({-1, -1, 4, -1, 3, -1, -1, 3});

	X[772] = std::vector<int>({5, 1, 5, 4, 8, 4, 5, 5});
	X[773] = std::vector<int>({-1, 2, -1, -1, 2, -1, -1, -1});

	X[774] = std::vector<int>({3, 8, 5, 8, 2, 1, 5, 4});
	X[775] = std::vector<int>({4, 3, -1, 3, 3, 3, -1, -1});

	X[776] = std::vector<int>({4, 8, 4, 5, 1, 4, 3, 1});
	X[777] = std::vector<int>({-1, 1, -1, -1, 2, -1, 4, 3});

	X[778] = std::vector<int>({2, 4, 2, 1, 4, 4, 3, 5});
	X[779] = std::vector<int>({4, -1, 3, 3, -1, -1, 4, -1});

	X[780] = std::vector<int>({4, 4, 3, 4, 8, 8, 8, 1});
	X[781] = std::vector<int>({-1, -1, 4, -1, 2, 3, 3, 2});

	X[782] = std::vector<int>({5, 8, 5, 4, 8, 4, 4, 3});
	X[783] = std::vector<int>({-1, 1, -1, -1, 1, -1, -1, 4});

	X[784] = std::vector<int>({5, 4, 8, 3, 5, 4, 1, 2});
	X[785] = std::vector<int>({-1, -1, 3, 5, -1, -1, 2, 4});

	X[786] = std::vector<int>({5, 8, 8, 5, 4, 2, 4, 2});
	X[787] = std::vector<int>({-1, 3, 1, -1, -1, 3, -1, 4});

	X[788] = std::vector<int>({5, 2, 2, 2, 3, 3, 4, 4});
	X[789] = std::vector<int>({-1, 4, 4, 3, 5, 5, -1, -1});

	X[790] = std::vector<int>({5, 8, 3, 5, 5, 8, 4, 1});
	X[791] = std::vector<int>({-1, 3, 5, -1, -1, 2, -1, 2});

	X[792] = std::vector<int>({2, 1, 4, 1, 3, 5, 3, 5});
	X[793] = std::vector<int>({4, 2, -1, 3, 5, -1, 4, -1});

	X[794] = std::vector<int>({5, 5, 3, 1, 5, 4, 8, 1});
	X[795] = std::vector<int>({-1, -1, 5, 2, -1, -1, 1, 2});

	X[796] = std::vector<int>({3, 8, 5, 5, 4, 5, 3, 8});
	X[797] = std::vector<int>({4, 1, -1, -1, -1, -1, 5, 3});

	X[798] = std::vector<int>({5, 3, 5, 2, 4, 1, 5, 8});
	X[799] = std::vector<int>({-1, 5, -1, 3, -1, 3, -1, 1});

	X[800] = std::vector<int>({2, 2, 2, 5, 5, 5, 5, 4});
	X[801] = std::vector<int>({3, 4, 3, -1, -1, -1, -1, -1});

	X[802] = std::vector<int>({2, 1, 8, 3, 4, 1, 5, 5});
	X[803] = std::vector<int>({3, 3, 3, 4, -1, 3, -1, -1});

	X[804] = std::vector<int>({4, 4, 2, 5, 1, 8, 5, 2});
	X[805] = std::vector<int>({-1, -1, 3, -1, 2, 3, -1, 3});

	X[806] = std::vector<int>({5, 1, 5, 3, 3, 4, 1, 3});
	X[807] = std::vector<int>({-1, 2, -1, 4, 4, -1, 3, 5});

	X[808] = std::vector<int>({8, 5, 8, 3, 4, 5, 5, 1});
	X[809] = std::vector<int>({2, -1, 2, 4, -1, -1, -1, 2});

	X[810] = std::vector<int>({3, 4, 1, 4, 1, 4, 8, 4});
	X[811] = std::vector<int>({4, -1, 3, -1, 3, -1, 3, -1});

	X[812] = std::vector<int>({8, 4, 4, 4, 4, 3, 1, 3});
	X[813] = std::vector<int>({2, -1, -1, -1, -1, 4, 3, 4});

	X[814] = std::vector<int>({5, 4, 8, 4, 3, 8, 8, 2});
	X[815] = std::vector<int>({-1, -1, 2, -1, 4, 3, 1, 4});

	X[816] = std::vector<int>({5, 2, 8, 3, 8, 4, 2, 3});
	X[817] = std::vector<int>({-1, 4, 1, 5, 3, -1, 4, 4});

	X[818] = std::vector<int>({8, 5, 4, 5, 2, 4, 5, 8});
	X[819] = std::vector<int>({2, -1, -1, -1, 4, -1, -1, 3});

	X[820] = std::vector<int>({8, 8, 5, 1, 2, 2, 5, 5});
	X[821] = std::vector<int>({3, 1, -1, 2, 4, 4, -1, -1});

	X[822] = std::vector<int>({4, 5, 8, 8, 4, 2, 8, 3});
	X[823] = std::vector<int>({-1, -1, 3, 3, -1, 3, 3, 4});

	X[824] = std::vector<int>({5, 5, 3, 4, 8, 4, 5, 5});
	X[825] = std::vector<int>({-1, -1, 5, -1, 2, -1, -1, -1});

	X[826] = std::vector<int>({8, 8, 1, 5, 8, 4, 8, 8});
	X[827] = std::vector<int>({2, 1, 3, -1, 3, -1, 3, 1});

	X[828] = std::vector<int>({2, 5, 5, 5, 5, 5, 2, 4});
	X[829] = std::vector<int>({4, -1, -1, -1, -1, -1, 3, -1});

	X[830] = std::vector<int>({3, 8, 5, 4, 5, 5, 3, 3});
	X[831] = std::vector<int>({4, 3, -1, -1, -1, -1, 4, 5});

	X[832] = std::vector<int>({5, 1, 5, 2, 2, 5, 4, 3});
	X[833] = std::vector<int>({-1, 2, -1, 3, 4, -1, -1, 5});

	X[834] = std::vector<int>({5, 5, 5, 4, 5, 5, 8, 5});
	X[835] = std::vector<int>({-1, -1, -1, -1, -1, -1, 2, -1});

	X[836] = std::vector<int>({5, 5, 3, 2, 3, 4, 4, 5});
	X[837] = std::vector<int>({-1, -1, 5, 4, 4, -1, -1, -1});

	X[838] = std::vector<int>({2, 8, 1, 1, 4, 5, 8, 1});
	X[839] = std::vector<int>({4, 2, 3, 3, -1, -1, 2, 2});

	X[840] = std::vector<int>({4, 1, 2, 2, 8, 4, 4, 2});
	X[841] = std::vector<int>({-1, 2, 3, 4, 1, -1, -1, 4});

	X[842] = std::vector<int>({4, 5, 8, 8, 5, 4, 5, 8});
	X[843] = std::vector<int>({-1, -1, 3, 3, -1, -1, -1, 2});

	X[844] = std::vector<int>({2, 2, 1, 5, 4, 3, 8, 1});
	X[845] = std::vector<int>({4, 3, 2, -1, -1, 5, 1, 3});

	X[846] = std::vector<int>({4, 1, 3, 3, 4, 5, 3, 1});
	X[847] = std::vector<int>({-1, 3, 4, 4, -1, -1, 4, 3});

	X[848] = std::vector<int>({8, 2, 3, 8, 4, 3, 3, 8});
	X[849] = std::vector<int>({2, 3, 5, 2, -1, 4, 5, 3});

	X[850] = std::vector<int>({8, 3, 3, 3, 1, 8, 2, 8});
	X[851] = std::vector<int>({2, 4, 4, 5, 3, 3, 4, 3});

	X[852] = std::vector<int>({4, 3, 2, 4, 1, 5, 1, 5});
	X[853] = std::vector<int>({-1, 5, 3, -1, 2, -1, 3, -1});

	X[854] = std::vector<int>({8, 3, 2, 5, 3, 1, 5, 2});
	X[855] = std::vector<int>({3, 4, 3, -1, 5, 3, -1, 4});

	X[856] = std::vector<int>({5, 2, 4, 1, 4, 2, 8, 2});
	X[857] = std::vector<int>({-1, 3, -1, 3, -1, 3, 3, 4});

	X[858] = std::vector<int>({2, 4, 5, 4, 3, 5, 5, 4});
	X[859] = std::vector<int>({3, -1, -1, -1, 4, -1, -1, -1});

	X[860] = std::vector<int>({8, 8, 3, 3, 1, 1, 5, 5});
	X[861] = std::vector<int>({3, 3, 4, 4, 2, 3, -1, -1});

	X[862] = std::vector<int>({8, 5, 5, 5, 5, 5, 2, 4});
	X[863] = std::vector<int>({2, -1, -1, -1, -1, -1, 4, -1});

	X[864] = std::vector<int>({5, 8, 4, 2, 1, 1, 4, 4});
	X[865] = std::vector<int>({-1, 3, -1, 4, 3, 2, -1, -1});

	X[866] = std::vector<int>({8, 4, 5, 2, 5, 2, 2, 4});
	X[867] = std::vector<int>({2, -1, -1, 3, -1, 4, 3, -1});

	X[868] = std::vector<int>({3, 2, 1, 4, 1, 2, 4, 3});
	X[869] = std::vector<int>({4, 3, 2, -1, 2, 4, -1, 5});

	X[870] = std::vector<int>({8, 5, 2, 5, 3, 4, 5, 8});
	X[871] = std::vector<int>({3, -1, 4, -1, 5, -1, -1, 3});

	X[872] = std::vector<int>({8, 5, 3, 8, 1, 1, 8, 8});
	X[873] = std::vector<int>({2, -1, 5, 1, 3, 3, 2, 1});

	X[874] = std::vector<int>({3, 1, 1, 5, 2, 8, 3, 3});
	X[875] = std::vector<int>({5, 3, 2, -1, 3, 2, 4, 5});

	X[876] = std::vector<int>({4, 5, 2, 5, 8, 8, 3, 4});
	X[877] = std::vector<int>({-1, -1, 4, -1, 1, 2, 5, -1});

	X[878] = std::vector<int>({4, 4, 4, 4, 5, 4, 3, 8});
	X[879] = std::vector<int>({-1, -1, -1, -1, -1, -1, 5, 2});

	X[880] = std::vector<int>({3, 8, 4, 5, 4, 4, 1, 2});
	X[881] = std::vector<int>({5, 1, -1, -1, -1, -1, 3, 4});

	X[882] = std::vector<int>({5, 2, 8, 1, 4, 4, 1, 1});
	X[883] = std::vector<int>({-1, 3, 3, 3, -1, -1, 2, 2});

	X[884] = std::vector<int>({2, 1, 5, 8, 3, 2, 3, 3});
	X[885] = std::vector<int>({4, 3, -1, 3, 4, 3, 4, 4});

	X[886] = std::vector<int>({4, 3, 2, 8, 4, 4, 8, 1});
	X[887] = std::vector<int>({-1, 4, 3, 1, -1, -1, 3, 3});

	X[888] = std::vector<int>({3, 4, 4, 5, 5, 5, 3, 4});
	X[889] = std::vector<int>({4, -1, -1, -1, -1, -1, 4, -1});

	X[890] = std::vector<int>({3, 5, 8, 4, 2, 5, 2, 3});
	X[891] = std::vector<int>({4, -1, 1, -1, 3, -1, 4, 4});

	X[892] = std::vector<int>({4, 2, 2, 8, 3, 8, 4, 8});
	X[893] = std::vector<int>({-1, 4, 4, 2, 5, 2, -1, 3});

	X[894] = std::vector<int>({3, 2, 3, 4, 4, 3, 2, 5});
	X[895] = std::vector<int>({4, 4, 4, -1, -1, 5, 3, -1});

	X[896] = std::vector<int>({5, 2, 5, 4, 3, 1, 4, 8});
	X[897] = std::vector<int>({-1, 3, -1, -1, 5, 3, -1, 2});

	X[898] = std::vector<int>({5, 3, 5, 3, 3, 4, 1, 5});
	X[899] = std::vector<int>({-1, 4, -1, 4, 5, -1, 2, -1});

	X[900] = std::vector<int>({4, 1, 8, 3, 1, 2, 8, 2});
	X[901] = std::vector<int>({-1, 3, 2, 4, 2, 3, 2, 4});

	X[902] = std::vector<int>({8, 8, 3, 2, 3, 4, 8, 1});
	X[903] = std::vector<int>({2, 3, 4, 4, 4, -1, 2, 3});

	X[904] = std::vector<int>({4, 3, 4, 8, 5, 5, 8, 8});
	X[905] = std::vector<int>({-1, 4, -1, 2, -1, -1, 2, 1});

	X[906] = std::vector<int>({4, 5, 5, 2, 2, 2, 5, 2});
	X[907] = std::vector<int>({-1, -1, -1, 3, 3, 3, -1, 4});

	X[908] = std::vector<int>({4, 8, 4, 3, 4, 5, 4, 1});
	X[909] = std::vector<int>({-1, 2, -1, 4, -1, -1, -1, 2});

	X[910] = std::vector<int>({5, 4, 8, 5, 4, 8, 4, 5});
	X[911] = std::vector<int>({-1, -1, 3, -1, -1, 3, -1, -1});

	X[912] = std::vector<int>({3, 3, 8, 4, 4, 4, 3, 4});
	X[913] = std::vector<int>({5, 4, 2, -1, -1, -1, 5, -1});

	X[914] = std::vector<int>({5, 3, 4, 8, 3, 2, 1, 2});
	X[915] = std::vector<int>({-1, 4, -1, 3, 5, 4, 3, 4});

	X[916] = std::vector<int>({3, 8, 1, 1, 8, 1, 5, 8});
	X[917] = std::vector<int>({5, 3, 2, 3, 2, 2, -1, 3});

	X[918] = std::vector<int>({3, 3, 5, 4, 3, 5, 5, 8});
	X[919] = std::vector<int>({4, 5, -1, -1, 4, -1, -1, 1});

	X[920] = std::vector<int>({8, 2, 8, 5, 8, 3, 3, 4});
	X[921] = std::vector<int>({2, 4, 3, -1, 3, 4, 4, -1});

	X[922] = std::vector<int>({2, 3, 2, 4, 4, 1, 4, 1});
	X[923] = std::vector<int>({4, 5, 4, -1, -1, 3, -1, 3});

	X[924] = std::vector<int>({5, 4, 8, 4, 5, 8, 5, 3});
	X[925] = std::vector<int>({-1, -1, 1, -1, -1, 1, -1, 4});

	X[926] = std::vector<int>({5, 5, 8, 3, 8, 8, 5, 4});
	X[927] = std::vector<int>({-1, -1, 3, 5, 1, 1, -1, -1});

	X[928] = std::vector<int>({3, 4, 8, 2, 2, 1, 8, 2});
	X[929] = std::vector<int>({5, -1, 1, 4, 3, 3, 2, 4});

	X[930] = std::vector<int>({4, 5, 8, 2, 4, 4, 8, 1});
	X[931] = std::vector<int>({-1, -1, 2, 3, -1, -1, 2, 2});

	X[932] = std::vector<int>({5, 4, 4, 1, 2, 5, 5, 8});
	X[933] = std::vector<int>({-1, -1, -1, 2, 3, -1, -1, 3});

	X[934] = std::vector<int>({2, 1, 1, 8, 4, 2, 1, 5});
	X[935] = std::vector<int>({3, 3, 3, 2, -1, 4, 3, -1});

	X[936] = std::vector<int>({8, 4, 3, 5, 1, 2, 2, 8});
	X[937] = std::vector<int>({2, -1, 5, -1, 3, 4, 4, 3});

	X[938] = std::vector<int>({5, 1, 5, 1, 3, 8, 5, 1});
	X[939] = std::vector<int>({-1, 3, -1, 2, 4, 3, -1, 3});

	X[940] = std::vector<int>({4, 3, 5, 5, 4, 5, 2, 5});
	X[941] = std::vector<int>({-1, 5, -1, -1, -1, -1, 4, -1});

	X[942] = std::vector<int>({4, 8, 1, 1, 5, 5, 8, 5});
	X[943] = std::vector<int>({-1, 2, 2, 2, -1, -1, 3, -1});

	X[944] = std::vector<int>({3, 4, 4, 8, 2, 1, 3, 5});
	X[945] = std::vector<int>({5, -1, -1, 2, 3, 2, 5, -1});

	X[946] = std::vector<int>({3, 1, 4, 1, 1, 8, 5, 8});
	X[947] = std::vector<int>({5, 2, -1, 2, 3, 2, -1, 2});

	X[948] = std::vector<int>({5, 3, 4, 2, 5, 5, 1, 1});
	X[949] = std::vector<int>({-1, 4, -1, 3, -1, -1, 3, 3});

	X[950] = std::vector<int>({5, 8, 4, 5, 2, 4, 5, 1});
	X[951] = std::vector<int>({-1, 2, -1, -1, 4, -1, -1, 2});

	X[952] = std::vector<int>({5, 4, 5, 4, 2, 4, 4, 4});
	X[953] = std::vector<int>({-1, -1, -1, -1, 4, -1, -1, -1});

	X[954] = std::vector<int>({4, 2, 2, 8, 2, 3, 1, 1});
	X[955] = std::vector<int>({-1, 4, 4, 1, 4, 5, 3, 2});

	X[956] = std::vector<int>({8, 5, 4, 8, 5, 8, 2, 1});
	X[957] = std::vector<int>({3, -1, -1, 1, -1, 1, 4, 2});

	X[958] = std::vector<int>({5, 8, 5, 8, 8, 2, 4, 2});
	X[959] = std::vector<int>({-1, 2, -1, 1, 3, 4, -1, 4});

	X[960] = std::vector<int>({4, 4, 3, 5, 3, 1, 4, 2});
	X[961] = std::vector<int>({-1, -1, 4, -1, 4, 3, -1, 4});

	X[962] = std::vector<int>({8, 8, 5, 8, 5, 8, 5, 8});
	X[963] = std::vector<int>({3, 2, -1, 1, -1, 1, -1, 1});

	X[964] = std::vector<int>({8, 3, 8, 4, 1, 8, 4, 2});
	X[965] = std::vector<int>({2, 5, 2, -1, 2, 3, -1, 3});

	X[966] = std::vector<int>({4, 2, 8, 4, 5, 1, 1, 4});
	X[967] = std::vector<int>({-1, 3, 3, -1, -1, 2, 2, -1});

	X[968] = std::vector<int>({4, 2, 3, 5, 2, 3, 5, 1});
	X[969] = std::vector<int>({-1, 3, 4, -1, 3, 4, -1, 3});

	X[970] = std::vector<int>({3, 5, 4, 8, 8, 5, 4, 5});
	X[971] = std::vector<int>({4, -1, -1, 3, 2, -1, -1, -1});

	X[972] = std::vector<int>({3, 4, 2, 5, 4, 1, 1, 2});
	X[973] = std::vector<int>({5, -1, 4, -1, -1, 3, 2, 4});

	X[974] = std::vector<int>({3, 1, 1, 5, 8, 8, 1, 3});
	X[975] = std::vector<int>({4, 2, 2, -1, 2, 3, 3, 4});

	X[976] = std::vector<int>({8, 4, 5, 8, 2, 5, 4, 8});
	X[977] = std::vector<int>({2, -1, -1, 3, 4, -1, -1, 3});

	X[978] = std::vector<int>({8, 4, 3, 2, 1, 3, 5, 4});
	X[979] = std::vector<int>({2, -1, 4, 3, 3, 5, -1, -1});

	X[980] = std::vector<int>({5, 1, 5, 2, 2, 4, 4, 8});
	X[981] = std::vector<int>({-1, 3, -1, 4, 3, -1, -1, 1});

	X[982] = std::vector<int>({4, 3, 5, 1, 1, 1, 8, 4});
	X[983] = std::vector<int>({-1, 4, -1, 2, 3, 2, 3, -1});

	X[984] = std::vector<int>({5, 3, 3, 5, 4, 1, 1, 8});
	X[985] = std::vector<int>({-1, 4, 4, -1, -1, 2, 3, 3});

	X[986] = std::vector<int>({3, 4, 2, 1, 1, 8, 8, 4});
	X[987] = std::vector<int>({4, -1, 4, 3, 3, 1, 2, -1});

	X[988] = std::vector<int>({2, 4, 8, 5, 5, 4, 1, 1});
	X[989] = std::vector<int>({3, -1, 2, -1, -1, -1, 3, 3});

	X[990] = std::vector<int>({2, 4, 5, 4, 1, 4, 3, 5});
	X[991] = std::vector<int>({4, -1, -1, -1, 2, -1, 5, -1});

	X[992] = std::vector<int>({8, 3, 4, 5, 1, 1, 3, 4});
	X[993] = std::vector<int>({2, 5, -1, -1, 3, 2, 4, -1});

	X[994] = std::vector<int>({2, 2, 8, 8, 8, 3, 8, 4});
	X[995] = std::vector<int>({3, 3, 1, 2, 2, 4, 3, -1});

	X[996] = std::vector<int>({5, 2, 2, 4, 4, 4, 8, 5});
	X[997] = std::vector<int>({-1, 4, 4, -1, -1, -1, 3, -1});

	X[998] = std::vector<int>({3, 4, 3, 1, 4, 5, 8, 4});
	X[999] = std::vector<int>({5, -1, 4, 3, -1, -1, 2, -1});

	for (int i = 0; i < 250; i++) {
		data_outputs[i] = 0.01;
	}

	for (int i = 250; i < 500; i++) {
		data_outputs[i] = 0.99;
	}

	
#pragma endregion


	int dimension = 23;
	int num_data_points = 500;

	theta = TheRegression::bestFitF(basis, X, data_outputs, dimension,
																	num_data_points);
	
}

} // namespace EvaluationFunction