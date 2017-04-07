#ifndef CHESSBOARD_H
#define CHESSBOARD_H

// Characters used to display board information
#define BLANK	'.'
#define ERROR	'!'
#define WPAWN	'P'
#define WKNIGHT 'N'
#define WBISHOP	'B'
#define	WROOK	'R'
#define WQUEEN	'Q'
#define WKING	'K'
#define BPAWN	'p'
#define BKNIGHT 'n'
#define BBISHOP	'b'
#define	BROOK	'r'
#define BQUEEN	'q'
#define BKING	'k'

// Characters only used internally for checking board logic
#define WBISHOP_W '0'
#define WBISHOP_B '1'
#define BBISHOP_W '2'
#define BBISHOP_B '3'

struct BoardCoordinate
{
	char x;
	char y;
};

struct PieceList
{
	char pieces[16];
};

struct ChessBoard
{
	char board[8][8];
	// En passant
	// Stores a number between 0 and 7 in bits 0 - 3 of which column just had a pawn make a double move in
	// Stores if an en passant move may be made in bit 4
	char en_passant;

	// Promotion
	// Stores a number between 0 and 7 in bits 0 - 3 of which column just had a pawn make a promotion move in
	// Stores if an promotion move has just been made in bit 4
	char promotion;

	char white_turn;// = 1;

	char white_piece_count;// = 16;

	char black_piece_count;// = 16;


	
	// WKing Moved	0x01
	// WRookR Moved 0x02
	// WRookL Moved	0x04
	// WKing Check	0x08
	// BKing Moved	0x10
	// BRookR Moved 0x20
	// BRookL Moved	0x40
	// BKing Check	0x80
	char king_info;// = 0x00;

	// White checkmated			0x01
	// Black checkmated			0x02
	// Stalemate				0x04
	// Insufficient material	0x08
	// 50 move rule				0x10
	// Threefold repetition		0x20
	char end_game;// = 0x00;

	// Counts to 100 (as each move made by a player is considered as a half move)
	char fifty_move_counter;// = 0;
};

struct BoardCoordinate BoardCoordinateConstructor(char x, char y);

struct ChessBoard initChessBoard(void);

struct ChessBoard initCastlingTestChessBoard(void);

struct ChessBoard initEnPassantTestChessBoard(void);

struct ChessBoard initPromotionTestChessBoard(void);

struct ChessBoard initCheckTestChessBoard(void);

struct ChessBoard initPawnCheckTestChessBoard(void);

struct ChessBoard initMaterialDrawTestChessBoard(void);

struct PieceList getWhitePieces(struct ChessBoard board);

struct PieceList getBlackPieces(struct ChessBoard board);

char getPieceAtCoordinate(struct ChessBoard board, struct BoardCoordinate coordinate);

char convertColumnToCoordinate(char column);

char convertRowToCoordinate(char row);

char convertCoordinateToBoardRow(struct BoardCoordinate coordinate);

char convertCoordinateToBoardColumn(struct BoardCoordinate coordinate);

char isSpaceWhite(struct BoardCoordinate coordinate);

char isSpaceWhiteXY(char x, char y);

char isPieceWhite(struct ChessBoard board, struct BoardCoordinate coordinate);

char isPieceWhiteXY(struct ChessBoard board, char x, char y);

char isPieceAtLocation(struct ChessBoard board, struct BoardCoordinate location);

char isPieceAtXY(struct ChessBoard board, char x, char y);

struct BoardCoordinate convertNotationToBoardCoordinate(char position[2]);

#endif
