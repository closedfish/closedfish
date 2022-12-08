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
     
    int l_black_pons[8];
    int l_white_pons[8];
    for (int i = 0; i < 8; i ++){
        l_black_pons[i] = i - 1;
        l_white_pons[i] = i;
    }
    

    std::cout<< f.average_pos(l_black_pons, l_white_pons) << std::endl;
    
    //std::cout << m << f.getval(4) << std::endl;
}