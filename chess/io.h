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
#ifndef IO_H
#define IO_H

#include "Board.h"

#include <ostream>
#include <istream>

namespace chess {

char char_for_type(PieceType p);
char char_for_piece(Piece p);

std::ostream& operator<<(std::ostream& out, const Board& board);
std::ostream& operator<<(std::ostream& out, const Pos& p);
std::ostream& operator<<(std::ostream& out, const Move& m);

std::ostream& operator<<(std::ostream& out, Outcome o);
std::ostream& operator<<(std::ostream& out, Color c);

std::istream& operator>>(std::istream& in, Pos& pos);

}


#endif // IO_H
