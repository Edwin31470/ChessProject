
#include <string>;

using namespace std;

enum Type {
	none,
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
	noColour
};

class Piece {

	public:
		Type type;
		Colour colour;

		Piece() { // if no information given, assume an empty board square
			type = Type::none;
			colour = Colour::noColour;
		}

		Piece(Type t, Colour c) {
			type = t;
			colour = c;
		}

		void Clear() {
			type = Type::none;
			colour = Colour::noColour;
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