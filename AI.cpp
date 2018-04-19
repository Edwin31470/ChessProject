
#include <string>;
#include <iostream>;
#include <stdlib.h>;
#include <random>;
#include <algorithm>;

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

		void AIMove(Colour colour, Board& realBoard, int depth) {

			Move bestMove = negaMaxInitial(realBoard, depth, -1000, 1000, colour);
			moveHandler.movePiece(bestMove, colour, realBoard);

			cout << "AI move: " << realBoard.GetSquare(bestMove.newLet, bestMove.newNum).type << " to " << bestMove.newLet << bestMove.newNum << endl;
		}

		//h/ttps://chessprogramming.wikispaces.com/Negamax

		//initial call of negaMax. will return the actual move of the best predicted move
		Move negaMaxInitial(Board node, int depth, int alpha, int beta, Colour colour)
		{
			vector<Move> childNodes = moveHandler.validMoves(colour, node);
			//order nodes

			Move bestMove(0,0,0,0);
			int bestValue = -1000;
			for each (Move move in childNodes)
			{
				Board board = node;
				moveHandler.movePiece(move, colour, board);
				int value = negaMax(board, depth - 1, -beta, -alpha, (colour == white) ? black : white);
				if (value > bestValue)
					bestMove = move;
				alpha = max(alpha, value);
				bestValue = max(bestValue, value);
			}

			//if all best values are the same thus no moves are best do the first move in the vector
			if (bestMove.newLet + bestMove.newNum + bestMove.oldLet + bestMove.oldNum == 0)
				bestMove = childNodes[0];

			return bestMove;
		}

		int negaMax(Board node, int depth, int alpha, int beta, Colour colour)
		{
			if (depth == 0) // node is a terminal node
			{
				int nodeScore = evaluateNode(node, colour); //should be a quiescence search
				return nodeScore;
			}
			vector<Move> childNodes = moveHandler.validMoves(colour, node);
			//order nodes

			int bestValue = -1000;
			for each (Move move in childNodes)
			{
				Board board = node;
				moveHandler.movePiece(move, colour, board);
				int value = negaMax(board, depth - 1, -beta, -alpha, (colour == white) ? black : white);
				//cout << "Node evaluation of: " << value << " at depth " << depth << endl;
				bestValue = max(bestValue, value);
				alpha = max(alpha, value);
				if (alpha >= beta) {
					break;
				}
			}
			return bestValue;
		}

		int evaluateNode(Board board, Colour colour) {
			//Reinfeld values complementary to the Type enum
			// none, pawn, rook, bishop, knight, king, queen, pawnEnPassant, kingCastle, rookCastle
			int typeValue[10] = { 0, 1, 5, 3, 3, 200, 9, 1, 200, 5 };

			int total = 0;
			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
					if (board.GetSquare(numberCoord, letterCoord).type != none) {
						int value = typeValue[board.GetSquare(numberCoord, letterCoord).type];
						total += (board.GetSquare(numberCoord, letterCoord).colour == colour) ? value : -value;
					}
				}
			}
			return total;
		}
};