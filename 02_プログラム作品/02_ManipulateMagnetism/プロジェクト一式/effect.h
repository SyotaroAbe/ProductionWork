//=========================================================
//
// �G�t�F�N�g���� [effect.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _EFFECT_H_  // ���̃}�N����`������ĂȂ�������
#define _EFFECT_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "objectBillboard.h"

//===============================================
// �G�t�F�N�g�N���X
//===============================================
class CEffect : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CEffect();						// �f�t�H���g�R���X�g���N�^
	CEffect(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CEffect();						// �f�X�g���N�^;

	enum TYPE
	{
		TYPE_NORMAL = 0,	// �ʏ�
		TYPE_STEAM,			// ��
		TYPE_FIRE,			// ��
		TYPE_MAX
	};

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, TYPE type, float fRadios, int nLife, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXCOLOR col, const float fRadios, const int nLife);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxTexture[TYPE_MAX];	// �g�p����e�N�X�`���̔ԍ�
	static int m_nNumAll;				// ����
	TYPE m_type;						// ���
	D3DXCOLOR m_col;					// �F
	float m_nRadius;					// ���a�i�傫���j
	int m_nLife;						// ����
};

#endif