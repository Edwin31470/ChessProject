
#include <vector>;

#include "Board.cpp";


class Move { // information for an actual move

	public:
		int oldLet;
		int oldNum;
		int newLet;
		int newNum;
		Piece piece;

		Move()
		{

		}

		Move(int _oldLet, int _oldNum, int _newLet, int _newNum) {
			oldLet = _oldLet;
			oldNum = _oldNum;
			newLet = _newLet;
			newNum = _newNum;
		}

		Move(int _oldLet, int _oldNum, int _newLet, int _newNum, Piece _piece) {
			oldLet = _oldLet;
			oldNum = _oldNum;
			newLet = _newLet;
			newNum = _newNum;

			piece = Piece(_piece.GetType, _piece.GetType);
		}
};

class MoveHistoryItem {

	Move move;

	MoveHistoryItem(Move _move) {
		move = _move;
	}
};

class MoveHandler {

	void movePiece(Move& move, Board& board) {

		Piece pieceToTake = board.GetSquare(move.newNum , move.newLet );
		Piece pieceToMove = board.GetSquare(move.oldNum , move.oldLet);

		if (validateTaking(pieceToMove, pieceToTake) == 1
			&& validateMove(pieceToMove, pieceToTake, move) == 1)
		{
			board.SetSquare(move.newNum, move.newLet, pieceToMove);
			board.GetSquare(move.oldNum, move.oldLet).Clear();

			// if pawn becomes eligible for en passant

			// change all en passant pawns to normal pawns, except the piece that was just moved
		}

	}

	int letterToNum(string letter)
	{
		if (letter == "a") {
			return 1;
		}
		else if (letter == "b") {
			return 2;
		}
		else if (letter == "c") {
			return 3;
		}
		else if (letter == "d") {
			return 4;
		}
		else if (letter == "e") {
			return 5;
		}
		else if (letter == "f") {
			return 6;
		}
		else if (letter == "g") {
			return 7;
		}
		else if (letter == "h") {
			return 8;
		}

	}

	//return of 0 is an invalid move, 1 is normal valid, 2 is en passant move
	// 3 is castling kingside, 4 is castling queenside
	int validateMove(Piece& pieceToMove, Piece& pieceToTake, Move& move, Colour& turn, Board& board) // board is only used to check for en passant
	{
		Colour opponentColour = (turn == white) ? black : white;
		int pawnDirection = (turn == white) ? -1 : 1;
	
		//NORMAL MOVES
		if (pieceToTake.colour == opponentColour)
		{
			//for pawns
			if (pieceToMove.type == pawn)
			{
				//for movement
				if ((move.oldLet == move.newLet) && (pieceToTake.type == none))
				{
					//check for first move for white pawns
					if (opponentColour == black && move.oldNum == 1 && move.newNum == 3)
					{
						return 1;
					}
					//check for first move for black pawns
					if (opponentColour == white && move.oldNum == 6 && move.newNum == 4)
					{
						return 1;
					}
					//check for other moves
					else if ((move.oldNum + pawnDirection == move.newNum))
					{
						return 1;
					}
				}

				//check for normal piece taking
				if (move.oldNum + 1 == move.newNum)
				{
					if ((move.oldLet - 1 == move.newLet) || (move.oldLet + 1 == move.newLet)) {
						return 1;
					}
				}
			}

			//for rooks
			if (pieceToMove.type == rook)
			{
				//check if move could be valid vertically
				if (move.oldLet == move.newLet)
				{
					//check for obstacles
					int direction = (move.oldNum > move.newNum) ? 1 : -1;
					int i = move.oldNum + direction;
					while (i != move.newNum)
					{
						if (board.GetSquare(move.oldLet, i).type != none) {
							return 0;
						}
						i += direction;
						
					}
					return 1;
				} else if (move.oldNum == move.newNum) 
				{
					//check for obstacles
					int direction = (move.oldLet > move.newLet) ? 1 : -1;
					int i = move.oldLet + direction;
					while (i != move.newLet)
					{
						if (board.GetSquare(i, move.oldNum).type != none) {
							return 0;
						}
						i += direction;

					}
					return 1;
				}
			}

			//for bishops
			if (pieceToMove.type == bishop)
			{
				//if the differnce of both x and y is equal
				if (abs(move.oldLet - move.newLet) == abs(move.oldNum - move.newNum))
				{
					return 1;
				}
			}

			//for knights
			if (pieceToMove.type == "knight")
			{
				//if one axis difference is 2 and the other is 1
				if (((abs(toMoveLet - toTakeLet) == 2) && (abs(toMoveNum - toTakeNum) == 1))
					|| ((abs(toMoveLet - toTakeLet) == 1) && (abs(toMoveNum - toTakeNum) == 2)))
				{
					return 1;
				}
			}

			//for kings
			if (pieceToMove.type == "king")
			{
				if (((toMoveNum - 1 == toTakeNum) || (toMoveNum + 1 == toTakeNum))
					|| ((toMoveLet - 1 == toTakeLet) || (toMoveLet + 1 == toTakeLet)))
				{
					return 1;
				}
			}

			//for queen
			if (pieceToMove.type == "queen")
			{
				if ((toMoveNum == toTakeNum) || (toMoveLet == toTakeLet))
				{
					return 1;
				}
				else if (abs(toMoveLet - toTakeLet) == abs(toMoveNum - toTakeNum))
				{
					return 1;
				}
			}
		}

		//EN PASSANT
		if (pieceToTake.type == none && board.GetSquare(move.newLet + 1, move.newNum).type == pawnEnPassant &&
			(move.oldLet - 1 == move.newLet) || (move.oldLet + 1 == move.newLet))
		{
			return 2;
		}
		

		return 0;


	}

	vector<Move> validMoves(Piece& pieceToValidate, Colour& turn, Board& board) // return all valid moves for a given piece
	{
		Colour opponentColour = (turn == white) ? black : white;
		int pawnDirection = (turn == white) ? -1 : 1;
		vector<Move> moves;

		for (int letterCoord = 0; letterCoord < 8; letterCoord++) {
			for (int numberCoord = 0; numberCoord < 8; numberCoord++) {
				//NORMAL MOVES
				if()
			}
		}
		
		

		return moves;
	}

	//validate taking and correct turn. return of 0 means piece cannot be taken
	int validateTaking(Piece& pieceToMove, Piece& pieceToTake) {
		if (currentTurn == 0 && pieceToMove.colour == 1 && (pieceToTake.colour == 2 || pieceToTake.colour == 0))
		{
			return 1;
		}
		else if (currentTurn == 1 && pieceToMove.colour == 2 && (pieceToTake.colour == 1 || pieceToTake.colour == 0))
		{
			return 1;
		}
		else {
			return 0;
		}
};
