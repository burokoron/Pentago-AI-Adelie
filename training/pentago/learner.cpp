#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "learner.h"

#define ETA 1.0

extern float row1[36][729];
extern float row2[36][729];
extern float row3[36][729];
extern float srant1[36][729];
extern float srant2[36][243];

float ori_row1[36][729];
float ori_row2[36][729];
float ori_row3[36][729];
float ori_srant1[36][729];
float ori_srant2[36][243];

long double grad_row1[36][729];
long double grad_row2[36][729];
long double grad_row3[36][729];
long double grad_srant1[36][729];
long double grad_srant2[36][243];

long double sum_loss;

// 学習
void Learn() {
	BOARD board;
	int count, epoch = 0, total;
	float value;
	double sum_margin;
	char pos[37];
	FILE *fp;

	Reset_evaluate();
	Write_evaluate();
	
	//ShuffleKifu("delpos.txt");

	Init_Grad();
	epoch = 0;
	do {
		Init_Evaluate_function();
		Copy_evaluate();
		if(epoch !=0 ) ShuffleKifu("tmp.txt");

		if (fopen_s(&fp, "tmp.txt", "r") != 0) {
			printf("Can't open tmp.txt!!\n");
			return;
		}
		total = 0;
		while (fscanf_s(fp, "%s", pos, 37) != -1) {
			fscanf_s(fp, "%f", &value);
			total++;
		}
		fclose(fp);
		if (fopen_s(&fp, "tmp.txt", "r") != 0) {
			printf("Can't open tmp.txt!!\n");
			return;
		}

		count = 0;
		sum_margin = 0.0;
		sum_loss = 0.0;
		epoch++;
		//printf("\nLearning...\n");
		for (int i = 0;i < total;i++) {
			fscanf_s(fp, "%s %f", pos, 37, &value);
			SetBoard(&board, pos, BLACK);

			Update_evaluate(&board, value);
		}
		fclose(fp);

		sum_margin = Evaluate_margin();
		printf("\nepoch = %5d, value_loss = %10.8lf, evaluate_margin = %15.8lf\n", epoch, sum_loss / (long double)total, sum_margin);
		Write_evaluate();
	} while (sum_margin > 0.01 && epoch < 3);
}

// 評価関数の初期化
void Reset_evaluate() {
	for (int j = 0;j < 36;j++) {
		for (int i = 0;i < 729;i++) row1[j][i] = 0.0;
		for (int i = 0;i < 729;i++) row2[j][i] = 0.0;
		for (int i = 0;i < 729;i++) row3[j][i] = 0.0;
		for (int i = 0;i < 729;i++) srant1[j][i] = 0.0;
		for (int i = 0;i < 243;i++) srant2[j][i] = 0.0;
	}
}

// 損失の初期化
void Init_Grad() {
	for (int j = 0;j < 36;j++) {
		for (int i = 0;i < 729;i++) grad_row1[j][i] = 0.0;
		for (int i = 0;i < 729;i++) grad_row2[j][i] = 0.0;
		for (int i = 0;i < 729;i++) grad_row3[j][i] = 0.0;
		for (int i = 0;i < 729;i++) grad_srant1[j][i] = 0.0;
		for (int i = 0;i < 243;i++) grad_srant2[j][i] = 0.0;
	}
}

// 評価関数のコピー
void Copy_evaluate() {
	for (int j = 0;j < 36;j++) {
		for (int i = 0;i < 729;i++) ori_row1[j][i] = row1[j][i];
		for (int i = 0;i < 729;i++) ori_row2[j][i] = row2[j][i];
		for (int i = 0;i < 729;i++) ori_row3[j][i] = row3[j][i];
		for (int i = 0;i < 729;i++) ori_srant1[j][i] = srant1[j][i];
		for (int i = 0;i < 243;i++) ori_srant2[j][i] = srant2[j][i];
	}
}

