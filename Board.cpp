
#include "Piece.cpp";
#include <iostream>;

using namespace std;

class Board
{
	public:
		//8x8 board with 2 square padding around
		//Piece **pieceBoard = new Piece*[12];
		Piece pieceBoard[12][12];

		Board() {
			setupTestBoard();
		}

		Piece GetSquare(int number, int letter) {
			return pieceBoard[number][letter];
		}

		void SetSquare(int number, int letter, Piece piece)
		{
			pieceBoard[number][letter] = piece;
		}

		void ClearSquare(int number, int letter) {
			pieceBoard[number][letter].Clear();
		}

		
		void printBoard() {
			//used to get string of enum
			string TypeToString[] = {"none", "pawn", "rook", "bishop", "knight", "king",
				"queen", "pawnEnPassant", "kingCastle", "rookCastle", "outOfBoard"};

			cout << endl;
			//chess coordinates are bottom left to top right, c++ arrays are top left to bottom right
			//so vertical printing must be printed top to bottom to print accurately
			for (int i = 9; i > 1; --i) {
				for (int j = 2; j < 10; ++j) {
					if (pieceBoard[i][j].GetColour() == white) {
						cout << "W" << "|";
					}
					else if (pieceBoard[i][j].GetColour() == black) {
						cout << "B" << "|";
					}

					cout << TypeToString[pieceBoard[i][j].GetType()] << "   ";
				}
				cout << endl;
			}
			cout << endl;
		}

		void setupNormalBoard() {
			for (int i = 0; i < 12; ++i) {
				for (int j = 0; j < 12; ++j) {
					
					//set white
					if (i > 1 && i < 4){
						pieceBoard[i][j].SetColour(Colour::white);
					}
					//set black
					else if (i > 7 && i < 10) {
						pieceBoard[i][j].SetColour(Colour::black);
					}
					
					//set type
					if (i == 2 || i == 9) { // iterate across the non-pawn piece rows
						if (j == 2 || j == 9) {
							pieceBoard[i][j].SetType(Type::rook); //starting rooks can castle
						}
						else if (j == 3 || j == 8) {
							pieceBoard[i][j].SetType(Type::knight);
						}
						else if (j == 4 || j == 7) {
							pieceBoard[i][j].SetType(Type::bishop);
						}
						else if (j == 5) {
							pieceBoard[i][j].SetType(Type::queen);
						}
						else if (j == 6) {
							pieceBoard[i][j].SetType(Type::king); // starting kings as castle
						}
					}
					else if (i == 3 || i == 8) { // iterate across the pawn piece rows
						pieceBoard[i][j].SetType(Type::pawn);
					}

					//pad the outside with out of bounds
					if (i < 2 || i > 9 || j < 2 || j > 9)
					{
						pieceBoard[i][j].SetType(Type::outOfBoard);
						pieceBoard[i][j].SetColour(Colour::noColour);
					}
				}
			}
		}

		void setupTestBoard() {
			for (int i = 0; i < 12; ++i) {
				for (int j = 0; j < 12; ++j) {
					//pad the outside with out of bounds
					if (i < 2 || i > 9 || j < 2 || j > 9)
					{
						pieceBoard[i][j].SetType(Type::outOfBoard);
						pieceBoard[i][j].SetColour(Colour::noColour);
					}
				}
			}

			//e5
			pieceBoard[6][6].SetType(bishop);
			pieceBoard[6][6].SetColour(white);

			pieceBoard[6][4].SetType(pawn);
			pieceBoard[6][4].SetColour(black);

			pieceBoard[4][4].SetType(pawn);
			pieceBoard[4][4].SetColour(black);

		}
};