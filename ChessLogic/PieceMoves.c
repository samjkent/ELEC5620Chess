#include "PieceMoves.h"

struct MoveCoordinateList purgeIllegalMoves(struct ChessBoard board, struct BoardCoordinate start, struct MoveCoordinateList moveList)
{
	int n;
	int index = 0;
	int king_bitshift = 0;
	struct Move test_move;
	struct ChessBoard temp_board;
	struct MoveCoordinateList legalMoveList;

	if (!board.white_turn)
	{
		king_bitshift = 4;
	}

	test_move.start = start;
	test_move.piece = board.board[start.x][start.y];

	for (n = 0; n < moveList.move_count; n++)
	{
		temp_board = board;

		// Set end coordinate of move
		test_move.end = moveList.moves[n];
		test_move.captured_piece = getPieceAtCoordinate(board, moveList.moves[n]);

		// Test move on temp board
		temp_board = makeMove(temp_board, test_move);

		// If own coloured king not in check, add move to legal movel list
		if ((temp_board.king_info & (0x08 << king_bitshift)) == 0)
		{
			legalMoveList.moves[index] = moveList.moves[n];
			index++;
		}
	}

	legalMoveList.move_count = index;

	return legalMoveList;
}

struct MoveCoordinateList getCurrentTurnPieceMoves(struct ChessBoard board, struct BoardCoordinate piece_coordinates)
{
	return getPieceMoves(board, board.white_turn, piece_coordinates);
}

struct MoveCoordinateList getPieceMovesXY(struct ChessBoard board, char white_piece, char x, char y)
{
	return getPieceMoves(board, white_piece, BoardCoordinateConstructor(x, y));
}

struct MoveCoordinateList getPieceMoves(struct ChessBoard board, char white_piece, struct BoardCoordinate piece_coordinates)
{
	struct MoveCoordinateList moves;
	char piece = board.board[piece_coordinates.x][piece_coordinates.y];
	char ascii_conversion = 0;

	if (!white_piece)
	{
		ascii_conversion = 32;
	}


	if (piece == WPAWN + ascii_conversion)
	{
		moves = getPawnMoves(board, piece_coordinates);
	}
	else if (piece == WKNIGHT + ascii_conversion)
	{
		moves = getKnightMoves(board, piece_coordinates);
	}
	else if (piece == WBISHOP + ascii_conversion)
	{
		moves = getDiagonalMoves(board, piece_coordinates);
	}
	else if (piece == WROOK + ascii_conversion)
	{
		moves = getStraightMoves(board, piece_coordinates);
	}
	else if (piece == WQUEEN + ascii_conversion)
	{
		moves = getQueenMoves(board, piece_coordinates);
	}
	else if (piece == WKING + ascii_conversion)
	{
		moves = getKingMoves(board, piece_coordinates);
	}

	moves = purgeIllegalMoves(board, piece_coordinates, moves);

	return moves;

}

struct MoveCoordinateList getPieceCaptureMovesXY(struct ChessBoard board, char x, char y)
{
	return getPieceCaptureMoves(board, BoardCoordinateConstructor(x, y));
}

struct MoveCoordinateList getPieceCaptureMoves(struct ChessBoard board, struct BoardCoordinate piece_coordinates)
{
	struct MoveCoordinateList moves;
	char piece = board.board[piece_coordinates.x][piece_coordinates.y];

	// Change the turn over temporarily, as we are checking the opposing colour's moves
	//board.white_turn ^= 1;

	if (piece == WPAWN || piece == BPAWN)
	{
		moves = getPawnThreatMoves(board, piece_coordinates);
	}
	else if (piece == WKNIGHT || piece == BKNIGHT)
	{
		moves = getKnightMoves(board, piece_coordinates);
	}
	else if (piece == WBISHOP || piece == BBISHOP)
	{
		moves = getDiagonalMoves(board, piece_coordinates);
	}
	else if (piece == WROOK || piece == BROOK)
	{
		moves = getStraightMoves(board, piece_coordinates);
	}
	else if (piece == WQUEEN || piece == BQUEEN)
	{
		moves = getQueenMoves(board, piece_coordinates);
	}
	else if (piece == WKING || piece == BKING)
	{
		moves = getKingCaptureMoves(board, piece_coordinates);
	}

	// Change the turn over temporarily, as we are checking the opposing colour's moves
	//board.white_turn ^= 1;

	return moves;

}

struct MoveCoordinateList getStraightMoves(struct ChessBoard board, struct BoardCoordinate start)
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

	return moves;
}

struct MoveCoordinateList getDiagonalMoves(struct ChessBoard board, struct BoardCoordinate start)
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

	return moves;
}

struct MoveCoordinateList getQueenMoves(struct ChessBoard board, struct BoardCoordinate start)
{
	struct MoveCoordinateList strt_moves = getStraightMoves(board, start);
	struct MoveCoordinateList diag_moves = getDiagonalMoves(board, start);

	return mergeMoveCoordinateLists(strt_moves, diag_moves);
}