// 評価関数の更新
void Update_evaluate(const BOARD *board, float t_value) {
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

	for (int i = 0;i < 4;i++) value += row1[board->move_num][row1_index[i]];
	for (int i = 0;i < 4;i++) value += row2[board->move_num][row2_index[i]];
	for (int i = 0;i < 4;i++) value += row3[board->move_num][row3_index[i]];
	for (int i = 0;i < 2;i++) value += srant1[board->move_num][srant1_index[i]];
	for (int i = 0;i < 4;i++) value += srant2[board->move_num][srant2_index[i]];

	sum_loss += fabs(t_value - value);

	for (int i = 0;i < 4;i++) grad_row1[board->move_num][row1_index[i]] += (t_value - value)*(t_value - value);
	for (int i = 0;i < 4;i++) grad_row2[board->move_num][row2_index[i]] += (t_value - value)*(t_value - value);
	for (int i = 0;i < 4;i++) grad_row3[board->move_num][row3_index[i]] += (t_value - value)*(t_value - value);
	for (int i = 0;i < 2;i++) grad_srant1[board->move_num][srant1_index[i]] += (t_value - value)*(t_value - value);
	for (int i = 0;i < 4;i++) grad_srant2[board->move_num][srant2_index[i]] += (t_value - value)*(t_value - value);

	if (fabs(t_value - value) < 0.01) return;

	for (int i = 0;i < 4;i++) row1[board->move_num][row1_index[i]] -= (float)(ETA*(-(t_value - value) / sqrt(grad_row1[board->move_num][row1_index[i]])));
	for (int i = 0;i < 4;i++) row2[board->move_num][row2_index[i]] -= (float)(ETA*(-(t_value - value) / sqrt(grad_row2[board->move_num][row2_index[i]])));
	for (int i = 0;i < 4;i++) row3[board->move_num][row3_index[i]] -= (float)(ETA*(-(t_value - value) / sqrt(grad_row3[board->move_num][row3_index[i]])));
	for (int i = 0;i < 2;i++) srant1[board->move_num][srant1_index[i]] -= (float)(ETA*(-(t_value - value) / sqrt(grad_srant1[board->move_num][srant1_index[i]])));
	for (int i = 0;i < 4;i++) srant2[board->move_num][srant2_index[i]] -= (float)(ETA*(-(t_value - value) / sqrt(grad_srant2[board->move_num][srant2_index[i]])));
}

// 更新前後の評価関数の差
double Evaluate_margin() {
	double sum_margin = 0.0;

	for (int j = 0;j < 36;j++) {
		for (int i = 0;i < 729;i++) sum_margin += fabs(ori_row1[j][i] - row1[j][i]);
		for (int i = 0;i < 729;i++) sum_margin += fabs(ori_row2[j][i] - row2[j][i]);
		for (int i = 0;i < 729;i++) sum_margin += fabs(ori_row3[j][i] - row3[j][i]);
		for (int i = 0;i < 729;i++) sum_margin += fabs(ori_srant1[j][i] - srant1[j][i]);
		for (int i = 0;i < 243;i++) sum_margin += fabs(ori_srant2[j][i] - srant2[j][i]);
	}

	return sum_margin;
}

