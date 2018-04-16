
#include <string>;
#include <iostream>;

#include "Board.cpp";

using namespace std;

enum class Type {
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

enum class Colour {
	white,
	black,
	none
};

class TestingHandler {

	public:
		Board mainBoard;

		TestingHandler() {	
			mainBoard.printBoard();

			/*int x1 = 2;
			int y1 = 7;
			int x2 = 1;
			int y2 = 6;

			mainBoard.movePieceLegacy(y1, x1, y2, x2);
			mainBoard.printBoard();*/
		}

		void getMove() {
			string result;
			string letter1, letter2;
			int number1, number2;

			cout << "Input must be in example format 'a1b2':" << endl;

			cin >> result;

			letter1 = result.substr(0, 1);
			number1 = atoi(result.substr(1, 1).c_str());

			letter2 = result.substr(2, 1);
			number2 = atoi(result.substr(3, 1).c_str());

			mainBoard.movePiece(letter1, number1, letter2, number2);
			mainBoard.printBoard();
		};
};
