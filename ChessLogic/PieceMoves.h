#ifndef PIECEMOVES_H
#define PIECEMOVES_H

#include "ChessBoard.h"
#include "ChessMoves.h"

struct MoveCoordinateList
{
	struct BoardCoordinate moves[27];
	char move_count;// = 0;
};

struct MoveCoordinateList purgeIllegalMoves(struct ChessBoard board, struct BoardCoordinate start, struct MoveCoordinateList moveList);

struct MoveCoordinateList mergeMoveCoordinateLists(struct MoveCoordinateList a, struct MoveCoordinateList b);

struct MoveCoordinateList getCurrentTurnPieceMoves(struct ChessBoard board, struct BoardCoordinate piece_coordinates);

struct MoveCoordinateList getPieceMoves(struct ChessBoard board, char white_piece, struct BoardCoordinate piece_coordinates);

struct MoveCoordinateList getPieceMovesXY(struct ChessBoard board, char white_piece, char x, char y);

struct MoveCoordinateList getPieceCaptureMoves(struct ChessBoard board, struct BoardCoordinate piece_coordinates);

struct MoveCoordinateList getPieceCaptureMovesXY(struct ChessBoard board, char x, char y);

struct MoveCoordinateList getStraightMoves(struct ChessBoard board, struct  BoardCoordinate start);

struct MoveCoordinateList getDiagonalMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getQueenMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getKnightMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getKingMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getKingCaptureMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getPawnMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getPawnCaptureMoves(struct ChessBoard board, struct BoardCoordinate start);

struct MoveCoordinateList getPawnThreatMoves(struct ChessBoard board, struct BoardCoordinate start);

#endif
