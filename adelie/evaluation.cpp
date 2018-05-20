#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "evaluation.h"

float row1[729];
float row2[729];
float row3[729];
float srant1[729];
float srant2[243];

// 実行ファイルのパスを取得
char *getmodulefilename()
{
	static char buf[1024] = {};

	readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	return buf;
}

//　評価関数を読み込む
void Init_Evaluate_function() {
	FILE *fp;
	char *fpath;

	fpath = getmodulefilename();
	for (int i = strlen(fpath);i > 0; i--) {
		if (fpath[i] == '/') {
			fpath[i + 1] = '\0';
			break;
		}
	}
	fpath = strcat(fpath, "evaluation.bin");

	if ((fp = fopen(fpath, "rb")) == NULL) {
		printf("No file\n");
		exit(1);
	}

	for (int i = 0;i < 729;i++) fread(&row1[i], sizeof(float), 1, fp);
	for (int i = 0;i < 729;i++) fread(&row2[i], sizeof(float), 1, fp);
	for (int i = 0;i < 729;i++) fread(&row3[i], sizeof(float), 1, fp);
	for (int i = 0;i < 729;i++) fread(&srant1[i], sizeof(float), 1, fp);
	for (int i = 0;i < 243;i++) fread(&srant2[i], sizeof(float), 1, fp);

	fclose(fp);
}

//	評価関数を書き込む
void Write_evaluate() {
	FILE *fp;
	char *fpath;

	fpath = getmodulefilename();
	for (int i = strlen(fpath);i > 0; i--) {
		if (fpath[i] == '/') {
			fpath[i + 1] = '\0';
			break;
		}
	}
	fpath = strcat(fpath, "evaluation.bin");

	if ((fp = fopen(fpath, "wb+")) == NULL) {
		printf("No file\n");
		exit(1);
	}

	for (int i = 0;i < 729;i++) fwrite(&row1[i], sizeof(float), 1, fp);
	for (int i = 0;i < 729;i++) fwrite(&row2[i], sizeof(float), 1, fp);
	for (int i = 0;i < 729;i++) fwrite(&row3[i], sizeof(float), 1, fp);
	for (int i = 0;i < 729;i++) fwrite(&srant1[i], sizeof(float), 1, fp);
	for (int i = 0;i < 243;i++) fwrite(&srant2[i], sizeof(float), 1, fp);

	fclose(fp);
}

