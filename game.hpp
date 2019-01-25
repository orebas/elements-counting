

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
	if (!v.empty()) {
		out << '[';
		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ""));
		out << "]";
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


template <typename T, typename U>
std::ostream& operator<< (std::ostream& out, const std::pair<T, U>& v) {
	if (!v.empty()) {
		out << '[';
		out << v.first << ", " << v.second;
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
typedef std::array<card, 6> hand;
typedef std::pair<hand, hand> handpair;
typedef std::vector<card> smallstack;
typedef std::vector<move_type> smallmovestack;



void count_hand_possibilities();
int count_games_restricted(const hand& firsthand, std::vector<handpair> & gamelist);
int  count_initial_games(std::vector<handpair> &  gamelist);
int enumerate_games(const handpair& init);


//The things below referred to as "Stacks" are actually vectors, and use the vector API.  we may change that.

class game {
	hand hidden[2];
	hand shown[2];
	int sums[2] = { 0,0 };
	int pilesum = 0;
	smallstack pile;
	smallstack drawstack;
	smallmovestack history;
	int nextplayer = 0;
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
		if (ret_move_list.size() == 0) {
			//std::cout << "There were no moves, so we added FOLD\n";

			ret_move_list.push_back(move_type::fold);
		}
		return ret_move_list;

	}
	int move(move_type m) { //returns 0 if game continues, 1 if p0 wins, -1 if p1 wins
		switch (m) {
		case move_type::knock:
			history.push_back(m);
			if (sums[nextplayer] <= pilesum) {
				if (sums[1 - nextplayer] > pilesum)
					return nextplayer * -2 + 1;
				else if (sums[1 - nextplayer] >= sums[nextplayer])
					return nextplayer * 2 - 1;
				else return nextplayer * -2 + 1;
			}
			else {
				std::cout << "Got an error" << nextplayer << "  " << " " << sums << " " << pilesum << "\n";
				display();
				assert(0); //shouldn't be able to knock
			}

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
			history.push_back(m);
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
	int enumerate_games_recurse(int depth) {
		static int maxprint = 0;
		auto my_move_list = this->move_list();

		auto rng = std::default_random_engine{};
		std::shuffle(my_move_list.begin(), my_move_list.end(), rng);
		//std::cout << depth << " ";
		//display();
		//std::cout << my_move_list;
		//int j;
		//std::cin >> j;
		maxprint += my_move_list.size();
		if (maxprint > 1000000000000)
			return 0;
		for (auto m : my_move_list) {

			if (m == move_type::knock || m == move_type::fold) {
				//std::cout << "HISTORY SIZE" << history.size() << history;
								//std::cout << "DONE: " << "\n";
				std::cout << history << "\n";
			}
			else {
				move(m);
				enumerate_games_recurse(depth + 1);
				unmove();
			}


		}
		return 0;
	}



};


