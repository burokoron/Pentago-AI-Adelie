#ifndef LEARNER_H
#define LEARNER_H

#include "board.h"
#include "evaluation.h"
#include "search.h"

// �w�K
void Learn();

// �]���֐��̏�����
void Reset_evaluate();

// �����̏�����
void Init_Grad();

// �]���֐��̃R�s�[
void Copy_evaluate();

// �]���֐��̍X�V
void Update_evaluate(const BOARD *board, float t_value);

// �X�V�O��̕]���֐��̍�
double Evaluate_margin();

// �����̃V���b�t��
void ShuffleKifu(char *name);

#endif LEARNER_H