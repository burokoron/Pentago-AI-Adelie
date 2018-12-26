#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	FILE *fp1, *fp2;
	char pos[37], tmp[37];
	float value;
	int total_pos = 0;

	fopen_s(&fp2, "augpos.txt", "w");
	fopen_s(&fp1, "pos.txt", "r");
	while (fscanf_s(fp1, "%s", pos, 37) != -1) {
		fscanf_s(fp1, "%f", &value);
		total_pos++;
		if (total_pos % 10000 == 0) printf(".");

		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 37;j++) tmp[j] = pos[j];
			for (int j = 0;j < 6;j++) pos[j] = tmp[30 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 6] = tmp[31 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 12] = tmp[32 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 18] = tmp[33 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 24] = tmp[34 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 30] = tmp[35 - j * 6];
			fprintf(fp2, "%s %f\n", pos, value);
		}

		for (int j = 0;j < 37;j++) tmp[j] = pos[j];
		for (int i = 0;i < 36;i++) pos[i] = tmp[i / 6 + i % 6 * 6];

		for (int i = 0;i < 4;i++) {
			for (int j = 0;j < 37;j++) tmp[j] = pos[j];
			for (int j = 0;j < 6;j++) pos[j] = tmp[30 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 6] = tmp[31 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 12] = tmp[32 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 18] = tmp[33 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 24] = tmp[34 - j * 6];
			for (int j = 0;j < 6;j++) pos[j + 30] = tmp[35 - j * 6];
			fprintf(fp2, "%s %f\n", pos, value);
		}
	}
	fclose(fp1);
	fclose(fp2);

	return 0;
}