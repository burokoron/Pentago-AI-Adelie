#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "tt.h"
#include "evaluation.h"

#define MY_INFINITY 10000.0

#define max(a, b) ((a)>(b)?(a):(b))
#define min(a, b) ((a)<(b)?(a):(b))

// xorshift
// ‰Šú‰»
void InitXor();

// —”
unsigned long Xor128();

// ’Pƒalpha-beta(}Š ‚È‚µ)
double NegaAlpha(BOARD *board, uint64_t *move_pos, CLUSTER *cluster, double alpha, double beta, int deep, int depth);


#endif