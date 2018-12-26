#ifndef BOARD_H
#define BOARD_H

//手番
enum TURN {
	BLACK = 1, //先手
	WHITE = 2 //後手
};

// 盤面
typedef struct BOARD {
	uint64_t black, white; //石の配置
	int turn; //手番
	int move_num; // 手数
	uint64_t key; //ハッシュ値
}BOARD;

// 盤面の初期化
void InitBoard(BOARD *board);

// 盤面を指定局面に初期化
void SetBoard(BOARD *board, const char *pos, int turn);

// ゲーム終了判定
int EndJudge(const BOARD *board);

// 第一象限を右回転
void RotateFirstQuadrantClockwise(BOARD *board);

// 第一象限を左回転
void RotateFirstQuadrantCounterClockwise(BOARD *board);

// 第二象限を右回転
void RotateSecondQuadrantClockwise(BOARD *board);

// 第二象限を左回転
void RotateSecondQuadrantCounterClockwise(BOARD *board);

// 第三象限を右回転
void RotateThirdQuadrantClockwise(BOARD *board);

// 第三象限を左回転
void RotateThirdQuadrantCounterClockwise(BOARD *board);

// 第四象限を右回転
void RotateFourthQuadrantClockwise(BOARD *board);

// 第四象限を左回転
void RotateFourthQuadrantCounterClockwise(BOARD *board);

// ハッシュ値の計算
void SetHash(BOARD *board);

// 1手進める(hash有り)
void DoMoveHash(BOARD *board, uint64_t *move_pos);

// 1手戻す(hash有り)
void UnDoMoveHash(BOARD *board, uint64_t *move_pos);

// 1手進める(hash無し)
void DoMoveNoHash(BOARD *board, uint64_t *move_pos);

// 1手戻す(hash無し)
void UnDoMoveNoHash(BOARD *board, uint64_t *move_pos);

#endif