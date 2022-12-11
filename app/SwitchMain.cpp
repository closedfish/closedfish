#include <iostream>
#include <nlohmann/json.hpp>
//#include "config.hpp"
#include "SwitchMain.hpp"
#include "PlayMain.hpp"
int main()
{
print_hello_world();
    std::cout << "JSON:"
        << NLOHMANN_JSON_VERSION_MAJOR << "."
        << NLOHMANN_JSON_VERSION_MINOR << "."
        << NLOHMANN_JSON_VERSION_PATCH << "\n";
return 0;
}