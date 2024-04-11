//=========================================================
//
// �I�u�W�F�N�gX�t�@�C������ [objectX.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "objectX.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "texture.h"
#include "xfile.h"
#include "player.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
int CObjectX::m_aIdxXFile[MODEL_MAX] = {};	// �g�p����X�t�@�C���̔ԍ�

//===============================================
// �R���X�g���N�^
//===============================================
CObjectX::CObjectX() : CObject(3)
{
	// �l���N���A����
	m_modelType = MODEL_NONE;
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	// �l���N���A����
	m_modelType = MODEL_NONE;
	m_mtxWorld;
	m_nIdxShadow = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// �f�X�g���N�^
//===============================================
CObjectX::~CObjectX()
{

}

//===============================================
// �ǂݍ��ݏ���
//===============================================
void CObjectX::Load(HWND hWnd)
{
	// ���f���ǂݍ���
	m_aIdxXFile[MODEL_HOUSE00] = CManager::GetXFile()->Regist("data\\MODEL\\House000.x");
	m_aIdxXFile[MODEL_HOUSE01] = CManager::GetXFile()->Regist("data\\MODEL\\House001.x");

	FILE *pFile;

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

//===============================================
// Script���ǂݍ��ݏ���
//===============================================
void CObjectX::Script(FILE *pFile)
{
	char aStr[MAX_NAME] = {};

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "MODELSET") == 0)
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
void CObjectX::ModelSet(FILE *pFile)
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

		if (strcmp(&aStr[0], "END_MODELSET") == 0)
		{// �Ō�܂œǂݍ���
			CObjectX::Create(pos, rot, type, 3);
			break;
		}
	}
}

//===============================================
// ��������
//===============================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority)
{
	CObjectX *pObjX;

	// �I�u�W�F�N�g�̐���
	pObjX = new CObjectX(nPriority);

	// ��ނ̐ݒ�
	pObjX->SetType(TYPE_XFAIL);

	// ���f���̐ݒ�
	pObjX->SetModel(type);

	// ����������
	pObjX->Init(pos);

	// �����ݒ�
	pObjX->SetRot(rot);

	return pObjX;
}

//===============================================
// ����������
//===============================================
HRESULT CObjectX::Init(D3DXVECTOR3 pos)
{
	//// �e��ݒ�
	//m_nIdxShadow = SetShadow();

	// �ʒu�𔽉f
	m_pos = pos;

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CObjectX::Uninit(void)
{
	this->Release();
}

//===============================================
// �X�V����
//===============================================
void CObjectX::Update(void)
{
	
}

//===============================================
// �`�揈��
//===============================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// X�t�@�C���̕`��
	CManager::GetXFile()->Draw(m_aIdxXFile[m_modelType], CXFile::COL_NORMAL);

	//// ���݂̃}�e���A�����擾
	//pDevice->GetMaterial(&matDef);

	//// �}�e���A���f�[�^�ւ̃|�C���^���擾
	//pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	//{
	//	// �}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, m_apTexture[nCntMat]);

	//	// ���f���i�p�[�c�j�̕`��
	//	m_pMesh->DrawSubset(nCntMat);
	//}

	//// �ۑ����Ă����}�e���A����߂�
	//pDevice->SetMaterial(&matDef);
}

//===============================================
// �`�揈��
//===============================================
void CObjectX::DrawXFile(int nIdx, CXFile::COL col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;										// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// X�t�@�C���̕`��
	CManager::GetXFile()->Draw(nIdx, col);
}

//===============================================
// ���f���^�C�v�ݒ�
//===============================================
void CObjectX::SetModel(MODEL type)
{
	m_modelType = type;
}

//===============================================
// �ʒu�ݒ�
//===============================================
void CObjectX::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===============================================
// �����ݒ�
//===============================================
void CObjectX::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===============================================
// �ړ��ʐݒ�
//===============================================
void CObjectX::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//===============================================
// �T�C�Y�擾
//===============================================
D3DXVECTOR3 CObjectX::GetSize(void)
{
	return CManager::GetXFile()->GetSize(0);
}