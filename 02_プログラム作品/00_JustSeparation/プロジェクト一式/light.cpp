//=========================================================
// 
// ���C�g���� [light.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

//===============================================
// �R���X�g���N�^
//===============================================
CLight::CLight()
{
	for (int nCntLight = 0; nCntLight < MAX_LGHT; nCntLight++)
	{// ���C�g�̍ő吔���J��Ԃ�
		// �����N���A����
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));
	}
}

//===============================================
// �f�X�g���N�^
//===============================================
CLight::~CLight()
{

}

//===============================================
// ����������
//===============================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;													// �ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LGHT; nCntLight++)
	{// ���C�g�̍ő吔���J��Ԃ�
		// �����N���A����
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ��ނ�ݒ�
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// �g�U����ݒ�
		m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	}

	// ������ݒ�
	vecDir = D3DXVECTOR3(0.22f, -0.87f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);		// �x�N�g���𐳋K������
	m_aLight[0].Direction = vecDir;
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �g�U����ݒ�

	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);		// �x�N�g���𐳋K������
	m_aLight[1].Direction = vecDir;
	m_aLight[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);	// �g�U����ݒ�

	vecDir = D3DXVECTOR3(-0.79f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);		// �x�N�g���𐳋K������
	m_aLight[2].Direction = vecDir;
	m_aLight[2].Diffuse = D3DXCOLOR(0.35f, 0.35f, 0.35f, 1.0f);	// �g�U����ݒ�

	for (int nCntLight = 0; nCntLight < MAX_LGHT; nCntLight++)
	{// ���C�g�̍ő吔���J��Ԃ�
		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CLight::Uninit(void)
{
	
}

//===============================================
// �X�V����
//===============================================
void CLight::Update(void)
{

}