#include <stdint.h>
#include <stdlib.h>

#include "tt.h"

int entry_count;

// �u���\�̊m��(�K��2�ׂ̂����)
void InitTT(TTENTRY *ttentry, int entry_count) {
	ttentry = (TTENTRY*)calloc(entry_count, sizeof(TTENTRY));
}

// �u���\�̍폜
void DeleteTT(TTENTRY *ttentry) {
	free(ttentry);
}

// �u���\�ɕۑ�
void SaveTT(TTENTRY *ttentry, uint64_t *move_pos, uint64_t key,BOARD *board, float alpha, float beta, int depth) {
	ttentry[key&(entry_count - 1)].black = board->black;
	ttentry[key&(entry_count - 1)].white = board->white;
	ttentry[key&(entry_count - 1)].move_pos[0] = move_pos[0];
	ttentry[key&(entry_count - 1)].move_pos[1] = move_pos[1];
	ttentry[key&(entry_count - 1)].lower = alpha;
	ttentry[key&(entry_count - 1)].upper = beta;
	ttentry[key&(entry_count - 1)].depth = depth;
}