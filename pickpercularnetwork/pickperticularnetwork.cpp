/****this program will pick some perticular interaction from given file
 **@author YB.Ma
 **@date 2013.09.07
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

#include "../header/basicmatrix.h"
#include "../header/basicsequence.h"
#include "../header/helpfun.h"

MyInteraction getMinInteraction(MyInteraction& seq,std::vector<MyInteraction> &seqTable_min)
{
  int index = 0;
  int size = seqTable_min.size();
  for(; index < size; ++index){
    if(seq.isSubSeq(seqTable_min[index]))
       return seqTable_min[index];
  }
  return seq;
}

//pick some suitable interaction
void pickNetwork(int * minNetwork,int nodeNum)
{
  MyInteraction seqTemp(nodeNum);
  MyInteraction seqMin(nodeNum);
  std::string *filenames = genStrList(nodeNum);
  std::string *filenames2 = genStrList(nodeNum,"_v1");
  std::ifstream inFile;
  std::ofstream outFile;

  int temp;
  for(int n = 0; n < nodeNum; ++n){
    inFile.open(filenames[n]);
    outFile.open(filenames2[n]);
    if(!inFile.is_open()) //fail to open a file
    {
      std::cout<<"Could not open a file:"<<filenames[n]<<std::endl;
      exit(EXIT_FAILURE);
    }
    //read minimum network 
    for(int i = 0; i < nodeNum; ++i)
      seqMin.set(i,minNetwork[nodeNum * n + i]);
    //read all possible network from file
    while(inFile>>temp){
      if(temp < 50) seqTemp.set(0,temp);
      else break;
      for(int i = 1; i < nodeNum; ++i){
	inFile>>temp;
	seqTemp.set(i,temp);
      }
      if(seqTemp.isSubSeq(seqMin))
	outFile<<seqTemp<<std::endl;
    }
    inFile.close();
    outFile<<100<<std::endl;
    outFile<<seqMin<<std::endl;
    outFile.close();
  }

}

int main(int argc, char *argv[])
{
  if (argc != 3){
    std::cerr<<"Usage: "<<argv[0]<<"  "<<" min-network "<<" nodeNum "<<std::endl;
    exit(EXIT_FAILURE);
  }
  int nodeNum = atoi(argv[2]);
  int *minNetwork = genMatrix(argv[1],nodeNum,nodeNum);

  std::clock_t c_start = std::clock();
 
  pickNetwork(minNetwork,nodeNum);

  std::clock_t c_end = std::clock();
  std::cout<<"It costs : "<<(c_end-c_start)<<" ms."<<std::endl;
  return 0;
}
