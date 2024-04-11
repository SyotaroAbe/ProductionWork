//=========================================================
//
// �e�N�X�`���Ǘ����� [texture.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CTexture::m_nNumAll = 0;								// �e�N�X�`���̑���
const char *CTexture::m_apFileName[MAX_TEXTURE] = {};		// �e�N�X�`���t�@�C����

//===============================================
// �R���X�g���N�^
//===============================================
CTexture::CTexture()
{
	// �l���N���A����
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		m_apTexture[nCntTexture] = NULL;
	}
}

//===============================================
// �f�X�g���N�^
//===============================================
CTexture::~CTexture()
{

}

//===============================================
// ���f���̐����i�ǂݍ��݁j
//===============================================
HRESULT CTexture::Load(void)
{
	// �e�N�X�`�������O�ɓǂݍ���

	return S_OK;
}

//===============================================
// �e�N�X�`���̔j��
//===============================================
void CTexture::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < m_nNumAll; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != NULL)
		{// �g�p����Ă���
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//===============================================
// �e�N�X�`���̔j��
//===============================================
void CTexture::Delete(int nIdx)
{
	if (nIdx >= 0)
	{
		if (m_apTexture[nIdx] != NULL)
		{// ���ɓǂݍ��܂ꂽ�e�N�X�`���t�@�C��
			m_apTexture[nIdx]->Release();
			m_apTexture[nIdx] = NULL;
		}
	}
}

//===============================================
// �e�N�X�`���̓ǂݍ���
//===============================================
int CTexture::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	for (int nCntTexture = 0; nCntTexture < m_nNumAll; nCntTexture++)
	{
		if (m_apFileName[nCntTexture] == pFilename)
		{// ���ɓǂݍ��܂ꂽ�e�N�X�`���t�@�C��
			return nCntTexture;		// �����ԍ��ŕԂ�
		}
	}

	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] == NULL)
		{// �g�p����Ă��Ȃ�
			m_apFileName[nCntTexture] = pFilename;

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pFilename,
				&m_apTexture[nCntTexture]);

			m_nNumAll++;
			return nCntTexture;
		}
	}

	return -1;
}