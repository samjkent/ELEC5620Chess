#include "PieceMoves.h"
#include "ChessBoard.h"
#include "ChessMoves.h"

struct MoveCoordinateList inputMoveStart(struct ChessBoard board, struct BoardCoordinate start);

struct ChessBoard inputEndMove(struct ChessBoard board, struct BoardCoordinate start, struct BoardCoordinate end);

struct ChessBoard inputPawnPromotion(struct ChessBoard board, int input);
