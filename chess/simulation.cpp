/*******************************next
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
#include <functional>
#include <numeric>

namespace chess {

int signed_value(PieceType p) {
	int values[] = {0, 1, 5, 3, 3, 9, 10000};
	return values[p];
}

int signed_value(Piece p) {
	return signed_value(p.type) * int(p.color);
}

int signed_value(const Board& board) {
	return std::accumulate(board.begin(), board.end(), 0, [](int v, const auto& p) { return v + signed_value(p); });
}

int signed_value(const Board& board, Color color) {
	return signed_value(board) * color;
}

int signed_value(Outcome o) {
	int w = signed_value(King);
	return o == Draw ? -(w / 2) : w * o;
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

static void king_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves) {
	Color color = b[pos].color;

	std::array<Pos, 8> offsets = {{Pos(1, 1), Pos(-1, 1), Pos(1, -1), Pos(-1, -1),
								   Pos(1, 0), Pos(-1, 0), Pos(0, 1), Pos(0, -1)}};
	array_moves(b, color, pos, offsets, moves);
}

void legal_moves(const Board& board, const Pos& pos, core::Vector<Pos>& moves) {
	switch(board[pos].type) {
		case Pawn:
			return pawn_moves(board, pos, moves);

		case Rook:
			return rook_moves(board, pos, moves);

		case Knight:
			return knight_moves(board, pos, moves);

		case Bishop:
			return bishop_moves(board, pos, moves);

		case Queen:
			return queen_moves(board, pos, moves);

		case King:
			return king_moves(board, pos, moves);

		default:
			return;
	}
}

void all_legal_moves(const Board &board, Color color, core::Vector<Move> &moves) {
	for(const auto& pos : board.positions()) {
		auto p = board[pos];
		if(p.color == color) {
			core::Vector<Pos> m;
			legal_moves(board, pos, m);

			moves.append(core::range(m)
				.map([=](auto dst) {
					return std::make_pair(pos, dst);
				}));
		}
	}
}


void all_legal_moves_conservative(const Board& board, Color color, core::Vector<Move>& moves) {
	for(const auto& pos : board.positions()) {
		auto p = board[pos];
		if(p.color == color) {
			core::Vector<Pos> m;
			legal_moves(board, pos, m);
			if(p.type == King) {
				BitBoard cover = coverage(board, -color);
				for(const auto& dst : m) {
					if(!cover[board.to_index(dst)]) {
						moves << std::make_pair(pos, dst);
					}
				}
			} else {
				moves.append(core::range(m)
					.map([=](auto dst) {
						return std::make_pair(pos, dst);
					}));
			}
		}
	}
}

void all_legal_dst(const Board &board, Color color, core::Vector<Pos> &dsts) {
	for(const auto& pos : board.positions()) {
		auto p = board[pos];
		if(p.color == color) {
			legal_moves(board, pos, dsts);
		}
	}
}

BitBoard coverage(const Board &board, Color color) {
	core::Vector<Pos> dsts;
	all_legal_dst(board, color, dsts);
	BitBoard bits;
	for(const auto& p : dsts) {
		bits[board.to_index(p)] = true;
	}
	return bits;
}

bool is_covered(const Board &board, const Pos& pos, Color color) {
	core::Vector<Pos> dsts;
	all_legal_dst(board, color, dsts);
	return std::any_of(dsts.begin(), dsts.end(), [=](const Pos& dst) { return dst == pos; });
}


bool is_check(const Board& board, Color color) {
	return is_covered(board, board.king(color), -color);
}

Outcome monte_carlo(Board& board, Color color) {
	{
		core::Vector<std::pair<Pos, Pos>> moves;
		all_legal_moves(board, color, moves);
		if(moves.is_empty()) {
			return Draw;
		}
		auto king_pos = board.king(-color);
		if(std::any_of(moves.begin(), moves.end(), [=](const Move& m) { return m.second == king_pos; })) {
			return Win;
		}
		board = board(moves[rand() % moves.size()]);
	}

	Outcome outcome = board.immediate_status(color);
	if(!outcome) {
		return -monte_carlo(board, -color);
	}
	return outcome;
}


static int minimax_rec(const Board& board, Color color, usize rec_limit) {
	if(!rec_limit || board.immediate_status(color)) {
		return signed_value(board, color);
	}

	core::Vector<std::pair<Pos, Pos>> moves;
	all_legal_moves_conservative(board, color, moves);

	if(moves.is_empty()) {
		return -(is_check(board, color) ? signed_value(Win) : signed_value(Draw));
	}

	int max = std::numeric_limits<int>::min();
	for(const auto& move : moves) {
		max = std::max(max, -minimax(board(move), -color, rec_limit - 1));
	}
	return max;
}

int minimax(const Board& board, Color color, usize rec_limit) {
	return minimax_rec(board, color, rec_limit);
}


}
