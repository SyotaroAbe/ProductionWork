//=========================================================
//
// ���d�̔w�i���� [multibg.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "multibg.h"
#include "manager.h"
#include "renderer.h"
#include "bg.h"
#include "texture.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CObject2D *CMultiBg::m_apObject2D[MAX_OBJECT] = {};		// �I�u�W�F�N�g2D�N���X�̃|�C���^
const char *CMultiBg::m_apTextureData[NUM_BG] = {};		// �e�N�X�`���t�@�C����
int CMultiBg::m_aIdxTexture[NUM_BG] = {};				// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CMultiBg::CMultiBg() : CObject(0)
{
	// �l���N���A
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fTexU = 0.0f;
	m_fTexV = 0.0f;
}

//===============================================
// �f�X�g���N�^
//===============================================
CMultiBg::~CMultiBg()
{

}

//===============================================
// ��������
//===============================================
CMultiBg *CMultiBg::Create(int nPriority)
{
	CMultiBg *pMultiBg;

	// �w�i�̐���
	pMultiBg = new CMultiBg;

	// ��ނ̐ݒ�
	pMultiBg->SetType(TYPE_BG);

	// ����������
	pMultiBg->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	return pMultiBg;
}

//===============================================
// ����������
//===============================================
HRESULT CMultiBg::Init(D3DXVECTOR3 pos)
{
	int nCntBg = 0;

	// �e�N�X�`���t�@�C����
	const char *m_apTextureData[NUM_BG] =
	{
		{ "data\\TEXTURE\\BgMulti000.png" },	// �w�i0
		{ "data\\TEXTURE\\BgMulti001.png" },	// �w�i1
		{ "data\\TEXTURE\\BgMulti002.png" }		// �w�i2
	};

	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (m_apObject2D[nCntObj] == NULL)
		{
			// �w�i�̐���
			m_apObject2D[nCntObj] = CBg::Create(0);
			m_apObject2D[nCntObj]->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

			// �e�N�X�`���̊��蓖��
			m_aIdxTexture[nCntObj] = CManager::GetTexture()->Regist(m_apTextureData[nCntObj]);
			nCntBg++;

			if (nCntBg == NUM_BG)
			{// �w�i�̖������ő吔�ɂȂ���
				break;
			}
		}
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CMultiBg::Uninit(void)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != NULL)
		{// �g�p����Ă���
			m_apObject2D[nCntObj] = NULL;
		}
	}

	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CMultiBg::Update(void)
{
	for (int nCntBg = 0; nCntBg < MAX_OBJECT; nCntBg++)
	{
		if (m_apObject2D[nCntBg] != NULL)
		{
			TYPE type;
			type = m_apObject2D[nCntBg]->GetType();

			if (type == TYPE_BG)
			{// �w�i
				// �I�u�W�F�N�g2D�̍X�V����
				m_apObject2D[nCntBg]->Update();
			}
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CMultiBg::Draw(void)
{
	for (int nCntBg = 0; nCntBg < MAX_OBJECT; nCntBg++)
	{
		if (m_apObject2D[nCntBg] != NULL)
		{
			TYPE type;
			type = m_apObject2D[nCntBg]->GetType();

			if (type == TYPE_BG)
			{// �w�i
				// �I�u�W�F�N�g2D�̕`�揈��
				m_apObject2D[nCntBg]->Draw();
			}
		}
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CMultiBg::Set(const float TexU, const float TexV)
{
	m_fTexU = TexU;
	m_fTexV = TexV;
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CMultiBg::SetPos(const D3DXVECTOR3 pos)
{
	
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CMultiBg::SetMove(const D3DXVECTOR3 move)
{

}

//===============================================
// �����ݒ�
//===============================================
void CMultiBg::SetRot(const D3DXVECTOR3 rot)
{
	
}