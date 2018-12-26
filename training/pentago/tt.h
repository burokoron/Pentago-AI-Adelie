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

// �u���\�̊m��(�K��2�ׂ̂����)
void InitTT(TTENTRY *ttentry, int entry_count);

// �u���\�̍폜
void DeleteTT(TTENTRY *ttentry);

// �u���\�ɕۑ�
void SaveTT(TTENTRY *ttentry, uint64_t *move_pos, uint64_t key, BOARD *board, float alpha, float beta, int depth);

#endif