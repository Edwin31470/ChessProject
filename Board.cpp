
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

		}

		Piece GetSquare(int number, int letter) {
			return pieceBoard[number][letter];
		}

		void SetSquare(int number, int letter, Piece piece)
		{
			pieceBoard[number][letter] = piece;
		}

		//only used for board setup
		void SetTypeOfPiece(int number, int letter, Type type)
		{
			pieceBoard[number][letter].type = type;
		}

		//only used for board setup
		void SetColourOfPiece(int number, int letter, Colour colour)
		{
			pieceBoard[number][letter].colour = colour;
		}

		void ClearSquare(int number, int letter) {
			pieceBoard[number][letter].Clear();
		}

		//used to copy the state of one board to this board
		void Equals(Board& boardToCopy)
		{
			for (int numberCoord = 0; numberCoord < 12; numberCoord++) {
				for (int letterCoord = 0; letterCoord < 12; letterCoord++) {
					pieceBoard[numberCoord][letterCoord] = boardToCopy.GetSquare(numberCoord, letterCoord);
				}
			}
		}
};