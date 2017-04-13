#include "ChessInput.h"

struct MoveCoordinateList inputMoveStart(struct ChessBoard board, struct BoardCoordinate start)
{
	return getCurrentTurnPieceMoves(board, start);
}

struct ChessBoard inputEndMove(struct ChessBoard board, struct BoardCoordinate start, struct BoardCoordinate end)
{
	return makeMove(board, moveConstructor(board, start, end));
}

struct ChessBoard inputPawnPromotion(struct ChessBoard board, int input_no)
{
	return promotePawn(board, input_no);
}
