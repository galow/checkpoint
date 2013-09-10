/****this file will enum all possible integer Xi comtent X1+X2+X3+......+Xn = N
 **@author YB.Ma
 **@date 2013.09.10
 **@version 1.0.0
 *****/
#include <iostream>
#include <cstdlib>

//start from "from".
void f(int n,int *array,int from,int length){
  if(from + 1 == length){
    for(int i = 0; i < length - 1; ++i){
      std::cout<<array[i]<<" ";
    }
    std::cout<<n<<" ";
    std::cout<<std::endl;
    return;
  }

  if(n == 0){
    for(int i = 0; i < length; ++i)
      if(i < from) std::cout<<array[i]<<" ";
      else std::cout<<0<<" ";
    std::cout<<std::endl;
    return;
  }
  
  if(n == 1){
    for(int i = from; i < length; ++i){
      for(int j = 0; j < length; ++j){
	if(j < from) std::cout<<array[j]<<" ";
	else if( j == i) std::cout<<1<<" ";
	else std::cout<<0<<" ";
      }
      std::cout<<std::endl;
    }
    return;
  }
  
  for(int i = 0; i <= n; ++i){
    array[from] = i;
    //std::cout<<"call f n = "<<n<<" i = "<<i<<" from = "<<from<<std::endl;
    f(n - i,array,from + 1,length);
  }

}

int main(int argc, char *argv[])
{
  if (argc != 3){
    std::cerr<<"Usage: "<<argv[0]<<" total "<<" length "<<std::endl;
    exit(EXIT_FAILURE);
  }
  int total = atoi(argv[1]);
  int length = atoi(argv[2]);
  int *array = new int [length];
  f(total,array,0,length);
  delete array;
  return 0;
}
