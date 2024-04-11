//=========================================================
//
// ���f���`�揈�� [model.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "objectX.h"

//===============================================
// �}�N����`
//===============================================
#define MIN_VTX				(900.0f)	// vtx�ŏ��l�����l
#define MAX_VTX				(-900.0f)	// vtx�ő�l�����l

//===============================================
// �R���X�g���N�^
//===============================================
CModel::CModel()
{
	//�l���N���A����
	m_apTexture = NULL;
	m_apIdxTexture = 0;
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_dwNumMat = 0;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOri = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(MIN_VTX, MIN_VTX, MIN_VTX);
	m_vtxMax = D3DXVECTOR3(MAX_VTX, MAX_VTX, MAX_VTX);
	m_pParent = NULL;
}

//===============================================
// �f�X�g���N�^
//===============================================
CModel::~CModel()
{

}

//===============================================
// ��������
//===============================================
CModel *CModel::Create(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel *pModel;

	// �v���C���[�̐���
	pModel = new CModel();

	// ����������
	pModel->Init(pFilename, pos, rot);

	return pModel;
}

//===============================================
// ����������
//===============================================
HRESULT CModel::Init(const char *pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	// �w�t�@�C���ɓǂݍ���
	D3DXLoadMeshFromX(pFilename,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&m_pBuffMat,
						NULL,
						&m_dwNumMat,
						&m_pMesh);

	// X�t�@�C���Ɋ��蓖��
	BindX(m_pMesh, m_pBuffMat, m_dwNumMat);

	// �ʒu�̐ݒ�
	m_pos = pos;
	m_posOri = pos;

	// �����̐ݒ�
	m_rot = rot;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CModel::Uninit(void)
{
	// ���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	if (m_apTexture != NULL)
	{// �g�p����Ă���
		// �������̊J��
		delete[] m_apTexture;
		m_apTexture = NULL;
	}

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_apIdxTexture[nCntMat] != NULL)
		{// �e�N�X�`���t�@�C�������݂���
			// �e�N�X�`���̔j��
			CManager::GetTexture()->Delete(m_apIdxTexture[nCntMat]);
		}
	}

	if (m_apIdxTexture != NULL)
	{// �g�p����Ă���
		// �������̊J��
		delete[] m_apIdxTexture;
		m_apIdxTexture = NULL;
	}
}

//===============================================
// �X�V����
//===============================================
void CModel::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;												// �e�̃}�g���b�N�X
	//D3DXMATRIX mtxPlayer = CManager::GetPlayer()->GetMtxWorld();		// �v���C���[�̃}�g���b�N�X
	D3DMATERIAL9 matDef;												// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													// �}�e���A���f�[�^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �p�[�c�́u�e�}�g���b�N�X�v��ݒ�
	if (m_pParent != NULL)
	{// �e���f��������ꍇ
		mtxParent = m_pParent->m_mtxWorld;
	}
	else
	{// �e���f�����Ȃ��ꍇ
		//mtxParent = mtxPlayer;
		// ���݁i�ŐV�j�̃}�g���b�N�X���擾���� [ = �v���C���[�̃}�g���b�N�X ]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// �Z�o�����u�p�[�c�̃��[���h�}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// ���f���̃J���[�ύX
		//pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_apTexture[nCntMat]);

		// ���f���i�p�[�c�j�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===============================================
// �e�̕`�揈��
//===============================================
void CModel::DrawShadowmtx(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DLIGHT9 light;			// ���C�g���
	D3DXVECTOR4 posLight;		// ���C�g�̈ʒu
	D3DXVECTOR3 pos, normal;	// ���ʏ�̔C�ӂ̓_�A�@���x�N�g��
	D3DXPLANE plane;			// ���ʏ��
	D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;			// �}�e���A���f�[�^

	// ���C�g�̈ʒu��ݒ�
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// ���ʏ����쐬
	pos = D3DXVECTOR3(m_pos.x, 0.5f, m_pos.z);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// �V���h�E�}�g���b�N�X�̍쐬
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// �V���h�E�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// ���f���̐F��ۑ�
		D3DXCOLOR DiffuseModel = pMat[nCntMat].MatD3D.Diffuse;

		// ���f���̃J���[�ύX
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_apTexture[nCntMat]);

		// ���f���i�p�[�c�j�̕`��
		m_pMesh->DrawSubset(nCntMat);

		// �ۑ����Ă����F��߂�
		pMat[nCntMat].MatD3D.Diffuse = DiffuseModel;
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===============================================
// X�t�@�C���̊��蓖��
//===============================================
void CModel::BindX(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_dwNumMat = dwNumMat;

	// Vtx�T�C�Y�ݒ�
	SetVtxSize();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATERIAL *pMat;													// �}�e���A���ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_apTexture = new LPDIRECT3DTEXTURE9[m_dwNumMat];
	m_apIdxTexture = new int[m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// �e�N�X�`���t�@�C�������݂���
			// �e�N�X�`���̐ݒ�
			m_apIdxTexture[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_apTexture[nCntMat]);

			//m_apTexture[nCntMat] = CManager::GetTexture()->GetAddress(m_apIdxTexture[nCntMat]);
		}
		else
		{// ���݂��Ȃ�
			m_apIdxTexture[nCntMat] = NULL;
			m_apTexture[nCntMat] = NULL;
		}
	}
}

//===============================================
// Vtx�T�C�Y�ݒ�
//===============================================
void CModel::SetVtxSize(void)
{
	int nNumVtx;		// ���_��
	DWORD dwSizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE * pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		// �S�Ă̒��_���r���ă��f���̍ŏ��l�E�ő�l�𔲂��o��
		// �ŏ��l�̔�r
		if (vtx.x <= m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}

		if (vtx.y <= m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}

		if (vtx.z <= m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		// �ő�l�̔�r
		if (vtx.x >= m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (vtx.y >= m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (vtx.z >= m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �����ݒ�
//===============================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===============================================
// �e�ݒ�
//===============================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}