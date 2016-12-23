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
#ifndef SIMULATION_H
#define SIMULATION_H

#include "Board.h"
#include <y/core/Vector.h>

namespace chess {

int value(PieceType p);
int signed_value(Piece p);

int signed_value(const Board& b);

void legal_moves(const Board& b, const Pos& pos, core::Vector<Pos>& moves);

}


#endif // SIMULATION_H
