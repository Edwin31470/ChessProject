

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
		//in order: "none", "pawn", "rook", "bishop", "knight", "king", "queen", "pawnEnPassant", "kingCastle", "rookCastle", "outOfBoard" (outOfBoard shouldn't be shown anyway)
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
			cout << i - 1 << endl;
		}
		cout << "  A  B  C  D  E  F  G  H" << endl << endl;
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
						board.SetTypeOfPiece(i, j, Type::rookCastle); //starting rooks can castle
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
						board.SetTypeOfPiece(i, j, Type::kingCastle); // starting kings as castle
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

	void setupCastlingBoard(Board& board) {
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
						board.SetTypeOfPiece(i, j, Type::rookCastle); //starting rooks can castle
					}
					else if (j == 3 || j == 8) {
						board.SetColourOfPiece(i, j, Colour::noColour);
					}
					else if (j == 4 || j == 7) {
						board.SetColourOfPiece(i, j, Colour::noColour);
					}
					else if (j == 5) {
						board.SetColourOfPiece(i, j, Colour::noColour);
					}
					else if (j == 6) {
						board.SetTypeOfPiece(i, j, Type::kingCastle); // starting kings as castle
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
					board.SetTypeOfPiece(i, j, Type::outOfBoard);
					board.SetColourOfPiece(i, j, Colour::noColour);
				}
			}
		}

		//e5
		//board.SetTypeOfPiece(6, 6, Type::rook);
		//board.SetColourOfPiece(6, 6, Colour::black);

		board.SetTypeOfPiece(5, 4, Type::pawn);
		board.SetColourOfPiece(5, 4, Colour::black);

		board.SetTypeOfPiece(3, 3, Type::pawn);
		board.SetColourOfPiece(3, 3, Colour::white);
	}

	void setupBoardNoPawns(Board& board) {
		for (int i = 0; i < 12; ++i) {
			for (int j = 0; j < 12; ++j) {

				//set white
				if (i > 1 && i < 3) {
					board.SetColourOfPiece(i, j, Colour::white);
				}
				//set black
				else if (i > 8 && i < 10) {
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

				//pad the outside with out of bounds
				if (i < 2 || i > 9 || j < 2 || j > 9)
				{
					board.SetTypeOfPiece(i, j, Type::outOfBoard);
					board.SetColourOfPiece(i, j, Colour::noColour);
				}
			}
		}
	}
};