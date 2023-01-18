#include <iostream>
#include "closenessAI.h"

int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    int l_1[8];
    int l_2[8];
    Func* b = Basis1::GenerateBasis();
    for (int i = 0; i < 8; i ++)
    {
        l_1[i] = i;
        l_2[i] = i * i - 1;
    }
    for (int i = 0; i < 23; i ++)
    {
        std::cout<< b[i].Eval(l_1, l_2) <<std::endl;
    }
    return 0;
}