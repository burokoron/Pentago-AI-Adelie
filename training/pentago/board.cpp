#include <stdio.h>
#include <stdint.h>

#include "board.h"

uint64_t board_key[2][36]={
	0x288952392e80456c, 0x1161f85b24b8f054,
	0x59a21c01b8c164f1, 0x5f7d93624b43595,
	0xa507464b7954d7d, 0x1119be3313e351f6,
	0x71daa2936dce7405, 0x630c56bff30c3f67,
	0x4e1eaf3823351e0, 0x2f7d819e100fd85d,
	0x2b042f9d5036257, 0x31a3c0385ced6109,
	0x22e8ad043d51e261, 0x4dd09626c9750d9c,
	0x7961f644b6540803, 0x8ad34aec2df63c3,
	0x2a6a08399bcf3745, 0x19c2491ca2ec0e4a,
	0x12dfb82cd42102f8, 0x7a793242dafd04cc,
	0x4aab267bba09619c, 0x37617150f13c0497,
	0x59c8668cd9746f33, 0x71d5b54b6b81e8d2,
	0x3f37a916c160bbab, 0x282a5f65748503b,
	0x41664fac35a214ad, 0x51948a1171ac0afb,
	0x7e37045d2e20212, 0x3d03d5ca752312bb,
	0xafbefebf6fe9127, 0x33a9d59b191c73d,
	0x1b0f6edf4d24550a, 0x68ea836b945d5994,
	0x2d91cd4b22afecab, 0x18a79bc6e6cc319c,
	0x5b6a720ebed3e728, 0x1801ca3e47d0faab,
	0x11fdc9640335e3e, 0x5103cefac1ff6cdb,
	0x34e2a8acc659847, 0x437fee9ddab1bb13,
	0x1dbc18bd51371f4b, 0x1e3d3b552ba5e983,
	0x7f3adc311059f92, 0x60c4180f12187c8f,
	0x666010f3291481f7, 0x349c8fc3782e8dd3,
	0x723b322ce121f080, 0x5ce15765ea8e1ba5,
	0x2b53f3053ce4823c, 0x2f407ab64fa2a1ca,
	0x18212d65fe46c2d1, 0x4d256db38bc8294e,
	0x749d0109f432a836, 0x37f5b2339c8a7626,
	0x1c4dcaaf2efe2596, 0xdbaef3f7886b73,
	0x30711013b8ae1228, 0x67da0d560ea7b128,
	0x444e6767496b5b7a, 0x38c1770ae9d18d68,
	0x223d86676cb55f56, 0x49a448a5edef1272,
	0x59893a3a103b5b59, 0x65eaf2fda8bca0ee,
	0x1b656612dcab9d21, 0x4b1f6e30b1defcb6,
	0x70183b7a9196ddff, 0xbd2b8032fac2793,
	0x4a0ba83f79e41a51, 0x78f6a7269582329f
}; // 盤面のハッシュ値
uint64_t turn_key = 0x5dcb464576269bdb; // 手番のハッシュ値

//盤面の初期化
void InitBoard(BOARD *board) {
	board->key = 0x622b24abed22bd34;
	board->black = (uint64_t)0;
	board->white = (uint64_t)0;
	board->turn = BLACK;
	board->move_num = 0;
}

// 盤面を指定局面に初期化
void SetBoard(BOARD *board, const char *pos, int turn) {
	board->turn = turn;
	board->key = 0x622b24abed22bd34;
	board->black = (uint64_t)0;
	board->white = (uint64_t)0;
	board->move_num = 0;

	for (int i = 0;i<36;i++) {
		if (pos[i] == 'O') {
			board->white |= ((uint64_t)1 << (35 - i));
			board->key ^= board_key[WHITE - 1][35 - i];
			board->move_num++;
		}
		else if (pos[i] == 'X') {
			board->black |= ((uint64_t)1 << (35 - i));
			board->key ^= board_key[BLACK - 1][35 - i];
			board->move_num++;
		}
	}
	if (board->turn == WHITE) board->key ^= turn_key;
}

