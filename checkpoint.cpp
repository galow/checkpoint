/****for calculate the b,w,detB,detD
 **@author YB.Ma
 **@date 2013.08.27
 **@version 1.0.0
 *****/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>

#include <array>
#include <stack>
#include <vector>
#include <unordered_map>
#include <tuple>

#include <cstdlib>
#include <ctime>
#include <climits>

#include "./header/basicmatrix.h"
#include "./header/basicsequence.h"
#include "./header/helpfun.h"

/******************************calculate the dynamical network************************/

//calculate the next end state 
void nextEndState(int *network,Sequence& start_state,Sequence& end_state,int size)
{
  int temp;

  /****forbid domination model****/
  for(int i = 0;i < size;i++){//for every onde i
    temp = 0;     //result
    for(int j = 0;j < size;j++) //for all j
      if(start_state[j]){  //j is active
  	if(network[i*size+j] == -1){
  	  if(j != i) {
  	    temp = -100; //strong inhibitory,special case
  	    break;
  	  }
  	}
      	temp += network[i*size+j];
      }

    if(temp < 0)
      end_state[i] = 0;
    //!!!!! add && network[i*size+i] != 1 condition !!!!!!
    else if(temp == 0 && network[i*size+i] != 1)
      end_state[i] = start_state[i];
    else
      end_state[i] = true;
  }

   /*********boolean model********/
  // for(int i = 0;i < size;i++){//for every onde i
  //   temp = 0;     //result
  //   for(int j = 0;j < size;j++) //for all j
  //     if(start_state[j])
  //     	temp += (network[i*size+j]==-1)?-1:network[i*size+j]; //b = (a==0)?1:2

  //   if(temp < 0)
  //     end_state[i] = 0;
  //   else if(temp == 0)
  //     end_state[i] = start_state[i];
  //   else
  //     end_state[i] = true;
  // }
}

//give a network,and generate its corresponding DN:dynamical-network or SN:state-network
std::unordered_multimap<int,std::pair<int,int>> genDN(int *network,int size)
{
  bool set[2] = {false,true}; //it is useless
  Sequence start_state(size,2);
  Sequence end_state(size,2);

  std::unordered_multimap<int,std::pair<int,int>> stateNetwork;
  int stateSum = pow(2,size);
  for(int i = 0;i < size;i++)
    start_state[i] = true;
  for(int stateNum = 0;stateNum < stateSum;stateNum++){
    //calculate the end state
    nextEndState(network,start_state,end_state,size);
    //store it to a map:start_state->(end_state,b)
    stateNetwork.insert(std::unordered_multimap<int,std::pair<int,int>>::value_type 
			((int)end_state,std::make_pair((int)start_state,0)));
    //for every starting state,get the next state
    start_state.nextSequence();
  }  
  return stateNetwork;
}

/***************************************************************************************/


/***************************************calculate the B & w*****************************/

//calculate B recursion function
int calBRec(std::unordered_multimap<int,std::pair<int,int>> &SN,int start)
{
  int b = 1,temp;
  auto its = SN.equal_range(start);
  for (auto it = its.first; it != its.second; ++it)
    if(it->first != it->second.first){
      temp = calBRec(SN,it->second.first);
      b += temp;
      it->second.second = temp;//traffic flow
    }
  return b;
}

//calculate the <Wn>
double calWRec(std::unordered_multimap<int,std::pair<int,int>> &SN,
	       int start,int L,int total_trafic)
{
  int total_traffic2;
  double w = 0;
  auto its = SN.equal_range(start);
  for (auto it = its.first; it != its.second; ++it)
    if(it->first != it->second.first){
      //total traffic flow from it->second.second to the attractor
      total_traffic2 = total_trafic + it->second.second;
      w += (double)total_traffic2/L + calWRec(SN,it->second.first,L+1,total_traffic2);
    }
  return w;
}

//calculate the given dynamical-network's basin:B and its Wn
std::pair<int,double> calBW(std::unordered_multimap<int,std::pair<int,int>> &SN,int endState)
{
  int maxB = 0,temp,attractor = 0;
  double Wn;
  for(auto it = SN.begin(); it != SN.end(); it++){
    if(it->first == it->second.first && it->first == endState){
      temp = calBRec(SN,it->second.first);
      if(temp > maxB){
	maxB = temp;
	attractor = it->first;//main attractor
      }
    }
  }
  Wn = calWRec(SN,attractor,1,0);
  return std::make_pair(maxB,Wn/maxB);
}

