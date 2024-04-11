//=========================================================
//
// �q���g�`�揈�� [hint.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _HINT_H_     // ���̃}�N����`������ĂȂ�������
#define _HINT_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �q���g�`��N���X
//===============================================
class CHint : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CHint();						// �f�t�H���g�R���X�g���N�^
	CHint(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CHint();						// �f�X�g���N�^

	static CHint *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_IdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	int m_nTime;						// �o�ߎ���
	bool m_bDisp;						// �\�����邩
};

#endif