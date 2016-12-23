#include <iostream>
#include <utility>


#include <chess/io.h>
#include <chess/simulation.h>

using namespace chess;

int main() {
	Board board = Board::wins(White, false);

	{
		std::cout << board;


		Move best;
		int score = minmax(best, board, Black, 1);

		std::cout << "\nbest = " << best.first << best.second << " (score = " << score << ")" << std::endl;

		std::cout << board(best) << std::endl;

	}

	return 0;
}






