#ifndef BOARD_H
#define BOARD_H

//���
enum TURN {
	BLACK = 1, //���
	WHITE = 2 //���
};

// �Ֆ�
typedef struct BOARD {
	uint64_t black, white; //�΂̔z�u
	int turn; //���
	int move_num; // �萔
	uint64_t key; //�n�b�V���l
}BOARD;

// �Ֆʂ̏�����
void InitBoard(BOARD *board);

// �Ֆʂ��w��ǖʂɏ�����
void SetBoard(BOARD *board, const char *pos, int turn);

// �Q�[���I������
int EndJudge(const BOARD *board);

// ���ی����E��]
void RotateFirstQuadrantClockwise(BOARD *board);

// ���ی�������]
void RotateFirstQuadrantCounterClockwise(BOARD *board);

// ���ی����E��]
void RotateSecondQuadrantClockwise(BOARD *board);

// ���ی�������]
void RotateSecondQuadrantCounterClockwise(BOARD *board);

// ��O�ی����E��]
void RotateThirdQuadrantClockwise(BOARD *board);

// ��O�ی�������]
void RotateThirdQuadrantCounterClockwise(BOARD *board);

// ��l�ی����E��]
void RotateFourthQuadrantClockwise(BOARD *board);

// ��l�ی�������]
void RotateFourthQuadrantCounterClockwise(BOARD *board);

// �n�b�V���l�̌v�Z
void SetHash(BOARD *board);

// 1��i�߂�(hash�L��)
void DoMoveHash(BOARD *board, uint64_t *move_pos);

// 1��߂�(hash�L��)
void UnDoMoveHash(BOARD *board, uint64_t *move_pos);

// 1��i�߂�(hash����)
void DoMoveNoHash(BOARD *board, uint64_t *move_pos);

// 1��߂�(hash����)
void UnDoMoveNoHash(BOARD *board, uint64_t *move_pos);

#endif