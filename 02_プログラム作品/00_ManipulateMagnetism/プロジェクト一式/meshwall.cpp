//=========================================================
//
// ���b�V���E�H�[���`�揈�� [meshwall.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "meshwall.h"
#include "bullet.h"
#include "camera.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CMeshwall::m_nIdxTexture = NULL;				// �g�p����e�N�X�`���̔ԍ�
LPDIRECT3DTEXTURE9 CMeshwall::m_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CMeshwall::CMeshwall() : CObjectMesh(1)
{
	//�l���N���A����
	
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CMeshwall::CMeshwall(int nPriority) : CObjectMesh(nPriority)
{
	//�l���N���A����
}

//===============================================
// �f�X�g���N�^
//===============================================
CMeshwall::~CMeshwall()
{

}

//===============================================
// ��������
//===============================================
CMeshwall *CMeshwall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority)
{
	CMeshwall *pObjectMesh;

	// ���b�V���t�B�[���h�̐���
	pObjectMesh = new CMeshwall(nPriority);

	// ��ނ̐ݒ�
	pObjectMesh->SetType(TYPE_FIELD);

	// �T�C�Y�̐ݒ�
	pObjectMesh->SetSize(fSizeX, fSizeZ);

	// ����������
	pObjectMesh->Init(pos);

	// �ݒ菈��
	//pObjectMesh->Set(pos, rot);

	// �e�N�X�`���̊��蓖��
	pObjectMesh->BindTexture(m_nIdxTexture);

	return pObjectMesh;
}

//===============================================
// ����������
//===============================================
HRESULT CMeshwall::Init(D3DXVECTOR3 pos)
{
	// �I�u�W�F�N�g���b�V���̏���������
	CObjectMesh::Init(pos);

	//m_PosMin = D3DXVECTOR3(MIN_FIELDPOS, MIN_FIELDPOS, MIN_FIELDPOS);
	//m_PosMax = D3DXVECTOR3(MAX_FIELDPOS, MAX_FIELDPOS, MAX_FIELDPOS);

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
void CMeshwall::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���̏I������
	CObjectMesh::Uninit();
}

//===============================================
// �X�V����
//===============================================
void CMeshwall::Update(void)
{

}

//===============================================
// �`�揈��
//===============================================
void CMeshwall::Draw(void)
{
	// �I�u�W�F�N�g���b�V���̕`�揈��
	CObjectMesh::Draw();
}

//===============================================
// �ݒ菈��
//===============================================
void CMeshwall::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
{
	m_pos = pos;
	m_rot = rot;

	srand(timeGetTime());										// rand�̒l��������
	int nNumXRand = 1;											// rand�̒l���擾�i��X�j
	int nNumYRand = rand() % 2 + 1;								// rand�̒l���擾�i��Y�j
}