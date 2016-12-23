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
#ifndef CHESS_H
#define CHESS_H

#include <y/utils.h>
#include <y/math/Vec.h>

namespace chess {

using namespace y;

enum Color : i8 {
	White = 1,
	Black = -1
};

inline Color operator-(Color c) {
	return Color(-int(c));
}




enum Outcome {
	Win = 1,
	Draw = 0,
	Lose = -1
};

inline Outcome operator-(Outcome o) {
	return Outcome(-int(o));
}





enum PieceType : u8 {
	None = 0,
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King
};





struct Piece {
	Color color;
	PieceType type;

	bool operator==(const Piece& other) const {
		return type == other.type && (type ? other.color == color : true);
	}

	bool operator!=(const Piece& other) const {
		return !operator==(other);
	}
};

inline Piece operator|(Color c, PieceType t) {
	return Piece{c, t};
}





using Pos = math::Vec<2, i8>;
using Move = std::pair<Pos, Pos>;

}

#endif // CHESS_H
