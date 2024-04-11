//=========================================================
//
// X�t�@�C���Ǘ����� [xfile.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "xfile.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define MIN_VTX				(900.0f)	// vtx�ŏ��l�����l
#define MAX_VTX				(-900.0f)	// vtx�ő�l�����l

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CXFile::m_nNumAll = 0;								// X�t�@�C���̑���
const char *CXFile::m_apFileName[MAX_XFILE] = {};		// X�t�@�C���t�@�C����

//===============================================
// �R���X�g���N�^
//===============================================
CXFile::CXFile()
{
	// �l���N���A����
	for (int nCntXFile = 0; nCntXFile < MAX_XFILE; nCntXFile++)
	{
		m_aXFile[nCntXFile].m_dwNumMat = 0;
		m_aXFile[nCntXFile].m_pBuffMat = NULL;
		m_aXFile[nCntXFile].m_pIdxTexture = NULL;
		m_aXFile[nCntXFile].m_pMesh = NULL;
		m_aXFile[nCntXFile].m_pTexture = NULL;
		m_aXFile[nCntXFile].m_vtxMin = D3DXVECTOR3(MIN_VTX, MIN_VTX, MIN_VTX);
		m_aXFile[nCntXFile].m_vtxMax = D3DXVECTOR3(MAX_VTX, MAX_VTX, MAX_VTX);
	}
	m_nIdx = 0;
	m_col = COL_NORMAL;
}

//===============================================
// �f�X�g���N�^
//===============================================
CXFile::~CXFile()
{

}

//===============================================
// ���f���̐����i�ǂݍ��݁j
//===============================================
HRESULT CXFile::Load(void)
{
	return S_OK;
}

//===============================================
// X�t�@�C���̔j��
//===============================================
void CXFile::Unload(void)
{
	// ���b�V���̔j��
	for (int nCntMesh = 0; nCntMesh < MAX_XFILE; nCntMesh++)
	{
		if (m_aXFile[nCntMesh].m_pMesh != NULL)
		{// �g�p����Ă���
			m_aXFile[nCntMesh].m_pMesh->Release();
			m_aXFile[nCntMesh].m_pMesh = NULL;
		}
	}

	// �}�e���A���̔j��
	for (int nCntBuff = 0; nCntBuff < MAX_XFILE; nCntBuff++)
	{
		if (m_aXFile[nCntBuff].m_pBuffMat != NULL)
		{// �g�p����Ă���
			m_aXFile[nCntBuff].m_pBuffMat->Release();
			m_aXFile[nCntBuff].m_pBuffMat = NULL;
		}
	}

	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < MAX_XFILE; nCntTexture++)
	{
		if (m_aXFile[nCntTexture].m_pTexture != NULL)
		{// �g�p����Ă���
			// �������̊J��
			delete[] m_aXFile[nCntTexture].m_pTexture;
			m_aXFile[nCntTexture].m_pTexture = NULL;
		}
	}

	// �e�N�X�`���ԍ��̃|�C���^�̔j��
	for (int nCntIdTexture = 0; nCntIdTexture < MAX_XFILE; nCntIdTexture++)
	{
		if (m_aXFile[nCntIdTexture].m_pIdxTexture != NULL)
		{// �g�p����Ă���
			// �������̊J��
			delete[] m_aXFile[nCntIdTexture].m_pIdxTexture;
			m_aXFile[nCntIdTexture].m_pIdxTexture = NULL;
		}
	}
}

//===============================================
// X�t�@�C���̓ǂݍ���
//===============================================
int CXFile::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	for (int nCntXFile = 0; nCntXFile < m_nNumAll; nCntXFile++)
	{
		if (m_apFileName[nCntXFile] == pFilename)
		{// ���ɓǂݍ��܂ꂽX�t�@�C���t�@�C��
			return nCntXFile;		// �����ԍ��ŕԂ�
		}
	}

	for (int nCntXFile = 0; nCntXFile < MAX_XFILE; nCntXFile++)
	{
		if (m_apFileName[nCntXFile] == NULL)
		{// �g�p����Ă��Ȃ�
			m_apFileName[nCntXFile] = pFilename;

			// �w�t�@�C���ɓǂݍ���
			D3DXLoadMeshFromX(m_apFileName[nCntXFile],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_aXFile[nCntXFile].m_pBuffMat,
				NULL,
				&m_aXFile[nCntXFile].m_dwNumMat,
				&m_aXFile[nCntXFile].m_pMesh);

			// X�t�@�C���̊��蓖��
			BindX(nCntXFile);

			m_nNumAll++;
			return nCntXFile;
		}
	}

	return -1;
}

//===============================================
// X�t�@�C���̊��蓖��
//===============================================
void CXFile::BindX(int nIdx)
{
	// Vtx�T�C�Y�ݒ�
	SetVtxSize(nIdx);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATERIAL *pMat;		// �}�e���A���ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_aXFile[nIdx].m_pBuffMat->GetBufferPointer();

	m_aXFile[nIdx].m_pTexture = new LPDIRECT3DTEXTURE9[m_aXFile[nIdx].m_dwNumMat];
	m_aXFile[nIdx].m_pIdxTexture = new int[m_aXFile[nIdx].m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_aXFile[nIdx].m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �e�N�X�`���t�@�C�������݂���
			// �e�N�X�`���̐ݒ�
			m_aXFile[nIdx].m_pIdxTexture[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
			m_aXFile[nIdx].m_pTexture[nCntMat] = CManager::GetTexture()->GetAddress(m_aXFile[nIdx].m_pIdxTexture[nCntMat]);
		}
		else
		{// ���݂��Ȃ�
			m_aXFile[nIdx].m_pIdxTexture[nCntMat] = NULL;
			m_aXFile[nIdx].m_pTexture[nCntMat] = NULL;
		}
	}
}