// 棋譜のシャッフル
void ShuffleKifu(char *name) {
	int flag = 0, total;
	float *many_value, value;
	char **many_pos, pos[37];
	FILE *fp1, *fp2, *fp3, *fp4, *fp5;

	if (fopen_s(&fp1, name, "r") != 0) {
		printf("Can't open %s!!\n", name);
		return;
	}
	if (fopen_s(&fp2, "tmp1.txt", "w") != 0) {
		printf("Can't open tmp1.txt!!\n");
		return;
	}
	if (fopen_s(&fp3, "tmp2.txt", "w") != 0) {
		printf("Can't open tmp2.txt!!\n");
		return;
	}
	if (fopen_s(&fp4, "tmp3.txt", "w") != 0) {
		printf("Can't open tmp3.txt!!\n");
		return;
	}
	if (fopen_s(&fp5, "tmp4.txt", "w") != 0) {
		printf("Can't open tmp4.txt!!\n");
		return;
	}
	printf("Shuffle_position...\n");
	while (fscanf_s(fp1, "%s", pos, 37) != -1) {
		fscanf_s(fp1, "%f", &value);
		if (flag == 0) {
			fprintf(fp2, "%s %f\n", pos, value);
			flag = 1;
		}
		else if(flag == 1) {
			fprintf(fp3, "%s %f\n", pos, value);
			flag = 2;
		}
		else if (flag == 2) {
			fprintf(fp4, "%s %f\n", pos, value);
			flag = 3;
		}
		else {
			fprintf(fp5, "%s %f\n", pos, value);
			flag = 0;
		}
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);

	if (fopen_s(&fp1, "tmp.txt", "w") != 0) {
		printf("Can't open tmp.txt!!\n");
		return;
	}
	if (fopen_s(&fp2, "tmp1.txt", "r") != 0) {
		printf("Can't open tmp1.txt!!\n");
		return;
	}
	total = 0;
	while (fscanf_s(fp2, "%s", pos, 37) != -1) {
		fscanf_s(fp2, "%f", &value);
		total++;
	}
	fclose(fp2);
	many_pos = (char **)calloc(total, sizeof(many_pos));
	if (many_pos == NULL) {
		printf("Error : many_pos = calloc(total, sizeof(many_pos));\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		many_pos[i] = (char *)calloc(37, sizeof(many_pos[i]));
		if (many_pos[i] == NULL) {
			printf("Error : many_pos[%d] = calloc(total, sizeof(many_pos[%d]));\n", i, i);
			return;
		}
	}
	many_value = (float *)calloc(total, sizeof(many_value));
	if (many_value == NULL) {
		printf("Error : many_value = calloc(total, sizeof(many_value));");
		return;
	}
	if (fopen_s(&fp2, "tmp1.txt", "r") != 0) {
		printf("Can't open tmp.txt!!\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		fscanf_s(fp2, "%s %f", many_pos[i], 37, &many_value[i]);
	}
	fclose(fp2);
	char tmp_pos[37];
	float tmp_value;
	int tmp;
	for (int i = 0;i < total;i++) {
		tmp = Xor128() % total;
		strcpy_s(tmp_pos, many_pos[i]);
		strcpy_s(many_pos[i], 37, many_pos[tmp]);
		strcpy_s(many_pos[tmp], 37, tmp_pos);
		tmp_value = many_value[i];
		many_value[i] = many_value[tmp];
		many_value[tmp] = tmp_value;
	}
	for (int i = 0;i < total;i++) fprintf(fp1, "%s %f\n", many_pos[i], many_value[i]);
	for (int i = 0;i < total;i++) free(many_pos[i]);
	free(many_pos);
	free(many_value);
	if (fopen_s(&fp2, "tmp2.txt", "r") != 0) {
		printf("Can't open tmp2.txt!!\n");
		return;
	}
	total = 0;
	while (fscanf_s(fp2, "%s", pos, 37) != -1) {
		fscanf_s(fp2, "%f", &value);
		total++;
	}
	fclose(fp2);
	many_pos = (char **)calloc(total, sizeof(many_pos));
	if (many_pos == NULL) {
		printf("Error : many_pos = calloc(total, sizeof(many_pos));\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		many_pos[i] = (char *)calloc(37, sizeof(many_pos[i]));
		if (many_pos[i] == NULL) {
			printf("Error : many_pos[%d] = calloc(total, sizeof(many_pos[%d]));\n", i, i);
			return;
		}
	}
	many_value = (float *)calloc(total, sizeof(many_value));
	if (many_value == NULL) {
		printf("Error : many_value = calloc(total, sizeof(many_value));");
		return;
	}
	if (fopen_s(&fp2, "tmp2.txt", "r") != 0) {
		printf("Can't open tmp.txt!!\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		fscanf_s(fp2, "%s %f", many_pos[i], 37, &many_value[i]);
	}
	fclose(fp2);
	for (int i = 0;i < total;i++) {
		tmp = Xor128() % total;
		strcpy_s(tmp_pos, many_pos[i]);
		strcpy_s(many_pos[i], 37, many_pos[tmp]);
		strcpy_s(many_pos[tmp], 37, tmp_pos);
		tmp_value = many_value[i];
		many_value[i] = many_value[tmp];
		many_value[tmp] = tmp_value;
	}
	for (int i = 0;i < total;i++) fprintf(fp1, "%s %f\n", many_pos[i], many_value[i]);
	for (int i = 0;i < total;i++) free(many_pos[i]);
	free(many_pos);
	free(many_value);
	if (fopen_s(&fp3, "tmp3.txt", "r") != 0) {
		printf("Can't open tmp3.txt!!\n");
		return;
	}
	total = 0;
	while (fscanf_s(fp3, "%s", pos, 37) != -1) {
		fscanf_s(fp3, "%f", &value);
		total++;
	}
	fclose(fp3);
	many_pos = (char **)calloc(total, sizeof(many_pos));
	if (many_pos == NULL) {
		printf("Error : many_pos = calloc(total, sizeof(many_pos));\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		many_pos[i] = (char *)calloc(37, sizeof(many_pos[i]));
		if (many_pos[i] == NULL) {
			printf("Error : many_pos[%d] = calloc(total, sizeof(many_pos[%d]));\n", i, i);
			return;
		}
	}
	many_value = (float *)calloc(total, sizeof(many_value));
	if (many_value == NULL) {
		printf("Error : many_value = calloc(total, sizeof(many_value));");
		return;
	}
	if (fopen_s(&fp3, "tmp3.txt", "r") != 0) {
		printf("Can't open tmp3.txt!!\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		fscanf_s(fp3, "%s %f", many_pos[i], 37, &many_value[i]);
	}
	fclose(fp3);
	for (int i = 0;i < total;i++) {
		tmp = Xor128() % total;
		strcpy_s(tmp_pos, many_pos[i]);
		strcpy_s(many_pos[i], 37, many_pos[tmp]);
		strcpy_s(many_pos[tmp], 37, tmp_pos);
		tmp_value = many_value[i];
		many_value[i] = many_value[tmp];
		many_value[tmp] = tmp_value;
	}
	for (int i = 0;i < total;i++) fprintf(fp1, "%s %f\n", many_pos[i], many_value[i]);
	for (int i = 0;i < total;i++) free(many_pos[i]);
	free(many_pos);
	free(many_value);
	if (fopen_s(&fp4, "tmp4.txt", "r") != 0) {
		printf("Can't open tmp4.txt!!\n");
		return;
	}
	total = 0;
	while (fscanf_s(fp4, "%s", pos, 37) != -1) {
		fscanf_s(fp4, "%f", &value);
		total++;
	}
	fclose(fp4);
	many_pos = (char **)calloc(total, sizeof(many_pos));
	if (many_pos == NULL) {
		printf("Error : many_pos = calloc(total, sizeof(many_pos));\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		many_pos[i] = (char *)calloc(37, sizeof(many_pos[i]));
		if (many_pos[i] == NULL) {
			printf("Error : many_pos[%d] = calloc(total, sizeof(many_pos[%d]));\n", i, i);
			return;
		}
	}
	many_value = (float *)calloc(total, sizeof(many_value));
	if (many_value == NULL) {
		printf("Error : many_value = calloc(total, sizeof(many_value));");
		return;
	}
	if (fopen_s(&fp4, "tmp4.txt", "r") != 0) {
		printf("Can't open tmp4.txt!!\n");
		return;
	}
	for (int i = 0;i < total;i++) {
		fscanf_s(fp4, "%s %f", many_pos[i], 37, &many_value[i]);
	}
	fclose(fp4);
	for (int i = 0;i < total;i++) {
		tmp = Xor128() % total;
		strcpy_s(tmp_pos, many_pos[i]);
		strcpy_s(many_pos[i], 37, many_pos[tmp]);
		strcpy_s(many_pos[tmp], 37, tmp_pos);
		tmp_value = many_value[i];
		many_value[i] = many_value[tmp];
		many_value[tmp] = tmp_value;
	}
	for (int i = 0;i < total;i++) fprintf(fp1, "%s %f\n", many_pos[i], many_value[i]);
	fclose(fp1);
	for (int i = 0;i < total;i++) free(many_pos[i]);
	free(many_pos);
	free(many_value);
}