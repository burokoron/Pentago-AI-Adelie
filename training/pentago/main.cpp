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
				scanf_s(" %s", move, 5);
			}
			else {
				printf("Input move(White): ");
				scanf_s(" %s", move, 5);
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

// 人間vs人間
void ManVsMan(void)
{
	BOARD board;
	uint64_t move_pos[2];
	int winner_flag;

	InitBoard(&board);

	OutputBoard(&board);

	do {
		Input(&board, move_pos);
		DoMoveHash(&board, move_pos);
		OutputBoard(&board);
		winner_flag = EndJudge(&board);
	} while (winner_flag == 0);

	if (winner_flag == BLACK) printf("BLACK WIN\n");
	if (winner_flag == WHITE) printf("WHITE WIN\n");
	if (winner_flag == 3) printf("DRAW\n");
}

// コンピュータの思考
void ComThink(BOARD *board, uint64_t *move_pos, int level) {
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
		printf("depth:%d, value:%f, node:%d, hash_rate:%f, time:%f\n", i + 1, var, node, (float)(hash_rate * 100) / (float)(entry_count * 3), (end - start) / CLOCKS_PER_SEC);
		Sort_history();
		// 読み切りなら探索終了
		if (var < -9000 || 9000 < var) break;
		if (node > 1000000) break;
	}

	for (int i = 0;i<3;i++) {
		DeleteTT(cluster[i].ttentry);
	}
}

// コンピュータの打ち手の出力
void OutputComMove(const BOARD *board, const uint64_t *move_pos) {
	uint64_t pos = 1;
	int i;
	for (i = 0;i<36;i++) {
		if ((move_pos[0]&pos) != 0) break;
		pos <<= 1;
	}

	if (board->turn == BLACK) printf("BLACK move: ");
	else printf("WHITE move: ");
	printf("%c%d%d", 5 - i % 6 + 'a', 6 - i / 6, (int)move_pos[1] / 2 + 1);
	if (move_pos[1] % 2 == 0) printf("r\n");
	else printf("l\n");
}

// 人間vsコンピュータ
void Man_vs_com(int level) {
	BOARD board;
	uint64_t move_pos[2];
	int turn_man, winner_flag;

	InitBoard(&board);
	srand((unsigned)time(NULL));
	if (rand() % 2 == 0) turn_man = BLACK;
	else turn_man = WHITE;

	OutputBoard(&board);
	do {
		if (board.turn == turn_man) Input(&board, move_pos);
		else {
			ComThink(&board, move_pos, level);
			OutputComMove(&board, move_pos);
		}
		DoMoveHash(&board, move_pos);
		OutputBoard(&board);
		winner_flag = EndJudge(&board);
	} while (winner_flag == 0);

	if (winner_flag == BLACK) printf("BLACK WIN\n");
	if (winner_flag == WHITE) printf("WHITE WIN\n");
	if (winner_flag == 3) printf("DRAW\n");
}

// コンピュータvsコンピュータ
int Com_vs_com(int com_1_level, int com_2_level, FILE *file) {
	BOARD board;
	uint64_t move_pos[2];
	int winner_flag;

	InitBoard(&board);

	//OutputBoard(&board);

	for (int j = 0;j < 4;j++) {
		RandomMove(&board, move_pos);
		uint64_t pos = 1;
		int i;
		for (i = 0;i < 36;i++) {
			if ((move_pos[0] & pos) != 0) break;
			pos <<= 1;
		}
		fprintf(file, "%c%d%d", 5 - i % 6 + 'a', 6 - i / 6, (int)move_pos[1] / 2 + 1);
		if (move_pos[1] % 2 == 0) fprintf(file, "r");
		else fprintf(file, "l");
		DoMoveHash(&board, move_pos);
	}

	do {
		if (board.turn == BLACK) ComThink(&board, move_pos, com_1_level);
		else ComThink(&board, move_pos, com_2_level);
		OutputComMove(&board, move_pos);
		uint64_t pos = 1;
		int i;
		for (i = 0;i<36;i++) {
			if ((move_pos[0] & pos) != 0) break;
			pos <<= 1;
		}
		fprintf(file, "%c%d%d", 5 - i % 6 + 'a', 6 - i / 6, (int)move_pos[1] / 2 + 1);
		if (move_pos[1] % 2 == 0) fprintf(file, "r");
		else fprintf(file, "l");
		DoMoveHash(&board, move_pos);
		OutputBoard(&board);
		winner_flag = EndJudge(&board);
	} while (winner_flag == 0);

	if (winner_flag == BLACK) fprintf(file, " 100\n");
	else if (winner_flag == WHITE) fprintf(file, " -100\n");
	else fprintf(file, " 0\n");

	return winner_flag;
}

int main(void)
{
	int number, level[2], winner, win[2], game;
	double start, end;
	char str[100], f_name[100];
	FILE *file;

	Init_Evaluate_function();
	InitXor();

	do {
		do {
			printf("1. MAN VS MAN  2. MAN VS COM  3. COM VS COM 4. Learn 5. Benchmark\n");
			printf("Input number: ");
			scanf_s("%s", str, 100);
			number = atoi(str);
		} while (number<1 || 5<number);

		switch (number) {
		case 1:
			ManVsMan();
			break;
		case 2:
			do {
				printf("COMのレベル(1-34)を入力: ");
				scanf_s("%s", str, 100);
				level[0] = atoi(str);
			} while (level[0]<1 || 34<level[0]);
			Man_vs_com(level[0]);
			break;
		case 3:
			do {
				printf("COM_1のレベル(1-34)を入力: ");
				scanf_s("%s", str, 100);
				level[0] = atoi(str);
			} while (level[0]<1 || 34<level[0]);
			do {
				printf("COM_2のレベル(1-34)を入力: ");
				scanf_s("%s", str, 100);
				level[1] = atoi(str);
			} while (level[1]<1 || 34<level[1]);
			do {
				printf("対局数(偶数)を入力: ");
				scanf_s("%s", str, 100);
				game = atoi(str);
			} while (game % 2 == 1);
			printf("File name: ");
			scanf_s("%s", f_name, 100);

			fopen_s(&file, f_name, "w");

			win[0] = 0;
			win[1] = 0;

			start = clock();
			for (int i = 0;i<game / 2;i++) {
				winner = Com_vs_com(level[0], level[1], file);
				switch (winner) {
				case BLACK:
					win[0]++;
					printf("BLACK:%dwin\n", win[0]);
					break;
				case WHITE:
					win[1]++;
					printf("WHITE:%dwin\n", win[1]);
					break;
				}
			}
			for (int i = 0;i < game / 2;i++) {
				winner = Com_vs_com(level[1], level[0], file);
				switch (winner) {
				case BLACK:
					win[1]++;
					printf("BLACK:%dwin\n", win[1]);
					break;
				case WHITE:
					win[0]++;
					printf("WHITE:%dwin\n", win[0]);
					break;
				}
			}
			end = clock();
			printf("COM_1: %d勝\n", win[0]);
			printf("COM_2: %d勝\n", win[1]);
			printf("引分 : %d回\n", game - win[0] - win[1]);
			printf("総思考時間: %f\n", (end - start) / CLOCKS_PER_SEC);

			fclose(file);
			break;
		case 4:
			Learn();
			break;
		/*case 5:
			ffo();
			break;*/
		}

		do {
			printf("Retry? (y/n): ");
			scanf_s(" %s", str, 100);
			printf("\n");
		} while (str[0] != 'y'&&str[0] != 'n');
	} while (str[0] == 'y');

	return 0;
}