//calculate the given dynamical-network's maxminum B and its corresponding w
std::pair<int,double> calmaxBW(std::unordered_multimap<int,std::pair<int,int>> &SN)
{
  int maxB = 0,temp,attractor = 0;
  double Wn;
  for(auto it = SN.begin(); it != SN.end(); it++){
    if(it->first == it->second.first){
      temp = calBRec(SN,it->second.first);
      if(temp > maxB){
	maxB = temp;
	attractor = it->first;//main attractor
      }
    }
  }
  Wn = calWRec(SN,attractor,1,0);
  return std::make_pair(maxB,Wn/maxB);
}

/********************************************************************************************/


/***********************************calculate the detB and detD******************************/

//store every state-network node's B in "TF_B"
int calDeltaBRec(std::unordered_multimap<int,std::pair<int,int>> &SN,int start,double *TF_B)
{
  int b = 1,temp;
  auto its = SN.equal_range(start);
  for (auto it = its.first; it != its.second; ++it)
    if(it->first != it->second.first){
      temp = calDeltaBRec(SN,it->second.first,TF_B);
      b += temp;
    }
  TF_B[start] = b;
  return b;
}

//store every state-network node's D in "TF_D"
int calDeltaDRec(std::unordered_multimap<int,std::pair<int,int>> &SN,int start,double *TF_D)
{
  int d = 0,temp;
  auto its = SN.equal_range(start);
  for (auto it = its.first; it != its.second; ++it)
    if(it->first != it->second.first){
      temp = calDeltaDRec(SN,it->second.first,TF_D);
      d += 1;
      it->second.second = temp;//traffic flow
    }
  TF_D[start] = d;
  return d;
}

//calculate the delta-B & delta-D for a given network
double* calBD(int *network,int size)
{
  auto stateNetwork = genDN(network,size);

  int stateSum = pow(2,size);
  Sequence start_state(size,2);
  double *TF_B = new double [stateSum];
  double *TF_D = new double [stateSum];
  //initialization
  for (int i = 0; i < stateSum; ++i)
  {
    TF_B[i] = 0;
    TF_D[i] = 0;
  }

  calDeltaBRec(stateNetwork,76,TF_B); //!!!!!
  calDeltaDRec(stateNetwork,76,TF_D);
  //statistics the checkpoint
  double *weight_b1 = new double[size];
  double *weight_b2 = new double[size];
  double *weight_b = new double[size];
  double *weight_d1 = new double[size];
  double *weight_d2 = new double[size];
  double *weight_d = new double[size];

  //initialization
  for (int i = 0; i < size; ++i)
  {
    weight_b1[i] = 0;
    weight_b2[i] = 0;
    weight_b[i] = 0;
    weight_d1[i] = 0;
    weight_d2[i] = 0;
    weight_d[i] = 0;
  }
  int state;
  //calculate the b
  start_state.reset();
  for(int i = 0; i < stateSum;i++){
    state = (int)start_state;
    if(TF_B[state] >= 1){
      for(int j = 0; j < size; j++){
	if(start_state[j] == 1)
	  weight_b1[j] += TF_B[state];
	else
	  weight_b2[j] -= TF_B[state];
	
	weight_b[j] += TF_B[state];
      }
    }
    start_state.nextSequence();
  }
  //calculate the d
  start_state.reset();
  for(int i = 0; i < stateSum;i++){
    state = (int)start_state;
    if(TF_D[state] >= 1){
      for(int j = 0; j < size; j++){
	if(start_state[j] == 1)
	  weight_d1[j] += TF_D[state];
	else
	  weight_d2[j] -= TF_D[state];
	
	weight_d[j] += TF_D[state];
      }
    }
    start_state.nextSequence();
  }

 // for(int j = 0; j < size; j++)
 //    std::cout<<(j+1)<<" "<<(weight_b1[j]+weight_b2[j])/weight_b[j]<<
 //      " "<<(weight_d1[j]+weight_d2[j])/weight_d[j]<<std::endl;

  double *bd = new double [size];
  for(int j = 0; j < size; ++j){
    bd[j] = ((weight_b1[j]+weight_b2[j])/weight_b[j]+
	     (weight_d1[j]+weight_d2[j])/weight_d[j])/2;
  }
 
  // for(int i = 0; i < size; ++i){
  //   std::cout<<" "<<bd[i]<<" "<<std::endl;
  // }

  delete weight_b1;
  delete weight_b2;
  delete weight_b;
  delete weight_d1;
  delete weight_d2;
  delete weight_d;
  delete TF_B;
  delete TF_D;
  return bd;
}


