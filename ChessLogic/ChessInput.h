#ifndef CHESSINPUT_H
#define CHESSINPUT_H

#include "ChessMoves.h"

void inputMoveStart(struct ChessBoard *board, struct BoardCoordinate start, struct MoveCoordinateList *end_move_list);

void inputEndMove(struct ChessBoard *board, struct BoardCoordinate start, struct BoardCoordinate end);

void inputPawnPromotion(struct ChessBoard *board, int input);

#endif
