#ifndef TT_H
#define TT_H

#include "board.h"

typedef struct TTENTRY {
	uint64_t black, white, move_pos[2];
	float lower, upper;
	int depth;
}TTENTRY;

typedef struct CLUSTER {
	TTENTRY *ttentry;
}CLUSTER;

// 置換表の確保(必ず2のべき乗個)
void InitTT(TTENTRY *ttentry, int entry_count);

// 置換表の削除
void DeleteTT(TTENTRY *ttentry);

// 置換表に保存
void SaveTT(TTENTRY *ttentry, uint64_t *move_pos, uint64_t key, BOARD *board, float alpha, float beta, int depth);

#endif