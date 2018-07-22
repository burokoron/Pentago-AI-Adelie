#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "main.h"

// 手の入力
void Input(const BOARD *board, uint64_t *move_pos)
{
	char move[5];
	do {
		do {
			if (board->turn == BLACK) {
				printf("Input move(Black): ");
				scanf(" %s", move);
			}
			else {
				printf("Input move(White): ");
				scanf(" %s", move);
			}
		} while ((move[0]<'a' || 'f'<move[0]) || (move[1]<'1' || '6'<move[1])
			        || (move[2]<'1' || '4'<move[2]) || (move[3] != 'r' && move[3] != 'l'));

		move_pos[0] = (uint64_t)1 << (5 - move[0] + 'a' + 6 * (5 - move[1] + '1'));

	} while ((board->black&move_pos[0]) != 0 || (board->white&move_pos[0]) != 0);

	if (move[3] == 'r') {
		move_pos[1] = (uint64_t)(move[2] - '1') * 2;
	}
	else {
		move_pos[1] = (uint64_t)(move[2] - '1') * 2 + 1;
	}
}

// 盤面の出力
void OutputBoard(const BOARD *board)
{
	uint64_t pos = (uint64_t)1 << 35;

	printf(" abcdef\n");
	for (int i = 0;i<36;i++) {
		if (i % 6 == 0) {
			printf("%d", i / 6 + 1);
		}
		if ((board->black&pos) != 0) printf("X");
		else if ((board->white&pos) != 0) printf("O");
		else printf("-");

		if (i % 6 == 5) printf("\n");

		pos >>= 1;
	}

	printf("\n");
}

// コンピュータの思考
void ComThink(BOARD *board, uint64_t *move_pos, int level, int turn, float limit) {
	double var, start, end, hash_rate;
	CLUSTER cluster[3];

	node = 0;
	entry_count = 524288;


	for (int i = 0;i < 3;i++) {
		cluster[i].ttentry = (TTENTRY*)calloc(entry_count, sizeof(TTENTRY));
	}
	// 反復深化法
	start = clock();
	for (int i = 0;i < level;i++) {
		Init_history();
		var = NegaAlpha(board, move_pos, &cluster[0], -MY_INFINITY * 10, MY_INFINITY * 10, 0, i + 1);
		end = clock();
		hash_rate = 0;
		for (int j = 0;j<3;j++) {
			for (int i = 0;i<entry_count;i++) {
				if (cluster[j].ttentry[i].depth>0) hash_rate++;
			}
		}
		//printf("depth:%d, value:%f, node:%d, hash_rate:%f, time:%f\n", i + 1, var, node, (float)(hash_rate * 100) / (float)(entry_count * 3), (end - start) / CLOCKS_PER_SEC);
		Sort_history();
		// 読み切りなら探索終了
		if (var < -9000 || 9000 < var) break;
		// 思考時間制限を超えそうなら探索終了
		if ((end - start) / CLOCKS_PER_SEC * min(pow(2.0, i), (37 - turn) * 8) > limit) break;
	}

	for (int i = 0;i<3;i++) {
		DeleteTT(cluster[i].ttentry);
	}
}

// コンピュータの打ち手の出力
void OutputComMove(const uint64_t *move_pos) {
	uint64_t pos = 1;
	int i;

	for (i = 0;i<36;i++) {
		if ((move_pos[0]&pos) != 0) break;
		pos <<= 1;
	}

	printf("%d %d %d ", 5 - i % 6, 5 - i / 6, 3 - (int)move_pos[1] / 2);
	if (move_pos[1] % 2 == 0) printf("1\n");
	else printf("0\n");

	/* バッファのフラッシュ */
	fflush(stdout);
}

int main(void)
{
	int h, w, id;

	Init_Evaluate_function();
	InitXor();

	scanf("%d%d%d", &w, &h, &id);

	while (true) {
		int turn, time;
		char position[37];

		scanf("%d", &turn);
		scanf("%d", &time);

		for (int i = 0; i < 6; i++) {
			char pos[7] = "";
			scanf("%s", pos);
			for (int j = 0; j < 6; j++) position[i * 6 + j] = pos[j];
		}

		BOARD board;
		uint64_t move_pos[2];
		float limit[] = { 0.0111, 0.0225, 0.0345, 0.0476, 0.0625, 0.08, 0.101, 0.167,
			0.2, 0.222, 0.25, 0.286, 0.333, 0.4, 0.5, 0.667, 1 };

		if (turn % 2 == 1) SetBoard(&board, position, BLACK);
		else SetBoard(&board, position, WHITE);
		ComThink(&board, move_pos, 10, turn, time * limit[(turn - 1) / 2] * 0.001);
		OutputComMove(move_pos);
	}

	return 0;
}
