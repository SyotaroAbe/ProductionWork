//=========================================================
//
// �N���A�\������ [clear.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _CLEAR_H_  //���̃}�N����`������ĂȂ�������
#define _CLEAR_H_  //�Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//===============================================
// �N���A�\���N���X
//===============================================
class CClear : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CClear();						// �f�t�H���g�R���X�g���N�^
	CClear(int nPriority = 0);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CClear();						// �f�X�g���N�^

	static CClear *Create(int nPriority = 5);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_IdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	bool m_bEndAnim;					// �A�j���[�V�������I��������
};

#endif