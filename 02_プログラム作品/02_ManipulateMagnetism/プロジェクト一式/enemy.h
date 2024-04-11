//=========================================================
//
// �G���� [enemy.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _ENEMY_H_     // ���̃}�N����`������ĂȂ�������
#define _ENEMY_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object.h"
#include "fileload.h"

//===============================================
// �O���錾
//===============================================
class CModel;
class CMotion;

//===============================================
// �G�N���X
//===============================================
class CEnemy : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CEnemy();						// �f�t�H���g�R���X�g���N�^
	CEnemy(int nPriority = 3);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CEnemy();						// �f�X�g���N�^

	// ���
	enum EType
	{
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_WALK,			// ����
		TYPE_JUMP,			// ����
		TYPE_MAX
	};

	// ���
	enum EState
	{
		STATE_NONE = 0,		// �Ȃ�
		STATE_NORMAL,		// �ʏ�
		STATE_MOVERIGHT,	// �E�ړ�
		STATE_MOVELEFT,		// ���ړ�
		STATE_DASH,			// �_�b�V��
		STATE_ATTACK,		// �U��
		STATE_DAMAGE,		// �_���[�W
		STATE_INVINCIBLE,	// ���G
		STATE_DEATH,		// ���S
		STATE_CLEAR,		// �N���A
		STATE_MAX
	};

	// ���[�V�����̎��
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,	// �ҋ@
		MOTIONTYPE_MOVE,		// �ړ�
		MOTIONTYPE_ACTION,		// �A�N�V����
		MOTIONTYPE_JUMP,		// �W�����v
		MOTIONTYPE_LANDING,		// ���n
		MOTIONTYPE_MAX
	};

	static void Load(HWND hWnd);
	static CEnemy *Create(D3DXVECTOR3 pos, EType type, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionObjX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

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
	void SetEType(EType type);
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

	float m_fSpeed;							// �ړ����x�ύX�p
	bool m_bJump;							// �W�����v�������ǂ���
	float m_fRotDiff;						// �ړI�̌����܂ł̍���
	int m_nStateCounter;					// ��ԊǗ��J�E���^�[

	EType m_type;							// ���
	EState m_state;							// ���
	EState m_stateOld;						// �O��̏��

	CModel *m_apModel[MAX_MODEL];			// ���f���i�p�[�c�j�ւ̃|�C���^
	int m_nNumModel;						// ���f���i�p�[�c�j�̑���
	CMotion *m_pMotion;						// ���[�V�����̂ւ̃|�C���^
};

#endif