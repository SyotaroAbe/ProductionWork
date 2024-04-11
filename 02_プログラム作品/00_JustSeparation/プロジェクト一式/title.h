//=========================================================
//
// �^�C�g����ʏ��� [title.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _TITLE_H_     // ���̃}�N����`������ĂȂ�������
#define _TITLE_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "manager.h"

//===============================================
// �O���錾
//===============================================
class CPlayer;
class CBg;
class CFade;
class CLogo;
class CPressEnter;

//===============================================
// �^�C�g����ʃN���X
//===============================================
class CTitle : public CScene
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CTitle();						// �f�t�H���g�R���X�g���N�^
	~CTitle();						// �f�X�g���N�^

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetFade(void) { return m_bFade; }

	static CPlayer *GetPlayer() { return m_pPlayer; }
	static CBg *GetBg() { return m_pBg; }
	static CLogo *GetLogo() { return m_pLogo; }
	static CPressEnter *GetPressEnter() { return m_pPressEnter; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CPlayer *m_pPlayer;				// �v���C���[�N���X�̃|�C���^
	static CBg *m_pBg;						// �w�i�N���X�̃|�C���^
	static CLogo *m_pLogo;					// �^�C�g�����S�`��N���X�̃|�C���^
	static CPressEnter *m_pPressEnter;		// PressEnter�`��N���X�̃|�C���^

	int m_nTimeFade;	// �����t�F�[�h����
	bool m_bFade;		// �t�F�[�h�ɓ�������
};

#endif