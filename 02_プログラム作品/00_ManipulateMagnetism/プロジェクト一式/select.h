//=========================================================
//
// �Z���N�g��ʏ��� [select.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _SELECT_H_     // ���̃}�N����`������ĂȂ�������
#define _SELECT_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "manager.h"

//===============================================
// �O���錾
//===============================================
class CBg;
class CObject2D;

//===============================================
// �O���錾
//===============================================

//===============================================
// �Z���N�g��ʃN���X
//===============================================
class CSelect : public CScene
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CSelect();						// �f�t�H���g�R���X�g���N�^
	~CSelect();						// �f�X�g���N�^

	// �X�e�[�W
	enum STAGE
	{
		STAGE_TUTORIAL = 0,		// �`���[�g���A��
		STAGE_1,				// �X�e�[�W�P
		STAGE_2,				// �X�e�[�W�Q
		STAGE_3,				// �X�e�[�W�R
		STAGE_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg *GetBg() { return m_pBg; }
	static CObject2D* GetObject2D() { return m_pObject2D; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CBg *m_pBg;						// �w�i�N���X�̃|�C���^
	static CObject2D* m_pObject2D;			// �I�u�W�F�N�g2D�N���X�̃|�C���^

	CObject2D* m_pTutorial;		// �`���[�g���A��
	CObject2D* m_pFirstStage;	// �X�e�[�W�P
	CObject2D* m_pSecondStage;	// �X�e�[�W�Q
	CObject2D* m_pThirdStage;	// �X�e�[�W�R
	int m_nTime;				// �o�ߎ��Ԃ��J�E���g
	int m_nSelectStage;			// ���ݑI�𒆂̃X�e�[�W
	bool m_bFade;				// �t�F�[�h�ɓ�������
};

#endif