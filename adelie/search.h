#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "tt.h"
#include "evaluation.h"

#define MY_INFINITY 10000.0

#define max(a, b) ((a)>(b)?(a):(b))
#define min(a, b) ((a)<(b)?(a):(b))

// Å‘Pè‚É‚È‚Á‚½‰ñ”‚ğ‹L˜^
typedef struct HISTORY {
	int num;
	uint64_t move[2];
}HISTORY;

// xorshift
// ‰Šú‰»
void InitXor();

// —”
unsigned long Xor128();

// history
// ‰Šú‰»
void Init_history();

// Å‘Pè‚É‚È‚Á‚½‰ñ”‚Ì•Û‘¶
void Save_history(const uint64_t *best_move);

// qƒ\[ƒg”äŠrŠÖ”
int cmp(const void *p, const void *q);

// oŒ»‰ñ”‚ª‘½‚¢‡‚Éƒ\[ƒg
void Sort_history();

// ƒ‰ƒ“ƒ_ƒ€ƒ€[ƒu
void RandomMove(BOARD *board, uint64_t *best_move);

// ’Pƒalpha-beta(}Š ‚È‚µ)
double NegaAlpha(BOARD *board, uint64_t *move_pos, CLUSTER *cluster, double alpha, double beta, int deep, int depth);


#endif