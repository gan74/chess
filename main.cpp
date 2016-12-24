#include <iostream>
#include <utility>


#include <chess/io.h>
#include <chess/simulation.h>

using namespace chess;

Move best_move(const Board& board, Color color) {
	Move best;
	int max = std::numeric_limits<int>::min();

	core::Vector<std::pair<Pos, Pos>> moves;
	all_legal_moves(board, color, moves);

	//std::cout << color << " plays:" << std::endl;
	for(const auto& move : moves) {
		//std::cout << move << std::endl;
		int score = -minimax(board(move), -color, 3);
		if(score > max) {
			max = score;
			best = move;
		}
		//std::cout << score << std::endl;

	}
	if(max == std::numeric_limits<int>::min()) {
		fatal("Draw");
	}
	std::cout << color << ": " << best << "  " << max << std::endl;
	return best;
}

int main() {
	Board board = Board::start();
	Color color = White;

	while(true) {
		std::cout << board << std::endl;

		Move best = best_move(board, color);

		board = board(best);
		color = -color;
	}

	return 0;
}






