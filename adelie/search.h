#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "tt.h"
#include "evaluation.h"

#define MY_INFINITY 10000.0

#define max(a, b) ((a)>(b)?(a):(b))
#define min(a, b) ((a)<(b)?(a):(b))

// �őP��ɂȂ����񐔂��L�^
typedef struct HISTORY {
	int num;
	uint64_t move[2];
}HISTORY;

// xorshift
// ������
void InitXor();

// ����
unsigned long Xor128();

// history
// ������
void Init_history();

// �őP��ɂȂ����񐔂̕ۑ�
void Save_history(const uint64_t *best_move);

// q�\�[�g��r�֐�
int cmp(const void *p, const void *q);

// �o���񐔂��������Ƀ\�[�g
void Sort_history();

// �����_�����[�u
void RandomMove(BOARD *board, uint64_t *best_move);

// �P��alpha-beta(�}���Ȃ�)
double NegaAlpha(BOARD *board, uint64_t *move_pos, CLUSTER *cluster, double alpha, double beta, int deep, int depth);


#endif