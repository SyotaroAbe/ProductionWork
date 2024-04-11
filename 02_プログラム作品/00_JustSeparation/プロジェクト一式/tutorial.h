//=========================================================
//
// �`���[�g���A����ʏ��� [tutorial.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _TUTORIAL_H_     // ���̃}�N����`������ĂȂ�������
#define _TUTORIAL_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "manager.h"

//===============================================
// �O���錾
//===============================================
class CPlayer;
class CNumber;
class CScore;
class CMeshField;
class CTarget;
class CDumpster;
class CUiGage;
class CUiTarget;
class CUiOperation;
class CJustDust;
class CGageFull;

//===============================================
// �O���錾
//===============================================
#define MAX_DUMPSTER_TUTORIAL			(2)			// �ő�S�~�X�e�[�V�����z�u��

//===============================================
// �`���[�g���A����ʃN���X
//===============================================
class CTutorial : public CScene
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CTutorial();						// �f�t�H���g�R���X�g���N�^
	~CTutorial();						// �f�X�g���N�^

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *GetPlayer() { return m_pPlayer; }
	static CNumber *GetNumber() { return m_pNumber; }
	static CScore *GetScore() { return m_pScore; }
	static CGarbage *GetGarbage() { return m_pGarbage; }
	static CMeshField *GetMeshField() { return m_pMeshField; }
	static CTarget *GetTarget() { return m_pTarget; }
	static CDumpster *GetDumpster() { return m_pDumpster; }
	static CUiGage *GetUiGage() { return m_pUiGage; }
	static CUiTarget *GetUiTarget() { return m_pUiTarget; }
	static CUiOperation *GetUiOperation() { return m_pUiOperation; }
	static CJustDust *GetJustDust() { return m_pJustDust; }
	static CGageFull *GetGageFull() { return m_pGageFull; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CPlayer *m_pPlayer;				// �v���C���[�N���X�̃|�C���^
	static CNumber *m_pNumber;				// �i���o�[�N���X�̃|�C���^
	static CScore *m_pScore;				// �X�R�A�N���X�̃|�C���^
	static CGarbage *m_pGarbage;			// �S�~�N���X�̃|�C���^
	static CMeshField *m_pMeshField;		// ���b�V���t�B�[���h�N���X�̃|�C���^
	static CTarget *m_pTarget;				// �^�[�Q�b�g�N���X�̃|�C���^
	static CDumpster *m_pDumpster;			// �S�~�X�e�[�V�����N���X�̃|�C���^
	static CUiGage *m_pUiGage;				// �S�~�Q�[�W�N���X�̃|�C���^
	static CUiTarget *m_pUiTarget;			// �^�[�Q�b�gUI�N���X�̃|�C���^
	static CUiOperation *m_pUiOperation;	// ������@�\���N���X�̃|�C���^
	static CJustDust *m_pJustDust;			// JustDust�\���N���X�̃|�C���^
	static CGageFull *m_pGageFull;			// �Q�[�W�t���\���N���X�̃|�C���^

	int m_nTime;		// �o�ߎ��Ԃ��J�E���g
	bool m_bFade;		// �t�F�[�h�ɓ�������
};

#endif