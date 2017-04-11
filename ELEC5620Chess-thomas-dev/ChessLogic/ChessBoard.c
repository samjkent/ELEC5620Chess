#include "ChessBoard.h"

struct BoardCoordinate BoardCoordinateConstructor(char x, char y)
{
	struct BoardCoordinate coordinate;
	coordinate.x = x;
	coordinate.y = y;

	return coordinate;
}

char convertColumnToCoordinate(char column)
{
	// Convert uppercase to lowercase
	if (column < 97) {
		column += 32;
	}

	// Convert ASCII letter to chess column number
	return column - 97;
}

char convertRowToCoordinate(char row)
{
	// Convert ASCII number to integer
	return row - 49;
}

struct BoardCoordinate convertNotationToBoardCoordinate(char position[2])
{
	struct BoardCoordinate coordinate;

	coordinate.x = convertColumnToCoordinate(position[0]);
	coordinate.y = convertRowToCoordinate(position[1]);

	return coordinate;
}

char convertCoordinateToBoardColumn(struct BoardCoordinate coordinate)
{
	// Convert to ASCII
	return coordinate.x + 97;
}

char convertCoordinateToBoardRow(struct BoardCoordinate coordinate)
{
	// Convert to ASCII
	return coordinate.y + 49;
}

char getPieceAtCoordinate(struct ChessBoard board, struct BoardCoordinate coordinate)
{
	return board.board[coordinate.x][coordinate.y];
}

char isPieceWhite(struct ChessBoard board, struct BoardCoordinate coordinate)
{
	return isPieceWhiteXY(board, coordinate.x, coordinate.y);
}

char isPieceWhiteXY(struct ChessBoard board, char x, char y)
{
	// If ASCII code is greater than 97 (lower case alphabet)
	// Return that piece is not white
	if (board.board[x][y] >= 97)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

char isPieceAtLocation(struct ChessBoard board, struct BoardCoordinate location)
{
	return isPieceAtXY(board, location.x, location.y);
}

char isPieceAtXY(struct ChessBoard board, char x, char y)
{
	if (x > 7 || x < 0 || y > 7 || y < 0)
	{
		return 0;
	}

	if (board.board[x][y] == BLANK || board.board[x][y] == ERROR)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

char isSpaceWhite(struct BoardCoordinate coordinate) 
{
	return isSpaceWhiteXY(coordinate.x, coordinate.y);
}

char isSpaceWhiteXY(char x, char y)
{
	// If both squares have the same parity, the square is black
	if (x % 2 == y % 2)
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}

struct PieceList getWhitePieces(struct ChessBoard board)
{
	struct PieceList piece_list;
	int i;
	int j;
	int index = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++)
		{
			if (board.board[i][j] == WBISHOP && isSpaceWhiteXY(i, j))
			{
				piece_list.pieces[index] = WBISHOP_W;
				index++;
			}
			else if (board.board[i][j] == WBISHOP && !isSpaceWhiteXY(i, j))
			{
				piece_list.pieces[index] = WBISHOP_B;
				index++;
			}
			else if (board.board[i][j] != BLANK && isPieceWhiteXY(board, i, j))
			{
				piece_list.pieces[index] = board.board[i][j];
				index++;
			}
		}
	}

	// Fill up the rest of the piece list with blank spaces
	for (i = index; i < 16; i++)
	{
		piece_list.pieces[i] = BLANK;
	}

	return piece_list;
}

struct PieceList getBlackPieces(struct ChessBoard board)
{
	struct PieceList piece_list;
	int i;
	int j;
	int index = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++)
		{
			if (board.board[i][j] == BBISHOP && isSpaceWhiteXY(i, j))
			{
				piece_list.pieces[index] = BBISHOP_W;
				index++;
			}
			else if (board.board[i][j] == BBISHOP && !isSpaceWhiteXY(i, j))
			{
				piece_list.pieces[index] = BBISHOP_B;
				index++;
			}
			else if (board.board[i][j] != BLANK && !isPieceWhiteXY(board, i, j))
			{
				piece_list.pieces[index] = board.board[i][j];
				index++;
			}
		}
	}

	// Fill up the rest of the piece list with blank spaces
	for (i = index; i < 16; i++)
	{
		piece_list.pieces[i] = BLANK;
	}

	return piece_list;
}

