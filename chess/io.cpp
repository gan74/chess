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

#include "io.h"

#include <ios>

namespace chess {

char char_for_type(PieceType p) {
	char chars[] = {'.', 'p', 'r', 'n', 'b', 'q', 'k'};
	return chars[p];
}

char char_for_piece(Piece p) {
	char c = char_for_type(p.type);
	if(p.type && p.color == Color::White) {
		return char(toupper(c));
	}
	return c;
}



static void print_line(std::ostream& out) {
	out << "  +-";
	for(size_t x = 0; x != Board::Size; ++x) {
		out << "--";
	}
	out << "+\n";
}

static void print_columns(std::ostream& out) {
	out << "   ";
	for(size_t x = 0; x != Board::Size; ++x) {
		out << ' ' << char('a' + x);
	}
	out << '\n';
}

std::ostream& operator<<(std::ostream& out, const Board& board) {
	print_columns(out);
	print_line(out);
	for(size_t x = 0; x != Board::Size; ++x) {
		char digit = '0' + Board::Size - x;
		out << digit << " |";
		for(size_t y = 0; y != Board::Size; ++y) {
			out << ' ' << char_for_piece(board.board[y][x]);
		}
		out << " | " << digit << '\n';
	}
	print_line(out);
	print_columns(out);
	return out;
}

static bool validate_digit(char c) {
	return c >= '1' && c < char('1' + Board::Size);
}

static bool validate_alpha(char c) {
	return c >= 'a' && c < char('a' + Board::Size);
}

static int8_t decode_digit(char c) {
	return int8_t(Board::Size - (c - '0'));
}

static int8_t decode_alpha(char c) {
	return int8_t(c - 'a');
}

std::ostream& operator<<(std::ostream& out, const Pos& p) {
	out << char('a' + p.x()) << (Board::Size - p.y());
	return out;
}


std::istream& operator>>(std::istream& in, Pos& pos) {
	std::string str;
	in >> str;
	if(!in.fail()) {
		if(str.size() == 2 && validate_alpha(str[0]) && validate_digit(str[1])) {
			pos = Pos{decode_alpha(str[0]), decode_digit(str[1])};
		} else {
			in.setstate(in.rdstate() | std::ios_base::failbit);
		}
	}
	return in;
}

}
