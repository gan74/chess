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
#ifndef BOARD_H
#define BOARD_H

#include "chess.h"
#include <y/core/Range.h>
#include <bitset>

namespace chess {


struct Board {
	static constexpr usize Size = 8;
	std::array<std::array<Piece, Size>, Size> board = {};
	static Board start();
	static Board wins(Color color);

	using iterator = Piece*;
	using const_iterator = const Piece*;


	Piece& operator[](usize i);
	const Piece& operator[](usize i) const;

	Pos to_pos(usize i) const;
	usize to_index(const Pos& pos) const;

	Piece& operator[](const Pos& p);
	const Piece& operator[](const Pos& p) const;
	Piece& piece(const Pos& p);
	const Piece& piece(const Pos& p) const;

	Pos king(Color color) const;

	bool has_piece(const Pos& p) const;
	bool has_piece(const Pos& p, Color col) const;

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	Board operator()(const Move& m) const;
	Outcome immediate_status(Color color) const;


	auto const positions() const {
		return core::range(usize(0), Size * Size).map([](usize i) { return Pos(i % Size, i / Size); });
	}

	auto pieces() const {
		return core::range(*this).filter([](Piece p) { return !!p.type; });
	}

	auto pieces(Color col) const {
		return core::range(*this).filter([=](Piece p) { return !!p.type && p.color == col; });
	}

	auto pieces_pos() const {
		return positions().filter([&](Pos p) { return has_piece(p); }).map([&](Pos p) { return std::make_pair(p, piece(p)); });
	}

	auto pieces_pos(Color col) const {
		return positions().filter([&](Pos p) { return has_piece(p, col); }).map([&](Pos p) { return std::make_pair(p, piece(p)); });
	}

};

}

#endif // BOARD_H
