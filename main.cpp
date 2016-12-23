#include <iostream>
#include <utility>


#include <chess/io.h>
#include <chess/simulation.h>

using namespace chess;

int main() {
	Board board = Board::wins(Color::White);
	usize total = 10000;

	{
		std::cout << "\n\n\n" << board;
		std::cout << signed_value(board) << std::endl;

		core::Vector<Move> moves;
		all_legal_moves(board, Color::White, moves);
		std::cout << moves.size() << " moves" << std::endl;

		usize wins = 0;
		usize draws = 0;
		usize loses = 0;
		for(usize i = 0; i != total; ++i) {
			Board cpy = board;
			auto o = monte_carlo(cpy, Color::White);
			wins += o == Outcome::Win;
			draws += o == Outcome::Draw;
			loses += o == Outcome::Lose;
			if(o == Outcome::Lose) {
				std::cout << cpy << std::endl;
			}
		}

		std::cout << wins << " wins (" << (wins * 100) / total << "%)" << std::endl;
		std::cout << draws << " draws (" << (draws * 100) / total << "%)" << std::endl;
		std::cout << loses << " loses (" << (loses * 100) / total << "%)" << std::endl;
	}

	return 0;
}






