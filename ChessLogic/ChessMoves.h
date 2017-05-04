#ifndef CHESSMOVES_H
#define CHESSMOVES_H

#include "ChessBoard.h"
#include <string.h>
#include <stdio.h>


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

struct Move moveConstructor(struct ChessBoard *board, struct BoardCoordinate start, struct BoardCoordinate end);

void makeMove(struct ChessBoard *board, struct Move move);

// Promotion piece = 1 for queen
// Promotion piece = 2 for knight
// Promotion piece = 3 for rook
// Promotion piece = 4 for bishop
void promotePawn(struct ChessBoard *board, int promotion_piece);

void endGameCheck(struct ChessBoard *board);

char insufficientMaterialCheck(struct ChessBoard *board);

// 0x08 for white king in check
// 0x80 for black king in check
void updateKingChecks(struct ChessBoard *board);

char canWhiteMove(struct ChessBoard *board);

char canBlackMove(struct ChessBoard *board);

char moveCheck(struct ChessBoard *board, char white_move, char x, char y);

char isSpaceAttacked(struct ChessBoard *board, char x, char y, char white_piece);


char isMoveInMoveList(struct MoveCoordinateList moveList, struct BoardCoordinate move);

void purgeIllegalMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *moveList);

void mergeMoveCoordinateLists(struct MoveCoordinateList *a, struct MoveCoordinateList *b);

void getCurrentTurnPieceMoves(struct ChessBoard *board, struct BoardCoordinate piece_coordinates, struct MoveCoordinateList *move_list);

void getPieceMoves(struct ChessBoard *board, char white_piece, struct BoardCoordinate piece_coordinates, struct MoveCoordinateList *move_list);

void getPieceMovesXY(struct ChessBoard *board, char white_piece, char x, char y, struct MoveCoordinateList *move_list);

void getPieceCaptureMoves(struct ChessBoard *board, struct BoardCoordinate piece_coordinates, struct MoveCoordinateList *move_list);

void getPieceCaptureMovesXY(struct ChessBoard *board, char x, char y, struct MoveCoordinateList *move_list);

void getStraightMoves(struct ChessBoard *board, struct  BoardCoordinate start, struct MoveCoordinateList *move_list);

void getDiagonalMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getQueenMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getKnightMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getKingMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getKingCaptureMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getPawnMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getPawnCaptureMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

void getPawnThreatMoves(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *move_list);

#endif
