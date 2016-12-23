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
		b.board[x][1]				= Color::Black | PieceType::Pawn;
		b.board[x][Board::Size - 2]	= Color::White | PieceType::Pawn;
	}
	PieceType back_line[] = {PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::King, PieceType::Queen, PieceType::Bishop, PieceType::Knight, PieceType::Rook};
	for(size_t x = 0; x != Board::Size; ++x) {
		b.board[x][0]				= Color::Black | back_line[x];
		b.board[x][Board::Size - 1]	= Color::White | back_line[x];
	}
	return b;
}



Piece& Board::operator[](const Pos& p) {
	return board[p.x()][p.y()];
}

const Piece& Board::operator[](const Pos& p) const {
	return board[p.x()][p.y()];
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
