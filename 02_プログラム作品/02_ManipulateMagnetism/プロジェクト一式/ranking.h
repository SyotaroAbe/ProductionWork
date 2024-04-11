//=========================================================
//
// �����L���O���� [ranking.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _RANKING_H_     // ���̃}�N����`������ĂȂ�������
#define _RANKING_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "number.h"
#include "score.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_RANK		(6)				// �\�����郉���L���O��
#define YOUR_SCORE		(5)				// ����̃X�R�A

//===============================================
// �����L���O�N���X
//===============================================
class CRanking
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CRanking();						// �f�t�H���g�R���X�g���N�^
	CRanking(int nPriority = 5);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CRanking();					// �f�X�g���N�^

	static CRanking *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(void);
	void Add(const int nScore);

	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CNumber *m_apNumber[MAX_RANK][MAX_NUMBER];		// �i���o�[�N���X�̃|�C���^
	static int m_nIdxTexture;								// �g�p����e�N�X�`���̔ԍ�

	float m_fSizeX;											// ��
	float m_fSizeY;											// ����
	int m_aScore[MAX_RANK];									// �X�R�A�̒l
	int m_nYourRank;										// ����̃����N

	float m_fBrightness;				// ���邳�̒l
	bool m_bFlash;						// ���邭���邩�Â����邩
};

#endif