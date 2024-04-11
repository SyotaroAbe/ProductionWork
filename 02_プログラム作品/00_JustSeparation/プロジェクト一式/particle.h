//=========================================================
//
// �p�[�e�B�N������ [particle.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _PARTICLE_H_  // ���̃}�N����`������ĂȂ�������
#define _PARTICLE_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "objectBillboard.h"
#include "effect.h"

//===============================================
// �p�[�e�B�N���N���X
//===============================================
class CParticle : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CParticle();		// �f�t�H���g�R���X�g���N�^
	~CParticle();		// �f�X�g���N�^;

	// ���
	typedef enum
	{
		TYPE_PLAYER = 0,	// �v���C���[�̔���
		TYPE_ENEMY,			// �G�̔���
		TYPE_BURN,			// �R����S�~
		TYPE_NONFLAMMABLE,	// �R���Ȃ��S�~
		TYPE_RECYCLABLE,		// �����S�~
		TYPE_MOVE,			// �ړ�
		TYPE_CURVE,			// �J�[�u
		TYPE_MAX
	}TYPE;

	static CParticle *Create(int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(D3DXVECTOR3 pos, TYPE type);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	TYPE m_type;			// ���
	CEffect::TYPE m_tex;	// �e�N�X�`���̎��
	D3DXCOLOR m_col;		// �F
	int m_nRadius;			// ���a�i�傫���j
	int m_nLife;			// ����
	int m_nMaxApeear;		// ��x�ɕ��o�����
	int m_nCntAppear;		// ��x�ɕ��o����ʂ��J�E���g
};

#endif