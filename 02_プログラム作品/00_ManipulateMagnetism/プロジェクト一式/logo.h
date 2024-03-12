//=========================================================
//
// �^�C�g�����S�`�揈�� [logo.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _LOGO_H_     // ���̃}�N����`������ĂȂ�������
#define _LOGO_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �^�C�g�����S�`��N���X
//===============================================
class CLogo : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CLogo();						// �f�t�H���g�R���X�g���N�^
	CLogo(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CLogo();						// �f�X�g���N�^

	static CLogo *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetEndAnim(void) { return m_bEndAnim; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_IdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	bool m_bEndAnim;					// �A�j���[�V�������I��������
};

#endif