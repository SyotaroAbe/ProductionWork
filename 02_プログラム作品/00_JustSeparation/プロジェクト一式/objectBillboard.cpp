//=========================================================
//
// �r���{�[�h���� [objectBillboard.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "sound.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define BILLBOARD_LIFE		(150)		// �r���{�[�h�̗̑�
#define MAX_DIFF			(1.0f)		// �ő�␳�l
#define TRUE_DIFF			(3.0f)		// �␳�����l

//===============================================
// �R���X�g���N�^
//===============================================
CObjectBillboard::CObjectBillboard() : CObject(4)
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
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
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
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CObjectBillboard::~CObjectBillboard()
{

}

//===============================================
// ��������
//===============================================
CObjectBillboard *CObjectBillboard::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority)
{
	CObjectBillboard *pObjBillboard;

	// �I�u�W�F�N�g�r���{�[�h�̐���
	pObjBillboard = new CObjectBillboard(nPriority);

	// ��ނ̐ݒ�
	pObjBillboard->SetType(TYPE_BILLBOARD);

	// �T�C�Y�̐ݒ�
	pObjBillboard->SetSize(fSizeX, fSizeZ);

	// ����������
	pObjBillboard->Init(pos);

	return pObjBillboard;
}

//===============================================
// ����������
//===============================================
HRESULT CObjectBillboard::Init(D3DXVECTOR3 pos)
{
	// �ʒu��ݒ�
	m_pos = pos;

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
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeZ, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeZ, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CObjectBillboard::Uninit(void)
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
void CObjectBillboard::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxTrans;												// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;													// �r���[�}�g���b�N�X�擾�p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// �t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===============================================
// �G�t�F�N�g�̕`�揈��
//===============================================
void CObjectBillboard::DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxTrans;												// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;													// �r���[�}�g���b�N�X�擾�p

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// �t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===============================================
// �ʒu�̍X�V����
//===============================================
void CObjectBillboard::UpdatePos(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	m_pos = pos;
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeZ, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeZ, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �ʒu�␳����
//===============================================
void CObjectBillboard::RevisionPos(const D3DXVECTOR3 pos, float fMalti)
{
	if (fMalti > MAX_DIFF)
	{// �␳�l�����l�𒴂���
		fMalti = MAX_DIFF;
	}

	// �ړI�̃T�C�Y�܂ł̍������v�Z
	D3DXVECTOR3 posDiff = pos - m_pos;

	// �T�C�Y�̕␳
	m_pos.x += posDiff.x * fMalti;
	m_pos.z += posDiff.z * fMalti;
}

//===============================================
// �g��k������
//===============================================
bool CObjectBillboard::ScalingSize(const float fSize, float fMalti)
{
	// �ړI�̃T�C�Y�܂ł̍������v�Z
	float fSizeDiff = fSize - m_fSizeX;

	// �T�C�Y�̕␳
	m_fSizeX += fSizeDiff * fMalti;
	m_fSizeZ = m_fSizeX;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, m_fSizeZ, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fSizeX, -m_fSizeZ, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, -m_fSizeZ, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	if (m_fSizeX <= fSize + TRUE_DIFF && m_fSizeX >= fSize - TRUE_DIFF)
	{
		return true;
	}

	return false;
}

//===============================================
// �_�ŏ���
//===============================================
void CObjectBillboard::Brightness(float fBrightness)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^
	D3DXCOLOR colDiff = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (m_bFlash == false)
	{
		m_fBrightness -= fBrightness;	// ���邳�����Z
	}
	else
	{
		m_fBrightness += fBrightness;	// ���邳�����Z
	}

	if (m_fBrightness > MAX_DIFF)
	{// �l�����l�𒴂���
		m_fBrightness = MAX_DIFF;
		m_bFlash = false;
	}
	else if (m_fBrightness < 0.0f)
	{// �l�����l�𒴂���
		m_fBrightness = 0.0f;
		m_bFlash = true;
	}

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);
	pVtx[1].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);
	pVtx[2].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);
	pVtx[3].col = D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, m_fBrightness);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �e�N�X�`���̊��蓖��
//===============================================
void CObjectBillboard::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetTexture()->GetAddress(nIdx);
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CObjectBillboard::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CObjectBillboard::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �F�̐ݒ菈��
//===============================================
void CObjectBillboard::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �T�C�Y�̐ݒ菈��
//===============================================
void CObjectBillboard::SetSize(const float fSizeX, const float fSizeZ)
{
	m_fSizeX = fSizeX;
	m_fSizeZ = fSizeZ;
}