#include "ChessMoves.h"

struct Move moveConstructor(struct ChessBoard *board, struct BoardCoordinate start, struct BoardCoordinate end)
{
	struct Move move;

	move.start = start;
	move.end = end;

	move.piece = getPieceAtCoordinate(board, start);
	move.captured_piece = getPieceAtCoordinate(board, end);

	return move;
}

char isSpaceAttacked(struct ChessBoard *board, char x, char y)
{
	struct MoveCoordinateList moves;

	char white_piece = isPieceWhiteXY(board, x, y);

	int i;
	int j;
	int n;

	int pieces_checked = 0;

	// Iterate through all opponent pieces and get potential moves for each piece
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			// If opponent colour's piece at location
			if (isPieceAtXY(board, i, j) && isPieceWhiteXY(board, i, j) != white_piece)
			{
				// Find potential moves for that piece
				getPieceCaptureMovesXY(board, i, j, &moves);
				// Check if the space is in the given MoveCoordinateList
				for (n = 0; n < moves.move_count; n++)
				{
					if (moves.moves[n].x == x && moves.moves[n].y == y)
					{
						return 1;
					}
				}

				pieces_checked++;
			}

			
			// If all the opponent's pieces have been checked, end
			if ((white_piece && pieces_checked == board->black_piece_count) || (!white_piece && pieces_checked == board->white_piece_count))
			{
				return 0;
			}
			
		}
	}

	return 0;
}

void makeMove(struct ChessBoard *board, struct Move move)
{
	int en_passant_bitshift = 0;

	board->board[move.start.x][move.start.y] = BLANK;
	board->board[move.end.x][move.end.y] = move.piece;

	// Update number of pieces for each colour
	if (board->white_turn && move.captured_piece != BLANK)
	{
		board->black_piece_count--;
	}
	else if (!board->white_turn && move.captured_piece != BLANK)
	{
		board->white_piece_count--;
	}

	// Check for castling moves
	// Kingside castling
	if ((move.piece == WKING || move.piece == BKING) && (move.end.x - move.start.x) == 2)
	{
		board->board[move.end.x - 1][move.start.y] = board->board[7][move.start.y]; // Move rook next to king
		board->board[7][move.start.y] = BLANK;
		board->fifty_move_counter = 0;
	}
	// Queenside castling
	else if ((move.piece == WKING || move.piece == BKING) && (move.start.x - move.end.x) == 2)
	{
		board->board[move.end.x + 1][move.start.y] = board->board[0][move.start.y]; // Move rook next to king
		board->board[0][move.start.y] = BLANK;
		board->fifty_move_counter = 0;
	}

	// Update castling logic

	// Kings have moved
	if (move.piece == WKING)
	{
		board->king_info = (board->king_info & 0xF8) | 0x07;
	}
	else if (move.piece == BKING)
	{
		board->king_info = (board->king_info & 0x8F) | 0x70;
	}

	// Rooks have been moved or taken
	if ((move.start.x == 0 && move.start.y == 0) || (move.end.x == 0 && move.end.y == 0))
	{
		board->king_info = (board->king_info & 0xFB) | 0x03;
	}
	if ((move.start.x == 7 && move.start.y == 0) || (move.end.x == 7 && move.end.y == 0))
	{
		board->king_info = (board->king_info & 0xFD) | 0x02;
	}
	if ((move.start.x == 0 && move.start.y == 7) || (move.end.x == 0 && move.end.y == 7))
	{
		board->king_info = (board->king_info & 0xBF) | 0x30;
	}
	if ((move.start.x == 7 && move.start.y == 7) || (move.end.x == 7 && move.end.y == 7))
	{
		board->king_info = (board->king_info & 0xDF) | 0x20;
	}

	// Check for en passant capture
	if (board->white_turn)
	{
		en_passant_bitshift = 4;
	}

	if ((board->en_passant & (0x8 << en_passant_bitshift)) && ((move.piece == WPAWN && move.start.y == 4 && move.end.y == 5) || (move.piece == BPAWN && move.start.y == 3 && move.end.y == 2)) && move.end.x == (board->en_passant & (0x07 << en_passant_bitshift)) >> en_passant_bitshift)
	{
		// Remove en passant captured piece
		board->board[move.end.x][move.start.y] = BLANK;

		if (board->white_turn)
		{
			board->black_piece_count--;
		}
		else
		{
			board->white_piece_count--;
		}
	}

	// Reset en passant checks
	board->en_passant = 0;

	// Update en passant checking
	if (move.piece == WPAWN && move.start.y == 1 && move.end.y == 3)
	{
		board->en_passant = (0x8 + move.start.x);
	}
	else if (move.piece == BPAWN && move.start.y == 6 && move.end.y == 4)
	{
		board->en_passant = (0x8 + move.start.x) << 4;
	}

	// Pawn Promotion

	// Reset pawn promotion
	board->promotion = 0;

	// White pawn promotion
	if (move.piece == WPAWN && move.end.y == 7)
	{
		board->promotion = 0x08 + move.end.x;
	}
	// Black pawn promotion
	else if (move.piece == BPAWN && move.end.y == 0)
	{
		board->promotion = (0x08 + move.end.x) << 4;
	}

	// Update 50 move rule logic
	// If a capture has been made, a king has castled or a pawn has moved, reset the counter to 0
	// Note: Counter resetting from castling is done in the castling logic
	if (move.piece == WPAWN || move.piece == BPAWN || move.captured_piece != BLANK)
	{
		board->fifty_move_counter = 0;
	}
	else 
	{
		board->fifty_move_counter += 1;
	}
	// Else increment by 1

	// Check for check
	updateKingChecks(board);

	// Switch player turn
	board->white_turn ^= 1;
}

