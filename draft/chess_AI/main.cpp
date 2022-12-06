#include <iostream>
#include "closenessAI.h"

int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    //std::cout<<regression1::test() << std::endl;
    std::cout << m << std::endl;
}