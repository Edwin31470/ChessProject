
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
	private:
		MoveHandler moveHandler;

		int maxDepth;

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


	public:
		AI() {

		}

		void AIMove(Colour colour, Board& realBoard, int depth) {

			clock_t timeOfStart = clock();
			//Move bestMove = negaMaxInitial(realBoard, depth, colour);
			Move bestMove = negaMaxAlphaBetaInitial(realBoard, depth, INT_MIN, INT_MAX, colour); // arbitarily high value of 1 million is used as initial alpha and beta
			clock_t timeOfEnd = clock();
			
			moveHandler.movePiece(bestMove, colour, realBoard);

			cout << "AI move: " << realBoard.GetSquare(bestMove.newLet, bestMove.newNum).type << " to " << bestMove.newLet << bestMove.newNum << endl;
			cout << "Time Taken: " << (timeOfEnd - timeOfStart) / (double) CLOCKS_PER_SEC << " seconds" << endl;
			cout << "Number of nodes scanned: " << numberOfNodesScanned << endl;
			cout << "Number of cutoffs made: " << numberOfCutoffs << endl;
		}


		//ALPHA BETA
		//initial call of negaMax. will return the first move of the best predicted move path
		Move negaMaxInitial(Board startNode, int depth, Colour colour)
		{
			vector<Move> childNodes = moveHandler.validMoves(colour, startNode);

			//if all best values are the same thus no moves are best do the first move in the vector
			Move bestMove = childNodes[0];

			int bestValue = INT_MIN; // arbitarily high value of 1 million is used
			for each (Move move in childNodes)
			{
				Board board = startNode;
				moveHandler.movePiece(move, colour, board);
				int value = negaMax(board, depth - 1, (colour == white) ? black : white);
				if (value > bestValue)
					bestMove = move;
				bestValue = max(bestValue, value);
				numberOfNodesScanned += 1;
			}

			return bestMove;
		}

		int negaMax(Board node, int depth, Colour colour)
		{
			int bestValue = INT_MIN; // arbitarily high value of 1000 million is used

			if (depth == 0) // node is a terminal node
			{
				numberOfNodesScanned += 1;
				return evaluateNode(node, colour); //should be a quiescence search
			}
			vector<Move> childNodes = moveHandler.validMoves(colour, node);

			for each (Move move in childNodes)
			{
				Board board = node;
				moveHandler.movePiece(move, colour, board);
				int value = negaMax(board, depth - 1, (colour == white) ? black : white);
				bestValue = max(bestValue, value);
				numberOfNodesScanned += 1;
			}
			return bestValue;
		}





		//ALPHA BETA
		//initial call of negaMax. will return the first move of the best predicted move path
		Move negaMaxAlphaBetaInitial(Board startNode, int depth, int alpha, int beta, Colour colour)
		{
			vector<Move> childNodes = moveHandler.validMoves(colour, startNode);

			//if all best values are the same thus no moves are best do the first move in the vector
			Move bestMove = childNodes[0];

			int bestValue = INT_MIN; // arbitarily high value of 1 million is used
			for each (Move move in childNodes)
			{
				Board board = startNode;
				moveHandler.movePiece(move, colour, board);
				int value = negaMaxAlphaBeta(board, depth - 1, -beta, -alpha, (colour == white) ? black : white);
				if (value > bestValue)
					bestMove = move;
				alpha = max(alpha, value);
				bestValue = max(bestValue, value);
				numberOfNodesScanned += 1;
			}

			return bestMove;
		}

		int negaMaxAlphaBeta(Board node, int depth, int alpha, int beta, Colour colour)
		{
			int bestValue = INT_MIN; // arbitarily high value of 100 million is used
			vector<Move> childNodes;

			if (depth == 0) // node is a terminal node
			{
				numberOfNodesScanned += 1;
				return evaluateNode(node, colour); //should be a quiescence search
			}
			else if (depth == 1) { // if depth is 1 (so next node will be evaluation) only get quescient moves
				childNodes = moveHandler.quietMoves(colour, node);
			}
			else {
				childNodes = moveHandler.validMoves(colour, node);
			}
			

			for each (Move move in childNodes)
			{
				Board board = node;
				moveHandler.movePiece(move, colour, board);
				int value = negaMaxAlphaBeta(board, depth - 1, -beta, -alpha, (colour == white) ? black : white);
				bestValue = max(bestValue, value);
				alpha = max(alpha, value);

				if (alpha >= beta) {
					numberOfCutoffs += 1;
					break;
				}
				else {
					numberOfNodesScanned += 1;
				}
			}
			return bestValue;
		}

		//NULL MOVE ALPHA BETA (Is not working)
		//null move is not working
		int negaMaxAlphaBetaNull(Board node, int depth, int alpha, int beta, Colour colour, bool nullMove)
		{
			int bestValue = INT_MIN; // arbitarily high value of 100 million is used

			if (depth == 0) // node is a terminal node
			{
				numberOfNodesScanned += 1;
				return evaluateNode(node, colour); //should be a quiescence search
			}
			if (nullMove) { //if doing a null move
				bestValue = negaMaxMakeNullMove(node, -beta, -beta + 1, (colour == white) ? black : white);

				if (bestValue >= beta) {
					numberOfCutoffs += 1;
					return beta;
				}
				nullMove = false;
			}
			else if (depth > 3) {
				nullMove = true;
			}
			vector<Move> childNodes = moveHandler.validMoves(colour, node);

			for each (Move move in childNodes)
			{
				Board board = node;
				moveHandler.movePiece(move, colour, board);
				int value = negaMaxAlphaBetaNull(board, depth - 1, -beta, -alpha, (colour == white) ? black : white, nullMove);
				bestValue = max(bestValue, value);
				alpha = max(alpha, value);

				if (alpha > beta) {
					numberOfCutoffs += 1;
					break;
				}
				else {
					numberOfNodesScanned += 1;
				}
			}
			return bestValue;
		}

		//null move is not working
		int negaMaxMakeNullMove(Board node, int alpha, int beta, Colour colour)
		{
			int bestValue = INT_MIN; // arbitarily high value of 100 million is used

			Move nullMove = moveHandler.getNullMove(colour, node);

			moveHandler.movePiece(nullMove, colour, node);
			int value = negaMaxAlphaBetaNull(node, 2, -beta, -alpha, (colour == white) ? black : white, false);
			bestValue = max(bestValue, value);
			alpha = max(alpha, value);

			if (alpha > beta) {
				numberOfCutoffs += 1;
				return value;
			}
			else {
				numberOfNodesScanned += 1;
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