//===============================================
// Vtx�T�C�Y�ݒ�
//===============================================
void CXFile::SetVtxSize(int nIdx)
{
	int nNumVtx;		// ���_��
	DWORD dwSizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE * pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = m_aXFile[nIdx].m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(m_aXFile[nIdx].m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_aXFile[nIdx].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// �S�Ă̒��_���r���ă��f���̍ŏ��l�E�ő�l�𔲂��o��
		// �ŏ��l�̔�r
		if (vtx.x <= m_aXFile[nIdx].m_vtxMin.x)
		{
			m_aXFile[nIdx].m_vtxMin.x = vtx.x;
		}

		if (vtx.y <= m_aXFile[nIdx].m_vtxMin.y)
		{
			m_aXFile[nIdx].m_vtxMin.y = vtx.y;
		}

		if (vtx.z <= m_aXFile[nIdx].m_vtxMin.z)
		{
			m_aXFile[nIdx].m_vtxMin.z = vtx.z;
		}

		// �ő�l�̔�r
		if (vtx.x >= m_aXFile[nIdx].m_vtxMax.x)
		{
			m_aXFile[nIdx].m_vtxMax.x = vtx.x;
		}

		if (vtx.y >= m_aXFile[nIdx].m_vtxMax.y)
		{
			m_aXFile[nIdx].m_vtxMax.y = vtx.y;
		}

		if (vtx.z >= m_aXFile[nIdx].m_vtxMax.z)
		{
			m_aXFile[nIdx].m_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_aXFile[nIdx].m_pMesh->UnlockVertexBuffer();
}

//===============================================
// �`�揈��
//===============================================
void CXFile::Draw(int nIdx, COL col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DMATERIAL9 matDef;												// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													// �}�e���A���f�[�^

	m_col = col;		// �F������

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_aXFile[nIdx].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aXFile[nIdx].m_dwNumMat; nCntMat++)
	{
		// �}�e���A����ۑ�
		D3DXMATERIAL pmat = pMat[nCntMat];

		// �F���ʂɐF��ݒ�
		switch (m_col)
		{
		case COL_NORMAL:		// �ʏ�
			break;

		case COL_RED:			// ��
			pmat.MatD3D.Diffuse.r = 1.0f;
			pmat.MatD3D.Diffuse.g = 0.0f;
			pmat.MatD3D.Diffuse.b = 0.0f;
			break;

		case COL_GREEN:			// ��
			pmat.MatD3D.Diffuse.r = 0.0f;
			pmat.MatD3D.Diffuse.g = 1.0f;
			pmat.MatD3D.Diffuse.b = 0.0f;
			break;

		case COL_BLUE:			// ��
			pmat.MatD3D.Diffuse.r = 0.0f;
			pmat.MatD3D.Diffuse.g = 0.0f;
			pmat.MatD3D.Diffuse.b = 1.0f;
			break;

		case COL_YELLOW:		// ��
			pmat.MatD3D.Diffuse.r = 0.9f;
			pmat.MatD3D.Diffuse.g = 0.75f;
			pmat.MatD3D.Diffuse.b = 0.2f;
			break;

		case COL_ORANGE:		// ��
			pmat.MatD3D.Diffuse.r = 0.9f;
			pmat.MatD3D.Diffuse.g = 0.5f;
			pmat.MatD3D.Diffuse.b = 0.2f;
			break;

		case COL_LIGHTBLUE:		// ���F
			pmat.MatD3D.Diffuse.r = 0.4f;
			pmat.MatD3D.Diffuse.g = 0.9f;
			pmat.MatD3D.Diffuse.b = 1.0f;
			break;

		case COL_YELLOWGREEN:	// ����
			pmat.MatD3D.Diffuse.r = 0.2f;
			pmat.MatD3D.Diffuse.g = 1.0f;
			pmat.MatD3D.Diffuse.b = 0.2f;
			break;
		}

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pmat.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_aXFile[nIdx].m_pTexture[nCntMat]);

		// ���f���i�p�[�c�j�̕`��
		m_aXFile[nIdx].m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===============================================
// �����`�揈��
//===============================================
void CXFile::CleannessDraw(int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DMATERIAL9 matDef;												// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													// �}�e���A���f�[�^
	D3DMATERIAL9 mat[256];

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_aXFile[nIdx].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aXFile[nIdx].m_dwNumMat; nCntMat++)
	{
		// �}�e���A����ۑ�
		mat[nCntMat] = pMat[nCntMat].MatD3D;
		mat[nCntMat].Diffuse.a = 0.5f;
		mat[nCntMat].Ambient.a = 0.5f;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&mat[nCntMat]);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_aXFile[nIdx].m_pTexture[nCntMat]);

		// ���f���i�p�[�c�j�̕`��
		m_aXFile[nIdx].m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}