#include <iostream>
#include "general_regression.h"
#include <cmath>


int main()
{

    Func* basis = AbsErfBasis::GenerateBasis();
    int* X[88];
    X[0] = new int[8]{3, 5, 3, 4, 1, 1, 1, 2};
    X[1] = new int[8]{4, 6, 4, 5, 2, 2, 2, 3};
    X[2] = new int[8]{5, 3, 2, 3, 5, 5, 2, 1};
    X[3] = new int[8]{6, 4, 3, 4, 6, 6, 3, 2};
    X[4] = new int[8]{3, 2, 5, 4, 2, 5, 5, 2};
    X[5] = new int[8]{4, 3, 6, 5, 3, 6, 6, 3};
    X[6] = new int[8]{3, 2, 4, 4, 1, 5, 1, 5};
    X[7] = new int[8]{4, 3, 5, 5, 2, 6, 2, 6};
    X[8] = new int[8]{4, 2, 3, 5, 2, 5, 1, 2};
    X[9] = new int[8]{5, 3, 4, 6, 3, 6, 2, 3};
    X[10] = new int[8]{5, 5, 4, 3, 3, 4, 5, 3};
    X[11] = new int[8]{6, 6, 5, 4, 4, 5, 6, 4};
    X[12] = new int[8]{5, 3, 3, 2, 1, 4, 2, 4};
    X[13] = new int[8]{6, 4, 4, 3, 2, 5, 3, 5};
    X[14] = new int[8]{4, 1, 1, 3, 1, 3, 1, 3};
    X[15] = new int[8]{5, 2, 2, 4, 2, 4, 2, 4};
    X[16] = new int[8]{3, 4, 3, 2, 5, 4, 1, 1};
    X[17] = new int[8]{4, 5, 4, 3, 6, 5, 2, 2};
    X[18] = new int[8]{5, 2, 4, 1, 1, 1, 1, 3};
    X[19] = new int[8]{6, 3, 5, 2, 2, 2, 2, 4};
    X[20] = new int[8]{4, 4, 2, 3, 2, 3, 5, 8};
    X[21] = new int[8]{5, 5, 3, 4, 3, -1, 6, 4};
    X[22] = new int[8]{4, 4, 1, 3, 8, 2, 5, 5};
    X[23] = new int[8]{5, -1, 2, 4, 5, 3, 6, 6};
    X[24] = new int[8]{3, 4, 8, 3, 1, 5, 2, 1};
    X[25] = new int[8]{4, 5, 4, 4, 2, -1, 3, 2};
    X[26] = new int[8]{4, 8, 3, 2, 8, 5, 8, 3};
    X[27] = new int[8]{5, 2, 4, -1, -1, 6, -1, 4};
    X[28] = new int[8]{4, 8, 4, 8, 8, 5, 4, 4};
    X[29] = new int[8]{5, 2, 5, -1,5, 6, -1, 5};
    X[30] = new int[8]{4, 4, 8, 2, 8, 5, 8, 1};
    X[31] = new int[8]{5, -1, 4, -1, 2, -1, 4, 2};
    X[32] = new int[8]{2, 5, 8, 8, 5, 8, 8, 1};
    X[33] = new int[8]{3, 6, -1, 6, -1, 3, -1, 2};
    X[34] = new int[8]{3, 4, 8, 3, 8, 5, 3, 2};
    X[35] = new int[8]{4, 5, 5, 4, 3, -1, 4, 3};
    X[36] = new int[8]{4, 3, 5, 8, 8, 8, 4, 3};
    X[37] = new int[8]{5, 4, 6, 2, 3, -1, 5, 4};
    X[38] = new int[8]{5, 3, 5, 3, 5, 3, 2, 2};
    X[39] = new int[8]{6, 4, -1, 4, -1, 4, 3, 3};
    
    double data_outputs[20];
    for (int i = 0; i < 10; i ++)
    {
        data_outputs[i] = 0.01;
    }

    for (int j = 10; j < 20; j ++)
    {
        data_outputs[j] = 0.99;
    }
    


    int dimension = 23;
    int num_data_points = 20;

    Eigen::VectorXd theta = TheRegression::bestFitF(basis, X, data_outputs, dimension, num_data_points);

    float s = 0;
    int l_1[8];
    int l_2[8];
    for (int i = 0; i < 8; i++) 
    {
        l_1[i] = i;
        l_2[i] = i - 1;
    }

   
 
 
   
    std::cout << EvaluationFunction::Evaluate(basis, theta, l_1, l_2, dimension) << std::endl;
    std::cout << EvaluationFunction::TestAi(basis, theta, X, data_outputs, dimension, num_data_points);
    return 0;
}