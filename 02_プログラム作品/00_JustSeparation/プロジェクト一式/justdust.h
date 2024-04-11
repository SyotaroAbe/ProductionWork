//=========================================================
//
// JustDust�\������ [justdust.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _JUSTDUST_H_     // ���̃}�N����`������ĂȂ�������
#define _JUSTDUST_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "objectBillboard.h"

//===============================================
// �}�N����`
//===============================================

//===============================================
// �W���X�g�^�C�~���O�\���N���X
//===============================================
class CJustDust : public CObjectBillboard
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CJustDust();					// �f�t�H���g�R���X�g���N�^
	CJustDust(int nPriority = 5);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CJustDust();					// �f�X�g���N�^

	static CJustDust *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(bool bScall);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;				// �g�p����e�N�X�`���̔ԍ�
	int m_nTime;							// �o�ߎ���
	bool m_bScall;							// �T�C�Y�ύX�t���O
};

#endif