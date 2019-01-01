

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>  
#include <set>

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '[';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}

int main(){

  std::vector<int> deck = {1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6} ;
  // std::cout << deck;
  
  std::sort(deck.begin(), deck.end());
  std::cout << "Here is the count of permutations" << std::endl;
  long long count =0;
  std::set< std::pair<std::vector<int>, std::vector<int> >> gameset;
  do {
		count++;
		if (count % 1000000 == 0)
			std::cout << count / 1000000 << std::endl;
		std::vector<int> hand1 = std::vector<int>(deck.begin(), deck.begin() + 6);
		std::vector<int>  hand2 = std::vector<int>(deck.begin()+6, deck.begin() +12);
		//std::cout << hand1 << std::endl;
		//std::cout << hand2 << std::endl;
		std::sort(hand1.begin(), hand1.end());
		std::sort(hand2.begin(), hand2.end());
		std::pair<std::vector<int>, std::vector<int> > handpair(hand1, hand2);
		gameset.insert(handpair);

  } while (std::next_permutation(deck.begin(), deck.end()));
  std::cout << count << std::endl;
  std::cout << gameset.size() << std::endl;
  int dummy = 0;
  std::cin >> dummy;
}



void count_hand_possibilities() {

	int maxes[6] = { 2,2,2,2,2,6 };


}
