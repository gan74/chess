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

namespace chess {

struct Board {
	static constexpr usize Size = 8;
	std::array<std::array<Piece, Size>, Size> board = {};

	static Board start();

	using iterator = Piece*;
	using const_iterator = const Piece*;


	Piece& operator[](const Pos& p);
	const Piece& operator[](const Pos& p) const;

	bool has_piece(const Pos& p) const;
	bool has_piece(const Pos& p, Color col) const;

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

};

}

#endif // BOARD_H
