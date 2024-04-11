//=========================================================
//
// ���[�V�������� [motion.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "corecrt_wstdio.h"
#include <stdio.h>

//===============================================
// �R���X�g���N�^
//===============================================
CMotion::CMotion()
{
	// �l���N���A����
	m_aInfo[MAX_MOTION];
	m_nNumAll = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = false;
	m_ppModel = NULL;
	m_nNumModel = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CMotion::~CMotion()
{

}

//===============================================
// ����������
//===============================================
HRESULT CMotion::Init(void)
{

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CMotion::Uninit(void)
{

}

//===============================================
// �ݒ菈��
//===============================================
void CMotion::Set(int nType)
{
	if (m_nType != nType)
	{// ��ނ��ύX���ꂽ
		m_nType = nType;		// ��ނ�ݒ�

		// �l��������
		m_nKey = 0;
		m_nCounter = 0;
		m_bFinish = false;
	}
}

//===============================================
// �X�V����
//===============================================
void CMotion::Update(void)
{
	D3DXVECTOR3 posDest, rotDest;	// �ړI�̈ʒu�A�ړI�̌���
	D3DXVECTOR3 posOri;				// �����ʒu
	int nNextKey = 0;				// ���̃L�[

	if (m_aInfo[m_nType].bLoop == true || m_nKey < m_aInfo[m_nType].nNumKey)
	{// ���[�v����������̓L�[�̑����ɒB���Ă��Ȃ�
		// ���̃L�[�����݂̃L�[��1��ɐݒ肷��
		nNextKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

		// �S���f���i�p�[�c�j�̍X�V
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			posOri = m_ppModel[nCntModel]->GetPosOri();		// �����ʒu���擾

			// �L�[��񂩂�ʒu�E�������Z�o
			posDest.x = (m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fPosX
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX);
			posDest.y = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fPosY
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
			posDest.z = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fPosZ
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;
			rotDest.x = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fRotX
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX;
			rotDest.y = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fRotY
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY;
			rotDest.z = m_aInfo[m_nType].aKeyInfo[nNextKey].aKey[nCntModel].fRotZ
						- m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ;

			// �p�[�c�̈ʒu�E������ݒ�
			D3DXVECTOR3 pos = posOri + D3DXVECTOR3(
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX + posDest.x
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY + posDest.y
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ + posDest.z
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame));
			D3DXVECTOR3 rot = D3DXVECTOR3(
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX + rotDest.x
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY + rotDest.y
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
				m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ + rotDest.z
				* ((float)m_nCounter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame));

			// ���f���ɒl��ݒ�
			m_ppModel[nCntModel]->SetPos(pos);
			m_ppModel[nCntModel]->SetRot(rot);
		}
		m_nCounter++;	// ���[�V�����J�E���^�[��i�߂�

		if (m_nCounter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		{// ���[�V�����J�E���^�[���Đ��t���[���ɒB����
			m_nKey++;				// ���݂̃L�[��1�i�߂�
			m_nCounter = 0;			// �J�E���^�[������������
		}

		if (m_nKey >= m_aInfo[m_nType].nNumKey)
		{// �L�[No.�������ȏ�
			if (m_aInfo[m_nType].bLoop == true)
			{// ���[�v���郂�[�V������
				m_nKey = 0;				// ���݂̃L�[������������
			}
			else
			{// ���[�v���Ȃ�
				m_bFinish = true;
			}
		}
	}
}

//===============================================
// ���ݒ菈��
//===============================================
void CMotion::SetInfo(INFO info)
{
	m_aInfo[m_nType] = info;
}

//===============================================
// ���f���ݒ菈��
//===============================================
void CMotion::SetModel(CModel **ppModel, int nNumModel)
{
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}