struct ChessBoard initChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;
	// Create new chess board with pieces in standard layout

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (2 <= j && j <= 5) {
				board.board[i][j] = BLANK;
			}
			else if (j == 1) {
				board.board[i][j] = WPAWN;
			}
			else if (j == 6)
			{
				board.board[i][j] = BPAWN;
			}
			else if (j == 0 && (i == 0 || i == 7))
			{
				board.board[i][j] = WROOK;
			}
			else if (j == 7 && (i == 0 || i == 7))
			{
				board.board[i][j] = BROOK;
			}
			else if (j == 0 && (i == 1 || i == 6))
			{
				board.board[i][j] = WKNIGHT;
			}
			else if (j == 7 && (i == 1 || i == 6))
			{
				board.board[i][j] = BKNIGHT;
			}
			else if (j == 0 && (i == 2 || i == 5))
			{
				board.board[i][j] = WBISHOP;
			}
			else if (j == 7 && (i == 2 || i == 5))
			{
				board.board[i][j] = BBISHOP;
			}
			else if (j == 0 && i == 3)
			{
				board.board[i][j] = WQUEEN;
			}
			else if (j == 7 && i == 3)
			{
				board.board[i][j] = BQUEEN;
			}
			else if (j == 0 && i == 4)
			{
				board.board[i][j] = WKING;
			}
			else if (j == 7 && i == 4)
			{
				board.board[i][j] = BKING;
			}
			else
			{
				board.board[i][j] = ERROR;
			}
		}
	}
	
	board.en_passant = 0x00;
	board.promotion = 0x00;
	board.white_turn = 0x01;
	board.white_piece_count = 16;
	board.black_piece_count = 16;
	board.king_info = 0x00;
	board.end_game = 0x00;
	board.fifty_move_counter = 0;
	
	
	return board;
}

struct ChessBoard initCastlingTestChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;

	// Create new chess board with pieces in layout to test for white castling
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (j == 0 && (i == 0 || i == 7))
			{
				board.board[i][j] = WROOK;
			}
			else if (j == 7 && (i == 0 || i == 7))
			{
				board.board[i][j] = BROOK;
			}
			else if (j == 0 && i == 4)
			{
				board.board[i][j] = WKING;
			}
			else if (j == 7 && i == 4)
			{
				board.board[i][j] = BKING;
			}
			else
			{
				board.board[i][j] = BLANK;
			}
		}
	}

	board.white_piece_count = 3;
	board.black_piece_count = 3;

	return board;
}

struct ChessBoard initPromotionTestChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;

	// Create new chess board with pieces in layout to test for en passant

	// Create blank board
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board.board[i][j] = BLANK;
		}
	}

	board.board[1][6] = WPAWN;
	board.board[6][1] = BPAWN;

	board.board[4][0] = WKING;
	board.board[4][7] = BKING;

	board.king_info = 0x77;

	return board;
}

struct ChessBoard initMaterialDrawTestChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;

	// Create new chess board with pieces in layout to test for en passant

	// Create blank board
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board.board[i][j] = BLANK;
		}
	}

	board.board[1][6] = WKNIGHT;

	board.board[4][0] = WKING;
	board.board[4][7] = BKING;

	board.black_piece_count = 1;
	board.white_piece_count = 2;

	return board;
}

struct ChessBoard initEnPassantTestChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;

	// Create new chess board with pieces in layout to test for en passant

	// Create blank board
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board.board[i][j] = BLANK;
		}
	}

	board.board[3][1] = WPAWN;
	board.board[4][3] = BPAWN;

	board.board[1][4] = BPAWN;
	board.board[2][4] = WPAWN;

	board.en_passant = 0x11;

	return board;
}

struct ChessBoard initPawnCheckTestChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;

	// Create new chess board with pieces in layout to test for en passant

	// Create blank board
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board.board[i][j] = BLANK;
		}
	}

	board.board[7][3] = WPAWN;
	board.board[7][4] = BPAWN;

	board.board[6][5] = BKING;

	board.white_turn = 0;

	return board;
}

struct ChessBoard initCheckTestChessBoard(void)
{
	int i;
	int j;

	struct ChessBoard board;

	// Create new chess board with pieces in layout to test for en passant

	// Create blank board
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			board.board[i][j] = BLANK;
		}
	}

	board.board[4][0] = WKING;
	board.board[4][6] = BQUEEN;
	board.board[6][0] = WKNIGHT;
	board.board[1][3] = WBISHOP;

	board.king_info = 0x0F;
	//board.en_passant = 0x11;

	return board;
}
