#include <iostream>
#include "general_regression.h"
#include <cmath>


int main()
{

    Func* basis = AbsErfBasis::GenerateBasis();
    int* X[88];
    
    
    double data_outputs[20];
    for (int i = 0; i < 10; i ++)
    {
        data_outputs[i] = 0;
    }

    for (int j = 10; j < 20; j ++)
    {
        data_outputs[j] = 1;
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