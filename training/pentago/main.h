#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "search.h"
#include "tt.h"
#include "learner.h"

extern int node;
extern int entry_count;

// 手の入力
void Input(const BOARD *board, uint64_t move_pos);

// 盤面の出力
void OutputBoard(const BOARD *board);

// 人間vs人間
void ManVsMan(void);

// コンピュータの思考
void ComThink(BOARD *board, uint64_t *move_pos, int level);

// コンピュータの打ち手の出力
void OutputComMove(const BOARD *board, const uint64_t *move_pos);

// 人間vsコンピュータ
void Man_vs_com(int level);

// コンピュータvsコンピュータ
int Com_vs_com(int com_1_level, int com_2_level, FILE *file);

#endif