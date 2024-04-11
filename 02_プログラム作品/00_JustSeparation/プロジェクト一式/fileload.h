//=========================================================
//
// �t�@�C���ǂݍ��ݏ��� [fileload.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _FILELOAD_H_     // ���̃}�N����`������ĂȂ�������
#define _FILELOAD_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "motion.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_MODEL	(20)	// �ő僂�f����
#define MAX_NAME	(256)	// �ő啶����

//===============================================
// ���[�h�N���X
//===============================================
class CFileLoad
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CFileLoad();						// �f�t�H���g�R���X�g���N�^
	~CFileLoad();						// �f�X�g���N�^

	typedef struct
	{
		char aFileName[MAX_NAME];	// ���f���t�@�C����
		const char *apFileName;		// ���f���t�@�C����

		D3DXVECTOR3 pos;			// �ʒu
		D3DXVECTOR3 rot;			// ����
		int aParent;				// �e
	}MODEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);

	void Name(HWND hWnd, const char* pFileName);
	void Script(FILE *pFile);
	void CharacterSet(FILE *pFile);
	void PartsSet(FILE *pFile);
	void MotionSet(FILE *pFile);
	void KeySet(FILE *pFile, int nCntKey);
	void Key(FILE *pFile, int nCntKey, int nCntModel);

	CMotion::INFO GetInfo(int nIdx) { return m_aInfo[nIdx]; }
	int GetNumModel(void) { return m_nNumModel; }
	const char *GetFileName(int nIdx) { return m_ModelInfo[nIdx].apFileName; }
	D3DXVECTOR3 GetPos(int nIdx) { return m_ModelInfo[nIdx].pos; }
	D3DXVECTOR3 GetRot(int nIdx) { return m_ModelInfo[nIdx].rot; }
	int GetParent(int nIdx) { return m_ModelInfo[nIdx].aParent; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	CMotion::INFO m_aInfo[MAX_MOTION];					// ���[�V�������
	int m_nNumModel;									// ���f���̑���
	int m_nMotionType;									// ���[�V�����̎��
	MODEL m_ModelInfo[MAX_MODEL];						// ���f�����
};

#endif