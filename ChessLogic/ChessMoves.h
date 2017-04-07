#ifndef CHESSMOVES_H
#define CHESSMOVES_H

#include "PieceMoves.h"
//#include <stdlib.h>

/*
// Doubly linked list used to store all possible attacking moves of a given colour
// Struct used as a node in the linked list
struct ColourAttackNode
{
	struct BoardCoordinate move;
	struct ColourAttackNode *next;
	struct ColourAttackNode *previous;
};
*/

struct Move
{
	char piece;
	char captured_piece; // = BLANK; // BLANK if no piece captured
	struct BoardCoordinate start;
	struct BoardCoordinate end;
};

struct Move moveConstructor(struct ChessBoard board, struct BoardCoordinate start, struct BoardCoordinate end);

struct ChessBoard makeMove(struct ChessBoard board, struct Move move);

// Promotion piece = 1 for queen
// Promotion piece = 2 for knight
// Promotion piece = 3 for rook
// Promotion piece = 4 for bishop
struct ChessBoard promotePawn(struct ChessBoard board, int promotion_piece);

struct ChessBoard endGameCheck(struct ChessBoard board);

char insufficientMaterialCheck(struct ChessBoard board);

// 0x08 for white king in check
// 0x80 for black king in check
struct ChessBoard updateKingChecks(struct ChessBoard board);

char canWhiteMove(struct ChessBoard board);

char canBlackMove(struct ChessBoard board);

char isMoveInMoveList(struct MoveCoordinateList moveList, struct BoardCoordinate move);

char moveCheck(struct ChessBoard board, char white_move, char x, char y);

char isSpaceAttacked(struct ChessBoard board, char x, char y);


#endif