// ゲーム終了判定
int EndJudge(const BOARD *board) {
	uint64_t pos;
	int b = 0, w = 0;

	if ((board->black | board->white) == 0xFFFFFFFFF) return 3;

	// 横
	pos = 0x1F;
	for (int i = 0;i < 6;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 6;
	}
	pos = 0x3E;
	for (int i = 0;i < 6;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 6;
	}
	// 縦
	pos = 0x1041041;
	for (int i = 0;i < 6;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 1;
	}
	pos = 0x41041040;
	for (int i = 0;i < 6;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 1;
	}
	// 斜め
	pos = 0x1084210;
	for (int i = 0;i < 2;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 1;
	}
	pos = 0x10204081;
	for (int i = 0;i < 2;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 1;
	}
	pos = 0x42108400;
	for (int i = 0;i < 2;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 1;
	}
	pos = 0x408102040;
	for (int i = 0;i < 2;i++) {
		if ((board->black & pos) == pos) b = 1;
		if ((board->white & pos) == pos) w = 1;
		pos = pos << 1;
	}

	if (b == 0 && w == 0) return 0;
	if (b == 0 && w == 1) return WHITE;
	if (b == 1 && w == 0) return BLACK;
	else return 3;
}

// 第一象限を右回転
void RotateFirstQuadrantClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0xFFFFF8EB8;
	board->white = tmp.white & 0xFFFFF8EB8;

	board->black ^= (tmp.black & 0x1) << 2;
	board->white ^= (tmp.white & 0x1) << 2;
	board->black ^= (tmp.black & 0x2) << 7;
	board->white ^= (tmp.white & 0x2) << 7;
	board->black ^= (tmp.black & 0x4) << 12;
	board->white ^= (tmp.white & 0x4) << 12;
	board->black ^= (tmp.black & 0x40) >> 5;
	board->white ^= (tmp.white & 0x40) >> 5;
	board->black ^= (tmp.black & 0x100) << 5;
	board->white ^= (tmp.white & 0x100) << 5;
	board->black ^= (tmp.black & 0x1000) >> 12;
	board->white ^= (tmp.white & 0x1000) >> 12;
	board->black ^= (tmp.black & 0x2000) >> 7;
	board->white ^= (tmp.white & 0x2000) >> 7;
	board->black ^= (tmp.black & 0x4000) >> 2;
	board->white ^= (tmp.white & 0x4000) >> 2;
}

// 第一象限を左回転
void RotateFirstQuadrantCounterClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0xFFFFF8EB8;
	board->white = tmp.white & 0xFFFFF8EB8;

	board->black ^= (tmp.black & 0x1) << 12;
	board->white ^= (tmp.white & 0x1) << 12;
	board->black ^= (tmp.black & 0x2) << 5;
	board->white ^= (tmp.white & 0x2) << 5;
	board->black ^= (tmp.black & 0x4) >> 2;
	board->white ^= (tmp.white & 0x4) >> 2;
	board->black ^= (tmp.black & 0x40) << 7;
	board->white ^= (tmp.white & 0x40) << 7;
	board->black ^= (tmp.black & 0x100) >> 7;
	board->white ^= (tmp.white & 0x100) >> 7;
	board->black ^= (tmp.black & 0x1000) << 2;
	board->white ^= (tmp.white & 0x1000) << 2;
	board->black ^= (tmp.black & 0x2000) >> 5;
	board->white ^= (tmp.white & 0x2000) >> 5;
	board->black ^= (tmp.black & 0x4000) >> 12;
	board->white ^= (tmp.white & 0x4000) >> 12;
}

