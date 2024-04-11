//=========================================================
//
// �S�~�X�e�[�V�������� [dumpster.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "dumpster.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "xfile.h"
#include "game.h"
#include "target.h"
#include "tutorial.h"
#include "player.h"

//===============================================
// �}�N����`
//===============================================
#define DELETE_POSITION			(550.0f)	// ����j������ʒu

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CDumpster::m_nIdxXFile = 0;			// �g�p����X�t�@�C���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CDumpster::CDumpster() : CObjectX(4)
{
	// �l�̃N���A
	m_bRockon = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CDumpster::CDumpster(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_bRockon = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CDumpster::~CDumpster()
{
	
}

//===============================================
// ���f���̐����i�ǂݍ��݁j
//===============================================
void CDumpster::Load(HWND hWnd)
{
	FILE *pFile;
	
	// �w�t�@�C���ɓǂݍ���
	m_nIdxXFile = CManager::GetXFile()->Regist("data\\MODEL\\Dumpster001.x");

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// �Q�[�����[�h
		// �t�@�C�����J��
		pFile = fopen("data\\TXT\\model.txt", "r");

		if (pFile != NULL)
		{// �ǂݍ��ݐ���
			char aStr[MAX_NAME] = {};

			while (1)
			{
				int nResult = fscanf(pFile, "%s", &aStr[0]);

				if (strcmp(&aStr[0], "SCRIPT") == 0)
				{// SCRIPT���ǂݍ���
					Script(pFile);
					break;
				}
				else if (nResult == EOF)
				{// �Ō�܂œǂݍ���
					break;
				}
			}

			// �t�@�C������
			fclose(pFile);
		}
		else
		{
			MessageBox(hWnd, "�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}
	}
}

//===============================================
// Script���ǂݍ��ݏ���
//===============================================
void CDumpster::Script(FILE *pFile)
{
	char aStr[MAX_NAME] = {};

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "DUMPSTERSET") == 0)
		{// ���f�����ǂݍ���
			ModelSet(pFile);
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// �Ō�܂œǂݍ���
			break;
		}
	}
}

//===============================================
// Model���ǂݍ��ݏ���
//===============================================
void CDumpster::ModelSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};
	MODEL type;					// ���
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 size;			// �T�C�Y

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TYPE") == 0)
		{// ��ޓǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &type);
		}
		else if (strcmp(&aStr[0], "POS") == 0)
		{// �ʒu�ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// �����ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);
		}

		if (strcmp(&aStr[0], "END_DUMPSTERSET") == 0)
		{// �Ō�܂œǂݍ���
			Create(pos, rot, 3);
			break;
		}
	}
}

//===============================================
// ��������
//===============================================
CDumpster *CDumpster::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CDumpster *pGarbage = NULL;

	// �S�~�̐���
	pGarbage = new CDumpster(nPriority);

	// ��ނ̐ݒ�
	pGarbage->SetType(CObject::TYPE_DUMPSTER);

	// ����������
	pGarbage->Init(pos);

	// �����̐ݒ�
	pGarbage->SetRot(rot);

	return pGarbage;
}

//===============================================
// ����������
//===============================================
HRESULT CDumpster::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init(pos);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CDumpster::Uninit(void)
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CDumpster::Update(void)
{
	// �O��̈ʒu��ۑ�
	m_posOld = m_pos;

	if (CManager::GetMode() == CScene::MODE_TUTORIAL 
		&& (m_pos.x <= CTutorial::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.x >= CTutorial::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
		&& m_pos.z <= CTutorial::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.z >= CTutorial::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEX)
		|| CManager::GetMode() == CScene::MODE_GAME 
		&& (m_pos.x <= CGame::GetTarget()->GetPos().x + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.x >= CGame::GetTarget()->GetPos().x - DUMPSTER_LENTH - TARGET_SIZEX
		&& m_pos.z <= CGame::GetTarget()->GetPos().z + DUMPSTER_LENTH + TARGET_SIZEX && m_pos.z >= CGame::GetTarget()->GetPos().z - DUMPSTER_LENTH - TARGET_SIZEX))
	{// �^�[�Q�b�g���S�~�X�e�[�V�����𗘗p�ł���͈͓��ɓ�����
		m_bRockon = true;	// �^�[�Q�b�g�͈͓̔��֐؂�ւ���
	}
	else
	{
		m_bRockon = false;	// �^�[�Q�b�g�͈̔͊O�֐؂�ւ���
	}

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (CTutorial::GetPlayer()->GetPos().z < m_pos.z - DELETE_POSITION)
		{// ��ʊO�ւ�����
			// �I������
			Uninit();
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CDumpster::Draw(void)
{
	if (m_bRockon == true)
	{// �^�[�Q�b�g�͈͓̔�
		// �I�u�W�F�N�gX�̕`�揈���i�ΐF�j
		CObjectX::DrawXFile(m_nIdxXFile, CXFile::COL_GREEN);
	}
	else
	{// �^�[�Q�b�g�͈̔͊O
		// �I�u�W�F�N�gX�̕`�揈���i�ʏ�F�j
		CObjectX::DrawXFile(m_nIdxXFile, CXFile::COL_NORMAL);
	}
}