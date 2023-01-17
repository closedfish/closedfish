#include "test_concrete.h"
#include "test_abstract.h"

void Hello::say() { std::cout << "Hello, world!" << std::endl; }

int main() {
	Hello h;
	h.say();
	return 0;
}