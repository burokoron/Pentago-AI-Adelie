#ifndef EVALUATION_H
#define EVALUATION_H

#include "board.h"

// ���s�t�@�C���̃p�X���擾
char *getmodulefilename();

//�@�]���֐���ǂݍ���
void Init_Evaluate_function();

//	�]���֐�����������
void Write_evaluate();

//�@�]���l��Ԃ�
double Evaluate(const BOARD *board);

#endif