#include <iostream>
#include <nlohmann/json.hpp>
//#include "config.hpp"
#include "SwitchMain.hpp"
#include "PlayMain.hpp"
#include "ConnectUI-SAI.h"
int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    std::cout<<m<<"\n";
    int B[8][8];
    for(int i=0;i<=7;i++)
    for(int j=0;j<=7;j++)
    B[i][j]=rand()%6;
return 0;
}