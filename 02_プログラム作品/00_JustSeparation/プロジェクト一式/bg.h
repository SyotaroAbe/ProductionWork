//=========================================================
//
// �w�i���� [bg.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _BG_H_     // ���̃}�N����`������ĂȂ�������
#define _BG_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"
#include "multibg.h"

//===============================================
// �w�i�N���X
//===============================================
class CBg : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CBg();						// �f�t�H���g�R���X�g���N�^
	CBg(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CBg();						// �f�X�g���N�^

	// �e�N�X�`���̎��
	enum TYPE
	{
		TYPE_TITLE = 0,		// �^�C�g�����
		TYPE_RESULT,		// ���U���g���
		TYPE_MAX
	};

	static CBg *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const bool bMove);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxTexture[TYPE_MAX];			// �g�p����e�N�X�`���̔ԍ�
	int m_nIdx;									// �����ڂ̔w�i��
	bool m_bMove;								// �e�N�X�`�����W�𓮂�����
	float m_fTexU;								// �e�N�X�`�����W�̊J�n�ʒu�iU�l�j
	float m_fTexV;								// �e�N�X�`�����W�̊J�n�ʒu�iV�l�j
};

#endif