#include "factorial.hpp"
#include <iostream>

void print_hello_world() { std::cout << "Hello World!\n"; }
int factorial(int number) {
	return number <= 1 ? number : factorial(number - 1) * number;
}