#include <iostream>
#include "closenessAI.h"


int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    Funk f;
    Funk f_1;
    Funk f_2;
    Funk l[3];
    l[0] = f;
    l[1] = f_1;
    l[2] = f_2;
    f.choose_function(false);

     

    std::cout<< f.eval(2, 4) << std::endl;
    
    //std::cout << m << f.getval(4) << std::endl;
}