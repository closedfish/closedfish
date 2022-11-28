#include<iostream>

#include "PlayMain.hpp"

void print_hello_world()
{
    std::cout <<"Hello World!\n";
}
uint32_t factorial( uint32_t number ) {
    return number <= 1 ? number : factorial(number-1) * number;
}