/*****this head file define some basic help function
****author YB.Ma
****date 2013.8.31
****version 1.0
*****/
#ifndef HELPFUN_H_
#define HELPFUN_H_

#include <iostream>
#include <stack>
#include <string>
#include <ctime>

//define a interaciotn-class for each j->i interaciton,like (-1,0,0,1,0,0,0,1)
class MyInteraction : public Sequence{
public:
  MyInteraction():Sequence(){
  }
  MyInteraction(int sl):Sequence(sl,3){
  }
  MyInteraction(const MyInteraction &seq):Sequence(seq){//copy constructor
  }
  ~MyInteraction(){~Sequence();}
  MyInteraction & operator=(const MyInteraction &seq){//copy operator
    Sequence::operator=(seq);
    return *this;
  }
  int operator [](int i) const {
    return Sequence::operator[](i)-1;
  }
  int set(int position,int num){
    Sequence::set(position,num+1);
  }
  //seq is subset or not
  bool isSubSeq(const MyInteraction & seq){
      if(Sequence::sequenceLength != seq.sequenceLength)
        return false;
      else
        for (int i = 0; i < Sequence::sequenceLength; ++i)
        {
          if(operator[](i) != seq[i] && seq[i] != 0)
            return false;
        }
      return true;
  }
  friend std::ostream& operator << (std::ostream& os,const MyInteraction& seq){
    for(int i = 0;i < seq.sequenceLength;i++)
      os<<std::setw(4)<<seq[i];
    return os;
  }
  int getLength(){
    int count = 0;
    for(int i = 0; i < Sequence::sequenceLength; ++i)
      if(operator[](i) != 0) count++;
    return count;
  }
};


//clear the stack
template<typename T>
void clearStack(std::stack<T> &theStack)
{ 
  while ( ! theStack.empty() ){
     theStack.pop();
  }
}


//gennerate string list
std::string *genStrList(int num)
{
  std::string * st = new std::string[num];
  std::string suffix = ".txt";
  //std::string dir = "/full/";
  char tempStr[3];
  std::string str;
  for(int i = 0; i < num; ++i){
    //str = std::to_string(i+1);
    sprintf(tempStr, "%d",i+1); 
    str = std::string(tempStr);
    st[i] = str + suffix;
  }
  return st;
}

//gennerate string list with a suffix
std::string *genStrList(int num,std::string suffix)
{
  std::string * st = new std::string[num];
  std::string suffix2 = suffix + ".txt";
  //std::string dir = "/full/";
  char tempStr[3];
  std::string str;
  for(int i = 0; i < num; ++i){
    //str = std::to_string(i+1);
    sprintf(tempStr, "%d",i+1); 
    str = std::string(tempStr);
    st[i] = str + suffix2;
  }
  return st;
}


//generate a random num
int *genRandNumList(int from ,int to,int count){
  //srand(1);
  srand((unsigned)time(0));
  int *result = new int [count];
  for (int i = 0; i < count; ++i)
    result[i] = rand() % (to - from) + from;
  return result;
}

//exchange two number
template <typename T>
void swap2(T& a,T& b){
  T temp;
  temp = a;
  a = b;
  b = temp;
}

//get a abosolute number
template <typename T>
T abs2(T& a){
  if(a < 0.0) return -a;
  else return a;
}

//continue multiplication
template <typename T>
T product(T *all,int length){
  T result = 1;
  for (int i = 0; i < length; ++i)
    result *= all[i];
  return result;
}

#endif
