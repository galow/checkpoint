/****find all possible network for a given dynamical process
 **@author YB.Ma
 **@date 2013.08.27
 **@version 1.0.0
 *****/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>

#include <stack>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <climits>

#include "../header/basicmatrix.h"
#include "../header/basicsequence.h"
#include "../header/helpfun.h"


/*******************************************************************************/

/********************find the appropriate interaction*********************************/
//check the given interaction wheather contents the dynamical-process 
bool logical_exp(MyInteraction &interaction ,int *state,int inNode,int periods,int nodeNum){
  for(int t = 0;t < periods-1;t++){
    bool left;
    if(interaction[inNode] != 1)//left expression,self degradation
      left = state[t*nodeNum+inNode] && (! interaction[inNode]);
    else
      left = true; /*! nodeState[t][toNode];*/     // self activation
    for(int j = 0;j < nodeNum;j++){
      if(j == inNode) continue;
      if(interaction[j] == -1 && state[t*nodeNum+j]){//right expression
	if(state[(t+1)*nodeNum+inNode])
	  return false;
	else{
	  left = state[(t+1)*nodeNum+inNode];  //make sure this time must be right
	  break;
	}	
      }
      left = left || (interaction[j] && state[t*nodeNum+j]);
    }
    if(left != state[(t+1)*nodeNum+inNode]) return false;
  }
  return true;
}

//using the brute force search to find the existent network
//and save it to files
void sNetworkBFS(char * fileName,int nodeNum,int duration,int threshold){
  int *state = genMatrix(fileName,duration,nodeNum);
  MyInteraction seq(nodeNum);
  int startIndex = 0,count;
  std::stack<MyInteraction> minInteraction; //store the minimum interaction
  int allSeq = pow(3,nodeNum);  //iterate all sequence
  
  std::string *filenames = genStrList(nodeNum);
  std::ofstream myfile;
  for(int n = 0;n < nodeNum;n++){ //for every onde
    int minInteractSize = nodeNum,tempSize;
    myfile.open(filenames[n]);
    
    count = 0;
    for(int t = 0;t < allSeq;t++){ //for every sequence
      //if it contents logical_expresison and not self activation
      if(logical_exp(seq,state,n,duration,nodeNum) /*&& seq[n] != 1*/){
	if(seq.getLength() < threshold) myfile<<seq<<std::endl;
	//pick the minimum interaction
  	tempSize = 0;
  	for(int i = 0;i < nodeNum;i++)
  	  if(seq[i] != 0) tempSize++;
  	if(tempSize < minInteractSize){
	  clearStack(minInteraction);
  	  minInteractSize = tempSize;
	  count = 0;
  	}
	if(tempSize == minInteractSize){
	    minInteraction.push(seq);
	    count++;
	}	
      }
      seq.nextSequence();
    }
    //print the minimum interaction
    myfile<<100<<std::endl;
    for(int i = 0;i < count;i++){
      myfile<<minInteraction.top()<<std::endl;
      std::cout<<minInteraction.top()<<std::endl;
      minInteraction.pop();
    }
    std::cout<<std::endl;
    myfile.close();
  }
  delMatrix(state);
}

/**********************************************************************************/

int main(int argc,char * argv[])
{
  if (argc != 5) //quit if not input "filename" "size"
  {
    std::cerr<<"Usage: "<<argv[0]<<" network-process "
	     <<" nodeSize "<<" period-time "<<" threshold "<<std::endl;
    exit(EXIT_FAILURE);
  }
  int nodeNum = atoi(argv[2]);
  int duration = atoi(argv[3]);
  int threshold = atoi(argv[4]);
  std::clock_t c_start = std::clock();
  
  sNetworkBFS(argv[1],nodeNum,duration,threshold);

  std::clock_t c_end = std::clock();
  std::cout<<"It costs : "<<(c_end-c_start)<<" ms."<<std::endl;
}