void promotePawn(struct ChessBoard *board, int promotion_piece)
{
	char pawn_column;

	// White promotion
	if (board->promotion & 0x08)
	{
		pawn_column = board->promotion & 0x07;

		switch (promotion_piece)
		{
		case 1 :
			board->board[pawn_column][7] = WQUEEN;
			break;
		case 2:
			board->board[pawn_column][7] = WKNIGHT;
			break;
		case 3:
			board->board[pawn_column][7] = WROOK;
			break;
		case 4:
			board->board[pawn_column][7] = WBISHOP;
			break;
		default:
			board->board[pawn_column][7] = ERROR;
			break;
		}
	}
	// Black promotion
	else if (board->promotion & 0x80)
	{
		pawn_column = (board->promotion & 0x70) >> 4;

		switch (promotion_piece)
		{
		case 1:
			board->board[pawn_column][0] = BQUEEN;
			break;
		case 2:
			board->board[pawn_column][0] = BKNIGHT;
			break;
		case 3:
			board->board[pawn_column][0] = BROOK;
			break;
		case 4:
			board->board[pawn_column][0] = BBISHOP;
			break;
		default:
			board->board[pawn_column][0] = ERROR;
			break;
		}
	}

	// Check for check
	updateKingChecks(board);
}

void endGameCheck(struct ChessBoard *board)
{
	char can_white_move = canWhiteMove(board);
	char can_black_move = canBlackMove(board);

	// Check for checkmate
	// White checkmated
	if ((board->king_info & 0x08) && !can_white_move)
	{
	board->end_game = 0x01;
	}
	// Black checkmated
	else if ((board->king_info & 0x80) && !can_black_move)
	{
	board->end_game = 0x02;
	}
	// Stalemate
	else if ((!can_white_move && board->white_turn) || (!can_black_move && !board->white_turn))
	{
		board->end_game = 0x04;
	}
	// Fifty move rule
	else if (board->fifty_move_counter >= 100)
	{
		board->end_game = 0x10;
	}
	// Insufficient material check
	else if (insufficientMaterialCheck(board))
	{
		board->end_game = 0x08;
	}
}

