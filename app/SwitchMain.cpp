#include <iostream>
#include <nlohmann/json.hpp>
//#include "config.hpp"
#include "SwitchMain.hpp"
#include "PlayMain.hpp"
int main()
{
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    std::cout<<m<<"\n";
    std::cout << "JSON:"
        << NLOHMANN_JSON_VERSION_MAJOR << "."
        << NLOHMANN_JSON_VERSION_MINOR << "."
        << NLOHMANN_JSON_VERSION_PATCH << "\n";
return 0;
}