
#include "Piece.cpp";
#include <iostream>;

using namespace std;

class Board
{
	public:
		Piece **pieceBoard = new Piece*[8];

		Board() {
			//for (int i = 0; i < 8; ++i) {
			//	binaryBoard[i] = new int[8];
			//	pieceBoard[i] = new Piece[8];
			//}
			setupBoard();
		}

		Piece GetSquare(int x, int y) {
			return pieceBoard[x - 1][y - 1];
		}

		void SetSquare(int x, int y, Piece piece)
		{
			pieceBoard[x - 1][y - 1] = piece;
		}

		void printBoard() {
			cout << endl;
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 8; ++j) {
					if (pieceBoard[i][j].GetColour() == 1) {
						cout << "W" << "|";
					}
					else if (pieceBoard[i][j].GetColour() == 2) {
						cout << "B" << "|";
					}
					cout << pieceBoard[i][j].GetType() << "   ";
				}
				cout << endl;
			}
			cout << endl;
		}

		void setupBoard() {
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 8; ++j) {
					
					//set white
					if (i <= 1){
						pieceBoard[i][j].SetColour(Colour::white);
					}
					else if (i >= 6) {
						pieceBoard[i][j].SetColour(Colour::black);
					}
					
					//set type
					if (i == 0 || i == 7) { // iterate across the non-pawn piece rows
						if (j == 0 || j == 7) {
							pieceBoard[i][j].SetType(Type::rookCastle); //starting rooks can castle
						}
						else if (j == 1 || j == 6) {
							pieceBoard[i][j].SetType(Type::knight);
						}
						else if (j == 2 || j == 5) {
							pieceBoard[i][j].SetType(Type::bishop);
						}
						else if ((j == 3 && i == 0) || (j == 4 && i == 7)) {
							pieceBoard[i][j].SetType(Type::queen);
						}
						else if ((j == 4 && i == 0) || (j == 3 && i == 7)) {
							pieceBoard[i][j].SetType(Type::kingCastle); // starting kings cas castle
						}
					}
					else if (i == 1 || i == 6) { // iterate across the pawn piece rows
						pieceBoard[i][j].SetType(Type::pawn);
					}
				}
			}
		}
};