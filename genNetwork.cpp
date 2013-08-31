#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
  if (argc != 2){
    std::cerr<<"Usage: "<<argv[0]<<"  "<<"nodeNum "<<std::endl;
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
