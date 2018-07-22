#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "search.h"
#include "tt.h"

extern int node;
extern int entry_count;

// 手の入力
void Input(const BOARD *board, uint64_t move_pos);

// 盤面の出力
void OutputBoard(const BOARD *board);

// コンピュータの思考
void ComThink(BOARD *board, uint64_t *move_pos, int level, int turn, float limit);

// コンピュータの打ち手の出力
void OutputComMove(const uint64_t *move_pos);

// 人間vsコンピュータ
void Man_vs_com(int level);

#endif