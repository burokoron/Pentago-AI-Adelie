#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "tt.h"
#include "evaluation.h"

#define MY_INFINITY 10000.0

#define max(a, b) ((a)>(b)?(a):(b))
#define min(a, b) ((a)<(b)?(a):(b))

// 最善手になった回数を記録
typedef struct HISTORY {
	int num;
	uint64_t move[2];
}HISTORY;

// xorshift
// 初期化
void InitXor();

// 乱数
unsigned long Xor128();

// history
// 初期化
void Init_history();

// 最善手になった回数の保存
void Save_history(const uint64_t *best_move);

// qソート比較関数
int cmp(const void *p, const void *q);

// 出現回数が多い順にソート
void Sort_history();

// ランダムムーブ
void RandomMove(BOARD *board, uint64_t *best_move);

// 単純alpha-beta(枝刈なし)
double NegaAlpha(BOARD *board, uint64_t *move_pos, CLUSTER *cluster, double alpha, double beta, int deep, int depth);


#endif