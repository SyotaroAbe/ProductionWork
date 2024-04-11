//=========================================================
//
// UI���� [ui.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _UI_H_     // ���̃}�N����`������ĂȂ�������
#define _UI_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// UI�N���X
//===============================================
class CUi : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CUi();						// �f�t�H���g�R���X�g���N�^
	CUi(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CUi();						// �f�X�g���N�^

	static CUi *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_IdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	bool m_bMove;								// �e�N�X�`�����W�𓮂�����
	float m_fTexU;								// �e�N�X�`�����W�̊J�n�ʒu�iU�l�j
	float m_fTexV;								// �e�N�X�`�����W�̊J�n�ʒu�iV�l�j
};

#endif