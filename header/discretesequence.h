/****this head file define a discrete sequence class,a sequence is like this:1201046178
example:DiscreteSequence seq(8,Choice) where Choice = {2,3,4,1,2,3,4,5},will create a sequence 
with length 8 and every opsiiton i should be a num smaller than Choice[i].
this a 2 bit sequence with length 10
@author YB.Ma
@date 2013.09.12
@version 1.0
*****/

#ifndef DISCRETESEQUENCE_H_
#define DISCRETESEQUENCE_H_

#include <iostream>
#include <iomanip>

class DiscreteSequence{
protected:
  int sequenceLength;
  int *sequence;//every element store a num from 0->setSize-1
  int *choice; //candidate's set size
  
  //set the s to next candidate,if it has carry return true
  virtual bool nextCand(int & p,int & s);
public:
  DiscreteSequence(); //default constructor
  DiscreteSequence(int sl,int *c);
  DiscreteSequence(const DiscreteSequence &seq);//copy constructor
  DiscreteSequence & operator=(const DiscreteSequence &);//copy operator
  ~DiscreteSequence(){delete sequence;}
  void reset();
  void set(int position,int num);
  //change to next sequence
  void nextSequence();
  //cast sequence to integer
  operator int () const;
  friend std::ostream& operator << (std::ostream& os,const DiscreteSequence& seq){
    for(int i = 0;i < seq.sequenceLength;i++)
      os<<std::setw(4)<<seq[i];
    return os;
  }
  int &operator [](int i) const {/*****your can modify sequence*****/
    if(i > sequenceLength-1){
      std::cerr<<"Sequence [] operator out of range."<<std::endl;
      i = 0;
    }
    return sequence[i];
  }
};

DiscreteSequence::DiscreteSequence(){ //a 0 sequence
  sequenceLength = 1;
  sequence = new int [sequenceLength];
  choice = new int[sequenceLength];
  sequence[0] = 0;
}

DiscreteSequence::DiscreteSequence(int sl,int *c){
  sequenceLength = sl;
  //create the sequence
  sequence = new int [sequenceLength];
  choice = new int [sequenceLength];
  //initialize the quence
  for(int i = 0;i < sequenceLength;i++){
    sequence[i] = 0;
    choice[i] = c[i];
  }
}

DiscreteSequence::DiscreteSequence(const DiscreteSequence & seq){
  sequenceLength = seq.sequenceLength;
  sequence = new int [sequenceLength];
  choice = new int [sequenceLength];
  for(int i = 0;i < sequenceLength;i++){
    sequence[i] = seq.sequence[i];
    choice[i] = seq.choice[i];
  }
}

DiscreteSequence & DiscreteSequence::operator=(const DiscreteSequence & seq){
  if(this != &seq){
    sequenceLength = seq.sequenceLength;
    delete sequence;
    sequence = new int [sequenceLength];
    delete choice;
    choice = new int [sequenceLength];
    for(int i = 0;i < sequenceLength;i++){
      sequence[i] = seq.sequence[i];
      choice[i] = seq.choice[i];
    }
  }
  return *this;
}

bool DiscreteSequence::nextCand(int & p,int & s){
  if(s < choice[p] - 1){
    s++;
    return false;
  }
  s = 0;
  return true;
}


void DiscreteSequence::reset(){
  for(int i = 0;i < sequenceLength;i++)
    sequence[i] = 0;
}

void DiscreteSequence::set(int position,int num){
  if(num >= choice[position] || num < 0 || position < 0 || position >= sequenceLength)
    std::cout<<"error:fail to change sequence"<<std::endl;
  else
    sequence[position] = num;
}

DiscreteSequence::operator int () const{
  int integer = 0;
  for(int i = sequenceLength-1;i > 0;i--)
    integer = (integer+sequence[i])*choice[i]; //can shift << 1
  integer += sequence[0];
  return integer;
}

void DiscreteSequence::nextSequence(){
  int index = 0,carry;
  do carry = nextCand(index,sequence[index]);
  while(carry && ++index < sequenceLength);
}

#endif
