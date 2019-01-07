

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>  
#include <set>
#include <stack>
#include <array>

#include <boost/variant.hpp>


// how manys ones, two, three, fours, fives, and sixes is what we keep.  note offset by one.
typedef std::array<int,6> hand;
typedef std::pair<hand, hand> handpair;

enum class move_type: unsigned int {
knock = 0,
one = 1,
two = 2,
three = 3,
four=4,
five=5,
six=6,
dropsix=7,
take=8,
fold=9
};


void count_hand_possibilities();

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '[';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}


template <typename T>
std::ostream& operator<< (std::ostream& out, const std::array<T,6>& v) {
	if (!v.empty()) {
		out << '[';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}


void count_hand_possibilities() {
	constexpr hand maxes = { 2,2,2,2,2,6 };
	hand counter = { 0,0,0,0,0,0 };
	std::vector<int> sums = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	for (counter[0] = 0; counter[0] <= maxes[0]; counter[0]++)
		for (counter[1] = 0; counter[1] <= maxes[1]; counter[1]++)
			for (counter[2] = 0; counter[2] <= maxes[2]; counter[2]++)
				for (counter[3] = 0; counter[3] <= maxes[3]; counter[3]++)
					for (counter[4] = 0; counter[4] <= maxes[4]; counter[4]++)
						for (counter[5] = 0; counter[5] <= maxes[5]; counter[5]++) {
							int sum = 0;
							for (auto x : counter)
								sum += x;
							sums[sum]++;
						}

	for (int j = 0; j < 17; j++)
		std::cout << j << "\t" << sums[j] << std::endl;
}


int count_games_restricted(const hand& firsthand, std::vector<handpair> & gamelist) {
	constexpr hand totalmaxes = { 2,2,2,2,2,6 };
	hand maxes;
	for (int i = 0; i < 6; i++)
		maxes[i] = totalmaxes[i] - firsthand[i];
	hand counter = { 0,0,0,0,0,0 };
	int count = 0;
	for (counter[0] = 0; counter[0] <= maxes[0]; counter[0]++)
		for (counter[1] = 0; counter[1] <= maxes[1]; counter[1]++)
			for (counter[2] = 0; counter[2] <= maxes[2]; counter[2]++)
				for (counter[3] = 0; counter[3] <= maxes[3]; counter[3]++)
					for (counter[4] = 0; counter[4] <= maxes[4]; counter[4]++)
						for (counter[5] = 0; counter[5] <= maxes[5]; counter[5]++) {
							int sum = 0;
							for (auto x : counter)
								sum += x;
							if (sum == 6) {
								count++;
								gamelist.push_back(handpair(firsthand, counter));
							}
						}
	return count;
}




int  count_initial_games(std::vector<handpair> &  gamelist) {
	constexpr hand maxes = { 2,2,2,2,2,6 };
	hand counter = { 0,0,0,0,0,0 };
	std::vector<int> sums = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int games = 0;
	for (counter[0] = 0; counter[0] <= maxes[0]; counter[0]++)
		for (counter[1] = 0; counter[1] <= maxes[1]; counter[1]++)
			for (counter[2] = 0; counter[2] <= maxes[2]; counter[2]++)
				for (counter[3] = 0; counter[3] <= maxes[3]; counter[3]++)
					for (counter[4] = 0; counter[4] <= maxes[4]; counter[4]++)
						for (counter[5] = 0; counter[5] <= maxes[5]; counter[5]++) {
							int sum = 0;
							for (auto x : counter)
								sum += x;
							if (sum == 6) {
								
								int c = count_games_restricted(counter,gamelist);
								games += c;
							}
						}
	return games;
}



int enumerate_games(const handpair& init) {
	int player = 0;
	int level = 0;
	std::stack<move_type> m;
}


int main(){
  std::vector<int> deck = {1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6} ;
  // std::cout << deck;
  
  std::sort(deck.begin(), deck.end());
  std::cout << "Here is the count of permutations" << std::endl;
  long long count =0;
  std::set< std::pair<std::vector<int>, std::vector<int> >> gameset;
  
  count_hand_possibilities();
  std::vector<handpair> gamelist;
  std::cout << "Here is the count of initial games:  " << count_initial_games(gamelist) << std::endl;
  for(auto x: gamelist) {
	  std::cout << x.first << x.second << std::endl;
  }
  
  
  int dummy = 0;
  std::cin >> dummy;
}


//The below code counts permutations of the deck
/*
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
*/
