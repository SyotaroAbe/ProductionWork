//=========================================================
//
// �I�u�W�F�N�g3D���� [object3D.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "player.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CObject3D::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CObject3D::CObject3D() : CObject(3)
{
	// �l���N���A����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_mtxWorld;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;
}

//===============================================
// �f�X�g���N�^
//===============================================
CObject3D::~CObject3D()
{

}

//===============================================
// ��������
//===============================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, float m_fSizeX, float m_fSizeZ, int nPriority)
{
	CObject3D *pObject3D;

	// �v���C���[�̐���
	pObject3D = new CObject3D(nPriority);

	// ��ނ̐ݒ�
	pObject3D->SetType(TYPE_FIELD);

	// �T�C�Y�̐ݒ�
	pObject3D->SetSize(m_fSizeX, m_fSizeZ);

	// ����������
	pObject3D->Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\field000.jpg");

	// �e�N�X�`���̊��蓖��
	pObject3D->BindTexture(m_nIdxTexture);

	return pObject3D;
}

//===============================================
// ����������
//===============================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;   // ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, 0.0f, m_fSizeZ);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, 100.0f, m_fSizeZ);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, 100.0f, -m_fSizeZ);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, 0.0f, -m_fSizeZ);

	// �x�N�g���̌v�Z
	m_vec0 = pVtx[1].pos - pVtx[0].pos;
	m_vec1 = pVtx[2].pos - pVtx[1].pos;

	D3DXVECTOR3 Vec0;
	Vec0.x = (m_vec0.z * m_vec1.y) - (m_vec0.y * m_vec1.z);
	Vec0.y = (m_vec0.z * m_vec1.x) - (m_vec0.x * m_vec1.z);
	Vec0.z = (m_vec0.y * m_vec1.x) - (m_vec0.x * m_vec1.y);

	// ���K������
	float fVec = sqrtf((m_vec0.x * m_vec1.x + m_vec0.y  * m_vec1.y + m_vec0.z * m_vec1.z)
		* (m_vec0.x * m_vec1.x + m_vec0.y  * m_vec1.y + m_vec0.z * m_vec1.z));

	Vec0.x /= fVec;
	Vec0.y /= fVec;
	Vec0.z /= fVec;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = Vec0;
	pVtx[1].nor = Vec0;
	pVtx[2].nor = Vec0;
	pVtx[3].nor = Vec0;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_���W�̑��
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		m_aVtx[nCntVtx] = pVtx[nCntVtx].pos;
	}

	// �@���x�N�g���̑��
	m_nor = pVtx[0].nor;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CObject3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CObject3D::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CObject3D::Draw(void)
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

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===============================================
// �`�揈��
//===============================================
void CObject3D::DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
// �e�N�X�`���̊��蓖��
//===============================================
void CObject3D::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetTexture()->GetAddress(nIdx);
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CObject3D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CObject3D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �T�C�Y�̐ݒ菈��
//===============================================
void CObject3D::SetSize(const float fSizeX, const float fSizeZ)
{
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;
}

//===============================================
// �����蔻��
//===============================================
float CObject3D::CollisionVec(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0ToPos = pos - m_aVtx[0];
	D3DXVECTOR3 vec2ToPos = pos - m_aVtx[2];
	D3DXVECTOR3 vecLine0 = m_aVtx[1] - m_aVtx[0];
	D3DXVECTOR3 vecLine1 = m_aVtx[2] - m_aVtx[0];
	D3DXVECTOR3 vecLine2 = m_aVtx[1] - m_aVtx[2];

	if ((vecLine0.z * vec0ToPos.x) - (vecLine0.x * vec0ToPos.z) > 0.0f
		&& (vecLine1.z * vec0ToPos.x) - (vecLine1.x * vec0ToPos.z) < 0.0f
		&& (vecLine2.z * vec2ToPos.x) - (vecLine2.x * vec2ToPos.z) < 0.0f)
	{// �v���C���[���|���S���̓����ɂ���
		float fHeight;	// ���߂鍂��

		// 2�̃x�N�g������@�������߂�
		D3DXVec3Cross(&m_nor, &m_vec0, &m_vec1);

		// �@���𐳋K������
		D3DXVec3Normalize(&m_nor, &m_nor);

		if (m_nor.y != 0.0f)
		{
			fHeight = ((m_aVtx[0].x - pos.x) * -m_nor.x + (m_aVtx[0].z - pos.z) * -m_nor.z + m_aVtx[0].y) / -m_nor.y;
		}

		return fHeight;
	}

	return pos.y;
}