/********************************************************************************************/

/**********************************define main function**************************************/

void mainfunction(int *network,int *minNetwork,int size)
{
  //calculate the b w
  auto dn = genDN(network,size);
  auto bw = calmaxBW(dn); //76,272
  //std::cout<<"B = "<<bw.first<<" W = "<<bw.second<<std::endl;

  //calculate the supplement node
  int* supp_node = new int [size];
  for(int i = 0; i < size; ++i)
    supp_node[i] = 0;
  int *supp_network = subMatrix(network,minNetwork,size,size);
  for(int i = 0; i < size; ++i){
    for(int j = 0; j < size; ++j){
      if(supp_network[i*size+j] != 0){
	supp_node[j] ++; //outdegree!!!
	supp_node[i] ++; //indegree!!!
      }
    }
  }

  //calculate the b d
  int *node_num = new int[size];
  for(int i = 0; i < size; ++i){
    node_num[i] = i+1; //set node index
  }
  double *bd = calBD(network,size);
  //pop sort
  for(int i = 0; i < size; ++i){
    for(int j = 0; j < size; ++j){
      if(bd[i]>bd[j]){
	swap2(bd[i],bd[j]);
	swap2(node_num[i],node_num[j]);
	swap2(supp_node[i],supp_node[j]);
      }
    }
  }
  
  //print the result
  //for(int i = 0; i < size; ++i){
   //std::cout<<std::setw(2)<<node_num[i]<<" "<<std::setw(10)<<bd[i]<<" "<<supp_node[i]<<std::endl;
  //}
  //calculate the correlation
  double correlation = 0;
  double bd_total = 0;
  double indegree_total = 0;
  double deviation = 0;
  for(int i = 1; i < size; ++i){
    bd_total += bd[i] * bd[i];
    indegree_total += supp_node[i] * supp_node[i];
  }
  
  for(int i = 1; i < size; ++i){
    correlation += (abs2(bd[i])/sqrt(bd_total)) * (supp_node[i]/sqrt(indegree_total));
    deviation += supp_node[i] * supp_node[i];
  }
  //std::cout<<sqrt(deviation)/indegree_total<<" "<<correlation<<std::endl;
  
  std::cout<<bw.first<<" "<<correlation<<std::endl;
  //else std::cout<<0.1<<" "<<correlation<<std::endl;
  delete supp_node;
  delete node_num;
  delete bd;
}


/********************************************************************************************/



// int main(int argc,char * argv[])
// {
//   if (argc != 4) //quit if not input "filename" "size"
//   {
//     std::cerr<<"Usage: "<<argv[0]<<" fullnetwork "<<" min-network "<<" nodeSize "<<std::endl;
//     exit(EXIT_FAILURE);
//   }
//   int nodeNum = atoi(argv[3]);
//   std::clock_t c_start = std::clock();
  
//   int *network = genMatrix(argv[1],nodeNum,nodeNum);
//   int *minNetwork = genMatrix(argv[2],nodeNum,nodeNum);

//   mainfunction(network,minNetwork,nodeNum);
//   // int genNetNum = 1000;
//   // std::string *filenames_full = genStrList(genNetNum,"_full");
//   // std::string *filenames_min = genStrList(genNetNum,"_min");
//   // for(int i = 0; i < genNetNum; ++i){
//   //   int *network = genMatrix(const_cast<char*>(filenames_full[i].c_str()),nodeNum,nodeNum);
//   //   int *minNetwork = genMatrix(const_cast<char*>(filenames_min[i].c_str()),nodeNum,nodeNum);
//   //   mainfunction(network,minNetwork,nodeNum);
//   // }
//   // std::clock_t c_end = std::clock();
//   // std::cout<<"It costs : "<<(c_end-c_start)<<" ms."<<std::endl;
// }
