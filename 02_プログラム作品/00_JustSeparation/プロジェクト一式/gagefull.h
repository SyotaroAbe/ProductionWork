//=========================================================
//
// �Q�[�W�t���`�揈�� [gagefull.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _GAGEFULL_H_     // ���̃}�N����`������ĂȂ�������
#define _GAGEFULL_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "objectBillboard.h"

//===============================================
// �Q�[�W�t���N���X
//===============================================
class CGageFull : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CGageFull();						// �f�t�H���g�R���X�g���N�^
	CGageFull(int nPriority = 2);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CGageFull();						// �f�X�g���N�^

	static CGageFull *Create(int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(const bool bMax);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	bool m_bMaxScaling;					// �ő�̃T�C�Y�ɂȂ�����
	bool m_bMaxGage;					// �Q�[�W���ő傩
};

#endif