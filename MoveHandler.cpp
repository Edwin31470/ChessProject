
#include <vector>;

#include "Board.cpp";


class Move { // information for an actual move

	public:
		int oldNum;
		int oldLet;
		int newNum;
		int newLet;
		bool pieceTaken = false;

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

class MoveHandler {

	private:
		//set of coordiate differences to evaluate for knight
		int knightMoveset[8][2] = { { -2,-1 },{ -2,1 },{ 2,-1 },{ 2,1 },{ -1,-2 },{ 1,-2 },{ -1,2 },{ 1,2 } };

	public:
		//assumes move is already valid
		void movePiece(Move& move, Colour currentTurn, Board& board) {

			//take en passant pawns on en passant moves
			if (board.GetSquare(move.oldNum,move.oldLet).type == pawn && board.GetSquare(move.oldNum, move.newNum).type == none) {
				if (currentTurn == white && board.GetSquare(move.newNum - 1, move.newLet).type == pawnEnPassant) {
					board.ClearSquare(move.newNum - 1, move.newLet);
				}
				if (currentTurn == black && board.GetSquare(move.newNum + 1, move.newLet).type == pawnEnPassant) {
					board.ClearSquare(move.newNum + 1, move.newLet);
				}
			}

			//clear all pawns of en passant flags
			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
					if (board.GetSquare(numberCoord, letterCoord).type == pawnEnPassant)
						board.SetTypeOfPiece(numberCoord, letterCoord, pawn);
				}
			}

			//move rooks on en castling moves
			if (board.GetSquare(move.oldNum, move.oldLet).type == kingCastle && (move.newLet == 4 || move.newLet == 8)) {
				Move rookMove;
				if (move.newLet == 4) { //queenside
					rookMove = Move(move.oldNum, 2, move.newNum, 4);
				} else if (move.newLet == 8) { //queenside
					rookMove = Move(move.oldNum, 9, move.newNum, 7);
				}
				board.SetTypeOfPiece(rookMove.oldNum, rookMove.oldLet, rook); //clear moving rook of castling potential
				// do rook move
				board.SetSquare(rookMove.newNum, rookMove.newLet, board.GetSquare(rookMove.oldNum, rookMove.oldLet));
				board.ClearSquare(rookMove.oldNum, rookMove.oldLet);
			}

			//set pawns that will move two squares to en passant potential
			if (board.GetSquare(move.oldNum, move.oldLet).type == pawn && abs(move.oldNum - move.newNum) == 2) {
				board.SetTypeOfPiece(move.oldNum, move.oldLet, pawnEnPassant);
			}
			//clear moving kings and rooks of castling
			else if (board.GetSquare(move.oldNum, move.oldLet).type == kingCastle)
				board.SetTypeOfPiece(move.oldNum, move.oldLet, king);
			else if (board.GetSquare(move.oldNum, move.oldLet).type == rookCastle)
				board.SetTypeOfPiece(move.oldNum, move.oldLet, rook);

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

