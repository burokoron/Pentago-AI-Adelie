#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "search.h"

int node;
extern int entry_count;
unsigned long w;

int killer[] = { 7, 10, 25, 28,
				 8,  9, 13, 16, 19, 22, 26, 27,
				 1,  4,  6, 11, 24, 29, 31, 34,
				14, 15, 20, 21,
				 2,  3, 12, 17, 18, 23, 32, 33,
				 0,  5, 30, 35 };

HISTORY history1[288], history2[288];

// xorshift
// 初期化
void InitXor() {
	w = (unsigned)time(NULL);
}

// 乱数
unsigned long Xor128() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629;
	unsigned long t;
	t = (x ^ (x << 11));x = y;y = z;z = w; return(w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

// ランダムムーブ
void RandomMove(BOARD *board, uint64_t *best_move) {
	do {
		best_move[0] = (uint64_t)1 << Xor128() % 36;
		best_move[1] = Xor128() % 8;
	} while ((board->black & best_move[0]) != 0 || (board->white & best_move[0]) != 0);
}

// history
// 初期化
void Init_history() {
	for (int i = 0;i < 288;i++) {
		history1[i].num = 0;
	}
}

// 最善手になった回数の保存
void Save_history(const uint64_t *best_move) {
	uint64_t pos = 1;
	int i;
	for (i = 0;i<36;i++) {
		if ((best_move[0] & pos) != 0) break;
		pos <<= 1;
	}

	history1[i * 8 + int(best_move[1])].move[0] = best_move[0];
	history1[i * 8 + int(best_move[1])].move[1] = best_move[1];
	history1[i * 8 + int(best_move[1])].num++;
}

// qソート比較関数
int cmp(const void *p, const void *q) {
	return ((HISTORY*)q)->num - ((HISTORY*)p)->num;
}

// 出現回数が多い順にソート
void Sort_history() {
	for (int i = 0;i < 288; i++) {
		history2[i].move[0] = history1[i].move[0];
		history2[i].move[1] = history1[i].move[1];
		history2[i].num = history1[i].num;
	}

	qsort(history2, 288, sizeof(HISTORY), cmp);
}

// 単純alpha-beta(枝刈あり)
double NegaAlpha(BOARD *board, uint64_t *best_move, CLUSTER *cluster, double alpha, double beta, int deep, int depth) {
	int winner_flag, entry_num, save_cluster[2] = { -1, 100 };
	int rand_margin = 5, futility_margin;
	double var, max = -MY_INFINITY;
	uint64_t move_pos[2], best_pos[2] = { (uint64_t)100, (uint64_t)100 }, pos = (uint64_t)1, save_move[2] = {100, 100};

	// ゲーム終了なら値を返す
	winner_flag = EndJudge(board);
	if (winner_flag == 3) return 0;
	if (winner_flag == BLACK){
		if (board->turn == BLACK) return MY_INFINITY - deep;
		else return -MY_INFINITY + deep;
	}
	if (winner_flag == WHITE) {
		if (board->turn == BLACK) return -MY_INFINITY + deep;
		else return MY_INFINITY - deep;
	}

	// 置換表のチェック
	entry_num = board->key&(entry_count - 1);
	if (board->key != 0x622b24abed22bd34) {
		for (int i = 0;i < 3;i++) {
			if (cluster[i].ttentry[entry_num].black == board->black && cluster[i].ttentry[entry_num].white == board->white) {
				best_pos[0] = cluster[i].ttentry[entry_num].move_pos[0];
				best_pos[1] = cluster[i].ttentry[entry_num].move_pos[1];
				if (cluster[i].ttentry[entry_num].depth >= depth - deep&&cluster[i].ttentry[entry_num].depth != 0) {
					if (cluster[i].ttentry[entry_num].lower >= beta) {
						if (deep == 0) {
							best_move[0] = cluster[i].ttentry[entry_num].move_pos[0];
							best_move[1] = cluster[i].ttentry[entry_num].move_pos[1];
						}
						return cluster[i].ttentry[entry_num].lower;
					}
					if (cluster[i].ttentry[entry_num].upper <= alpha || cluster[i].ttentry[entry_num].upper == cluster[i].ttentry[entry_num].lower) {
						if (deep == 0) {
							best_move[0] = cluster[i].ttentry[entry_num].move_pos[0];
							best_move[1] = cluster[i].ttentry[entry_num].move_pos[1];
						}
						return cluster[i].ttentry[entry_num].upper;
					}
					alpha = max(alpha, cluster[i].ttentry[entry_num].lower);
					beta = min(beta, cluster[i].ttentry[entry_num].upper);
				}
				break;
			}
			else if (cluster[i].ttentry[entry_num].black == 0) break;
		}
	}

	// 局面の評価値を返す
	if (deep >= depth) return Evaluate(board) + ((float)(Xor128() % (101 * rand_margin)) - (51 * rand_margin)) / 100.0;
	
	// futility cut
	if (futility_margin = depth - deep <= 1) {
		var = Evaluate(board);
		if (var < alpha - futility_margin * (37 - board->turn) * 2.5 || beta + futility_margin * (37 - board->turn) * 2.5 < var) return var;
	}

	// 次の1手を読む
	// 置換表に登録されていた手を先に読む
	if (best_pos[0] != 100 && (board->black & best_pos[0]) == 0 && (board->white & best_pos[0]) == 0) {
		DoMoveHash(board, best_pos);
		node++;
		var = -NegaAlpha(board, best_move, &cluster[0], -beta, -alpha, deep + 1, depth);
		UnDoMoveHash(board, best_pos);
		if (var > alpha) {
			alpha = var;
			save_move[0] = best_pos[0];
			save_move[1] = best_pos[1];
			if (deep == 0) {
				best_move[0] = best_pos[0];
				best_move[1] = best_pos[1];
			}
		}
		if (var > max) max = var;
	}

	// 過去の探索で多く最善手となった手を優先して読む
	for (int i = 0;i < 288 && alpha < beta && history2[i].num != 0;i++) {
		if ((board->black & history2[i].move[0]) == 0 && (board->white & history2[i].move[0]) == 0) {
			DoMoveHash(board, history2[i].move);
			node++;
			if (i == 0) var = -NegaAlpha(board, history2[i].move, &cluster[0], -beta, -alpha, deep + 1, depth);
			else {
				// null window search
				var = -NegaAlpha(board, history2[i].move, &cluster[0], -alpha - 0.01, -alpha, deep + 1, depth);
				if (alpha < var&&var < beta) var = -NegaAlpha(board, history2[i].move, &cluster[0], -beta, -var, deep + 1, depth);
			}
			UnDoMoveHash(board, history2[i].move);
			if (var > alpha) {
				alpha = var;
				save_move[0] = history2[i].move[0];
				save_move[1] = history2[i].move[1];
				if (deep == 0) {
					best_move[0] = history2[i].move[0];
					best_move[1] = history2[i].move[1];
				}
			}
			if (var > max) max = var;
		}
	}

	// 全合法手を読む
	for (int i = 0;i < 36 && alpha < beta;i++) {
		uint64_t pos = (uint64_t)1 << killer[i];
		if ((board->black & pos) == 0 && (board->white & pos) == 0) {
			move_pos[0] = pos;
			for (int j = 0;j < 8;j++) {
				move_pos[1] = j;
				DoMoveHash(board, move_pos);
				node++;
				if (i == 0 && j == 0) var = -NegaAlpha(board, best_move, &cluster[0], -beta, -alpha, deep + 1, depth);
				else {
					// null window search
					var = -NegaAlpha(board, best_move, &cluster[0], -alpha - 0.01, -alpha, deep + 1, depth);
					if (alpha < var&&var < beta) var = -NegaAlpha(board, best_move, &cluster[0], -beta, -var, deep + 1, depth);
				}
				UnDoMoveHash(board, move_pos);
				if (var > alpha) {
					alpha = var;
					save_move[0] = move_pos[0];
					save_move[1] = move_pos[1];
					if (deep == 0) {
						best_move[0] = move_pos[0];
						best_move[1] = move_pos[1];
					}
				}
				if (var > max) max = var;
				if(alpha >= beta) break;
			}
		}
	}

	// 最善手になった回数を数える
	if(save_move[1] != 100) Save_history(save_move);

	// 置換表に保存
	for (int i = 0;i < 3;i++) {
		if (cluster[i].ttentry[entry_num].black == board->black && cluster[i].ttentry[entry_num].white == board->white || cluster[i].ttentry[entry_num].black == 0) {
			if (cluster[i].ttentry[entry_num].depth<depth - deep) {
				if (max <= alpha) SaveTT(&cluster[i].ttentry[0], save_move, board->key, board, (float)cluster[i].ttentry[entry_num].lower, (float)max, depth - deep - 1);
				else if (max >= beta) SaveTT(&cluster[i].ttentry[0], save_move, board->key, board, (float)max, (float)cluster[i].ttentry[entry_num].upper, depth - deep - 1);
				else SaveTT(&cluster[i].ttentry[0], save_move, board->key, board, (float)max, (float)max, depth - deep - 1);
			}
			return max;
		}
		else if (save_cluster[1]>cluster[i].ttentry[entry_num].depth) {
			save_cluster[1] = cluster[i].ttentry[entry_num].depth;
			save_cluster[0] = i;
		}
	}
	if (save_cluster[1]<depth - deep) {
		if (max <= alpha) SaveTT(&cluster[save_cluster[0]].ttentry[0], save_move, board->key, board, (float)cluster[save_cluster[0]].ttentry[entry_num].lower, (float)max, depth - deep - 1);
		else if (max >= beta) SaveTT(&cluster[save_cluster[0]].ttentry[0], save_move, board->key, board, (float)max, (float)cluster[save_cluster[0]].ttentry[entry_num].upper, depth - deep - 1);
		else SaveTT(&cluster[save_cluster[0]].ttentry[0], save_move, board->key, board, (float)max, (float)max, depth - deep - 1);
	}

	return max;
}