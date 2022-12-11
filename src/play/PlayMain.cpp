#include <iostream>
#include "closenessAI.h"

int main()
{
    Func* basis = SqrtDifBasis::GenerateBasis();
    Func f;
    f.InitFunc(1, 2, 1, 1);
    int l_1[8];
    int l_2[8];
    int l_3[8];
    int l_4[8];

    for (int i = 0; i < 8; i ++)
    {
        l_1[i] = i + 1;
        l_2[i] = i + 1;
        l_3[i] = i;
        l_4[i] = i;
    }

    int* X[4];
    X[0] = l_1;
    X[1] = l_2;
    X[2] = l_3;
    X[3] = l_4;

    int dimension = 23;
    int num_data_points = 2;

    Eigen::MatrixXd Q(num_data_points, dimension);



    for (int j = 0; j < dimension; j++){
        for (int i = 0; i < num_data_points; i++){

            //float a =  basis[i].Eval(X[2 * j], X[2 * j + 1]);
            std::cout << basis[i].Eval(X[2 * j], X[2 * j + 1]) << std::endl;

            //Q(i, j) = basis[i].Eval(X[2 * j], X[2 * j + 1]);
            Q(i, j) = 1;
        }
    }

        
    std::cout << Q;
    return 0;
}