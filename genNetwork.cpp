/****generate sutible network from given sequence
 **@author YB.Ma
 **@date 2013.09.01
 **@version 1.0.0
 *****/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include <cmath>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <climits>

#include "basicmatrix.h"
#include "basicsequence.h"
#include "helpfun.h"


//generate some suitable network
void genNetwork(int nodeNum)
{
  std::vector<MyInteraction> seqTable;
  MyInteraction seqTemp(nodeNum);
  std::string *filenames = genStrList(nodeNum);
  std::ifstream inFile;
  
  int temp;
  for(int i = 0; i < 1; ++i){
    inFile.open(filenames[i]);
    if(!inFile.is_open()) //fail to open a file
    {
      std::cout<<"Could not open a file:"<<filenames[i]<<std::endl;
      exit(EXIT_FAILURE);
    }
    inFile>>temp;
    
    inFile.close();
  }

}

int main(int argc, char *argv[])
{
  if (argc != 2){
    std::cerr<<"Usage: "<<argv[0]<<"  "<<"nodeNum "<<std::endl;
    exit(EXIT_FAILURE);
  }
  int nodeNum = atoi(argv[1]);
  std::clock_t c_start = std::clock();
 
  //genNetwork(nodeNum);
  MyInteraction seqTemp(nodeNum);
  seqTemp.set(1,1);
  std::cout<<seqTemp<<std::endl;

  std::clock_t c_end = std::clock();
  std::cout<<"It costs : "<<(c_end-c_start)<<" ms."<<std::endl;
  return 0;
}
