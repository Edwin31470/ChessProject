
#include <string>;
#include <iostream>;
#include <stdlib.h>;
#include <random>;

#include "MoveHandler.cpp";

using namespace std;

class AI
{
	MoveHandler moveHandler;

	public:
		AI() {

		}

		void randomMove(Colour colour, Board& realBoard) {
			vector<Move> validMoves = moveHandler.validMoves(colour, realBoard);
			int random = (rand() % validMoves.size());
			Move move = validMoves[random];
			moveHandler.movePiece(move, colour, realBoard);
			
			cout << "AI move: " << realBoard.GetSquare(move.newLet, move.newNum).type << " to " << move.newLet << move.newNum << endl;
		}
};