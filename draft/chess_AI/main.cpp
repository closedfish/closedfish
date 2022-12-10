#include <iostream>
#include "closenessAI.h"


int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;

    Funk f(1, 5, 1, 1);

    int l_1[8];
    int l_2[8];

    for (int i = 0; i < 8; i ++)
    {
        l_1[i] = i;
        l_2[i] = i * i - 1;
    }


     

    std::cout<< f.Eval(l_1, l_2) << std::endl;
    
    //std::cout << m << f.getval(4) << std::endl;
}