// 第二象限を右回転
void RotateSecondQuadrantClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0xFFFFC75C7;
	board->white = tmp.white & 0xFFFFC75C7;

	board->black ^= (tmp.black & 0x8) << 2;
	board->white ^= (tmp.white & 0x8) << 2;
	board->black ^= (tmp.black & 0x10) << 7;
	board->white ^= (tmp.white & 0x10) << 7;
	board->black ^= (tmp.black & 0x20) << 12;
	board->white ^= (tmp.white & 0x20) << 12;
	board->black ^= (tmp.black & 0x200) >> 5;
	board->white ^= (tmp.white & 0x200) >> 5;
	board->black ^= (tmp.black & 0x800) << 5;
	board->white ^= (tmp.white & 0x800) << 5;
	board->black ^= (tmp.black & 0x8000) >> 12;
	board->white ^= (tmp.white & 0x8000) >> 12;
	board->black ^= (tmp.black & 0x10000) >> 7;
	board->white ^= (tmp.white & 0x10000) >> 7;
	board->black ^= (tmp.black & 0x20000) >> 2;
	board->white ^= (tmp.white & 0x20000) >> 2;
}

// 第二象限を左回転
void RotateSecondQuadrantCounterClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0xFFFFC75C7;
	board->white = tmp.white & 0xFFFFC75C7;

	board->black ^= (tmp.black & 0x8) << 12;
	board->white ^= (tmp.white & 0x8) << 12;
	board->black ^= (tmp.black & 0x10) << 5;
	board->white ^= (tmp.white & 0x10) << 5;
	board->black ^= (tmp.black & 0x20) >> 2;
	board->white ^= (tmp.white & 0x20) >> 2;
	board->black ^= (tmp.black & 0x200) << 7;
	board->white ^= (tmp.white & 0x200) << 7;
	board->black ^= (tmp.black & 0x800) >> 7;
	board->white ^= (tmp.white & 0x800) >> 7;
	board->black ^= (tmp.black & 0x8000) << 2;
	board->white ^= (tmp.white & 0x8000) << 2;
	board->black ^= (tmp.black & 0x10000) >> 5;
	board->white ^= (tmp.white & 0x10000) >> 5;
	board->black ^= (tmp.black & 0x20000) >> 12;
	board->white ^= (tmp.white & 0x20000) >> 12;
}

// 第三象限を右回転
void RotateThirdQuadrantClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0xE3AE3FFFF;
	board->white = tmp.white & 0xE3AE3FFFF;

	board->black ^= (tmp.black & 0x40000) << 2;
	board->white ^= (tmp.white & 0x40000) << 2;
	board->black ^= (tmp.black & 0x80000) << 7;
	board->white ^= (tmp.white & 0x80000) << 7;
	board->black ^= (tmp.black & 0x100000) << 12;
	board->white ^= (tmp.white & 0x100000) << 12;
	board->black ^= (tmp.black & 0x1000000) >> 5;
	board->white ^= (tmp.white & 0x1000000) >> 5;
	board->black ^= (tmp.black & 0x4000000) << 5;
	board->white ^= (tmp.white & 0x4000000) << 5;
	board->black ^= (tmp.black & 0x40000000) >> 12;
	board->white ^= (tmp.white & 0x40000000) >> 12;
	board->black ^= (tmp.black & 0x80000000) >> 7;
	board->white ^= (tmp.white & 0x80000000) >> 7;
	board->black ^= (tmp.black & 0x100000000) >> 2;
	board->white ^= (tmp.white & 0x100000000) >> 2;
}

// 第三象限を左回転
void RotateThirdQuadrantCounterClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0xE3AE3FFFF;
	board->white = tmp.white & 0xE3AE3FFFF;

	board->black ^= (tmp.black & 0x40000) << 12;
	board->white ^= (tmp.white & 0x40000) << 12;
	board->black ^= (tmp.black & 0x80000) << 5;
	board->white ^= (tmp.white & 0x80000) << 5;
	board->black ^= (tmp.black & 0x100000) >> 2;
	board->white ^= (tmp.white & 0x100000) >> 2;
	board->black ^= (tmp.black & 0x1000000) << 7;
	board->white ^= (tmp.white & 0x1000000) << 7;
	board->black ^= (tmp.black & 0x4000000) >> 7;
	board->white ^= (tmp.white & 0x4000000) >> 7;
	board->black ^= (tmp.black & 0x40000000) << 2;
	board->white ^= (tmp.white & 0x40000000) << 2;
	board->black ^= (tmp.black & 0x80000000) >> 5;
	board->white ^= (tmp.white & 0x80000000) >> 5;
	board->black ^= (tmp.black & 0x100000000) >> 12;
	board->white ^= (tmp.white & 0x100000000) >> 12;
}

