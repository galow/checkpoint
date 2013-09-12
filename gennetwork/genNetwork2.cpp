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

//generate some suitable network
void genNetwork(int nodeNum)
{
  int genNetNum = 10;

  std::unordered_multimap<int,MyInteraction> *seqTable = 
    new std::unordered_multimap<int,MyInteraction> [nodeNum];
  std::map<int,MyInteraction> *seqTable_pick = new std::map<int,MyInteraction> [nodeNum];
  //define minimum network and its node edge
  int *minNetwork = new int [nodeNum*nodeNum];
  int *minInteractionNum = new int[nodeNum];

  MyInteraction seqTemp(nodeNum);
  std::string *filenames = genStrList(nodeNum,"_v1");
  std::ifstream inFile;

  int temp;
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
    //read min network from file
    while(inFile>>temp){
      seqTemp.set(0,temp);
      minNetwork[n*nodeNum] = temp;
      for(int i = 1; i < nodeNum; ++i){
	inFile>>temp;
	minNetwork[n*nodeNum+i] = temp;
	seqTemp.set(i,temp);
      }
      minInteractionNum[n] = seqTemp.getLength();
    }
    inFile.close();  
  }

  int enumnum = 0;
  int *interactionNum = new int[nodeNum];
  int *interactionCount = new int[nodeNum];
  int countTemp;
  inFile.open("all.txt");
      
  while(enumnum++ < 3){
    for(int i = 0; i < nodeNum; ++i)
      inFile>>interactionNum[i];
    //find the appropriate network.total_edge <= ?
    for(int n = 0; n < nodeNum; ++n){
      countTemp = 0;
      auto its = seqTable[n].equal_range(interactionNum[n]+minInteractionNum[n]);
      for (auto it = its.first; it != its.second; ++it){
	countTemp++;
	seqTable_pick[n].insert(std::unordered_multimap<int,MyInteraction>::value_type
				(countTemp,it->second));
	//std::cout<<it->second<<std::endl;
      }
      if(countTemp == 0) break;
      else interactionCount[n] = countTemp;
    }
    
    //enum all possible network
    if(countTemp != 0){
      
      std::cout<<"do something."<<std::endl;
    }
    //clear the table
    for(int n = 0; n < nodeNum; ++n)
      seqTable_pick[n].clear();
  }
  inFile.close();
  
  for(int i = 0; i < nodeNum; ++i){
    seqTable[i].clear();
    seqTable_pick[i].clear();
  }
  delete minInteractionNum;
  delete interactionNum;
  
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
