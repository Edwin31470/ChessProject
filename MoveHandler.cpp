
#include <vector>;

#include "Board.cpp";


class Move { // information for an actual move

	public:
		int oldLet;
		int oldNum;
		int newLet;
		int newNum;

		Move()
		{

		}

		Move(int _oldLet, int _oldNum, int _newLet, int _newNum) {
			oldLet = _oldLet;
			oldNum = _oldNum;
			newLet = _newLet;
			newNum = _newNum;
		}
};

class MoveHistoryItem {

	Move move;
	Piece piece;

	MoveHistoryItem(Move _move) {
		move = _move;
	}
};

class MoveHandler {

	public:
		//assumes move is already valid
		void movePiece(Move& move, Board& board) {

			Piece pieceToTake = board.GetSquare(move.newNum , move.newLet );
			Piece pieceToMove = board.GetSquare(move.oldNum , move.oldLet);

			//generic move
			board.SetSquare(move.newNum, move.newLet, pieceToMove);
			board.GetSquare(move.oldNum, move.oldLet).Clear();

		}



		vector<Move> validMoves(Colour& playerColour, Board& board) // return all valid moves for a given player
		{
			Colour opponentColour = (playerColour == white) ? black : white;
			int pawnDirection = (playerColour == white) ? -1 : 1;
			vector<Move> moves;

			//set of coordiate differences to evaluate for knight
			int knightMoveset[8][2] = { { -2,-1 },{ -2,1 },{ 2,-1 },{ 2,1 },{ -1,-2 },{ 1,-2 },{ -1,2 },{ 1,2 } };

			for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
				for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
					//NORMAL MOVES
					if (board.GetSquare(letterCoord, numberCoord).colour == playerColour)
					{
						switch (board.GetSquare(letterCoord, numberCoord).type)
						{
						case pawn:
							//for normal movement
							if ((board.GetSquare(letterCoord + pawnDirection, numberCoord).type == none))
								moves.push_back(Move(letterCoord, numberCoord, letterCoord + pawnDirection, numberCoord));
							//check for first move for black pawns
							if (playerColour == black && numberCoord == 6 && board.GetSquare(letterCoord, 4).type == none);
								moves.push_back(Move(letterCoord, numberCoord, letterCoord, 4));
							//check for first move for white pawns
							if (playerColour == white && numberCoord == 1 && board.GetSquare(letterCoord, 3).type == none);
								moves.push_back(Move(letterCoord, numberCoord, letterCoord, 3));
							//check for normal piece taking
							if (board.GetSquare(letterCoord + 1, numberCoord + pawnDirection).colour == opponentColour)
								moves.push_back(Move(letterCoord, numberCoord, letterCoord + 1, numberCoord + pawnDirection));
							if (board.GetSquare(letterCoord - 1, numberCoord + pawnDirection).colour == opponentColour) 
								moves.push_back(Move(letterCoord, numberCoord, letterCoord - 1, numberCoord + pawnDirection));
							break;
						case queen: // queen will fall through rook and bishop to combine moves
						case rookCastle: // rook with castle potential has the same moves as rook (castling is a king move)
						case rook:
							//check vertically
							for (int direction : { 1, -1 }){
								int i = numberCoord + direction;
								while (board.GetSquare(letterCoord, i).type == none // will immediately stop scanning when colliding with a non empty square, including out of bounds
									|| board.GetSquare(letterCoord, i).colour == opponentColour) // will stop it colliding when the piece can be taken so it can be added to list
								{
									moves.push_back(Move(letterCoord, numberCoord, letterCoord, i));
									//if piece can be taken break as further spaces are blocked
									if (board.GetSquare(letterCoord, i).colour == opponentColour) {
										break;
									}
									i += direction;
								}
							}
							//check horizontally
							for (int direction : { 1, -1 })
							{
								int i = letterCoord + direction;
								while (board.GetSquare(i, numberCoord).type == none
									&& board.GetSquare(i, numberCoord).colour == opponentColour)
								{
									moves.push_back(Move(letterCoord, numberCoord, i, numberCoord));
									//if piece can be taken break as further spaces are blocked
									if (board.GetSquare(i, numberCoord).colour == opponentColour) {
										break;
									}
									i += direction;
								}
							}
							if (board.GetSquare(letterCoord, numberCoord).type != queen)
								//if evaluating for queen do not break and fall through to bishop moves
								break;				
						case bishop:
							//do horizontal both directions
							for (int directionH : { 1, -1 }) {
								//do vertical both directions while in vertical
								for (int directionV : { 1, -1 }) {
									int H = letterCoord + directionH;
									int V = numberCoord + directionV;

									while (board.GetSquare(H, V).type == none
										&& board.GetSquare(H, V).colour == opponentColour)
									{
										moves.push_back(Move(letterCoord, numberCoord, H, V));
										//if piece can be taken break as further spaces are blocked
										if (board.GetSquare(H, V).colour == opponentColour) {
											break;
										}
										H += directionH;
										V += directionV;
									}
								}
							}
							break;
						case knight:
							

							for (int i = 0; i < 8; i++) {
								int H = letterCoord + knightMoveset[i][0];
								int V = numberCoord + knightMoveset[i][1];

								if (board.GetSquare(H, V).colour == opponentColour || board.GetSquare(H, V).type == none)
									moves.push_back(Move(letterCoord, numberCoord, H, V));
							}
							break;
						case king:
							//iterate over all adjacent spaces. 0,0 is unnecessarily evaluated but will not be added as king cannot take its own colour
							for (int directionH = -1; directionH <= 1; directionH++) {
								for (int directionV = -1; directionV <= 1; directionV++) {
									int H = letterCoord + directionH;
									int V = numberCoord + directionV;

									if (board.GetSquare(H, V).colour == opponentColour || board.GetSquare(H, V).type == none) {
										moves.push_back(Move(letterCoord, numberCoord, H, V));
									}
								}
							}
							break;
						default:
							break;
						}
					}
				}
			}
		
			return moves;
		}
};
