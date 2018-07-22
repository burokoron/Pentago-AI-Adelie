#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "search.h"
#include "tt.h"

extern int node;
extern int entry_count;

// ��̓���
void Input(const BOARD *board, uint64_t move_pos);

// �Ֆʂ̏o��
void OutputBoard(const BOARD *board);

// �R���s���[�^�̎v�l
void ComThink(BOARD *board, uint64_t *move_pos, int level, int turn, float limit);

// �R���s���[�^�̑ł���̏o��
void OutputComMove(const uint64_t *move_pos);

// �l��vs�R���s���[�^
void Man_vs_com(int level);

#endif