
#include <string>;

using namespace std;

enum Type {
	empty,
	pawn,
	rook,
	bishop,
	knight,
	king,
	queen,
	pawnEnPassant,
	kingCastle,
	rookCastle,
	outOfBoard
};

enum Colour {
	white,
	black,
	none
};

class Piece {

	public:
		Type type;
		Colour colour;

		Piece() { // if no information given, assume an empty board square
			type = Type::empty;
			colour = Colour::none;
		}

		Piece(Type t, Colour c) {
			type = t;
			colour = c;
		}

		void Clear() {
			type = Type::empty;
			colour = Colour::none;
		}

		Type GetType() {
			return type;
		}

		void SetType(Type t)
		{
			type = t;
		}

		Colour GetColour() {
			return colour;
		}

		void SetColour(Colour c)
		{
			colour = c;
		}

};