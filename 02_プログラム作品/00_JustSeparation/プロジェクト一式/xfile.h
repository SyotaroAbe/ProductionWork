//=========================================================
//
// X�t�@�C���Ǘ����� [xfile.cpp]
// Author = �����đ�Y
//
//=========================================================
#ifndef _XFILE_H_  // ���̃}�N����`������ĂȂ�������
#define _XFILE_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_XFILE		(256)		// �ő�X�t�@�C����

//===============================================
// X�t�@�C���N���X
//===============================================
class CXFile
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CXFile();	// �f�t�H���g�R���X�g���N�^
	~CXFile();	// �f�X�g���N�^

	// X�t�@�C���\����
	typedef struct
	{
		LPDIRECT3DTEXTURE9 *m_pTexture;			// �e�N�X�`���ւ̃|�C���^
		int *m_pIdxTexture;						// �g�p����e�N�X�`���̔ԍ�
		LPD3DXMESH m_pMesh;						// ���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER m_pBuffMat;				// �}�e���A���ւ̃|�C���^
		DWORD m_dwNumMat;						// �}�e���A���̐�
		D3DXVECTOR3 m_vtxMin, m_vtxMax;			// ���f���̍ŏ��l�E�ő�l
	}XFile;

	typedef enum
	{
		COL_NORMAL = 0,		// �ʏ�
		COL_RED,			// ��
		COL_GREEN,			// ��
		COL_BLUE,			// ��
		COL_YELLOW,			// ��
		COL_ORANGE,			// ��
		COL_LIGHTBLUE,		// ���F
		COL_YELLOWGREEN,	// ����
		COL_MAX
	}COL;

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pFilename);
	void BindX(int nIdx);
	void SetVtxSize(int nIdx);
	void Draw(int nIdx, COL col);
	void CleannessDraw(int nIdx);

	D3DXVECTOR3 GetSize(int nIdx) { return m_aXFile[nIdx].m_vtxMax; }
	D3DXVECTOR3 GetSizeMin(int nIdx) { return m_aXFile[nIdx].m_vtxMin; }

protected:	// �h���N���X������A�N�Z�X�ł���

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static const char *m_apFileName[MAX_XFILE];			// X�t�@�C���t�@�C����
	static int m_nNumAll;								// X�t�@�C���̑���
	
	int m_nIdx;											// ���f���̔ԍ�
	XFile m_aXFile[MAX_XFILE];							// X�t�@�C���̏��
	COL m_col;											// �F�̏��
};

#endif