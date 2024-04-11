//=========================================================
//
// �G�f�B�b�g���� [editor.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "xfile.h"
#include "player.h"
#include "debugproc.h"
#include "garbage.h"
#include "game.h"

//===============================================
// �}�N����`
//===============================================
#define MOVE_MODEL			(1.0f)					// ���f���̈ړ���
#define ROT_LEFTUP			(-0.25f)				// ����̊p�x
#define ROT_LEFT			(-0.5f)					// ���̊p�x
#define ROT_LEFTDOWN		(-0.75f)				// �����̊p�x
#define ROT_RIGHTUP			(0.25f)					// �E��̊p�x
#define ROT_RIGHT			(0.5f)					// �E�̊p�x
#define ROT_RIGHTDOWN		(0.75f)					// �E���̊p�x
#define ROT_UP				(0.0f)					// ��̊p�x
#define ROT_DOWN			(1.0f)					// ���̊p�x

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CObjectX *CEdit::m_apObjectX[MAX_MODELEDIT];			// �I�u�W�F�N�gX�N���X�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CEdit::CEdit()
{
	// �l���N���A����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nType = CGarbage::TYPE_BAG;
	m_nID = 0;
	m_bEdit = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CEdit::~CEdit()
{

}

//===============================================
// ����������
//===============================================
HRESULT CEdit::Init(void)
{
	for (int nCntObj = 0; nCntObj < 1; nCntObj++)
	{
		if (m_apObjectX[nCntObj] == NULL)
		{// �g�p����Ă��Ȃ�
			// ����
			m_apObjectX[nCntObj] = new CObjectX(3);

			// ��ނ̐ݒ�
			m_apObjectX[nCntObj]->SetType(CObject::TYPE_XFAIL);

			// ������
			m_apObjectX[nCntObj]->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CEdit::Uninit(void)
{
	// �I�u�W�F�N�gX�̏I������
	if (m_apObjectX[0] != NULL)
	{// �g�p����Ă���
		m_apObjectX[0]->Uninit();
	}
}

//===============================================
// �X�V����
//===============================================
void CEdit::Update(void)
{
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_A) == true)
	{// ���L�[�������ꂽ
		if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true)
		{// ����ړ�
			m_pos.x += sinf(D3DX_PI * ROT_LEFTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_LEFTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true)
		{// �����ړ�
			m_pos.x += sinf(D3DX_PI * ROT_LEFTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_LEFTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else
		{// ���ړ�
			m_pos.x += sinf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_LEFT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_D) == true)
	{// �E�L�[�������ꂽ
		if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true)
		{// �E��ړ�
			m_pos.x += sinf(D3DX_PI * ROT_RIGHTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_RIGHTUP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true)
		{// �E���ړ�
			m_pos.x += sinf(D3DX_PI * ROT_RIGHTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_RIGHTDOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
		else
		{// �E�ړ�
			m_pos.x += sinf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
			m_pos.z += cosf(D3DX_PI * ROT_RIGHT + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_W) == true)
	{// ��L�[�������ꂽ
		m_pos.x += sinf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		m_pos.z += cosf(D3DX_PI * ROT_UP + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_S) == true)
	{// ���L�[�������ꂽ
		m_pos.x += sinf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
		m_pos.z += cosf(D3DX_PI * ROT_DOWN + (ROT_CAMERA * CManager::GetCamera()->GetRot().y)) * MOVE_MODEL;
	}

	// ���f���̎�ނ̕ύX
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_LSHIFT) == true)
	{// ��SHIFT�L�[
		m_nType++;

		if (m_nType >= CGarbage::TYPE_MAX)
		{// �ő��ނ𒴂���
			// ��ނ�͈͓��ɐݒ肷��
			m_nType = CGarbage::TYPE_BAG;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_LCONTROL) == true)
	{// ��CTRL�L�[
		m_nType--;

		if (m_nType < CGarbage::TYPE_BAG)
		{// �ŏ���ނ𒴂���
			// ��ނ�͈͓��ɐݒ肷��
			m_nType = CGarbage::TYPE_MAX -1;
		}
	}

	// �ʒu�̐ݒ�
	m_apObjectX[m_nID]->SetPos(m_pos);

	CManager::GetDebugProc()->Print(" ���f���^�C�v�F%d\n", m_nType);
	CManager::GetDebugProc()->Print(" ���f��posX�F%f\n", m_pos.x);
	CManager::GetDebugProc()->Print(" ���f��posZ�F%f\n", m_pos.z);
}

//===============================================
// �`�揈��
//===============================================
void CEdit::Draw(void)
{
	// X�t�@�C���̓����`�揈��
	m_apObjectX[m_nID]->Draw();
	CManager::GetXFile()->CleannessDraw(m_nType);
}

//===============================================
// �G�f�B�b�g�؂�ւ�����
//===============================================
void CEdit::Set(bool bEdit)
{
	m_bEdit = bEdit;
	m_pos = CGame::GetPlayer()->GetPos();
}