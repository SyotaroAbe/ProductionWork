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
#define ROT_LEFTUP			(-0.25f)	// ����̊p�x
#define ROT_LEFT			(-0.5f)		// ���̊p�x
#define ROT_LEFTDOWN		(-0.75f)	// �����̊p�x
#define ROT_RIGHTUP			(0.25f)		// �E��̊p�x
#define ROT_RIGHT			(0.5f)		// �E�̊p�x
#define ROT_RIGHTDOWN		(0.75f)		// �E���̊p�x
#define ROT_UP				(0.0f)		// ��̊p�x
#define ROT_DOWN			(1.0f)		// ���̊p�x

//===============================================
// �v���C���[�N���X
//===============================================
class CPlayer : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CPlayer();						// �f�t�H���g�R���X�g���N�^
	CPlayer(int nPriority = 3);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CPlayer();						// �f�X�g���N�^

	// �v���C���[�̏��
	enum EState
	{
		STATE_NONE = 0,		// �Ȃ�
		STATE_NORMAL,		// �ʏ�
		STATE_DAMAGE,		// �_���[�W
		STATE_INVINCIBLE,	// ���G
		STATE_DEATH,		// ���S
		STATE_CLEAR,		// �N���A
		STATE_MAX
	};

	enum EMagnet
	{
		EMAGNET_NONE = 0,	// �Ȃ�
		EMAGNET_N,			// N�Ɂi�ԁj
		EMAGNET_S,			// S�Ɂi�j
		EMAGNET_MAX
	};

	// ���[�V�����̎��
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,	// �ҋ@
		MOTIONTYPE_MOVE,		// �ړ�
		MOTIONTYPE_PREATTACK,	// �󒆍U���O����
		MOTIONTYPE_JUMP,		// �W�����v
		MOTIONTYPE_LANDING,		// ���n
		MOTIONTYPE_JUMPAIR,		// �󒆃W�����v
		MOTIONTYPE_ATTACKL,		// �󒆍U����
		MOTIONTYPE_ATTACKR,		// �󒆍U���E
		MOTIONTYPE_HIPDROP,		// �q�b�v�h���b�v
		MOTIONTYPE_DASH,		// �_�b�V��
		MOTIONTYPE_DAMAGE,		// �_���[�W
		MOTIONTYPE_MAX
	};

	static CPlayer *Create(D3DXVECTOR3 pos, int nPriority = 3);
	static CPlayer* GetInstance(void);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Death(void);

	void CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	void CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetJump(const bool bJump);
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void) { return m_vtxMax; }
	void SetSizeMin(D3DXVECTOR3 size);
	D3DXVECTOR3 GetSizeMin(void) { return m_vtxMin; }
	void SetState(EState state);
	EState GetState(void) { return m_state; }
	void SetMotion(MOTIONTYPE type);
	void SetPosShadow(void);
	int GetMagnet(void) { return m_magnet; }
	int GetMagnetOld(void) { return m_magnetOld; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CPlayer* m_pPlayer;				// �v���C���[�N���X�̃|�C���^
	D3DXVECTOR3 m_pos;						// �ʒu
	D3DXVECTOR3 m_posOld;					// �O��̈ʒu
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_rotDest;					// �ړI�̌���
	D3DXVECTOR3 m_move;						// �ړ���
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtxMax;					// ���f���̍ő�l
	D3DXVECTOR3 m_vtxMin;					// ���f���̍ŏ��l
	D3DXVECTOR3 m_posShadow;				// �e�̈ʒu

	static int m_nPriority;
	float m_fSpeed;							// �ړ����x�ύX�p
	bool m_bJump;							// �W�����v�������ǂ���
	float m_fLenthCamera;					// �J�����̋���
	float m_fRotBullet;						// �Ə��̌���
	float m_fRotDiff;						// �ړI�̌����܂ł̍���
	int m_nParticleCounter;					// �p�[�e�B�N����������
	int m_nStateCounter;					// ��ԊǗ��J�E���^�[
	bool m_bInvincible;						// ���G��Ԃ��ǂ���
	int m_nInvincibleCounter;				// ���G��ԃJ�E���^�[
	bool m_bDisp;							// ��ʂɕ\�����邩

	EState m_state;							// ���
	int m_magnet;						// ���͏��
	int m_magnetOld;					// �O��̎��͏��

	CModel *m_apModel[MAX_MODEL];			// ���f���i�p�[�c�j�ւ̃|�C���^
	int m_nNumModel;						// ���f���i�p�[�c�j�̑���
	CMotion *m_pMotion;						// ���[�V�����̂ւ̃|�C���^
};

#endif