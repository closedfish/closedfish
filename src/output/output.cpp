#include <iostream>
#include <fstream>

#include "output.h"

void writting::write_in_file() {
  std::ofstream myfile;
  myfile.open("out.txt");
  myfile << "Hello world!";
  myfile.close();
}
 
void OutputToUI() {
  writting::write_in_file();
}