#include <iostream>
#include "closenessAI.h"
#include <cmath>
int main()
{
    Func* basis = AbsDifBasis::GenerateBasis();

    int l_1[8];
    int l_2[8];
    int l_3[8];
    int l_4[8];
    int l_5[8];
    int l_6[8];
    int l_7[8];
    int l_8[8];

    l_1[0] = 7;
    l_1[1] = 5;
    l_1[2] = 8;
    l_1[3] = 6;
    l_1[4] = 4;
    l_1[5] = 7;
    l_1[6] = 5;
    l_1[7] = 7;
    
    for (int i = 0; i < 8; i ++)
    {
        l_2[i] = 4;
    }
    l_2[4] = -1;

    l_4[0] = 9;
    l_4[1] = 0;
    l_4[2] = 8;
    l_4[3] = 12;
    l_4[4] = 4;
    l_4[5] = 7;
    l_4[6] = 8;
    l_4[7] = 7;
    
    for (int i = 0; i < 8; i ++)
    {
        l_3[i] = 4;
    }
    l_3[4] = -1;

    l_5[0] = 4;
    l_5[1] = 3;
    l_5[2] = 4;
    l_5[3] = 3;
    l_5[4] = 4;
    l_5[5] = 3;
    l_5[6] = 4;
    l_5[7] = 3;

    l_6[0] = 3;
    l_6[1] = 4;
    l_6[2] = 3;
    l_6[3] = 4;
    l_6[4] = 3;
    l_6[5] = 4;
    l_6[6] = 3;
    l_6[7] = 4;

    l_8[0] = 4;
    l_8[1] = 3;
    l_8[2] = 4;
    l_8[3] = 3;
    l_8[4] = 2;
    l_8[5] = 3;
    l_8[6] = 4;
    l_8[7] = 3;

    l_7[0] = 3;
    l_7[1] = 4;
    l_7[2] = 3;
    l_7[3] = 4;
    l_7[4] = 3;
    l_7[5] = 4;
    l_7[6] = 3;
    l_7[7] = 4;
    

    int* X[8];
    X[0] = l_1;
    X[1] = l_2;
    X[2] = l_3;
    X[3] = l_4;
    X[4] = l_5;
    X[5] = l_6;
    X[6] = l_7;
    X[7] = l_8;

    double data_outputs[4];
    data_outputs[0] = 1;
    data_outputs[1] = 1;
    data_outputs[2] = 0;
    data_outputs[3] = 0;


    int dimension = 23;
    int num_data_points = 3;

    Eigen::MatrixXd Q(num_data_points, dimension);
    Eigen::VectorXd Y(num_data_points);
    Q = TheRegression::setUpQ(basis, X, dimension, num_data_points);
    Y = TheRegression::setUpYVect(data_outputs, num_data_points);
    Eigen::MatrixXd trans_Q = Q.transpose();
    Eigen::MatrixXd helper_M = (trans_Q * Q).inverse();
    std::cout << trans_Q;
    std::cout << helper_M;
    // Eigen::MatrixXd helper_M = (trans_Q * Q).inverse();
    // Eigen::MatrixXd O = helper_M * trans_Q * Y;
    // std::cout << Q <<std::endl;
    // std::cout << Y <<std::endl;
    // std::cout << O <<std::endl;
    return 0;
}