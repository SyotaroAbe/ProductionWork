//=========================================================
//
// �Q�[����ʏ��� [game.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _GAME_H_     // ���̃}�N����`������ĂȂ�������
#define _GAME_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "manager.h"

//===============================================
// �}�N����`
//===============================================
#define TIME_STATEREADY		(120)	// �ҋ@��Ԃ։�ʂ��؂�ւ��܂ł̗]�C
#define TIME_STATEFEVER		(5400)	// �t�B�[�o�[��Ԃ։�ʂ��؂�ւ��܂ł̎���

//===============================================
// �O���錾
//===============================================
class CPlayer;
class CObjectX;
class CMeshField;
class CObject3D;
class CPause;
class CEnemy;
class CGameBg;
class CTime;
class CItem;
class CClear;

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
		STATE_END,		// �I�����
		STAET_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Reset(void);

	static CPlayer *GetPlayer() { return m_pPlayer; }
	static CMeshField *GetMeshField() { return m_pMeshField; }
	static CObject3D *GetObject3D() { return m_pObject3D; }
	static CPause *GetPause() { return m_pPause; }
	static CEnemy *GetEnemy() { return m_pEnemy; }
	static CGameBg *GetGameBg() { return m_pGameBg; }
	static CTime *GetTime() { return m_pTime; }
	static CItem* GetItem() { return m_pItem; }
	static CClear* GetClear() { return m_pClear; }

	static void SetEnablePause(const bool bPause);
	static bool GetPauseState(void) { return m_bPause; }
	static bool GetStateReady(void) { return m_bStateReady; }
	static bool GetPauseCamera(void) { return m_bPauseCamera; }

	STATE GetState(void) { return m_state; }
	static void SetClear(bool bClear) { m_bClear = bClear; }
	static bool GetBClear(void) { return m_bClear; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CPlayer *m_pPlayer;						// �v���C���[�N���X�̃|�C���^
	static CMeshField *m_pMeshField;				// ���b�V���t�B�[���h�N���X�̃|�C���^
	static CObject3D *m_pObject3D;					// �I�u�W�F�N�g3D�N���X�̃|�C���^
	static CPause *m_pPause;						// �|�[�Y�N���X�̃|�C���^
	static CEnemy *m_pEnemy;						// �G�N���X�̃|�C���^
	static CGameBg *m_pGameBg;						// �w�i�N���X�̃|�C���^
	static CTime *m_pTime;							// �^�C���N���X�̃|�C���^
	static CItem* m_pItem;							// �A�C�e���N���X�̃|�C���^
	static CClear* m_pClear;						// �N���A�\���N���X�̃|�C���^

	static bool m_bStateReady;		// GAMSESTATE_READY���ǂ���
	static bool m_bPause;			// ���݂̃|�[�Y�̏��
	static bool m_bPauseCamera;		// �|�[�Y���̃J��������\��
	static bool m_bClear;			// �N���A���Ă��邩
	STATE m_state;					// �Q�[���̏��
	int m_nCounterState;			// ��ԊǗ��J�E���^�[
	HWND m_hWnd;					// HWND�ۑ��p
};

#endif