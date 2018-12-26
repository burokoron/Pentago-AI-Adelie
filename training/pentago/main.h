#ifndef MAIN_H
#define MAIN_H

#include "board.h"
#include "search.h"
#include "tt.h"
#include "learner.h"

extern int node;
extern int entry_count;

// ��̓���
void Input(const BOARD *board, uint64_t move_pos);

// �Ֆʂ̏o��
void OutputBoard(const BOARD *board);

// �l��vs�l��
void ManVsMan(void);

// �R���s���[�^�̎v�l
void ComThink(BOARD *board, uint64_t *move_pos, int level);

// �R���s���[�^�̑ł���̏o��
void OutputComMove(const BOARD *board, const uint64_t *move_pos);

// �l��vs�R���s���[�^
void Man_vs_com(int level);

// �R���s���[�^vs�R���s���[�^
int Com_vs_com(int com_1_level, int com_2_level, FILE *file);

#endif