
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
			//for (int i = 0; i < 8; ++i) {
			//	binaryBoard[i] = new int[8];
			//	pieceBoard[i] = new Piece[8];
			//}
			setupBoard();
		}

		Piece GetSquare(int x, int y) {
			return pieceBoard[x][y];
		}

		void SetSquare(int x, int y, Piece piece)
		{
			pieceBoard[x][y] = piece;
		}

		void printBoard() {
			//used to get string of enum
			string TypeToString[] = {"none", "pawn", "rook", "bishop", "knight", "king",
				"queen", "pawnEnPassant", "kingCastle", "rookCastle", "outOfBoard"};

			cout << endl;
			for (int i = 2; i < 10; ++i) {
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

		void setupBoard() {
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
							pieceBoard[i][j].SetType(Type::rookCastle); //starting rooks can castle
						}
						else if (j == 3 || j == 8) {
							pieceBoard[i][j].SetType(Type::knight);
						}
						else if (j == 4 || j == 7) {
							pieceBoard[i][j].SetType(Type::bishop);
						}
						else if ((j == 5 && i == 2) || (j == 6 && i == 9)) {
							pieceBoard[i][j].SetType(Type::queen);
						}
						else if ((j == 6 && i == 2) || (j == 5 && i == 9)) {
							pieceBoard[i][j].SetType(Type::kingCastle); // starting kings as castle
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
};