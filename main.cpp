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
		int score = -minimax(board(move), -color, 5);
		if(score > max) {
			max = score;
			best = move;
		}
		//std::cout << score << std::endl;

	}
	//std::cout << color << ": " << best << "  " << max << std::endl;
	return best;
}

Move best_mt(const Board& board, Color color) {
	core::Vector<std::pair<Pos, Pos>> moves;
	all_legal_moves(board, color, moves);

	Move best;
	const usize max = 1000;
	usize best_w = 0;
	for(const auto& move : moves) {
		usize wins = 0;
		for(usize i = 0; i != max; i++) {
			Board next = board(move);
			wins += (monte_carlo(next, -color) == Lose);
		}
		if(wins > best_w) {
			best_w = wins;
			best = move;
		}
	}
	std::cout << best_w << std::endl;
	return best;
}

int main() {
	Board board = Board::start();
	Color color = White;

	while(!board.immediate_status(color)) {
		std::cout << board << std::endl;

		Chrono ch;
		Move best = color == White ? best_move(board, color) : best_mt(board, color);
		if(best == Move()) {
			break;
		}
		std::cout << color << "  " << best << " (" << ch.elapsed().to_millis() << "ms)" << std::endl;

		board = board(best);
		color = -color;
	}
	std::cout << White << " " << board.immediate_status(White) << std::endl;

	return 0;
}






