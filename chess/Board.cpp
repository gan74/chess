/*******************************
Copyright (C) 2013-2016 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/

#include "Board.h"

namespace chess {


Board Board::start() {
	Board b;
	for(size_t x = 0; x != Board::Size; ++x) {
		b.board[x][1]				= Black | Pawn;
		b.board[x][Board::Size - 2]	= White | Pawn;
	}
	PieceType back_line[] = {Rook, Knight, Bishop, King, Queen, Bishop, Knight, Rook};
	for(size_t x = 0; x != Board::Size; ++x) {
		b.board[x][0]				= Black | back_line[x];
		b.board[x][Board::Size - 1]	= White | back_line[x];
	}
	return b;
}

Board Board::wins(Color color) {
	Board b;
	b[Pos(0, 0)] = -color | King;
	b[Pos(Size - 1, Size - 1)] = color | King;

	b[Pos(Size - 1, 1)] = color | Rook;
	b[Pos(Size - 2, 2)] = color | Rook;
	return b;
}



Board Board::operator()(const Move& m) const {
	Board b(*this);
	b[m.second] = std::exchange(b[m.first], Piece{});
	return b;
}

Outcome Board::immediate_status(Color color) const {
	int score = 0;
	for(const auto& p : *this) {
		if(p.type == King) {
			score += p.color;
		}
	}
	score *= color;
	return score < 0 ? Lose : score > 0 ? Win : Draw;
}

Pos Board::to_pos(usize i) const {
	return Pos(i / Size, i % Size);
}

usize Board::to_index(const Pos& pos) const {
	return pos.x() * Size + pos.y();
}

Piece& Board::operator[](usize i) {
	return *(begin() + i);
}

const Piece& Board::operator[](usize i) const {
	return *(begin() + i);
}

Piece& Board::operator[](const Pos& p) {
	return board[p.x()][p.y()];
}

const Piece& Board::operator[](const Pos& p) const {
	return board[p.x()][p.y()];
}

Piece& Board::piece(const Pos& p) {
	return board[p.x()][p.y()];
}

const Piece& Board::piece(const Pos& p) const {
	return board[p.x()][p.y()];
}

Pos Board::king(Color color) const {
	for(const auto& pos : positions()) {
		auto p = piece(pos);
		if(p.type == King && p.color == color) {
			return pos;
		}
	}
	return Pos(-1);
}

Board::iterator Board::begin() {
	return &board[0][0];
}

Board::const_iterator Board::begin() const {
	return &board[0][0];
}

Board::iterator Board::end() {
	return &board[Size - 1][Size];
}

Board::const_iterator Board::end() const {
	return &board[Size - 1][Size];
}


bool Board::has_piece(const Pos& p) const {
	return !!board[p.x()][p.y()].type;
}

bool Board::has_piece(const Pos& p, Color col) const {
	auto piece = board[p.x()][p.y()];
	return piece.type && piece.color == col;
}


}
