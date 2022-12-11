#include <iostream>

#include "PlayMain.hpp"
#include "closenessAI.h"


void print_hello_world() { std::cout << "Hello World!\n"; }
int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    Funk f(2, 5);

    int l_1[8];
    int l_2[8];

    for (int i = 0; i < 8; i ++)
    {
        l_1[i] = i;
        l_2[i] = i - 1;
    }


     

    std::cout<< f.average_pos(l_2, l_1) << std::endl;
    
    //std::cout << m << f.getval(4) << std::endl;
}