char insufficientMaterialCheck(struct ChessBoard *board)
{
	struct PieceList white_piece_list;
	struct PieceList black_piece_list;
	char white_bishop_w_count = 0;
	char white_bishop_b_count = 0;
	char black_bishop_w_count = 0;
	char black_bishop_b_count = 0;


	int n;

	// King vs King
	if (board->white_piece_count == 1 && board->white_piece_count == 1)
	{
		return 1;
	}
	// If both players have 2 pieces or less (king + something)
	else if (board->white_piece_count < 3 && board->black_piece_count < 3)
	{
		white_piece_list = getWhitePieces(board);
		black_piece_list = getBlackPieces(board);

		for (n = 0; n < board->white_piece_count; n++)
		{
			// If white player has at least 1 rook, pawn or queen, checkmate can be made
			if (white_piece_list.pieces[n] == WROOK || white_piece_list.pieces[n] == WQUEEN || white_piece_list.pieces[n] == WPAWN)
			{
				return 0;
			}
			else if (white_piece_list.pieces[n] == WBISHOP_W)
			{
				white_bishop_w_count++;
			}
			else if (white_piece_list.pieces[n] == WBISHOP_B)
			{
				white_bishop_b_count++;
			}
		}
		for (n = 0; n < board->black_piece_count; n++)
		{
			// If black player has at least 1 rook, pawn or queen, checkmate can be made
			if (black_piece_list.pieces[n] == BROOK || black_piece_list.pieces[n] == BQUEEN || black_piece_list.pieces[n] == BPAWN)
			{
				return 0;
			}
			else if (black_piece_list.pieces[n] == BBISHOP_W)
			{
				black_bishop_w_count++;
			}
			else if (black_piece_list.pieces[n] == BBISHOP_B)
			{
				black_bishop_b_count++;
			}
		}

		// If either side only has a king (and due to previous checks, the other side has only a single knight or bishop)
		// Or the bishops on each side are of the same colour only
		if (board->white_piece_count == 1 || board->black_piece_count == 1 || (white_bishop_w_count == black_bishop_w_count && white_bishop_b_count == black_bishop_b_count))
		{
			return 1;
		}
	}
	// Check if both sides only have a king and only bishops of the same colour (incredibly unlikely, but it is possible to have 9 same coloured bishops due to promotions)
	else if (board->white_piece_count < 11 && board->black_piece_count < 11)
	{
		white_piece_list = getWhitePieces(board);
		black_piece_list = getBlackPieces(board);

		for (n = 0; n < board->white_piece_count; n++)
		{
			// If white player has at least 1 rook, pawn, knight or queen, checkmate can be made
			if (white_piece_list.pieces[n] == WROOK || white_piece_list.pieces[n] == WQUEEN || white_piece_list.pieces[n] == WKNIGHT || white_piece_list.pieces[n] == WPAWN)
			{
				return 0;
			}
			else if (white_piece_list.pieces[n] == WBISHOP_W)
			{
				white_bishop_w_count++;
			}
			else if (white_piece_list.pieces[n] == WBISHOP_B)
			{
				white_bishop_b_count++;
			}
		}
		for (n = 0; n < board->black_piece_count; n++)
		{
			// If black player has at least 1 rook, pawn, knight or queen, checkmate can be made
			if (black_piece_list.pieces[n] == BROOK || black_piece_list.pieces[n] == BQUEEN || black_piece_list.pieces[n] == BKNIGHT || black_piece_list.pieces[n] == BPAWN)
			{
				return 0;
			}
			else if (black_piece_list.pieces[n] == BBISHOP_W)
			{
				black_bishop_w_count++;
			}
			else if (black_piece_list.pieces[n] == BBISHOP_B)
			{
				black_bishop_b_count++;
			}
		}

		// Check if both sides contain only the same coloured bishops
		if ((white_bishop_w_count == 0 && black_bishop_w_count == 0) || (white_bishop_b_count == 0 && black_bishop_b_count == 0))
		{
			return 1;
		}
	}

	return 0;

}

void updateKingChecks(struct ChessBoard *board)
{
	int i;
	int j;

	char out = 0x00;

	// Get king's location
	// Check if space is attacked
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board->board[i][j] == WKING && isSpaceAttacked(board, i, j))
			{
				out |= 0x08;
			}
			else if (board->board[i][j] == BKING && isSpaceAttacked(board, i, j))
			{
				out |= 0x80;
			}
		}
	}

	// Update king check status
	board->king_info = (board->king_info & 0x77) | out;
}

