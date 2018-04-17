
#include <vector>;

#include "Board.cpp";


class Move { // information for an actual move

	public:
		int oldNum;
		int oldLet;
		int newNum;
		int newLet;

		Move()
		{

		}

		Move(int _oldNum, int _oldLet, int _newNum, int _newLet) {
			oldNum = _oldNum;
			oldLet = _oldLet;
			newNum = _newNum;
			newLet = _newLet;
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
		void movePiece(Move& move, Colour currentTurn, Board& board) {

			//clear en passant pawns on en passant moves
			if (board.GetSquare(move.oldNum,move.oldLet).type == pawn && board.GetSquare(move.oldNum, move.newNum).type == none) {
				if (currentTurn == white && board.GetSquare(move.newNum - 1, move.newLet).type == pawnEnPassant)
					board.ClearSquare(move.newNum - 1, move.newLet);
				if (currentTurn == black && board.GetSquare(move.newNum + 1, move.newLet).type == pawnEnPassant)
					board.ClearSquare(move.newNum + 1, move.newLet);
			}

			//clear all pawns of en passant flags
			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
					if (board.GetSquare(numberCoord, letterCoord).type == pawnEnPassant)
						board.SetSquare(numberCoord, letterCoord, Piece(pawn, board.GetSquare(numberCoord, letterCoord).colour));
				}
			}

			//set pawns that will move two squares to en passant potential
			if (board.GetSquare(move.oldNum, move.oldLet).type == pawn && abs(move.oldNum - move.newNum) == 2) {
				board.SetSquare(move.oldNum, move.oldLet, Piece(pawnEnPassant, currentTurn));
			}

			//generic move
			board.SetSquare(move.newNum, move.newLet, board.GetSquare(move.oldNum, move.oldLet));
			board.ClearSquare(move.oldNum, move.oldLet);

			//promotes pawns on the back row to queens
			int numberCoord = (currentTurn == white) ? 9 : 2;
			for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
				if (board.GetSquare(numberCoord, letterCoord).type == pawn) {
					board.SetSquare(numberCoord, letterCoord, Piece(queen, currentTurn));
				}
			}
		}

		vector<Move> validMoves(Colour playerColour, Board& board) // return all valid moves for a given player
		{
			Colour opponentColour = (playerColour == white) ? black : white;
			int pawnDirection = (playerColour == white) ? 1 : -1;
			vector<Move> moves;

			//set of coordiate differences to evaluate for knight
			int knightMoveset[8][2] = { { -2,-1 },{ -2,1 },{ 2,-1 },{ 2,1 },{ -1,-2 },{ 1,-2 },{ -1,2 },{ 1,2 } };

			//iterate over every non-padding square of the board
			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
					//only evaluate squares that contain the player's piece
					if (board.GetSquare(numberCoord, letterCoord).colour == playerColour)
					{
						switch (board.GetSquare(numberCoord, letterCoord).type)
						{
						case pawnEnPassant: //pawn with en passant potential has the same moves as pawn
						case pawn:
							//for normal movement
							if ((board.GetSquare(numberCoord + pawnDirection, letterCoord).type == none))
								moves.push_back(Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord));
							//check for first move for white pawns
							if (playerColour == white && numberCoord == 3 && board.GetSquare(5, letterCoord).type == none)
								moves.push_back(Move(numberCoord, letterCoord, 5, letterCoord));
							//check for first move for white pawns
							if (playerColour == black && numberCoord == 8 && board.GetSquare(6, letterCoord).type == none)
								moves.push_back(Move(numberCoord, letterCoord, 6, letterCoord));
							//check for normal piece taking
							if (board.GetSquare(numberCoord + pawnDirection, letterCoord + 1).colour == opponentColour)
								moves.push_back(Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord + 1));
							if (board.GetSquare(numberCoord + pawnDirection, letterCoord - 1).colour == opponentColour)
								moves.push_back(Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord - 1));
							break;
						case queen: // queen will fall through rook and bishop to combine moves
						case rookCastle: // rook with castle potential has the same moves as rook (castling is a king move)
						case rook:
							//check horizontally
							for (int direction : { 1, -1 }){
								int H = letterCoord + direction;
								while (board.GetSquare(numberCoord, H).type == none // will immediately stop scanning when colliding with a non empty square, including out of bounds
									|| board.GetSquare(numberCoord, H).colour == opponentColour) // will stop it colliding when the piece can be taken so it can be added to list
								{
									moves.push_back(Move(numberCoord, letterCoord, numberCoord, H));
									//if piece can be taken break as further spaces are blocked
									if (board.GetSquare(numberCoord, H).colour == opponentColour) {
										break;
									}
									H += direction;
								}
							}
							//check vertically
							for (int direction : { 1, -1 }){
								int V = numberCoord + direction;
								while (board.GetSquare(V, letterCoord).type == none
									|| board.GetSquare(V, letterCoord).colour == opponentColour)
								{
									moves.push_back(Move(numberCoord, letterCoord, V, letterCoord));
									//if piece can be taken break as further spaces are blocked
									if (board.GetSquare(V, letterCoord).colour == opponentColour) {
										break;
									}
									V += direction;
								}
							}
							if (board.GetSquare(numberCoord, letterCoord).type != queen)
								//if evaluating for queen do not break and fall through to bishop moves
								break;				
						case bishop:
							//do vertical both directions
							for (int directionV : { 1, -1 }) {
								//do horizontal both directions while in vertical
								for (int directionH : { 1, -1 }) {
									int V = numberCoord + directionV;
									int H = letterCoord + directionH;

									while (board.GetSquare(V, H).type == none
										|| board.GetSquare(V, H).colour == opponentColour)
									{
										moves.push_back(Move(numberCoord, letterCoord, V, H));
										//if piece can be taken break as further spaces are blocked
										if (board.GetSquare(V, H).colour == opponentColour) {
											break;
										}
										V += directionV;
										H += directionH;
									}
								}
							}
							break;
						case knight:
							for (int i = 0; i < 8; i++) {
								int V = numberCoord + knightMoveset[i][0];
								int H = letterCoord + knightMoveset[i][1];

								if (board.GetSquare(V, H).colour == opponentColour || board.GetSquare(V, H).type == none)
									moves.push_back(Move(numberCoord, letterCoord, V, H));
							}
							break;
						case kingCastle: //king with castle has same moves as king but with extra moves
						case king:
							//iterate over all adjacent spaces. 0,0 is unnecessarily evaluated but will not be added as king cannot take its own colour
							for (int directionV : { -1, 0, 1}) {
								for (int directionH : { -1, 0, 1}) {
									int V = numberCoord + directionV;
									int H = letterCoord + directionH;

									if (board.GetSquare(V, H).colour == opponentColour || board.GetSquare(V, H).type == none) {
										moves.push_back(Move(numberCoord, letterCoord, V, H));
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
