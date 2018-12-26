#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "board.h"

void OutputBoard(const BOARD *board)
{
	uint64_t pos = (uint64_t)1 << 35;

	printf(" abcdef\n");
	for (int i = 0; i<36; i++) {
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
};

int main() {
	FILE *fp1, *fp2;
	char kifu[500];
	float value;
	int total_kifu = 0, total_pos = 0;
	BOARD board;
	uint64_t move_pos[2];

	fopen_s(&fp2, "pos.txt", "w");
	for (char k = '1'; k < '2'; k++) {
		char name[] = "kifu0.txt";
		name[4] = k;
		fopen_s(&fp1, name, "r");
		while (fscanf_s(fp1, "%s", kifu, 500) != -1) {
			fscanf_s(fp1, "%f", &value);
			total_kifu++;
			if (total_kifu % 10000 == 0) printf(".");

			InitBoard(&board);
			for (int i = 0; i < strlen(kifu) - 4; i += 4) {
				char move[5];
				for (int j = 0; j < 4; j++) {
					move[j] = kifu[i + j];
				}
				move_pos[0] = (uint64_t)1 << (5 - move[0] + 'a' + 6 * (5 - move[1] + '1'));
				if (move[3] == 'r') {
					move_pos[1] = (uint64_t)(move[2] - '1') * 2;
				}
				else {
					move_pos[1] = (uint64_t)(move[2] - '1') * 2 + 1;
				}

				DoMoveHash(&board, move_pos);
				total_pos++;
				uint64_t pos = (uint64_t)1 << 35;
				for (int j = 0; j < 36; j++) {
					if ((board.black&pos) != 0) fprintf(fp2, "X");
					else if ((board.white&pos) != 0) fprintf(fp2, "O");
					else fprintf(fp2, "-");
					pos >>= 1;
				}
				fprintf(fp2, " %f\n", value * ((float)(i + 4) / (strlen(kifu) - 4)));
			}
		}printf("\ntotal_kifu : %d, total_pos : %d\n", total_kifu, total_pos);
		fclose(fp1);
	}
	fclose(fp2);

	return 0;
}