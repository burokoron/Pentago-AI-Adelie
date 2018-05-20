#ifndef EVALUATION_H
#define EVALUATION_H

#include "board.h"

// 実行ファイルのパスを取得
char *getmodulefilename();

//　評価関数を読み込む
void Init_Evaluate_function();

//	評価関数を書き込む
void Write_evaluate();

//　評価値を返す
double Evaluate(const BOARD *board);

#endif