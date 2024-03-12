//=========================================================
//
// �����N�C���`�揈�� [rankIn.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _RANKIN_H_  // ���̃}�N����`������ĂȂ�������
#define _RANKIN_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_COLA		(1.0f)		// �ő�A���t�@�l
#define MIN_COLA		(0.1f)		// �ŏ��A���t�@�l
#define COL_DARK		(-0.025f)	// �������Â�����
#define COL_LIGHT		(0.025f)	// ���������邭����

//===============================================
// �����N�C���N���X
//===============================================
class CRankIn : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CRankIn();						// �f�t�H���g�R���X�g���N�^
	CRankIn(int nPriority = 6);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CRankIn();						// �f�X�g���N�^

	static CRankIn *Create(int nPriority = 6);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nRank);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_pIdxTexture;			// �g�p����e�N�X�`���̔ԍ�
	float m_fBrightness;				// ���邳�̒l
	bool m_bFlash;						// ���邭���邩�Â����邩
	int m_nRank;						// �����L���O�̈ʒu
};

#endif