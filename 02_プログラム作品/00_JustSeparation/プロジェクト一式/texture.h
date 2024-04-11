//=========================================================
//
// �e�N�X�`���Ǘ����� [texture.cpp]
// Author = �����đ�Y
//
//=========================================================
#ifndef _TEXTURE_H_  // ���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_TEXTURE		(256)		// �ő�e�N�X�`����

//===============================================
// �e�N�X�`���N���X
//===============================================
class CTexture
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

	HRESULT Load(void);
	void Unload(void);

	void Delete(int nIdx);
	int Regist(const char *pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx) { return m_apTexture[nIdx]; }

protected:	// �h���N���X������A�N�Z�X�ł���

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
	static const char *m_apFileName[MAX_TEXTURE];			// �e�N�X�`���t�@�C����
	static int m_nNumAll;									// �e�N�X�`���̑���
};

#endif