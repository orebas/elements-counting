

#include <vector>
#include <iostream>

int main(){

  std::vector<int> deck = {1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6} ;
  // std::cout << deck;
  for(auto x : deck)
    std::cout << x;
}
