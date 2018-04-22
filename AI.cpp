
#include <string>;
#include <iostream>;
#include <stdlib.h>;
#include <random>;
#include <algorithm>;
#include <time.h>;
#include <chrono>;

#include "MoveHandler.cpp";

using namespace std;

class AI
{
	MoveHandler moveHandler;

	//variables for testing
	int numberOfNodesScanned = 0;
	int numberOfCutoffs = 0;

	//Values complementary to the Type enum
	// none, pawn, rook, bishop, knight, king, queen, pawnEnPassant, kingCastle, rookCastle
	int typeValue[10] = { 0, 100, 500, 330, 320, 20000, 900, 100, 20000, 500 };

	//arrays used for calculating the value of the positions of pieces
	//source: h/ttps://chessprogramming.wikispaces.com/Simplified%20evaluation%20function
	//arrays are from the perspective of white
	int positionalPawnArray[8][8] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 50, 50, 50, 50, 50, 50, 50, 50 },
		{ 10, 10, 20, 30, 30, 20, 10, 10 },
		{ 10, 10, 20, 30, 30, 20, 10, 10 },
		{ 5, 5, 10, 25, 25, 10, 5, 5 },
		{ 0, 0, 0, 20, 20, 0, 0, 0 },
		{ 5, -5, -10, 0, 0, -10, -5, 5 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	};

	int positionalKnightArray[8][8] =
	{
		{ -50, -40, -30, -30, -30, -30, -40, -50 },
		{ -40, -20, 0, 0, 0, 0, -20, -40 },
		{ -30, 0, 10, 15, 15, 10, 0, -30 },
		{ -30, 5, 15, 20, 20, 15, 5, -30 },
		{ -30, 0, 15, 20, 20, 15, 0, -30 },
		{ -30, 5, 10, 15, 15, 10, 5, -30 },
		{ -40, -20, 0, 5, 5, 0, -20, -40 },
		{ -50,-40, -10, -10, -10, -10, -40, -50 }
	};

	int positionalBishopArray[8][8] =
	{
		{ -20, -10, -10, -10, -10, -10, -10, -20 },
		{ -10, 0, 0, 0, 0, 0, 0, -10 },
		{ -10, 0, 5, 10, 10, 5, 0, -10 },
		{ -10, 5, 5, 10, 10, 5, 5, -10 },
		{ -10, 0, 10, 10, 10, 10, 10, -10 },
		{ -10, 10, 10, 10, 10, 10, 10, -10 },
		{ -10, 5, 0, 0, 0, 0, 5, -10 },
		{ -20, -10, -10, -10, -10, -10, -10, -20 },
	};

	int positionalRookArray[8][8] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0},
		{ 5, 10, 10, 10, 10, 10, 10, 5 },
		{ -5, 0, 0, 0, 0, 0, 0, -5 },
		{ -5, 0, 0, 0, 0, 0, 0, -5 },
		{ -5, 0, 0, 0, 0, 0, 0, -5 },
		{ -5, 0, 0, 0, 0, 0, 0, -5 },
		{ -5, 0, 0, 0, 0, 0, 0, -5 },
		{ 0, 0, 0, 5, 5, 0, 0, 0 },
	};

	int positionalQueenArray[8][8] =
	{
		{ -20, -10, -10, -5, -5, -10, 0, -20 },
		{ -10, 0, 0, 0, 0, 0, 0, -10 },
		{ -10, 0, 5, 5, 5, 5, 0, -10 },
		{ -5, 0, 5, 5, 5, 5, 0, -5 },
		{ 0, 0, 5, 5, 5, 5, 0, -5 },
		{ -10, 5, 5, 5, 5, 5, 0, -10 },
		{ -10, 0, 5, 0, 0, 0, 0, -10 },
		{ -20, -10, -10, -5, -5, -10, -10, -20 },
	};

	int positionalKingArray[8][8] =
	{
		{ -30, -40, -40, -50, -50, -40, -40, -30 },
		{ -30, -40, -40, -50, -50, -40, -40, -30 },
		{ -30, -40, -40, -50, -50, -40, -40, -30 },
		{ -30, -40, -40, -50, -50, -40, -40, -30 },
		{ -20, -30, -30, -40, -40, -30, -30, -20 },
		{ -10, -20, -20, -20, -20, -20, -20, -10 },
		{ 20, 20, 0, 0, 0, 0, 20, 20 },
		{ 20, 30, 10, 0, 0, 10, 30, 20 },
	};

	int positionalKingEndArray[8][8] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	};


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

			clock_t timeOfStart = clock();
			Move bestMove = negaMaxInitial(realBoard, depth, -1000000, 1000000, colour); // arbitarily high value of 1 million is used
			clock_t timeOfEnd = clock();
			
			moveHandler.movePiece(bestMove, colour, realBoard);

			cout << "AI move: " << realBoard.GetSquare(bestMove.newLet, bestMove.newNum).type << " to " << bestMove.newLet << bestMove.newNum << endl;
			cout << "Time Taken: " << (timeOfEnd - timeOfStart) / (double) CLOCKS_PER_SEC << " seconds" << endl;
			cout << "Number of nodes scanned: " << numberOfNodesScanned << endl;
			cout << "Number of cutoffs made: " << numberOfCutoffs << endl;
		}

		//initial call of negaMax. will return the first move of the best predicted move path
		Move negaMaxInitial(Board node, int depth, int alpha, int beta, Colour colour)
		{
			vector<Move> childNodes = moveHandler.validMoves(colour, node);
			numberOfNodesScanned += childNodes.size();
			//order nodes

			Move bestMove(0,0,0,0);
			int bestValue = -1000000; // arbitarily high value of 1 million is used
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
				numberOfNodesScanned += 1;
				return nodeScore;
			}
			vector<Move> childNodes = moveHandler.validMoves(colour, node);
			numberOfNodesScanned += childNodes.size();
			//order nodes

			int bestValue = -1000000; // arbitarily high value of 1 million is used
			for each (Move move in childNodes)
			{
				Board board = node;
				moveHandler.movePiece(move, colour, board);
				int value = negaMax(board, depth - 1, -beta, -alpha, (colour == white) ? black : white);
				//cout << "Node evaluation of: " << value << " at depth " << depth << endl;
				bestValue = max(bestValue, value);
				alpha = max(alpha, value);
				if (alpha >= beta) {
					numberOfCutoffs += 1;
					break;
				}
			}
			return bestValue;
		}

		
		//return a estimated value of the given board state. should always be from the perpective of the AI
		int evaluateNode(Board board, Colour colour) {

			int total = 0;
			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {

					//only calculate if the square is occupied
					if (board.GetSquare(numberCoord, letterCoord).type != none) {
						//get the type value of the piece
						int value = typeValue[board.GetSquare(numberCoord, letterCoord).type];

						//if the piece is black get the opposite side of the position array vertically
						int arrayNumberCoord = (board.GetSquare(numberCoord, letterCoord).colour == white) ? numberCoord : 7 - numberCoord;

						//get the positional value of the piece
						switch (board.GetSquare(numberCoord, letterCoord).type)
						{
						case pawnEnPassant:
						case pawn:
							value += positionalPawnArray[arrayNumberCoord - 2][letterCoord - 2];
							break;
						case knight:
							value += positionalKnightArray[arrayNumberCoord - 2][letterCoord - 2];
							break;
						case bishop:
							value += positionalBishopArray[arrayNumberCoord - 2][letterCoord - 2];
							break;
						case rookCastle:
						case rook:
							value += positionalRookArray[arrayNumberCoord - 2][letterCoord - 2];
							break;
						case queen:
							value += positionalQueenArray[arrayNumberCoord - 2][letterCoord - 2];
							break;
						case kingCastle:
						case king:
							value += positionalKingArray[arrayNumberCoord - 2][letterCoord - 2];
							break;
						default:
							break;
						}

						//invert the value if the colour is the opponent
						value = (board.GetSquare(numberCoord, letterCoord).colour == colour) ? value : -value;

						total += value;
					}
				}
			}

			return total;
		}
};