
//#include <boost/variant.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>  
#include <set>
#include <stack>
#include <array>
#include <cstdio>
#include <cassert>
#include <random>

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '[';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}


template <typename T, std::size_t n>
std::ostream& operator<< (std::ostream& out, const std::array<T, n>& v) {
	if (!v.empty()) {
		out << '[';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}


enum class move_type : unsigned int {
	knock = 0,
	one = 1,
	two = 2,
	three = 3,
	four = 4,
	five = 5,
	six = 6,
	dropsix = 7,
	take = 8,
	fold = 9
};

std::ostream& operator<< (std::ostream& out, const move_type  m) {
	out << static_cast<int> (m);
	return out;
}


// how manys ones, two, three, fours, fives, and sixes is what we keep.  note offset by one.
typedef int card;
typedef std::array<card,6> hand;
typedef std::pair<hand, hand> handpair;
typedef std::vector<card> smallstack;
typedef std::vector<move_type> smallmovestack;

class game {
	hand hidden[2];
	hand shown[2];
	int sums[2] = { 0,0 };
	int pilesum = 0;
	smallstack pile;
	smallstack drawstack;
	smallmovestack history;
	int nextplayer=0;
public: 
		game(const handpair& h) {
		hidden[0] = h.first;
		hidden[1] = h.second;
		shown[0] = { 0,0,0,0,0,0 };
		shown[1] = { 0,0,0,0,0,0 };
		for (int p = 0; p < 2; p++) {
			for (int i = 0; i < 6; i++) {
				sums[p] += (i + 1) * hidden[p][i];
			}
		}
	}
		smallmovestack move_list() {
			smallmovestack ret_move_list;
			for (int i = 0; i < 6; i++)
				if (hidden[nextplayer][i] > 0)
					ret_move_list.push_back(static_cast<move_type>(i + 1));
			if (hidden[nextplayer][5] > 0)
				ret_move_list.push_back(move_type::dropsix);
			if (pilesum > 0)
				ret_move_list.push_back(move_type::take);
			if (sums[nextplayer] <= pilesum)
				ret_move_list.push_back(move_type::knock);
			
			//  move_list.push(move_type::fold);
			return ret_move_list;

		}
		int move(move_type m) { //returns 0 if game continues, 1 if p0 wins, -1 if p1 wins
			switch (m) {
			case move_type::knock:
				if (sums[nextplayer] <= pilesum) {
					if(sums[1-nextplayer] > pilesum)
						return nextplayer * -2 + 1;
					else if (sums[1-nextplayer] >= sums[nextplayer])
						return nextplayer * 2 -1;
					else return nextplayer * -2 + 1;

				}
				else assert(0); //shouldn't be able to knock
				history.push_back(m);
				break;
			case move_type::one:
			case move_type::two:
			case move_type::three:
			case move_type::four:
			case move_type::five:
			case move_type::six: {
				card p = static_cast<int> (m);
				assert(hidden[nextplayer][p - 1] > 0);
				hidden[nextplayer][p - 1]--;
				pile.push_back(p);
				sums[nextplayer] -= p;
				pilesum += p;
				nextplayer = 1 - nextplayer;
				history.push_back(m);
			}
				break;
			case move_type::dropsix:
				assert(hidden[nextplayer][5] > 0);
				hidden[nextplayer][5] --;
				sums[nextplayer] -= 6;
				nextplayer = 1 - nextplayer;
				history.push_back(m);
				break;
			case move_type::take: {
				assert(pilesum > 0);
				card c = pile.back();
				pile.pop_back();
				shown[nextplayer][c - 1]++;
				sums[nextplayer] += c;
				pilesum -= c;
				nextplayer = 1 - nextplayer;
				history.push_back(m);
				drawstack.push_back(c);
			}
				break;
			case move_type::fold:
				return nextplayer * 2 - 1;
				break;
			}
			return 0;
		}
		void display() {
			std::cout << "Player 1:   H" << hidden[0] << "  S" << shown[0] << "\n";
			std::cout << "Player 2:   H" << hidden[1] << "  S" << shown[1] << "\n";
			std::cout << "Pile:        " << pile << "\n";
			std::cout << "History:     " << history << "\n";  //This won't work until the enum operator<< is overloaded.
			std::cout << "Sum1:  " << sums[0] << "  Sum2:   " << sums[1] << "  Pile sum: " << pilesum << "\n";
		}
		void unmove() {
			assert(history.size() > 0);
			move_type m = history.back();  
			history.pop_back();
			switch (m) {
			case move_type::knock:
				break;
			case move_type::one:
			case move_type::two:
			case move_type::three:
			case move_type::four:
			case move_type::five:
			case move_type::six: {
				card p = static_cast<int> (m);
				assert(pile.back() == p);
				nextplayer = 1 - nextplayer;
				sums[nextplayer] += p;
				pilesum -= p;
				hidden[nextplayer][p - 1]++;
				pile.pop_back();
			}
			break;
			case move_type::dropsix:
				nextplayer = 1 - nextplayer;
				hidden[nextplayer][5] ++;
				sums[nextplayer] += 6;
				break;
			case move_type::take: {
				card c = drawstack.back();
				nextplayer = 1 - nextplayer;
				pile.push_back(c);
				shown[nextplayer][c - 1]--;
				sums[nextplayer] -= c;
				pilesum += c;
				drawstack.pop_back();
			}
			break;
			case move_type::fold:
				break;
			}


		}
		void enumerate_games_recurse() {
			auto my_move_list = this->move_list();
			for (auto m : my_move_list) {
				
				//display();
				if (m == move_type::knock || m == move_type::fold) {
					//std::cout << "G";
				}
				else {
					int i = move(m);
					enumerate_games_recurse();
					unmove();
				}
			}
}



};



void count_hand_possibilities();

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
		std::cout << j << "\t" << sums[j] << "\n";
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
	game init_game(init);
	auto init_move_list = init_game.move_list();
	std::cout << init_move_list.size() << "\n";
	std::cout << init_move_list;
	for (auto m : init_move_list){
		init_game.move(m);
		init_game.enumerate_games_recurse();
		init_game.unmove();
	 }
	return 0;
}


