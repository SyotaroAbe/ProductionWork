//=========================================================
//
// �v���C���[���� [enemy.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "bullet.h"
#include "texture.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CEnemy::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CEnemy::CEnemy() : CObject2D(3)
{

}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CEnemy::CEnemy(int nPriority) : CObject2D(nPriority)
{
	
}

//===============================================
// �f�X�g���N�^
//===============================================
CEnemy::~CEnemy()
{

}

//===============================================
// ��������
//===============================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int nPriority)
{
	CEnemy *pEnemy;

	// �G�̐���
	pEnemy = new CEnemy(nPriority);

	// ��ނ̐ݒ�
	pEnemy->SetType(TYPE_ENEMY);

	// �T�C�Y�̐ݒ�
	pEnemy->SetSize(ENEMY_SIZEX, ENEMY_SIZEY);

	// ����������
	pEnemy->Init(pos);

	// �e�N�X�`���̊��蓖��
	pEnemy->BindTexture(m_nIdxTexture);

	return pEnemy;
}

//===============================================
// ����������
//===============================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g2D�̏���������
	CObject2D::Init(pos);

	// �e�N�X�`���̐ݒ�
	m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\enemy000.png");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CEnemy::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I������
	CObject2D::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CEnemy::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CEnemy::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`�揈��
	CObject2D::Draw();
}