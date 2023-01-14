#include<iostream>
#include<fstream>
#include "input.h"
int**  parsing::read_from_file()
{
  int** InputBoard[8][8];
  std::ifstream myfile;
  myfile.open ("in.txt");
  for(int i=0;i<=7;i+=1)
  for(int j=0;j<=7;j+=1)
  myfile >>**InputBoard[i][j];
  myfile.close();
  return **InputBoard;
}