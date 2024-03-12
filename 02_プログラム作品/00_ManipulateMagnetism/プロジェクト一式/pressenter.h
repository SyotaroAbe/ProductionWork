//=========================================================
//
// PressEnter�`�揈�� [pressenter.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _PRESSENTER_H_     // ���̃}�N����`������ĂȂ�������
#define _PRESSENTER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// PressEnter�`��N���X
//===============================================
class CPressEnter : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CPressEnter();						// �f�t�H���g�R���X�g���N�^
	CPressEnter(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CPressEnter();						// �f�X�g���N�^

	static CPressEnter *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetBrightnessFade(bool bFade);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_IdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	float m_fBrightness;				// ���邳�̒l
};

#endif