			//vector that the moves will be added to
			//if the move is a tactical (taking) move it will be added to the front of the array to be evaluated first
			//if the move is quiet it will be added to the end of the array
			vector<Move> moves;

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
								moves.insert(moves.begin(), Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord + 1));
							if (board.GetSquare(numberCoord + pawnDirection, letterCoord - 1).colour == opponentColour)
								moves.insert(moves.begin(), Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord - 1));
							//check for en passant taking
							if (board.GetSquare(numberCoord, letterCoord + 1).colour == opponentColour
								&& board.GetSquare(numberCoord, letterCoord + 1).type == pawnEnPassant)
								moves.insert(moves.begin(), Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord + 1));
							if (board.GetSquare(numberCoord, letterCoord - 1).colour == opponentColour
								&& board.GetSquare(numberCoord, letterCoord - 1).type == pawnEnPassant)
								moves.insert(moves.begin(), Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord - 1));
							break;
						case queen: // queen will fall through rook and bishop to combine moves
						case rookCastle: // rook with castle potential has the same moves as rook (castling is a king move)
						case rook:
							//check horizontally
							for (int direction : { 1, -1 }) {
								int H = letterCoord + direction;
								while (board.GetSquare(numberCoord, H).type == none // will immediately stop scanning when colliding with a non empty square, including out of bounds
									|| board.GetSquare(numberCoord, H).colour == opponentColour) // will stop it colliding when the piece can be taken so it can be added to list
								{
									//if piece can be taken break as further spaces are blocked and add to front rather than back
									if (board.GetSquare(numberCoord, H).colour == opponentColour) {
										moves.insert(moves.begin(), Move(numberCoord, letterCoord, numberCoord, H));
										break;
									}
									moves.push_back(Move(numberCoord, letterCoord, numberCoord, H));
									H += direction;
								}
							}
							//check vertically
							for (int direction : { 1, -1 }) {
								int V = numberCoord + direction;
								while (board.GetSquare(V, letterCoord).type == none
									|| board.GetSquare(V, letterCoord).colour == opponentColour)
								{
									//if piece can be taken break as further spaces are blocked
									if (board.GetSquare(V, letterCoord).colour == opponentColour) {
										moves.insert(moves.begin(), Move(numberCoord, letterCoord, V, letterCoord));
										break;
									}
									moves.push_back(Move(numberCoord, letterCoord, V, letterCoord));

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
										//if piece can be taken break as further spaces are blocked
										if (board.GetSquare(V, H).colour == opponentColour) {
											moves.insert(moves.begin(), (Move(numberCoord, letterCoord, V, H)));
											break;
										}
										moves.push_back(Move(numberCoord, letterCoord, V, H));

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

								if (board.GetSquare(V, H).type == none)
									moves.push_back(Move(numberCoord, letterCoord, V, H));
								else if (board.GetSquare(V, H).colour == opponentColour)
									moves.insert(moves.begin(), (Move(numberCoord, letterCoord, V, H)));
							}
							break;
						case kingCastle: //king with castle has same moves as king but with extra moves
										 //queenside castling
							if (board.GetSquare(numberCoord, 2).type == rookCastle && //rook has not moved
								board.GetSquare(numberCoord, 3).type == none && //all spaces between rook and king are empty
								board.GetSquare(numberCoord, 4).type == none &&
								board.GetSquare(numberCoord, 5).type == none)
							{
								moves.insert(moves.begin(), (Move(numberCoord, letterCoord, numberCoord, 4)));
							}
							//kingside castling
							if (board.GetSquare(numberCoord, 9).type == rookCastle && //rook has not moved
								board.GetSquare(numberCoord, 8).type == none && //all spaces between rook and king are empty
								board.GetSquare(numberCoord, 7).type == none)
							{
								moves.insert(moves.begin(), (Move(numberCoord, letterCoord, numberCoord, 8)));
							}
						case king:
							//iterate over all adjacent spaces. 0,0 is unnecessarily evaluated but will not be added as king cannot take its own colour
							for (int directionV : { -1, 0, 1}) {
								for (int directionH : { -1, 0, 1}) {
									int V = numberCoord + directionV;
									int H = letterCoord + directionH;

									if (board.GetSquare(V, H).type == none)
										moves.push_back(Move(numberCoord, letterCoord, V, H));
									else if (board.GetSquare(V, H).colour == opponentColour)
										moves.insert(moves.begin(), (Move(numberCoord, letterCoord, V, H)));
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

		vector<Move> quietMoves(Colour playerColour, Board& board) // return all quescient/quiet (non taking) moves
		{
			Colour opponentColour = (playerColour == white) ? black : white;
			int pawnDirection = (playerColour == white) ? 1 : -1;

			//vector that the moves will be added to
			//if the move is a tactical (taking) move it will be added to the front of the array to be evaluated first
			//if the move is quiet it will be added to the end of the array
			vector<Move> moves;

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
							break;
						case queen: // queen will fall through rook and bishop to combine moves
						case rookCastle: // rook with castle potential has the same moves as rook (castling is a king move)
						case rook:
							//check horizontally
							for (int direction : { 1, -1 }) {
								int H = letterCoord + direction;
								while (board.GetSquare(numberCoord, H).type == none // will immediately stop scanning when colliding with a non empty square, including out of bounds
									|| board.GetSquare(numberCoord, H).colour == opponentColour) // will stop it colliding when the piece can be taken so it can be added to list
								{
									//if piece can be taken break as further spaces are blocked and add to front rather than back
									if (board.GetSquare(numberCoord, H).colour == opponentColour) {
										break;
									}
									moves.push_back(Move(numberCoord, letterCoord, numberCoord, H));
									H += direction;
								}
							}
							//check vertically
							for (int direction : { 1, -1 }) {
								int V = numberCoord + direction;
								while (board.GetSquare(V, letterCoord).type == none
									|| board.GetSquare(V, letterCoord).colour == opponentColour)
								{
									//if piece can be taken break as further spaces are blocked
									if (board.GetSquare(V, letterCoord).colour == opponentColour) {
										break;
									}
									moves.push_back(Move(numberCoord, letterCoord, V, letterCoord));

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
										//if piece can be taken break as further spaces are blocked
										if (board.GetSquare(V, H).colour == opponentColour) {
											break;
										}
										moves.push_back(Move(numberCoord, letterCoord, V, H));

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

								if (board.GetSquare(V, H).type == none)
									moves.push_back(Move(numberCoord, letterCoord, V, H));
							}
							break;
						case kingCastle: //king with castle has same moves as king but with extra moves
										 //queenside castling
							if (board.GetSquare(numberCoord, 2).type == rookCastle && //rook has not moved
								board.GetSquare(numberCoord, 3).type == none && //all spaces between rook and king are empty
								board.GetSquare(numberCoord, 4).type == none &&
								board.GetSquare(numberCoord, 5).type == none)
							{
								moves.insert(moves.begin(), (Move(numberCoord, letterCoord, numberCoord, 4)));
							}
							//kingside castling
							if (board.GetSquare(numberCoord, 9).type == rookCastle && //rook has not moved
								board.GetSquare(numberCoord, 8).type == none && //all spaces between rook and king are empty
								board.GetSquare(numberCoord, 7).type == none)
							{
								moves.insert(moves.begin(), (Move(numberCoord, letterCoord, numberCoord, 8)));
							}
						case king:
							//iterate over all adjacent spaces. 0,0 is unnecessarily evaluated but will not be added as king cannot take its own colour
							for (int directionV : { -1, 0, 1}) {
								for (int directionH : { -1, 0, 1}) {
									int V = numberCoord + directionV;
									int H = letterCoord + directionH;

									if (board.GetSquare(V, H).type == none)
										moves.push_back(Move(numberCoord, letterCoord, V, H));
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

		//code no longer used. does not order moves into tactical moves first
		vector<Move> validMovesNoOrdering(Colour playerColour, Board& board) // return all valid moves for a given player
		{
			Colour opponentColour = (playerColour == white) ? black : white;
			int pawnDirection = (playerColour == white) ? 1 : -1;

			//vector that the moves will be added to
			vector<Move> moves;

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
							//check for en passant taking
							if (board.GetSquare(numberCoord, letterCoord + 1).colour == opponentColour
								&& board.GetSquare(numberCoord, letterCoord + 1).type == pawnEnPassant)
								moves.push_back(Move(numberCoord, letterCoord, numberCoord + pawnDirection, letterCoord + 1));
							if (board.GetSquare(numberCoord, letterCoord - 1).colour == opponentColour
								&& board.GetSquare(numberCoord, letterCoord - 1).type == pawnEnPassant)
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
							//queenside castling
							if (board.GetSquare(numberCoord,2).type == rookCastle && //rook has not moved
								board.GetSquare(numberCoord,3).type == none && //all spaces between rook and king are empty
								board.GetSquare(numberCoord,4).type == none &&
								board.GetSquare(numberCoord,5).type == none)
							{
								moves.push_back(Move(numberCoord, letterCoord, numberCoord, 4));
							}
							//kingside castling
							if (board.GetSquare(numberCoord, 9).type == rookCastle && //rook has not moved
								board.GetSquare(numberCoord, 8).type == none && //all spaces between rook and king are empty
								board.GetSquare(numberCoord, 7).type == none)
							{
								moves.push_back(Move(numberCoord, letterCoord, numberCoord, 8));
							}
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

		
		//to get a null move an own king to own king move is made
		Move getNullMove(Colour colour, Board& board) {

			for (int numberCoord = 2; numberCoord < 10; numberCoord++) {
				for (int letterCoord = 2; letterCoord < 10; letterCoord++) {
					if (board.GetSquare(numberCoord, letterCoord).type == king || board.GetSquare(numberCoord, letterCoord).type == kingCastle) {
						return Move(numberCoord, letterCoord, numberCoord, letterCoord);
					}
				}
			}
		}
};
