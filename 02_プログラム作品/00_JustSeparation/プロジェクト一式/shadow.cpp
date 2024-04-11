//=========================================================
//
// �e�̕`�揈�� [shadow.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CShadow::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CShadow::CShadow() : CObject3D(1)
{
	
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	
}

//===============================================
// �f�X�g���N�^
//===============================================
CShadow::~CShadow()
{

}

//===============================================
// ��������
//===============================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float m_fSizeX, float m_fSizeZ, int nPriority)
{
	CShadow *pShadow;

	// �e�̐���
	pShadow = new CShadow(nPriority);

	// ��ނ̐ݒ�
	pShadow->SetType(TYPE_SHADOW);

	// �T�C�Y�̐ݒ�
	pShadow->SetSize(m_fSizeX, m_fSizeZ);

	// ����������
	pShadow->Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg");

	// �e�N�X�`���̊��蓖��
	pShadow->BindTexture(m_nIdxTexture);

	return pShadow;
}

//===============================================
// ����������
//===============================================
HRESULT CShadow::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g3D�̏���������
	CObject3D::Init(pos);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CShadow::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CShadow::Update(void)
{

}

//===============================================
// �`�揈��
//===============================================
void CShadow::Draw(void)
{
	// �e�̕`�揈��
	CObject3D::DrawShadow();
}