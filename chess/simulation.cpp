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

#include "simulation.h"
#include <algorithm>
#include <numeric>

namespace chess {

int value(PieceType p) {
	int values[] = {0, 1, 5, 3, 3, 9, 10000};
	return values[p];
}

int signed_value(Piece p) {
	return value(p.type) * int(p.color);
}

int signed_value(const Board& b) {
	return std::accumulate(b.begin(), b.end(), 0, [](int v, const auto& p) { return v + signed_value(p); });
}



static bool pos_valid(const Pos& p) {
	return usize(p.x()) < Board::Size && usize(p.y()) < Board::Size;
}

static bool can_move(const Board& b, const Pos& p, Color col) {
	auto piece = b[p];
	return !piece.type || piece.color != col;
}


static void linear_move(const Board& b, Color color, Pos pos, const Pos& offset, core::Vector<Pos>& moves) {
	while(true) {
		pos += offset;
		if(!pos_valid(pos)) {
			return;
		}
		auto p = b[pos];
		if(!p.type) {
			moves << pos;
		} else {
			if(p.color != color) {
				moves << pos;
			}
			return;
		}
	}
}

template<usize N>
static void array_moves(const Board& b, Color color, const Pos& pos, const std::array<Pos, N>& offsets, core::Vector<Pos>& moves) {
	for(auto p : offsets) {
		p += pos;
		if(pos_valid(p) && can_move(b, p, color)) {
			moves << p;
		}
	}
}


static void rook_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	Color color = b[pos].color;
	linear_move(b, color, pos, Pos(-1, 0), moves);
	linear_move(b, color, pos, Pos(1, 0), moves);
	linear_move(b, color, pos, Pos(0, -1), moves);
	linear_move(b, color, pos, Pos(0, 1), moves);
}

static void bishop_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	Color color = b[pos].color;
	linear_move(b, color, pos, Pos(1, 1), moves);
	linear_move(b, color, pos, Pos(1, -1), moves);
	linear_move(b, color, pos, Pos(-1, -1), moves);
	linear_move(b, color, pos, Pos(-1, 1), moves);
}

static void queen_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	rook_moves(b, pos, moves);
	bishop_moves(b, pos, moves);
}

static void king_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	Color color = b[pos].color;

	std::array<Pos, 8> offsets = {{Pos(1, 1), Pos(-1, 1), Pos(1, -1), Pos(-1, -1),
								   Pos(1, 0), Pos(-1, 0), Pos(0, 1), Pos(0, -1)}};
	array_moves(b, color, pos, offsets, moves);
}

static void knight_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	Color color = b[pos].color;
	std::array<Pos, 8> offsets = {{Pos(2, 1), Pos(-2, 1), Pos(2, -1), Pos(-2, -1),
								   Pos(1, 2), Pos(1, -2), Pos(-1, 2), Pos(-1, -2)}};
	array_moves(b, color, pos, offsets, moves);
}

static void pawn_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	Color color = b[pos].color;
	int dir = -int(color);

	std::array<Pos, 2> cap_moves = {{Pos(1, dir), Pos(-1, dir)}};
	for(const auto& m : cap_moves) {
		auto p = pos + m;
		if(pos_valid(p) && b.has_piece(p, -color)) {
			moves << p;
		}
	}
	if(!b.has_piece(pos + Pos(0, dir))) {
		moves << pos + Pos(0, dir);
	}
}

void legal_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	switch(b[pos].type) {
		case PieceType::Pawn:
			return pawn_moves(b, pos, moves);

		case PieceType::Rook:
			return rook_moves(b, pos, moves);

		case PieceType::Knight:
			return knight_moves(b, pos, moves);

		case PieceType::Bishop:
			return bishop_moves(b, pos, moves);

		case PieceType::Queen:
			return queen_moves(b, pos, moves);

		case PieceType::King:
			return king_moves(b, pos, moves);

		default:
			return;
	}
}

}