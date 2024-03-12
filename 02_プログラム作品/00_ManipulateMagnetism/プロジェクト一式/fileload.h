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

	struct SModel
	{
		char aFileName[MAX_NAME];	// ���f���t�@�C����
		const char *apFileName;		// ���f���t�@�C����

		D3DXVECTOR3 pos;			// �ʒu
		D3DXVECTOR3 rot;			// ����
		int aParent;				// �e
	};

	struct SFile
	{
		CMotion::INFO m_aInfo[MAX_MOTION];					// ���[�V�������
		int m_nNumModel;									// ���f���̑���
		SModel m_ModelInfo[MAX_MODEL];						// ���f�����
	};

	enum EFile
	{
		FILE_PLAYER = 0,	// �v���C���[
		FILE_ENEMY,			// �G
		FILE_MAX
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);

	void Name(HWND hWnd, EFile type, const char* pFileName);
	void Script(FILE *pFile, EFile type);
	void CharacterSet(FILE *pFile, EFile type);
	void PartsSet(FILE *pFile, EFile type);
	void MotionSet(FILE *pFile, EFile type);
	void KeySet(FILE *pFile, EFile type, int nCntKey);
	void Key(FILE *pFile, EFile type, int nCntKey, int nCntModel);

	CMotion::INFO GetInfo(EFile type, int nIdx) { return m_fileInfo[type].m_aInfo[nIdx]; }
	int GetNumModel(EFile type) { return m_fileInfo[type].m_nNumModel; }
	const char *GetFileName(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].apFileName; }
	D3DXVECTOR3 GetPos(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].pos; }
	D3DXVECTOR3 GetRot(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].rot; }
	int GetParent(EFile type, int nIdx) { return m_fileInfo[type].m_ModelInfo[nIdx].aParent; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	SFile m_fileInfo[FILE_MAX];							// �t�@�C�����
	int m_nMotionType;									// ���[�V�����̎��
};

#endif