char canWhiteMove(struct ChessBoard *board)
{
	struct MoveCoordinateList moveList;
	int i;
	int j;

	// Check all possible white pieces for moves
	// If no pieces can move, white is checkmated

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board->board[i][j] != BLANK && isPieceWhiteXY(board, i, j))
			{
				getPieceMovesXY(board, 1, i, j, &moveList);

				if (moveList.move_count > 0)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

char canBlackMove(struct ChessBoard *board)
{
	struct MoveCoordinateList moveList;
	int i;
	int j;

	// Check all possible black pieces for moves
	// If no pieces can move, black is checkmated

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board->board[i][j] != BLANK && !isPieceWhiteXY(board, i, j))
			{
				getPieceMovesXY(board, 0, i, j, &moveList);

				if (moveList.move_count > 0)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

// Checks if a regular move, which can capture an opponent's piece, is valid
// Returns 1 when the given position is BLANK or an opponent's piece
char moveCheck(struct ChessBoard *board, char white_move, char x, char y)
{
	if (x > 7 || x < 0 || y > 7 || y < 0)
	{
		return 0;
	}

	if (board->board[x][y] == BLANK || isPieceWhite(board, BoardCoordinateConstructor(x, y)) != white_move)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char isMoveInMoveList(struct MoveCoordinateList moveList, struct BoardCoordinate move)
{
	int n;
	for (n = 0; n < moveList.move_count; n++)
	{
		if (moveList.moves[n].x == move.x && moveList.moves[n].y == move.y)
		{
			return 1;
		}
	}

	return 0;
}

void purgeIllegalMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *moveList)
{
	int n;
	int index = 0;
	int king_bitshift = 0;
	struct Move test_move;
	struct ChessBoard temp_board;
	struct MoveCoordinateList legalMoveList;

	if (!board->white_turn)
	{
		king_bitshift = 4;
	}

	test_move.start = start;
	test_move.piece = board->board[start.x][start.y];

	for (n = 0; n < moveList->move_count; n++)
	{
		temp_board = *board;

		// Set end coordinate of move
		test_move.end = moveList->moves[n];
		test_move.captured_piece = getPieceAtCoordinate(board, moveList->moves[n]);

		// Test move on temp board
		makeMove(&temp_board, test_move);

		// If own coloured king not in check, add move to legal movel list
		if ((temp_board.king_info & (0x08 << king_bitshift)) == 0)
		{
			legalMoveList.moves[index] = moveList->moves[n];
			index++;
		}
	}

	legalMoveList.move_count = index;

	*moveList = legalMoveList;
}

void getCurrentTurnPieceMoves(struct ChessBoard *board, struct BoardCoordinate piece_coordinates, struct MoveCoordinateList *move_list)
{
	getPieceMoves(board, board->white_turn, piece_coordinates, move_list);
}

void getPieceMovesXY(struct ChessBoard *board, char white_piece, char x, char y, struct MoveCoordinateList *move_list)
{
	getPieceMoves(board, white_piece, BoardCoordinateConstructor(x, y), move_list);
}

void getPieceMoves(struct ChessBoard *board, char white_piece, struct BoardCoordinate piece_coordinates, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;
	char piece = board->board[piece_coordinates.x][piece_coordinates.y];
	char ascii_conversion = 0;

	if (!white_piece)
	{
		ascii_conversion = 32;
	}


	if (piece == WPAWN + ascii_conversion)
	{
		getPawnMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WKNIGHT + ascii_conversion)
	{
		getKnightMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WBISHOP + ascii_conversion)
	{
		getDiagonalMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WROOK + ascii_conversion)
	{
		getStraightMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WQUEEN + ascii_conversion)
	{
		getQueenMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WKING + ascii_conversion)
	{
		getKingMoves(board, piece_coordinates, &moves);
	}

	purgeIllegalMoves(board, piece_coordinates, &moves);

	*move_list = moves;

}

void getPieceCaptureMovesXY(struct ChessBoard *board, char x, char y, struct MoveCoordinateList *move_list)
{
	getPieceCaptureMoves(board, BoardCoordinateConstructor(x, y), move_list);
}

void getPieceCaptureMoves(struct ChessBoard *board, struct BoardCoordinate piece_coordinates, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;
	char piece = board->board[piece_coordinates.x][piece_coordinates.y];

	// Change the turn over temporarily, as we are checking the opposing colour's moves
	//board->white_turn ^= 1;

	if (piece == WPAWN || piece == BPAWN)
	{
		getPawnThreatMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WKNIGHT || piece == BKNIGHT)
	{
		getKnightMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WBISHOP || piece == BBISHOP)
	{
		getDiagonalMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WROOK || piece == BROOK)
	{
		getStraightMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WQUEEN || piece == BQUEEN)
	{
		getQueenMoves(board, piece_coordinates, &moves);
	}
	else if (piece == WKING || piece == BKING)
	{
		getKingCaptureMoves(board, piece_coordinates, &moves);
	}

	// Change the turn over temporarily, as we are checking the opposing colour's moves
	//board->white_turn ^= 1;

	*move_list = moves;

}

void getStraightMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;

	char white_piece = isPieceWhite(board, start);

	int index = 0;
	int n;

	// Get moves to the right of the piece
	if (start.x != 7)
	{
		for (n = start.x + 1; n < 8; n++)
		{
			if (moveCheck(board, white_piece, n, start.y))
			{
				moves.moves[index] = BoardCoordinateConstructor(n, start.y);
				index++;
			}
			if (isPieceAtXY(board, n, start.y))
			{
				break;
			}
		}
	}

	// Get moves to the left of the piece
	if (start.x != 0)
	{
		for (n = start.x - 1; n > -1; n--)
		{
			if (moveCheck(board, white_piece, n, start.y))
			{
				moves.moves[index] = BoardCoordinateConstructor(n, start.y);
				index++;
			}
			if (isPieceAtXY(board, n, start.y))
			{
				break;
			}
		}
	}

	// Get moves above the piece
	if (start.y != 7)
	{
		for (n = start.y + 1; n < 8; n++)
		{
			if (moveCheck(board, white_piece, start.x, n))
			{
				moves.moves[index] = BoardCoordinateConstructor(start.x, n);
				index++;
			}
			if (isPieceAtXY(board, start.x, n))
			{
				break;
			}
		}
	}

	// Get moves below the piece
	if (start.y != 0)
	{
		for (n = start.y - 1; n > -1; n--)
		{
			if (moveCheck(board, white_piece, start.x, n))
			{
				moves.moves[index] = BoardCoordinateConstructor(start.x, n);
				index++;
			}
			if (isPieceAtXY(board, start.x, n))
			{
				break;
			}
		}
	}

	moves.move_count = index;

	*move_list = moves;
}

void getDiagonalMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;

	char white_piece = isPieceWhite(board, start);

	int index = 0;
	int n;

	// Get NE diagonal moves
	if (start.x != 7 && start.y != 7)
	{
		n = 1;
		while (1)
		{
			if (moveCheck(board, white_piece, start.x + n, start.y + n))
			{
				moves.moves[index] = BoardCoordinateConstructor(start.x + n, start.y + n);
				index++;
			}
			else
			{
				break;
			}

			if (isPieceAtXY(board, start.x + n, start.y + n))
			{
				break;
			}

			n++;
		}
	}

	// Get SE diagonal moves
	if (start.x != 7 && start.y != 0)
	{
		n = 1;
		while (1)
		{
			if (moveCheck(board, white_piece, start.x + n, start.y - n))
			{
				moves.moves[index] = BoardCoordinateConstructor(start.x + n, start.y - n);
				index++;
			}
			else
			{
				break;
			}

			if (isPieceAtXY(board, start.x + n, start.y - n))
			{
				break;
			}

			n++;
		}
	}

	// Get SW diagonal moves
	if (start.x != 0 && start.y != 0)
	{
		n = 1;
		while (1)
		{
			if (moveCheck(board, white_piece, start.x - n, start.y - n))
			{
				moves.moves[index] = BoardCoordinateConstructor(start.x - n, start.y - n);
				index++;
			}
			else
			{
				break;
			}

			if (isPieceAtXY(board, start.x - n, start.y - n))
			{
				break;
			}

			n++;
		}
	}

	// Get NW diagonal moves
	if (start.x != 0 && start.y != 7)
	{
		n = 1;
		while (1)
		{
			if (moveCheck(board, white_piece, start.x - n, start.y + n))
			{
				moves.moves[index] = BoardCoordinateConstructor(start.x - n, start.y + n);
				index++;
			}
			else
			{
				break;
			}

			if (isPieceAtXY(board, start.x - n, start.y + n))
			{
				break;
			}

			n++;
		}
	}

	moves.move_count = index;

	*move_list = moves;
}

void getQueenMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList diag_moves;

	getDiagonalMoves(board, start, &diag_moves);
	getStraightMoves(board, start, move_list);

	mergeMoveCoordinateLists(move_list, &diag_moves);
}

void getKnightMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;

	char white_piece = isPieceWhite(board, start);

	int index = 0;

	// 1R2U Move
	if (moveCheck(board, white_piece, start.x + 1, start.y + 2))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y + 2);
		index++;
	}
	// 1L2U Move
	if (moveCheck(board, white_piece, start.x - 1, start.y + 2))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y + 2);
		index++;
	}
	// 1R2D Move
	if (moveCheck(board, white_piece, start.x + 1, start.y - 2))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y - 2);
		index++;
	}
	// 1L2D Move
	if (moveCheck(board, white_piece, start.x - 1, start.y - 2))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y - 2);
		index++;
	}
	// 2R1U Move
	if (moveCheck(board, white_piece, start.x + 2, start.y + 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 2, start.y + 1);
		index++;
	}
	// 2R1D Move
	if (moveCheck(board, white_piece, start.x + 2, start.y - 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 2, start.y - 1);
		index++;
	}
	// 2L1U Move
	if (moveCheck(board, white_piece, start.x - 2, start.y + 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 2, start.y + 1);
		index++;
	}
	// 2L1D Move
	if (moveCheck(board, white_piece, start.x - 2, start.y - 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 2, start.y - 1);
		index++;
	}

	moves.move_count = index;

	*move_list = moves;
}

void getKingCaptureMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;

	char white_piece = isPieceWhite(board, start);

	int index = 0;
	// Regular Moves
	// N Move
	if (moveCheck(board, white_piece, start.x, start.y + 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y + 1);
		index++;
	}
	// NE Move
	if (moveCheck(board, white_piece, start.x + 1, start.y + 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y + 1);
		index++;
	}
	// E Move
	if (moveCheck(board, white_piece, start.x + 1, start.y))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y);
		index++;
	}
	// SE Move
	if (moveCheck(board, white_piece, start.x + 1, start.y - 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y - 1);
		index++;
	}
	// S Move
	if (moveCheck(board, white_piece, start.x, start.y - 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y - 1);
		index++;
	}
	// SW Move
	if (moveCheck(board, white_piece, start.x - 1, start.y - 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y - 1);
		index++;
	}
	// W Move
	if (moveCheck(board, white_piece, start.x - 1, start.y))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y);
		index++;
	}
	// NW Move
	if (moveCheck(board, white_piece, start.x - 1, start.y + 1))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y + 1);
		index++;
	}

	moves.move_count = index;

	*move_list = moves;
}

void getKingMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;
	char castle_row;
	char king_info_bitshift;
	int index = 0;

	if (isPieceWhite(board, start))
	{
		castle_row = 0;
		king_info_bitshift = 0;
	}
	else
	{
		castle_row = 7;
		king_info_bitshift = 4;
	}

	// Regular Moves
	 getKingCaptureMoves(board, start, &moves);

	index = moves.move_count;

	// Castling
	// Kingside castling
	// If the king is not in check, the spaces between the king and rook are empty, the spaces the king moves are not attacked and neither pieces have moved then castling is allowed
	if ((board->king_info & (0x0B << king_info_bitshift)) == 0x00 && board->board[5][castle_row] == BLANK && board->board[6][castle_row] == BLANK && !isSpaceAttacked(board, 5, castle_row) && !isSpaceAttacked(board, 6, castle_row))
	{
		// Castle
		moves.moves[index] = BoardCoordinateConstructor(start.x + 2, castle_row);
		index++;
	}

	// Queenside Castling
	if ((board->king_info & (0x0D << king_info_bitshift)) == 0x00 && board->board[3][castle_row] == BLANK && board->board[2][castle_row] == BLANK && board->board[1][castle_row] == BLANK && !isSpaceAttacked(board, 3, castle_row) && !isSpaceAttacked(board, 2, castle_row))
	{
		// Castle
		moves.moves[index] = BoardCoordinateConstructor(start.x - 2, castle_row);
		index++;
	}

	moves.move_count = index;

	*move_list = moves;
}

void getPawnThreatMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	char white_piece = isPieceWhite(board, start);

	struct MoveCoordinateList moves;
	int dir;
	int index = 0;

	if (white_piece)
	{
		dir = 1;
	}
	else
	{
		dir = -1;
	}

	if (start.x < 7 && (board->board[start.x + 1][start.y + dir] == BLANK || (isPieceAtXY(board, start.x + 1, start.y + dir) && isPieceWhiteXY(board, start.x + 1, start.y + dir) != white_piece)))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y + dir);
		index++;
	}
	if (start.x > 0 && (board->board[start.x - 1][start.y + dir] == BLANK || (isPieceAtXY(board, start.x - 1, start.y + dir) && isPieceWhiteXY(board, start.x - 1, start.y + dir) != white_piece)))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y + dir);
		index++;
	}

	// Check for en passant threat ?

	moves.move_count = index;

	*move_list = moves;
}

void getPawnCaptureMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	char white_piece = isPieceWhite(board, start);
	struct MoveCoordinateList moves;
	int dir;
	int index = 0;
	int en_passant_bitshift = 0;
	int en_passant_column;

	if (white_piece)
	{
		en_passant_bitshift = 4;
		dir = 1;
	}
	else
	{
		dir = -1;
	}

	// Regular capture
	if (start.x < 7 && board->board[start.x + 1][start.y + dir] != BLANK && (isPieceAtXY(board, start.x + 1, start.y + dir) && isPieceWhiteXY(board, start.x + 1, start.y + dir) != white_piece))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y + dir);
		index++;
	}
	if (start.x > 0 && board->board[start.x - 1][start.y + dir] != BLANK && (isPieceAtXY(board, start.x - 1, start.y + dir) && isPieceWhiteXY(board, start.x - 1, start.y + dir) != white_piece))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y + dir);
		index++;
	}

	// En passant
	if (board->en_passant & (0x08 << en_passant_bitshift))
	{
		en_passant_column = (board->en_passant & (0x07 << en_passant_bitshift)) >> en_passant_bitshift;

		// If black and on row 3 or white and on row 4 and pawn is on either side of the en passant column
		if (((dir == -1 && start.y == 3) || (dir == 1 && start.y == 4)) && (start.x == en_passant_column - 1 || start.x == en_passant_column + 1))
		{
			moves.moves[index] = BoardCoordinateConstructor(en_passant_column, start.y + dir);
			index++;
		}
	}

	moves.move_count = index;

	*move_list = moves;
}

void getPawnMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list)
{
	struct MoveCoordinateList moves;
	struct MoveCoordinateList capture_moves;
	int dir;
	int index = 0;

	// Get the piece colour at the given position
	if (isPieceWhite(board, start))
	{
		dir = 1;
	}
	else
	{
		dir = -1;
	}

	// Regular move
	if (board->board[start.x][start.y + dir] == BLANK)
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y + dir);
		index++;
	}

	// Starting Move
	// If white
	if (dir == 1 && start.y == 1 && board->board[start.x][start.y + 1] == BLANK && board->board[start.x][start.y + 2] == BLANK)
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y + 2);
		index++;
	}
	// If black
	else if (dir == -1 && start.y == 6 && board->board[start.x][start.y - 1] == BLANK && board->board[start.x][start.y - 2] == BLANK)
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y - 2);
		index++;
	}

	moves.move_count = index;

	getPawnCaptureMoves(board, start, &capture_moves);

	// Capturing move
	mergeMoveCoordinateLists(&moves, &capture_moves);

	// Promotion

	*move_list = moves;
}

void mergeMoveCoordinateLists(struct MoveCoordinateList *a, struct MoveCoordinateList *b)
{
	int n;
	struct MoveCoordinateList merge_list = *a;

	for (n = 0; n < b->move_count; n++)
	{
		merge_list.moves[merge_list.move_count + n] = b->moves[n];
	}

	merge_list.move_count += b->move_count;

	*a = merge_list;
}

