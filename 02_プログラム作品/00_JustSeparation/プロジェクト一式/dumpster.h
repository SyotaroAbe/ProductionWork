//=========================================================
//
// �S�~�X�e�[�V�������� [dumpster.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _DUMPSTER_H_     // ���̃}�N����`������ĂȂ�������
#define _DUMPSTER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "objectX.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_DUMPSTER			(11)			// �ő�S�~�X�e�[�V�����z�u��
#define DUMPSTER_LENTH			(160.0f)	// �S�~������ł���͈�

//===============================================
// �S�~�X�e�[�V�����N���X
//===============================================
class CDumpster : public CObjectX
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CDumpster();					// �f�t�H���g�R���X�g���N�^
	CDumpster(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CDumpster();					// �f�X�g���N�^

	static void Load(HWND hWnd);
	static void Script(FILE *pFile);
	static void ModelSet(FILE *pFile);
	static CDumpster *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetRockon(void) { return m_bRockon; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxXFile;					// �g�p����X�t�@�C���̔ԍ�
	bool m_bRockon;							// �^�[�Q�b�g�͈͓̔����ǂ���
};

#endif