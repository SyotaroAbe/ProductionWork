//=========================================================
//
// �I�u�W�F�N�g���b�V������ [objectMesh.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "objectMesh.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_MESH			(9)			// �ő僁�b�V����
#define INDEXMESH_BLOCK_X	(2)			// ���b�V���t�B�[���h�̃u���b�N���iX�j
#define INDEXMESH_SIZE_X	(200.0f)	// ���b�V���t�B�[���h�̃T�C�Y�iX�j

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CObjectMesh::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CObjectMesh::CObjectMesh() : CObject(3)
{
	// �l���N���A����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pldxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMeshX = 0;
	m_nCntMeshY = 0;
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObjectMesh::CObjectMesh(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pldxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMeshX = 0;
	m_nCntMeshY = 0;
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// �f�X�g���N�^
//===============================================
CObjectMesh::~CObjectMesh()
{

}

//===============================================
// ��������
//===============================================
CObjectMesh *CObjectMesh::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority)
{
	CObjectMesh *pObjectMesh;

	// �v���C���[�̐���
	pObjectMesh = new CObjectMesh(nPriority);

	// ��ނ̐ݒ�
	pObjectMesh->SetType(TYPE_FIELD);

	// �T�C�Y�̐ݒ�
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// ����������
	pObjectMesh->Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\field000.jpg");

	// �e�N�X�`���̊��蓖��
	pObjectMesh->BindTexture(m_nIdxTexture);

	return pObjectMesh;
}

//===============================================
// ����������
//===============================================
HRESULT CObjectMesh::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	int nMaxMesh = 9;

	// �ϐ��̏�����
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMaxMesh,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;   // ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	//for (int nCnt = 0; nCnt < m_nCntMeshX; nCnt++)
	//{
	//	for (int nCntMesh = 0; nCntMesh < m_nCntMeshY; nCntMesh++)
	//	{
	//		pVtx[nCnt * nCntMesh].pos = D3DXVECTOR3(-m_nCntMeshX + m_fSizeX * nCnt, 0.0f, m_nCntMeshY - m_fSizeZ * nCntMesh);
	//	}
	//}

	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, m_fSizeZ);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, m_fSizeZ);
	pVtx[2].pos = D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ);
	pVtx[3].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(m_fSizeX, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, -m_fSizeZ);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -m_fSizeZ);
	pVtx[8].pos = D3DXVECTOR3(m_fSizeX, 0.0f, -m_fSizeZ);

	// �@���x�N�g���̐ݒ�
	for (int nCntMesh = 0; nCntMesh < nMaxMesh; nCntMesh++)
	{
		pVtx[nCntMesh].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// ���_�J���[�̐ݒ�
	for (int nCntMesh = 0; nCntMesh < nMaxMesh; nCntMesh++)
	{
		pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	//for (nCntMesh = 0; nCntMesh < nMaxMesh; nCntMesh++)
	//{

	//}
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pldxBuff,
		NULL);

	WORD * pldx;	// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pldxBuff->Lock(0, 0, (void**)&pldx, 0);

	// ���_�ԍ��f�[�^�̐ݒ�
	pldx[0] = 3;
	pldx[1] = 0;
	pldx[2] = 4;
	pldx[3] = 1;
	pldx[4] = 5;
	pldx[5] = 2;
	pldx[6] = 2;
	pldx[7] = 6;
	pldx[8] = 6;
	pldx[9] = 3;
	pldx[10] = 7;
	pldx[11] = 4;
	pldx[12] = 8;
	pldx[13] = 5;

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pldxBuff->Unlock();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CObjectMesh::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pldxBuff != NULL)
	{
		m_pldxBuff->Release();
		m_pldxBuff = NULL;
	}

	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CObjectMesh::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;
}

//===============================================
// �`�揈��
//===============================================
void CObjectMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(m_pldxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,
		0,
		12);
}

//===============================================
// �e�N�X�`���̊��蓖��
//===============================================
void CObjectMesh::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetTexture()->GetAddress(nIdx);
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CObjectMesh::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CObjectMesh::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �T�C�Y�̐ݒ菈��
//===============================================
void CObjectMesh::SetSize(const float fSizeX, const float fSizeZ)
{
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;
}

//===============================================
// �F�̐ݒ菈��
//===============================================
void CObjectMesh::SetCol(const COL col)
{
	VERTEX_3D * pVtx;   // ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �w�肵���F�֕ύX
	switch (col)
	{
	case COL_NORMAL:	// �ʏ�
		// ���_�J���[�̐ݒ�
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;

	case COL_RED:		// ��
		// ���_�J���[�̐ݒ�
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		break;

	case COL_BLUE:		// ��
		// ���_�J���[�̐ݒ�
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		}
		break;

	case COL_GREEN:		//��
		// ���_�J���[�̐ݒ�
		for (int nCntMesh = 0; nCntMesh < MAX_MESH; nCntMesh++)
		{
			pVtx[nCntMesh].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		}
		break;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}