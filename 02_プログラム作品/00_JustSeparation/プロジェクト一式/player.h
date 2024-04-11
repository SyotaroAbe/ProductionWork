//=========================================================
//
// �v���C���[���� [player.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _PLAYER_H_     // ���̃}�N����`������ĂȂ�������
#define _PLAYER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object.h"
#include "fileload.h"

//===============================================
// �O���錾
//===============================================
class CModel;
class CMotion;

//===============================================
// �}�N����`
//===============================================
#define PLAYER_SIZEX		(30)		// ���i�����j
#define PLAYER_SIZEY		(105)		// �����i�����j
#define MOVE_GRAVITY		(0.75f)		// �d��
#define LAND_POS			(0.0f)		// �n��
#define ROT_LEFTUP			(-0.25f)	// ����̊p�x
#define ROT_LEFT			(-0.5f)		// ���̊p�x
#define ROT_LEFTDOWN		(-0.75f)	// �����̊p�x
#define ROT_RIGHTUP			(0.25f)		// �E��̊p�x
#define ROT_RIGHT			(0.5f)		// �E�̊p�x
#define ROT_RIGHTDOWN		(0.75f)		// �E���̊p�x
#define ROT_UP				(0.0f)		// ��̊p�x
#define ROT_DOWN			(1.0f)		// ���̊p�x
#define TIME_FEVER			(30)		// �t�B�[�o�[�ɂȂ鎞��

//===============================================
// �v���C���[�N���X
//===============================================
class CPlayer : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CPlayer();						// �f�t�H���g�R���X�g���N�^
	CPlayer(int nPriority = 3);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CPlayer();						// �f�X�g���N�^

	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	// �ҋ@
		MOTIONTYPE_MOVE,		// �ړ�
		MOTIONTYPE_ACTION,		// �A�N�V����
		MOTIONTYPE_JUMP,		// �W�����v
		MOTIONTYPE_LANDING,		// ���n
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	// �����ύX
	typedef enum
	{
		TURN_NONE = 0,		// �Ȃ�
		TURN_RIGHT,			// �E�֋Ȃ���
		TURN_LEFT,			// ���֋Ȃ���
		TURN_BACK,			// �܂�Ԃ�
		TURN_MAX
	}TURN;

	static CPlayer *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetJump(const bool bJump);
	D3DXVECTOR3 GetSize(void) { return m_vtxMax; }
	void SetTurn(TURN turn);
	TURN GetTurn(void){return m_turnType;}
	TURN GetTurnOld(void) { return m_turnTypeOld; }
	void SetMotion(MOTIONTYPE type);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_posOld;					// �O��̈ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_rotDest;					// �ړI�̌���
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMax;					// ���f���̍ő�l
	D3DXVECTOR3 m_vtxMin;					// ���f���̍ŏ��l

	float m_speed;							// �ړ����x�ύX�p
	bool m_bJump;							// �W�����v�������ǂ���
	float m_fRotBullet;						// �Ə��̌���
	float m_fRotDiff;						// �ړI�̌����܂ł̍���
	TURN m_turnType;						// �Ȃ������
	TURN m_turnTypeOld;						// �O��̋Ȃ������
	int m_nTurnCounter;						// �Ȃ����Ă��鎞��
	int m_nParticleCounter;					// �p�[�e�B�N����������

	CModel *m_apModel[MAX_MODEL];			// ���f���i�p�[�c�j�ւ̃|�C���^
	int m_nNumModel;						// ���f���i�p�[�c�j�̑���
	CMotion *m_pMotion;						// ���[�V�����̂ւ̃|�C���^
};

#endif