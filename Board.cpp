
#include "Piece.cpp";
#include <iostream>;

class Board
{
	public:
		//int **binaryBoard = new int*[8];
		Piece **pieceBoard = new Piece*[8];

		Board() {
			for (int i = 0; i < 8; ++i) {
				//binaryBoard[i] = new int[8];
				pieceBoard[i] = new Piece[8];
			}
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

		//temp function
		void setupBoard() {
			for (int i = 0; i < 8; ++i) {
				for (int j = 0; j < 8; ++j) {
					//set colour
					if (i <= 1){
						pieceBoard[i][j].SetColour(1);
					}
					else if (i >= 6) {
						pieceBoard[i][j].SetColour(2);
					}
					
					//set type
					if (i == 0 || i == 7) {
						if (j == 0 || j == 7) {
							pieceBoard[i][j].SetType("rook");
						}
						else if (j == 1 || j == 6) {
							pieceBoard[i][j].SetType("knight");
						}
						else if (j == 2 || j == 5) {
							pieceBoard[i][j].SetType("bishop");
						}
						else if ((j == 3 && i == 0) || (j == 4 && i == 7)) {
							pieceBoard[i][j].SetType("queen");
						}
						else if ((j == 4 && i == 0) || (j == 3 && i == 7)) {
							pieceBoard[i][j].SetType("king");
						}
					}
					else if (i == 1 || i == 6) {
						pieceBoard[i][j].SetType("pawn");
					}
				}
			}
		}


		void movePiece(int toMoveNum, int toMoveLet, int toTakeNum, int toTakeLet) {
			
			Piece pieceToTake = pieceBoard[toTakeNum - 1][toTakeLet - 1];
			Piece pieceToMove = pieceBoard[toMoveNum - 1][toMoveLet - 1];

			if (validateMove(pieceToMove, pieceToTake, toMoveNum, toMoveLet, toTakeNum, toTakeLet) == 1)
			{
				pieceBoard[toTakeNum - 1][toTakeLet - 1] = pieceBoard[toMoveNum - 1][toMoveLet - 1];
				pieceBoard[toMoveNum - 1][toMoveLet - 1].Clear();
			}
			
		}

		//return of 0 is an invalid move
		int validateMove(Piece& pieceToMove, Piece& pieceToTake, int& toMoveNum, int& toMoveLet, int& toTakeNum, int& toTakeLet)
		{
			//for white pawns
			if (pieceToMove.type == "pawn" && pieceToMove.colour == 1)
			{
				if (toMoveNum + 1 == toTakeNum)
				{
					if ((toMoveLet == toTakeLet + 1) || (toMoveLet == toTakeLet - 1)) {
						return 1;
					}
				}
			}

			//for black pawns
			if (pieceToMove.type == "pawn" && pieceToMove.colour == 2)
			{
				if (toMoveNum - 1 == toTakeNum)
				{
					if ((toMoveLet + 1 == toTakeLet) || (toMoveLet - 1 == toTakeLet)) {
						return 1;
					}
				}
			}
			
			return 0;
		}
};