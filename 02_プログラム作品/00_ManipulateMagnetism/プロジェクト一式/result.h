//=========================================================
//
// ���U���g��ʏ��� [result.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _RESULT_H_     // ���̃}�N����`������ĂȂ�������
#define _RESULT_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "manager.h"

//===============================================
// �O���錾
//===============================================
class CBg;
class CRanking;
class CRankIn;

//===============================================
// ���U���g��ʃN���X
//===============================================
class CResult : public CScene
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CResult();						// �f�t�H���g�R���X�g���N�^
	~CResult();						// �f�X�g���N�^

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg *GetBg() { return m_pBg; }
	static CRanking *GetRanking() { return m_pRanking; }
	static CRankIn *GetRankIn() { return m_pRankIn; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CBg *m_pBg;								// �w�i�N���X�̃|�C���^
	static CRanking *m_pRanking;					// �����L���O�N���X�̃|�C���^
	static CRankIn *m_pRankIn;						// �����N�C���N���X�̃|�C���^

	int m_nTimeFade;	// �����t�F�[�h����
	bool m_bFade;		// �t�F�[�h�ɓ�������
};

#endif