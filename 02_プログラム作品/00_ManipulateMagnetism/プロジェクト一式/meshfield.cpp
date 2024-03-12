//=========================================================
// 
// ���b�V���t�B�[���h�`�揈�� [meshfield.cpp]
// Author = �����đ�Y
// 
//=========================================================
#include "main.h"
#include "meshfield.h"
#include "texture.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "tutorial.h"
#include "title.h"
#include "renderer.h"

//===============================================
// �}�N����`
//===============================================
#define FIELD_COUNTX			(6)			// ���̔z�u���iX�j
#define FIELD_COUNTY			(6)			// ���̔z�u���iY�j
#define MIN_FIELDPOS			(9900.0f)	// ���ŏ��ʒu�����l
#define MAX_FIELDPOS			(-9900.0f)	// ���ő�ʒu�����l

#define TURN_ROT_DIFF			(0.001f)	// �Ȃ���p�x�̍���
#define TURN_LENTH				(0.25f)		// �Ȃ����͈�
#define TURN_DIFF				(50.0f)		// �Ȃ���鍷��

#define LENTH_FAR				(240.0f)	// ���̒��S����̋����i�����j
#define LENTH_GABAGE			(180.0f)	// ���̒��S����̋����i���j
#define LENTH_NEAR				(150.0f)	// ���̒��S����̋����i�߂��j
#define GABAGE_POSITION			(280.0f)	// �S�~�̒u�����ʒu
#define DISTANCE_HEIGHT			(100.0f)	// �S�~���m�̏c�Ԋu
#define DISTANCE_WIDTH			(220.0f)		// �S�~���m�̉��Ԋu

// TUTORIAL
#define DELETE_POSITION			(650.0f)	// ����j������ʒu

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CMeshField::m_nIdxTexture = NULL;				// �g�p����e�N�X�`���̔ԍ�
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
CMeshField::TYPE CMeshField::m_aType[MAX_FIELD] = {};		// ���̎��
int CMeshField::m_nNumAll = 0;								// ����

//===============================================
// �R���X�g���N�^
//===============================================
CMeshField::CMeshField() : CObjectMesh(3)
{
	//�l���N���A����
	m_nTimeFever = 0;
	m_nRandFever = 0;

	m_nNumAll++;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CMeshField::CMeshField(int nPriority) : CObjectMesh(nPriority)
{
	//�l���N���A����
	m_nTimeFever = 0;
	m_nRandFever = 0;

	m_nNumAll++;
}

//===============================================
// �f�X�g���N�^
//===============================================
CMeshField::~CMeshField()
{
	m_nNumAll--;
}

//===============================================
// ��������
//===============================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority)
{
	CMeshField *pObjectMesh;

	// ���b�V���t�B�[���h�̐���
	pObjectMesh = new CMeshField(nPriority);

	// ��ނ̐ݒ�
	pObjectMesh->SetType(TYPE_FIELD);

	// �T�C�Y�̐ݒ�
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// ����������
	pObjectMesh->Init(pos);

	// �ݒ菈��
	pObjectMesh->Set(pos, rot);

	// �e�N�X�`���̊��蓖��
	pObjectMesh->BindTexture(m_nIdxTexture);

	return pObjectMesh;
}

//===============================================
// ����������
//===============================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g���b�V���̏���������
	CObjectMesh::Init(pos);

	m_PosMin = D3DXVECTOR3(MIN_FIELDPOS, MIN_FIELDPOS, MIN_FIELDPOS);
	m_PosMax = D3DXVECTOR3(MAX_FIELDPOS, MAX_FIELDPOS, MAX_FIELDPOS);
	
	// ���̈ʒu�̍ŏ��l���r
	if (m_pos.x <= m_PosMin.x)
	{// �ŏ��l���X�V
		m_PosMin.x = m_pos.x;
	}

	if (m_pos.z <= m_PosMin.z)
	{// �ŏ��l���X�V
		m_PosMin.z = m_pos.z;
	}

	// ���̈ʒu�̍ő�l���r
	if (m_pos.x >= m_PosMax.x)
	{// �ő�l���X�V
		m_PosMax.x = m_pos.x;
	}

	if (m_pos.z >= m_PosMax.z)
	{// �ő�l���X�V
		m_PosMax.z = m_pos.z;
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CMeshField::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���̏I������
	CObjectMesh::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CMeshField::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CMeshField::Draw(void)
{
	// �I�u�W�F�N�g���b�V���̕`�揈��
	CObjectMesh::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CMeshField::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	srand(timeGetTime());										// rand�̒l��������
	int nNumXRand = 1;											// rand�̒l���擾�i��X�j
	int nNumYRand = rand() % 2 + 1;								// rand�̒l���擾�i��Y�j

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// �Q�[�����[�h
		
	}
}