struct MoveCoordinateList getKnightMoves(struct ChessBoard board, struct BoardCoordinate start)
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

	return moves;
}

struct MoveCoordinateList getKingCaptureMoves(struct ChessBoard board, struct BoardCoordinate start)
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

	return moves;
}

struct MoveCoordinateList getKingMoves(struct ChessBoard board, struct BoardCoordinate start)
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
	moves = getKingCaptureMoves(board, start);

	index = moves.move_count;

	// Castling 
	// Kingside castling
	// If the king is not in check, the spaces between the king and rook are empty, the spaces the king moves are not attacked and neither pieces have moved then castling is allowed
	if ((board.king_info & (0x0B << king_info_bitshift)) == 0x00 && board.board[5][castle_row] == BLANK && board.board[6][castle_row] == BLANK && !isSpaceAttacked(board, 5, castle_row) && !isSpaceAttacked(board, 6, castle_row))
	{
		// Castle
		moves.moves[index] = BoardCoordinateConstructor(start.x + 2, castle_row);
		index++;
	}

	// Queenside Castling
	if ((board.king_info & (0x0D << king_info_bitshift)) == 0x00 && board.board[3][castle_row] == BLANK && board.board[2][castle_row] == BLANK && board.board[1][castle_row] == BLANK && !isSpaceAttacked(board, 3, castle_row) && !isSpaceAttacked(board, 2, castle_row))
	{
		// Castle
		moves.moves[index] = BoardCoordinateConstructor(start.x - 2, castle_row);
		index++;
	}

	moves.move_count = index;

	return moves;
}

struct MoveCoordinateList getPawnThreatMoves(struct ChessBoard board, struct BoardCoordinate start)
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

	if (start.x < 7 && (board.board[start.x + 1][start.y + dir] == BLANK || (isPieceAtXY(board, start.x + 1, start.y + dir) && isPieceWhiteXY(board, start.x + 1, start.y + dir) != white_piece)))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y + dir);
		index++;
	}
	if (start.x > 0 && (board.board[start.x - 1][start.y + dir] == BLANK || (isPieceAtXY(board, start.x - 1, start.y + dir) && isPieceWhiteXY(board, start.x - 1, start.y + dir) != white_piece)))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y + dir);
		index++;
	}

	// Check for en passant threat ?

	moves.move_count = index;

	return moves;
}

struct MoveCoordinateList getPawnCaptureMoves(struct ChessBoard board, struct BoardCoordinate start)
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
	if (start.x < 7 && board.board[start.x + 1][start.y + dir] != BLANK && (isPieceAtXY(board, start.x + 1, start.y + dir) && isPieceWhiteXY(board, start.x + 1, start.y + dir) != white_piece))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x + 1, start.y + dir);
		index++;
	}
	if (start.x > 0 && board.board[start.x - 1][start.y + dir] != BLANK && (isPieceAtXY(board, start.x - 1, start.y + dir) && isPieceWhiteXY(board, start.x - 1, start.y + dir) != white_piece))
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x - 1, start.y + dir);
		index++;
	}

	// En passant
	if (board.en_passant & (0x08 << en_passant_bitshift))
	{
		en_passant_column = (board.en_passant & (0x07 << en_passant_bitshift)) >> en_passant_bitshift;

		// If black and on row 3 or white and on row 4 and pawn is on either side of the en passant column
		if (((dir == -1 && start.y == 3) || (dir == 1 && start.y == 4)) && (start.x == en_passant_column - 1 || start.x == en_passant_column + 1))
		{
			moves.moves[index] = BoardCoordinateConstructor(en_passant_column, start.y + dir);
			index++;
		}
	}

	moves.move_count = index;

	return moves;
}

struct MoveCoordinateList getPawnMoves(struct ChessBoard board, struct BoardCoordinate start)
{
	struct MoveCoordinateList moves;
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
	if (board.board[start.x][start.y + dir] == BLANK)
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y + dir);
		index++;
	}

	// Starting Move
	// If white
	if (dir == 1 && start.y == 1 && board.board[start.x][start.y + 1] == BLANK && board.board[start.x][start.y + 2] == BLANK)
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y + 2);
		index++;
	}
	// If black
	else if (dir == -1 && start.y == 6 && board.board[start.x][start.y - 1] == BLANK && board.board[start.x][start.y - 2] == BLANK)
	{
		moves.moves[index] = BoardCoordinateConstructor(start.x, start.y - 2);
		index++;
	}

	moves.move_count = index;

	// Capturing move
	moves = mergeMoveCoordinateLists(moves, getPawnCaptureMoves(board, start));

	// Promotion

	return moves;
}

struct MoveCoordinateList mergeMoveCoordinateLists(struct MoveCoordinateList a, struct MoveCoordinateList b)
{
	int n;
	struct MoveCoordinateList merge_list = a;

	for (n = 0; n < b.move_count; n++)
	{
		merge_list.moves[merge_list.move_count + n] = b.moves[n];
	}

	merge_list.move_count += b.move_count;

	return merge_list;
}