//　評価値を返す
double Evaluate(const BOARD *board) {
	double value = 0.0;
	int row1_index[4] = { 0 }, row2_index[4] = { 0 }, row3_index[4] = { 0 };
	int srant1_index[2] = { 0 }, srant2_index[4] = { 0 };

	if ((0x1 & board->black) != 0) {//f6
		row1_index[1] += 0xF3;
		row1_index[3] += 0xF3;
		srant1_index[1] += 0xF3;
	}
	else if ((0x1 & board->white) != 0) {
		row1_index[1] += 0x1E6;
		row1_index[3] += 0x1E6;
		srant1_index[1] += 0x1E6;
	}
	if ((0x2 & board->black) != 0) {//e6
		row1_index[1] += 0x51;
		row2_index[3] += 0xF3;
		srant2_index[3] += 0x51;
	}
	else if ((0x2 & board->white) != 0) {
		row1_index[1] += 0xA2;
		row2_index[3] += 0x1E6;
		srant2_index[3] += 0xA2;
	}
	if ((0x4 & board->black) != 0) {//d6
		row1_index[1] += 0x1B;
		row3_index[3] += 0xF3;
	}
	else if ((0x4 & board->white) != 0) {
		row1_index[1] += 0x36;
		row3_index[3] += 0x1E6;
	}
	if ((0x8 & board->black) != 0) {//c6
		row1_index[1] += 0x9;
		row3_index[2] += 0xF3;
	}
	else if ((0x8 & board->white) != 0) {
		row1_index[1] += 0x12;
		row3_index[2] += 0x1E6;
	}
	if ((0x10 & board->black) != 0) {//b6
		row1_index[1] += 0x3;
		row2_index[2] += 0xF3;
		srant2_index[1] += 0x51;
	}
	else if ((0x10 & board->white) != 0) {
		row1_index[1] += 0x6;
		row2_index[2] += 0x1E6;
		srant2_index[1] += 0xA2;
	}
	if ((0x20 & board->black) != 0) {///a6
		row1_index[1] += 0x1;
		row1_index[2] += 0xF3;
		srant1_index[0] += 0xF3;
	}
	else if ((0x20 & board->white) != 0) {
		row1_index[1] += 0x2;
		row1_index[2] += 0x1E6;
		srant1_index[0] += 0x1E6;
	}
	if ((0x40 & board->black) != 0) {//f5
		row2_index[1] += 0xF3;
		row1_index[3] += 0x51;
		srant2_index[2] += 0x51;
	}
	else if ((0x40 & board->white) != 0) {
		row2_index[1] += 0x1E6;
		row1_index[3] += 0xA2;
		srant2_index[2] += 0xA2;
	}
	if ((0x80 & board->black) != 0) {//e5
		row2_index[1] += 0x51;
		row2_index[3] += 0x51;
		srant1_index[1] += 0x51;
	}
	else if ((0x80 & board->white) != 0) {
		row2_index[1] += 0xA2;
		row2_index[3] += 0xA2;
		srant1_index[1] += 0xA2;
	}
	if ((0x100 & board->black) != 0) {//d5
		row2_index[1] += 0x1B;
		row3_index[3] += 0x51;
		srant2_index[3] += 0x1B;
	}
	else if ((0x100 & board->white) != 0) {
		row2_index[1] += 0x36;
		row3_index[3] += 0xA2;
		srant2_index[3] += 0x36;
	}
	if ((0x200 & board->black) != 0) {//c5
		row2_index[1] += 0x9;
		row3_index[2] += 0x51;
		srant2_index[1] += 0x1B;
	}
	else if ((0x200 & board->white) != 0) {
		row2_index[1] += 0x12;
		row3_index[2] += 0xA2;
		srant2_index[1] += 0x36;
	}
	if ((0x400 & board->black) != 0) {//b5
		row2_index[1] += 0x3;
		row2_index[2] += 0x51;
		srant1_index[0] += 0x51;
	}
	else if ((0x400 & board->white) != 0) {
		row2_index[1] += 0x6;
		row2_index[2] += 0xA2;
		srant1_index[0] += 0xA2;
	}
	if ((0x800 & board->black) != 0) {//a5
		row2_index[1] += 0x1;
		row1_index[2] += 0x51;
		srant2_index[0] += 0x51;
	}
	else if ((0x800 & board->white) != 0) {
		row2_index[1] += 0x2;
		row1_index[2] += 0xA2;
		srant2_index[0] += 0xA2;
	}
	if ((0x1000 & board->black) != 0) {//f4
		row3_index[1] += 0xF3;
		row1_index[3] += 0x1B;
	}
	else if ((0x1000 & board->white) != 0) {
		row3_index[1] += 0x1E6;
		row1_index[3] += 0x36;
	}
	if ((0x2000 & board->black) != 0) {//e4
		row3_index[1] += 0x51;
		row2_index[3] += 0x1B;
		srant2_index[2] += 0x1B;
	}
	else if ((0x2000 & board->white) != 0) {
		row3_index[1] += 0xA2;
		row2_index[3] += 0x36;
		srant2_index[2] += 0x36;
	}
	if ((0x4000 & board->black) != 0) {//d4
		row3_index[1] += 0x1B;
		row3_index[3] += 0x1B;
		srant2_index[1] += 0x9;
		srant1_index[1] += 0x1B;
	}
	else if ((0x4000 & board->white) != 0) {
		row3_index[1] += 0x36;
		row3_index[3] += 0x36;
		srant2_index[1] += 0x12;
		srant1_index[1] += 0x36;
	}
	if ((0x8000 & board->black) != 0) {//c4
		row3_index[1] += 0x9;
		row3_index[2] += 0x1B;
		srant1_index[0] += 0x1B;
		srant2_index[3] += 0x9;
	}
	else if ((0x8000 & board->white) != 0) {
		row3_index[1] += 0x12;
		row3_index[2] += 0x36;
		srant1_index[0] += 0x36;
		srant2_index[3] += 0x12;
	}
	if ((0x10000 & board->black) != 0) {//b4
		row3_index[1] += 0x3;
		row2_index[2] += 0x1B;
		srant2_index[0] += 0x1B;
	}
	else if ((0x10000 & board->white) != 0) {
		row3_index[1] += 0x6;
		row2_index[2] += 0x36;
		srant2_index[0] += 0x36;
	}
	if ((0x20000 & board->black) != 0) {//a4
		row3_index[1] += 0x1;
		row1_index[2] += 0x1B;
	}
	else if ((0x20000 & board->white) != 0) {
		row3_index[1] += 0x2;
		row1_index[2] += 0x36;
	}
	if ((0x40000 & board->black) != 0) {//f3
		row3_index[0] += 0xF3;
		row1_index[3] += 0x9;
	}
	else if ((0x40000 & board->white) != 0) {
		row3_index[0] += 0x1E6;
		row1_index[3] += 0x12;
	}
	if ((0x80000 & board->black) != 0) {//e3
		row3_index[0] += 0x51;
		row2_index[3] += 0x9;
		srant2_index[1] += 0x3;
	}
	else if ((0x80000 & board->white) != 0) {
		row3_index[0] += 0xA2;
		row2_index[3] += 0x12;
		srant2_index[1] += 0x6;
	}
	if ((0x100000 & board->black) != 0) {//d3
		row3_index[0] += 0x1B;
		row3_index[3] += 0x9;
		srant1_index[0] += 0x9;
		srant2_index[2] += 0x9;
	}
	else if ((0x100000 & board->white) != 0) {
		row3_index[0] += 0x36;
		row3_index[3] += 0x12;
		srant1_index[0] += 0x12;
		srant2_index[2] += 0x12;
	}
	if ((0x200000 & board->black) != 0) {//c3
		row3_index[0] += 0x9;
		row3_index[2] += 0x9;
		srant2_index[0] += 0x9;
		srant1_index[1] += 0x9;
	}
	else if ((0x200000 & board->white) != 0) {
		row3_index[0] += 0x12;
		row3_index[2] += 0x12;
		srant2_index[0] += 0x12;
		srant1_index[1] += 0x12;
	}
	if ((0x400000 & board->black) != 0) {//b3
		row3_index[0] += 0x3;
		row2_index[2] += 0x9;
		srant2_index[3] += 0x3;
	}
	else if ((0x400000 & board->white) != 0) {
		row3_index[0] += 0x6;
		row2_index[2] += 0x12;
		srant2_index[3] += 0x6;
	}
	if ((0x800000 & board->black) != 0) {//a3
		row3_index[0] += 0x1;
		row1_index[2] += 0x9;
	}
	else if ((0x800000 & board->white) != 0) {
		row3_index[0] += 0x2;
		row1_index[2] += 0x12;
	}
	if ((0x1000000 & board->black) != 0) {//f2
		row2_index[0] += 0xF3;
		row1_index[3] += 0x3;
		srant2_index[1] += 0x1;
	}
	else if ((0x1000000 & board->white) != 0) {
		row2_index[0] += 0x1E6;
		row1_index[3] += 0x6;
		srant2_index[1] += 0x2;
	}
	if ((0x2000000 & board->black) != 0) {//e2
		row2_index[0] += 0x51;
		row2_index[3] += 0x3;
		srant1_index[0] += 0x3;
	}
	else if ((0x2000000 & board->white) != 0) {
		row2_index[0] += 0xA2;
		row2_index[3] += 0x6;
		srant1_index[0] += 0x6;
	}
	if ((0x4000000 & board->black) != 0) {//d2
		row2_index[0] += 0x1B;
		row3_index[3] += 0x3;
		srant2_index[0] += 0x3;
	}
	else if ((0x4000000 & board->white) != 0) {
		row2_index[0] += 0x36;
		row3_index[3] += 0x6;
		srant2_index[0] += 0x6;
	}
	if ((0x8000000 & board->black) != 0) {//c2
		row2_index[0] += 0x9;
		row3_index[2] += 0x3;
		srant2_index[2] += 0x3;
	}
	else if ((0x8000000 & board->white) != 0) {
		row2_index[0] += 0x12;
		row3_index[2] += 0x6;
		srant2_index[2] += 0x6;
	}
	if ((0x10000000 & board->black) != 0) {//b2
		row2_index[0] += 0x3;
		row2_index[2] += 0x3;
		srant1_index[1] += 0x3;
	}
	else if ((0x10000000 & board->white) != 0) {
		row2_index[0] += 0x6;
		row2_index[2] += 0x6;
		srant1_index[1] += 0x6;
	}
	if ((0x20000000 & board->black) != 0) {//a2
		row2_index[0] += 0x1;
		row1_index[2] += 0x3;
		srant2_index[3] += 0x1;
	}
	else if ((0x20000000 & board->white) != 0) {
		row2_index[0] += 0x2;
		row1_index[2] += 0x6;
		srant2_index[3] += 0x2;
	}
	if ((0x40000000 & board->black) != 0) {//f1
		row1_index[0] += 0xF3;
		row1_index[3] += 0x1;
		srant1_index[0] += 0x1;
	}
	else if ((0x40000000 & board->white) != 0) {
		row1_index[0] += 0x1E6;
		row1_index[3] += 0x2;
		srant1_index[0] += 0x2;
	}
	if ((0x80000000 & board->black) != 0) {//e1
		row1_index[0] += 0x51;
		row2_index[3] += 0x1;
		srant2_index[0] += 0x1;
	}
	else if ((0x80000000 & board->white) != 0) {
		row1_index[0] += 0xA2;
		row2_index[3] += 0x2;
		srant2_index[0] += 0x2;
	}
	if ((0x100000000 & board->black) != 0) {//d1
		row1_index[0] += 0x1B;
		row3_index[3] += 0x1;
	}
	else if ((0x100000000 & board->white) != 0) {
		row1_index[0] += 0x36;
		row3_index[3] += 0x2;
	}
	if ((0x200000000 & board->black) != 0) {//c1
		row1_index[0] += 0x9;
		row3_index[2] += 0x1;
	}
	else if ((0x200000000 & board->white) != 0) {
		row1_index[0] += 0x12;
		row3_index[2] += 0x2;
	}
	if ((0x400000000 & board->black) != 0) {//b1
		row1_index[0] += 0x3;
		row2_index[2] += 0x1;
		srant2_index[2] += 0x1;
	}
	else if ((0x400000000 & board->white) != 0) {
		row1_index[0] += 0x6;
		row2_index[2] += 0x2;
		srant2_index[2] += 0x2;
	}
	if ((0x800000000 & board->black) != 0) {//a1
		row1_index[0] += 0x1;
		row1_index[2] += 0x1;
		srant1_index[1] += 0x1;
	}
	else if ((0x800000000 & board->white) != 0) {
		row1_index[0] += 0x2;
		row1_index[2] += 0x2;
		srant1_index[1] += 0x2;
	}

	for (int i = 0;i < 4;i++) value += row1[row1_index[i]];
	for (int i = 0;i < 4;i++) value += row2[row2_index[i]];
	for (int i = 0;i < 4;i++) value += row3[row3_index[i]];
	for (int i = 0;i < 2;i++) value += srant1[srant1_index[i]];
	for (int i = 0;i < 4;i++) value += srant2[srant2_index[i]];

	if (board->turn == BLACK) return value;
	else return -value;
}