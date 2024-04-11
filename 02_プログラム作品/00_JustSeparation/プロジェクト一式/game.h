//=========================================================
//
// �Q�[����ʏ��� [game.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _GAME_H_     // ���̃}�N����`������ĂȂ�������
#define _GAME_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "manager.h"
#include "dumpster.h"

//===============================================
// �}�N����`
//===============================================
#define TIME_STATEREADY		(120)	// �ҋ@��Ԃ։�ʂ��؂�ւ��܂ł̗]�C
#define TIME_STATEFEVER		(5400)	// �t�B�[�o�[��Ԃ։�ʂ��؂�ւ��܂ł̎���

//===============================================
// �O���錾
//===============================================
class CPlayer;
class CNumber;
class CTime;
class CScore;
class CObjectX;
class CGarbage;
class CMeshField;
class CObject3D;
class CTarget;
class CDumpster;
class CPause;
class CUi;
class CUiGage;
class CUiTarget;
class CJustDust;
class CFever;
class CHint;
class CGageFull;

//===============================================
// �Q�[����ʃN���X
//===============================================
class CGame : public CScene
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CGame();						// �f�t�H���g�R���X�g���N�^
	~CGame();						// �f�X�g���N�^

	// �Q�[���̏��
	enum STATE
	{
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_READY,	// �J�n�ҋ@���
		STATE_NORMAL,	// �ʏ���
		STATE_FEVER,	// �t�B�[�o�[���
		STATE_END,		// �I�����
		STAET_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *GetPlayer() { return m_pPlayer; }
	static CNumber *GetNumber() { return m_pNumber; }
	static CTime *GetTime() { return m_pTime; }
	static CScore *GetScore() { return m_pScore; }
	static CGarbage *GetGarbage() { return m_pGarbage; }
	static CMeshField *GetMeshField() { return m_pMeshField; }
	static CObject3D *GetObject3D() { return m_pObject3D; }
	static CTarget *GetTarget() { return m_pTarget; }
	static CDumpster *GetDumpster(int nIdx) { return m_pDumpster[nIdx]; }
	static CPause *GetPause() { return m_pPause; }
	static CUi *GetUi() { return m_pUi; }
	static CUiGage *GetUiGage() { return m_pUiGage; }
	static CUiTarget *GetUiTarget() { return m_pUiTarget; }
	static CJustDust *GetJustDust() { return m_pJustDust; }
	static CFever *GetFever() { return m_pFever; }
	static CHint *GetHint() { return m_pHint; }
	static CGageFull *GetGageFull() { return m_pGageFull; }

	static void SetEnablePause(const bool bPause);
	static bool GetPauseState(void) { return m_bPause; }
	static bool GetStateReady(void) { return m_bStateReady; }
	static bool GetPauseCamera(void) { return m_bPauseCamera; }

	STATE GetState(void) { return m_state; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CPlayer *m_pPlayer;						// �v���C���[�N���X�̃|�C���^
	static CNumber *m_pNumber;						// �i���o�[�N���X�̃|�C���^
	static CTime *m_pTime;							// �^�C���N���X�̃|�C���^
	static CScore *m_pScore;						// �X�R�A�N���X�̃|�C���^
	static CGarbage *m_pGarbage;					// �S�~�N���X�̃|�C���^
	static CMeshField *m_pMeshField;				// ���b�V���t�B�[���h�N���X�̃|�C���^
	static CObject3D *m_pObject3D;					// �I�u�W�F�N�g3D�N���X�̃|�C���^
	static CTarget *m_pTarget;						// �^�[�Q�b�g�N���X�̃|�C���^
	static CDumpster *m_pDumpster[MAX_DUMPSTER];	// �S�~�X�e�[�V�����N���X�̃|�C���^
	static CPause *m_pPause;						// �|�[�Y�N���X�̃|�C���^
	static CUi *m_pUi;								// UI�N���X�̃|�C���^
	static CUiGage *m_pUiGage;						// �S�~�Q�[�W�N���X�̃|�C���^
	static CUiTarget *m_pUiTarget;					// �^�[�Q�b�gUI�N���X�̃|�C���^
	static CJustDust *m_pJustDust;					// JustDust�\���N���X�̃|�C���^
	static CFever *m_pFever;						// Fever�\���N���X�̃|�C���^
	static CHint *m_pHint;							// �q���g�\���N���X�̃|�C���^
	static CGageFull *m_pGageFull;					// �Q�[�W�t���\���N���X�̃|�C���^

	static bool m_bStateReady;		// GAMSESTATE_READY���ǂ���
	static bool m_bPause;			// ���݂̃|�[�Y�̏��
	static bool m_bPauseCamera;		// �|�[�Y���̃J��������\��
	STATE m_state;					// �Q�[���̏��
	int m_nCounterState;			// ��ԊǗ��J�E���^�[
	int m_nTimeFever;				// �t�B�[�o�[��Ԃ̃^�C��
};

#endif