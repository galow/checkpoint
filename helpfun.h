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

#endif
