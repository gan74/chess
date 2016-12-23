#include <iostream>
#include <utility>


#include <chess/io.h>
#include <chess/simulation.h>

using namespace chess;

int main() {
	Board board = Board::start();

	while(true) {
		std::cout << "\n\n\n" << board;
		std::cout << signed_value(board) << std::endl;

		Pos pos;
		do {
			std::cout << ">>> ";
			std::cin.clear();
			std::cin >> pos;
		} while(!std::cin);

		core::Vector<Pos> moves;
		legal_moves(board, pos, moves);
		std::cout << moves.size() << " possible moves:" << std::endl;
		for(auto m : moves) {
			std::cout << " " << m << std::endl;
		}

		if(!moves.is_empty()) {
			board[rand() % moves.size()] = board[pos];
			board[pos].type = PieceType::None;
		}
	}

	return 0;
}