// 第四象限を右回転
void RotateFourthQuadrantClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0x1D71FFFFF;
	board->white = tmp.white & 0x1D71FFFFF;

	board->black ^= (tmp.black & 0x200000) << 2;
	board->white ^= (tmp.white & 0x200000) << 2;
	board->black ^= (tmp.black & 0x400000) << 7;
	board->white ^= (tmp.white & 0x400000) << 7;
	board->black ^= (tmp.black & 0x800000) << 12;
	board->white ^= (tmp.white & 0x800000) << 12;
	board->black ^= (tmp.black & 0x8000000) >> 5;
	board->white ^= (tmp.white & 0x8000000) >> 5;
	board->black ^= (tmp.black & 0x20000000) << 5;
	board->white ^= (tmp.white & 0x20000000) << 5;
	board->black ^= (tmp.black & 0x200000000) >> 12;
	board->white ^= (tmp.white & 0x200000000) >> 12;
	board->black ^= (tmp.black & 0x400000000) >> 7;
	board->white ^= (tmp.white & 0x400000000) >> 7;
	board->black ^= (tmp.black & 0x800000000) >> 2;
	board->white ^= (tmp.white & 0x800000000) >> 2;
}

// 第四象限を左回転
void RotateFourthQuadrantCounterClockwise(BOARD *board) {
	BOARD tmp;

	tmp.black = board->black;
	tmp.white = board->white;
	board->black = tmp.black & 0x1D71FFFFF;
	board->white = tmp.white & 0x1D71FFFFF;

	board->black ^= (tmp.black & 0x200000) << 12;
	board->white ^= (tmp.white & 0x200000) << 12;
	board->black ^= (tmp.black & 0x400000) << 5;
	board->white ^= (tmp.white & 0x400000) << 5;
	board->black ^= (tmp.black & 0x800000) >> 2;
	board->white ^= (tmp.white & 0x800000) >> 2;
	board->black ^= (tmp.black & 0x8000000) << 7;
	board->white ^= (tmp.white & 0x8000000) << 7;
	board->black ^= (tmp.black & 0x20000000) >> 7;
	board->white ^= (tmp.white & 0x20000000) >> 7;
	board->black ^= (tmp.black & 0x200000000) << 2;
	board->white ^= (tmp.white & 0x200000000) << 2;
	board->black ^= (tmp.black & 0x400000000) >> 5;
	board->white ^= (tmp.white & 0x400000000) >> 5;
	board->black ^= (tmp.black & 0x800000000) >> 12;
	board->white ^= (tmp.white & 0x800000000) >> 12;
}

// ハッシュ値の計算
void SetHash(BOARD *board) {
	uint64_t pos = (uint64_t)1;

	board->key = 0x622b24abed22bd34;
	for (int i = 0;i<36;i++) {
		if ((board->white & pos) != 0) {
			board->key ^= board_key[WHITE - 1][35 - i];
		}
		else if ((board->black & pos) != 0) {
			board->key ^= board_key[BLACK - 1][35 - i];
		}
		pos <<= 1;
	}
	if (board->turn == WHITE) board->key ^= turn_key;
}