int main(){

	std::random_device rd;
	std::mt19937 g(rd());

  std::ios_base::sync_with_stdio(false);
  std::vector<int> deck = {1,1,2,2,3,3,4,4,5,5,6,6,6,6,6,6} ;
  // std::cout << deck;
  
  std::sort(deck.begin(), deck.end());
  std::cout << "Here is the count of permutations" << "\n";
  long long count =0;
  std::set< std::pair<std::vector<int>, std::vector<int> >> gameset;
  
  count_hand_possibilities();
  std::vector<handpair> gamelist;
  std::cout << "Here is the count of initial games:  " << count_initial_games(gamelist) << "\n";
  std::random_shuffle(gamelist.begin(), gamelist.end());
  for(auto x: gamelist) {
	  int t=0;
	  game g(x);
	  g.display();
	  std::cin >> t;
	  enumerate_games(x);
	 // std::cout << x.first << x.second << "\n";
  }
  
  
  int dummy = 0;
  std::cin >> dummy;
}


//The below code counts permutations of the deck
/*
do {
	  count++;
	  if (count % 1000000 == 0)
		  std::cout << count / 1000000 << "\n";
	  std::vector<int> hand1 = std::vector<int>(deck.begin(), deck.begin() + 6);
	  std::vector<int>  hand2 = std::vector<int>(deck.begin()+6, deck.begin() +12);
	  //std::cout << hand1 << "\n";
	  //std::cout << hand2 << "\n";
	  std::sort(hand1.begin(), hand1.end());
	  std::sort(hand2.begin(), hand2.end());
	  std::pair<std::vector<int>, std::vector<int> > handpair(hand1, hand2);
	  gameset.insert(handpair);

} while (std::next_permutation(deck.begin(), deck.end()));
std::cout << count << "\n";
std::cout << gameset.size() << "\n";
*/
