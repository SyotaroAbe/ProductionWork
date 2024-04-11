//=========================================================
//
// �I�u�W�F�N�g2D���� [object2D.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "multibg.h"
#include "player.h"
#include "texture.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_DIFF			(1.0f)		// �ő�␳�l
#define TRUE_DIFF			(3.0f)		// �␳�����l
#define MAX_BRIGHTNESS		(1.0f)		// �ő喾�邳
#define MIN_BRIGHTNESS		(0.0f)		// �ŏ����邳

//===============================================
// �R���X�g���N�^�i�f�t�H���g�j
//===============================================
CObject2D::CObject2D() : CObject(3)
{
	// �l���N���A����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CObject2D::~CObject2D()
{

}

//===============================================
// ����������
//===============================================
HRESULT CObject2D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾
	VERTEX_2D *pVtx;													// ���_���ւ̃|�C���^

	// �l����
	m_pos = pos;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (m_pVtxBuff == NULL)
	{// �g�p����Ă��Ȃ�
		return -1;
	}

	// �Ίp���̒������Z�o����
	m_fLength = sqrtf(m_fSizeX * 2 * m_fSizeX * 2 + m_fSizeY * 2 * m_fSizeY * 2) * 0.5f;

	// �Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(m_fSizeX * 2, m_fSizeY * 2);

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	if (GetType() == TYPE_PLAYER)
	{// �v���C���[�̏ꍇ
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
	}
	else if (GetType() == TYPE_BLOCK)
	{// �u���b�N�̏ꍇ
		pVtx[0].pos = D3DXVECTOR3(m_pos.x,			m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x,			m_pos.y + m_fSizeY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	}
	else
	{// ���̑�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);
	}

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	if (GetType() == TYPE_EXPLOSION)
	{// �����̏ꍇ
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.125f * 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + 0.125f * 0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.125f * 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + 0.125f * 0.0f, 1.0f);
	}
	else
	{// ���̑�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===============================================
// �ʒu�̍X�V����
//===============================================
void CObject2D::UpdatePos(D3DXVECTOR3 pos, float m_fSizeX, float m_fSizeY)
{
	m_pos = pos;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	if (GetType() == TYPE_PLAYER)
	{// �v���C���[
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
	}
	else if (GetType() == TYPE_BLOCK)
	{// �u���b�N�̏ꍇ
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fSizeY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	}
	else
	{
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �p�x���g�p�����ʒu�̍X�V����
//===============================================
void CObject2D::UpdatePosAngle(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �F�̍X�V����
//===============================================
void CObject2D::SetCol(const D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

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
// �A�j���[�V�����̍X�V����
//===============================================
void CObject2D::UpdateAnim(int nPatternX, int nPatternY, int nSpeed)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2(0.0f + 1.0f / nPatternX * m_nPatternAnim, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.2f + 1.0f / nPatternX * m_nPatternAnim, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f + 1.0f / nPatternX * m_nPatternAnim, 0.5f);
	//pVtx[3].tex = D3DXVECTOR2(0.2f + 1.0f / nPatternX * m_nPatternAnim, 0.5f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �e�N�X�`�����W�̍X�V����
//===============================================
void CObject2D::UpdateTex(const float fTexU, const float fSubU, const float fAddU, const float fTexV, const float fSubV, const float fAddV)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].tex = D3DXVECTOR2(fTexU - fSubU, fTexV - fSubV);
	pVtx[1].tex = D3DXVECTOR2(fTexU + fAddU, fTexV - fSubV);
	pVtx[2].tex = D3DXVECTOR2(fTexU - fSubU, fTexV + fAddV);
	pVtx[3].tex = D3DXVECTOR2(fTexU + fAddU, fTexV + fAddV);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �ʒu�␳����
//===============================================
bool CObject2D::RevisionPos(const D3DXVECTOR3 pos, float fMalti, bool bWidth)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (fMalti >= MAX_DIFF)
	{// �␳�l�����l�𒴂���
		fMalti = MAX_DIFF;
	}

	if (bWidth == false && pos.y + TRUE_DIFF >= m_pos.y && pos.y - TRUE_DIFF <= m_pos.y)
	{// �␳����
		return true;
	}
	else if (bWidth == true && pos.x + TRUE_DIFF >= m_pos.x && pos.x - TRUE_DIFF <= m_pos.x)
	{// �␳����
		return true;
	}

	// �ړI�̃T�C�Y�܂ł̍������v�Z
	D3DXVECTOR3 posDiff = pos - m_pos;

	// �T�C�Y�̕␳
	m_pos.x += posDiff.x * fMalti;
	m_pos.y += posDiff.y * fMalti;

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
		m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return false;
}

//===============================================
// �G�t�F�N�g�̕`�揈��
//===============================================
void CObject2D::DrawEffect(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   // �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
// �_�ŏ���
//===============================================
void CObject2D::Brightness(float fBrightness)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
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
void CObject2D::BindTexture(int nIdx)
{
	m_pTexture = CManager::GetTexture()->GetAddress(nIdx);
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	if (GetType() == TYPE_PLAYER)
	{// �v���C���[
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - PLAYER_SIZEX, m_pos.y - PLAYER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + PLAYER_SIZEX, m_pos.y - PLAYER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - PLAYER_SIZEX, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + PLAYER_SIZEX, m_pos.y, 0.0f);
	}
	else
	{
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI + m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (D3DX_PI + m_fAngle)) * m_fLength, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (D3DX_PI - m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (D3DX_PI - m_fAngle)) * m_fLength, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (-m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (-m_fAngle)) * m_fLength, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.x + (m_fAngle)) * m_fLength,
			m_pos.y + cosf(m_rot.y + (m_fAngle)) * m_fLength, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CObject2D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �i���o�[�̐ݒ菈��
//===============================================
void CObject2D::SetNumber(const int nNumber)
{
	int nTexNumber = nNumber;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f *  nTexNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f *  nTexNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f *  nTexNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f *  nTexNumber), 1.0f);
		
	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===============================================
// �T�C�Y�̐ݒ菈��
//===============================================
void CObject2D::SetSize(const float fSizeX, const float fSizeY)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
}