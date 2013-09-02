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

MyInteraction getMinInteraction(MyInteraction & seq,std::vector<MyInteraction> &seqTable_min)
{
  int index = 0;
  int size = seqTable_min.size();
  for(; index < size; ++index){
    if(seq.isSubSeq(seqTable_min[index]))
       return seqTable_min[index];
  }
  return seq;
}

//generate some suitable network
void genNetwork(int nodeNum)
{
  std::vector<MyInteraction> seqTable;
  std::vector<MyInteraction> seqTable_min;
  MyInteraction seqTemp(nodeNum);
  std::string *filenames = genStrList(nodeNum);
  std::string *filenames_full = genStrList(10,"_full");
  std::string *filenames_min = genStrList(10,"_min");
  std::ifstream inFile;
  std::ofstream outFile_full;
  std::ofstream outFile_min;

  int temp;
  for(int n = 0; n < nodeNum; ++n){
    inFile.open(filenames[n]);
    if(!inFile.is_open()) //fail to open a file
    {
      std::cout<<"Could not open a file:"<<filenames[n]<<std::endl;
      exit(EXIT_FAILURE);
    }
    //read all possible network from file
    while(inFile>>temp){
      if(temp < 50) seqTemp.set(0,temp);
      else break;
      for(int i = 1; i < nodeNum; ++i){
	inFile>>temp;
	seqTemp.set(i,temp);
      }
      seqTable.push_back(seqTemp);
    }
    //read all min network from file
    while(inFile>>temp){
      seqTemp.set(0,temp);
      for(int i = 1; i < nodeNum; ++i){
	inFile>>temp;
	seqTemp.set(i,temp);
      }
      seqTable_min.push_back(seqTemp);
    }
    inFile.close();
    int seqTable_size = seqTable.size();
    int *randNum = genRandNumList(0,seqTable_size,10);
    //write network to files
    for(int i = 0; i < 10; ++i){
      outFile_full.open(filenames_full[i],std::ios::out | std::ios::app);
      outFile_full<<seqTable[randNum[i]]<<std::endl;
      outFile_full.close();

      outFile_min.open(filenames_min[i],std::ios::out | std::ios::app);
      outFile_min<<getMinInteraction(seqTable[randNum[i]],seqTable_min)<<std::endl;
      outFile_min.close();
    }
    seqTable.clear();
    seqTable_min.clear();
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
 
  genNetwork(nodeNum);

  std::clock_t c_end = std::clock();
  std::cout<<"It costs : "<<(c_end-c_start)<<" ms."<<std::endl;
  return 0;
}