// 1手進める(hash有り)
void DoMoveHash(BOARD *board, uint64_t *move_pos) {
	uint64_t key_pos = 1;

	if (board->turn == BLACK) {
		board->black ^= move_pos[0];
		board->turn = WHITE;
	}
	else {
		board->white ^= move_pos[0];
		board->turn = BLACK;
	}
	board->move_num++;

	switch (move_pos[1])
	{
	case 0:
		RotateFirstQuadrantClockwise(board);
		break;
	case 1:
		RotateFirstQuadrantCounterClockwise(board);
		break;
	case 2:
		RotateSecondQuadrantClockwise(board);
		break;
	case 3:
		RotateSecondQuadrantCounterClockwise(board);
		break;
	case 4:
		RotateThirdQuadrantClockwise(board);
		break;
	case 5:
		RotateThirdQuadrantCounterClockwise(board);
		break;
	case 6:
		RotateFourthQuadrantClockwise(board);
		break;
	case 7:
		RotateFourthQuadrantCounterClockwise(board);
		break;
	}

	SetHash(board);
}

// 1手戻す(hash有り)
void UnDoMoveHash(BOARD *board, uint64_t *move_pos) {
	uint64_t key_pos = 1;

	switch (move_pos[1])
	{
	case 0:
		RotateFirstQuadrantCounterClockwise(board);
		break;
	case 1:
		RotateFirstQuadrantClockwise(board);
		break;
	case 2:
		RotateSecondQuadrantCounterClockwise(board);
		break;
	case 3:
		RotateSecondQuadrantClockwise(board);
		break;
	case 4:
		RotateThirdQuadrantCounterClockwise(board);
		break;
	case 5:
		RotateThirdQuadrantClockwise(board);
		break;
	case 6:
		RotateFourthQuadrantCounterClockwise(board);
		break;
	case 7:
		RotateFourthQuadrantClockwise(board);
		break;
	}

	if (board->turn == WHITE) {
		board->black ^= move_pos[0];
		board->turn = BLACK;
	}
	else {
		board->white ^= move_pos[0];
		board->turn = WHITE;
	}
	board->move_num--;

	SetHash(board);
}

// 1手進める(hash無し)
void DoMoveNoHash(BOARD *board, uint64_t *move_pos) {
	uint64_t key_pos = 1;

	if (board->turn == BLACK) {
		board->black ^= move_pos[0];
		board->turn = WHITE;
	}
	else {
		board->white ^= move_pos[0];
		board->turn = BLACK;
	}
	board->move_num++;

	switch (move_pos[1])
	{
	case 0:
		RotateFirstQuadrantClockwise(board);
		break;
	case 1:
		RotateFirstQuadrantCounterClockwise(board);
		break;
	case 2:
		RotateSecondQuadrantClockwise(board);
		break;
	case 3:
		RotateSecondQuadrantCounterClockwise(board);
		break;
	case 4:
		RotateThirdQuadrantClockwise(board);
		break;
	case 5:
		RotateThirdQuadrantCounterClockwise(board);
		break;
	case 6:
		RotateFourthQuadrantClockwise(board);
		break;
	case 7:
		RotateFourthQuadrantCounterClockwise(board);
		break;
	}
}

// 1手戻す(hash無し)
void UnDoMoveNoHash(BOARD *board, uint64_t *move_pos) {
	uint64_t key_pos = 1;

	switch (move_pos[1])
	{
	case 0:
		RotateFirstQuadrantCounterClockwise(board);
		break;
	case 1:
		RotateFirstQuadrantClockwise(board);
		break;
	case 2:
		RotateSecondQuadrantCounterClockwise(board);
		break;
	case 3:
		RotateSecondQuadrantClockwise(board);
		break;
	case 4:
		RotateThirdQuadrantCounterClockwise(board);
		break;
	case 5:
		RotateThirdQuadrantClockwise(board);
		break;
	case 6:
		RotateFourthQuadrantCounterClockwise(board);
		break;
	case 7:
		RotateFourthQuadrantClockwise(board);
		break;
	}

	if (board->turn == WHITE) {
		board->black ^= move_pos[0];
		board->turn = BLACK;
	}
	else {
		board->white ^= move_pos[0];
		board->turn = WHITE;
	}
	board->move_num--;
}