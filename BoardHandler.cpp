

#include "AI.cpp";
#include <iostream>;

using namespace std;

class BoardHandler
{
public:

	BoardHandler() {

	}

	void printBoard(Board board) {
		//used to get string of enum
		//in order: "none", "pawn", "rook", "bishop", "knight", "king", "queen", "pawnEnPassant", "kingCastle", "rookCastle", "outOfBoard"
		string TypeToString[] = { "[]", "P", "R", "B", "N", "K", "Q", "E", "C", "H", "#" };

		//chess coordinates are bottom left to top right, c++ arrays are top left to bottom right
		//so vertical printing must be printed top to bottom to print accurately
		cout << endl << "  A  B  C  D  E  F  G  H" << endl;
		for (int i = 9; i > 1; --i) {
			cout << i - 1 << " ";
			for (int j = 2; j < 10; ++j) {
				if (board.GetSquare(i, j).colour == white) {
					cout << "w";
				}
				else if (board.GetSquare(i, j).colour == black) {
					cout << "b";
				}

				cout << TypeToString[board.GetSquare(i, j).type] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void setupNormalBoard(Board& board) {
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 12; ++j) {

				//set white
				if (i > 1 && i < 4) {
					board.SetColourOfPiece(i, j, Colour::white);
				}
				//set black
				else if (i > 7 && i < 10) {
					board.SetColourOfPiece(i, j, Colour::black);
				}

				//set type
				if (i == 2 || i == 9) { // iterate across the non-pawn piece rows
					if (j == 2 || j == 9) {
						board.SetTypeOfPiece(i, j, Type::rook); //starting rooks can castle
					}
					else if (j == 3 || j == 8) {
						board.SetTypeOfPiece(i, j, Type::knight);
					}
					else if (j == 4 || j == 7) {
						board.SetTypeOfPiece(i, j, Type::bishop);
					}
					else if (j == 5) {
						board.SetTypeOfPiece(i, j, Type::queen);
					}
					else if (j == 6) {
						board.SetTypeOfPiece(i, j, Type::king); // starting kings as castle
					}
				}
				else if (i == 3 || i == 8) { // iterate across the pawn piece rows
					board.SetTypeOfPiece(i, j, Type::pawn);
				}

				//pad the outside with out of bounds
				if (i < 2 || i > 9 || j < 2 || j > 9)
				{
					board.SetTypeOfPiece(i, j, Type::outOfBoard);
					board.SetColourOfPiece(i, j, Colour::noColour);
				}
			}
		}
	}

	void setupTestBoard(Board& board) {
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 12; ++j) {
				//pad the outside with out of bounds
				if (i < 2 || i > 9 || j < 2 || j > 9)
				{
					board.GetSquare(i, j).SetType(Type::outOfBoard);
					board.GetSquare(i, j).SetColour(Colour::noColour);
				}
			}
		}

		//e5
		board.GetSquare(6, 6).SetType(Type::bishop);
		board.GetSquare(6, 6).SetColour(Colour::white);

		board.GetSquare(6, 4).SetType(Type::pawn);
		board.GetSquare(6, 4).SetColour(Colour::black);

		board.GetSquare(4, 4).SetType(Type::pawn);
		board.GetSquare(4, 4).SetColour(Colour::black);

	}
};