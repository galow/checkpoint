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
#include <map>
#include <unordered_map>

#include <cmath>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <climits>

#include "../header/basicmatrix.h"
#include "../header/basicsequence.h"
#include "../header/helpfun.h"

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
  int genNetNum = 10,count = 0;

  std::unordered_multimap<int,MyInteraction> *seqTable = 
    new std::unordered_multimap<int,MyInteraction> [nodeNum];
  std::map<int,MyInteraction> *seqTable_min = new std::map<int,MyInteraction> [nodeNum];
  
  MyInteraction seqTemp(nodeNum);
  std::string *filenames = genStrList(nodeNum,"_v1");
  std::ifstream inFile;

  int temp,index;
  for(int n = 0; n < nodeNum; ++n){
    inFile.open(filenames[n]);
    //read all possible network from file
    while(inFile>>temp){
      if(temp < 50) seqTemp.set(0,temp);
      else break;
      for(int i = 1; i < nodeNum; ++i){
	inFile>>temp;
	seqTemp.set(i,temp);
      }
      seqTable[n].insert(std::unordered_multimap<int,MyInteraction>::value_type
			 (seqTemp.getLength(),seqTemp));
    }
    //read all min network from file
    index = 0;
    while(inFile>>temp){
      seqTemp.set(0,temp);
      for(int i = 1; i < nodeNum; ++i){
	inFile>>temp;
	seqTemp.set(i,temp);
      }
      seqTable_min[n].insert(std::map<int,MyInteraction>::value_type
			     (index,seqTemp));
      index++;
    }
    inFile.close();   
  }

  //find the appropriate network.total_edge <= ?
  for(int n = 0; n < 1; ++n){
    auto its = seqTable[n].equal_range(5);
    for (auto it = its.first; it != its.second; ++it)
      std::cout<<it->second<<std::endl;
  }

  for(int i = 0; i < nodeNum; ++i){
    seqTable[i].clear();
    seqTable_min[i].clear();
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
