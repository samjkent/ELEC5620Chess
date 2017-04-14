#include "ChessInput.h"


void inputMoveStart(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *end_move_list)
{
	getCurrentTurnPieceMoves(board, start, end_move_list);
}

void inputEndMove(struct ChessBoard *board, struct BoardCoordinate start, struct BoardCoordinate end)
{
	makeMove(board, moveConstructor(board, start, end));
}

void inputPawnPromotion(struct ChessBoard *board, int input_no)
{
	promotePawn(board, input_no);
}

