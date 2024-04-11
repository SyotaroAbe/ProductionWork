//=========================================================
//
// ���ݏ��� [garbage.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _GARBAGE_H_     // ���̃}�N����`������ĂȂ�������
#define _GARBAGE_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "objectX.h"
#include "particle.h"

//===============================================
// ���݃N���X
//===============================================
class CGarbage : public CObjectX
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CGarbage();						// �f�t�H���g�R���X�g���N�^
	CGarbage(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CGarbage();					// �f�X�g���N�^

	// �S�~�̎��
	enum TYPE
	{
		// �R����S�~
		TYPE_BOOKOLD = 0,			// �{
		TYPE_BOOT,					// ���C
		TYPE_BAG,					// ��
		// �R���Ȃ��S�~
		TYPE_LIGHT,					// ���C�g
		TYPE_CAMERA,				// �J����
		TYPE_BUCKET,				// �o�P�c
		// �����S�~
		TYPE_BIN,					// �r
		TYPE_CAN,					// ��
		TYPE_DANBORU,				// �i�{�[��

		TYPE_MAX
	};

	// �T�C�Y
	enum MODELSIZE
	{
		MODELSIZE_NORMAL = 0,	// �ʏ�
		MODELSIZE_SMALL,		// ������
		MODELSIZE_BIG,			// �傫��
		MODELSIZE_MAX
	};

	// �S�~�̕���
	enum SEPARATION
	{
		SEPARATION_NONE = 0,		// �Ȃ�
		SEPARATION_BURN,			// �R����S�~
		SEPARATION_NONFLAMMABLE,	// �R���Ȃ��S�~
		SEPARATION_RECYCLABLE,		// �����S�~
		SEPARATION_MAX
	};

	// �S�~�̏��
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃ�
		STATE_NORMAL,		// �ʏ���
		STATE_ROCKON,		// ���b�N�I��
		STATE_SEPARATION,	// ����
		STATE_DISPOSE,		// �̂Ă�
		STATE_COLLECT,		// ���
		STATE_INPLAYER,		// �`�悵�Ȃ�
		STATE_DUMPSTER,		// �S�~�X�e�[�V�����s��
		STATE_DELETE,		// �j��
		STATE_MAX
	};

	static HRESULT Load(void);
	static void Unload(void);
	static CGarbage *Create(D3DXVECTOR3 pos, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Separation(SEPARATION separation);

	D3DXVECTOR3 GetMove(void) { return m_move; }
	int GetNumAll(void) { return m_nNumAll; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxXFile[TYPE_MAX];		// �g�p����X�t�@�C���̔ԍ�
	static int m_nNumAll;					// ����
	static int m_nRandCounter;				// rand���Z�b�g�p

	CParticle::TYPE particleType;			// �p�[�e�B�N���̎��
	D3DXVECTOR3 m_posDest;					// �ړI�̈ʒu
	D3DXVECTOR3 m_posDiff;					// �ړI�̈ʒu�܂ł̍���
	D3DXVECTOR3 m_nextposDest;				// ���̖ړI�̈ʒu
	TYPE m_type;							// �S�~�̎��
	MODELSIZE m_size;						// ���f���̃T�C�Y
	STATE m_state;							// �S�~�̏��
	SEPARATION m_separation;				// ���ʂ̎��
	SEPARATION m_rockonType;				// ���b�N�I�����̕���
	bool m_bLand;							// �n�ɂ��Ă��邩�ǂ���
	bool m_bJustDust;						// JustDust���ǂ���
	float m_fMoveDiff;						// �ʒu�̕␳�W��
	int m_nWaitCounter;						// �Î~���Ă��鎞��
	int m_nIdxDumpster;						// �S�~�X�e�[�V�����̔ԍ�
};

#endif