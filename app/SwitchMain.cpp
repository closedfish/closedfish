#include <iostream>
#include <nlohmann/json.hpp>
//#include "config.hpp"
#include "SwitchMain.h"
int main()
{

double ClosenessCoef;
double BestStockfishScore;
double BestClosedfishScore;
BestStockfishScore=((double) rand() / (RAND_MAX)) + 1;
BestClosedfishScore=((double) rand() / (RAND_MAX)) + 1;
ClosenessCoef=((double) rand() / (RAND_MAX)) + 1;

if(ClosenessCoef*BestStockfishScore>(1-ClosenessCoef)*BestClosedfishScore){
    std::cout<<"We choose Closedfish";
}
else{
    std::cout<<"We choose Stockfish";
}
OutputToUI();
return 0;
}