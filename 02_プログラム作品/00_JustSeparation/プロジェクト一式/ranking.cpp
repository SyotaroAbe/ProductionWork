//=========================================================
//
// �����L���O���� [ranking.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "input.h"
#include "texture.h"
#include "rankIn.h"
#include "result.h"
#include "corecrt_wstdio.h"
#include <stdio.h>

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CNumber *CRanking::m_apNumber[MAX_RANK][MAX_NUMBER] = {};		// �i���o�[�N���X�̃|�C���^
int CRanking::m_nIdxTexture = 0;								// �g�p����e�N�X�`���̔ԍ�

//===============================================
// �}�N����`
//===============================================
#define NUM_PLACE			(8)				// �X�R�A�̌���
#define SCORE_SIZEX			(27)			// �X�R�A�̕��i�����j
#define SCORE_SIZEY			(60)			// �X�R�A�̍����i�����j
#define DISTANCE_SCOREX		(50.0f)			// �X�R�A�Ԃ̕�
#define DISTANCE_SCOREY		(1.65f)			// �X�R�A�Ԃ̍����̔{��

#define MAX_SCORE		(99999999)		// �ő�X�R�A

//===============================================
// �R���X�g���N�^
//===============================================
CRanking::CRanking()
{
	// �l�̃N���A
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_aScore[nCntRank] = 0;
	}
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_nYourRank = 0;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CRanking::CRanking(int nPriority)
{
	// �l�̃N���A
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_aScore[nCntRank] = 0;
	}
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_nYourRank = 0;
	m_fBrightness = 0.0f;
	m_bFlash = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CRanking::~CRanking()
{

}

//===============================================
// ��������
//===============================================
CRanking *CRanking::Create(int nPriority)
{
	CRanking *pScore;

	// �X�R�A�̐���
	pScore = new CRanking(nPriority);

	// ����������
	pScore->Init(D3DXVECTOR3(350.0f, 140.0f, 0.0f), SCORE_SIZEX, SCORE_SIZEY, nPriority);

	return pScore;
}

//===============================================
// ����������
//===============================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] == NULL)
			{// �g�p����Ă��Ȃ�
				// ����
				m_apNumber[nCntRank][nCntObj] = new CNumber(nPriority);

				// ��ނ̐ݒ�
				m_apNumber[nCntRank][nCntObj]->SetType(CNumber::TYPE_SCORE);

				// ������
				m_apNumber[nCntRank][nCntObj]->Init(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2 + nCntRank * DISTANCE_SCOREX, pos.y + nCntRank * fSizeY * DISTANCE_SCOREY, pos.z), fSizeX, fSizeY, nPriority);
				m_apNumber[nCntRank][nCntObj]->SetPos(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2 + nCntRank * DISTANCE_SCOREX, pos.y + nCntRank * fSizeY * DISTANCE_SCOREY, pos.z), fSizeX, fSizeY);

				// �e�N�X�`���̐ݒ�
				m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\score000.png");

				// �e�N�X�`���̊��蓖��
				m_apNumber[nCntRank][nCntObj]->BindTexture(m_nIdxTexture);
			}
		}
	}

	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data\\TXT\\ranking.txt", "r");

	if (pFile != NULL)
	{
		for (int nCntRank = 0; nCntRank < YOUR_SCORE; nCntRank++)
		{
			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%d", &m_aScore[nCntRank]);
		}
		// �t�@�C�������
		fclose(pFile);
	}

	// �T�C�Y��ݒ�
	m_fSizeX = SCORE_SIZEX;
	m_fSizeY = SCORE_SIZEY;

	// �l��������
	m_nYourRank = - 1;

	// �X�R�A�����ݒ�
	Set();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CRanking::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] != NULL)
			{// �g�p����Ă���
				// �i���o�[�̏I������
				m_apNumber[nCntRank][nCntObj]->Uninit();
				delete m_apNumber[nCntRank][nCntObj];
				m_apNumber[nCntRank][nCntObj] = NULL;
			}
		}
	}
}

//===============================================
// �X�V����
//===============================================
void CRanking::Update(void)
{
	if (m_fBrightness <= MIN_COLA)
	{// ������x�Â��Ȃ���
		m_bFlash = false;		// ���邭����
	}
	else if (m_fBrightness >= MAX_COLA)
	{// ������x���邭�Ȃ���
		m_bFlash = true;		// �Â�����
	}

	if (m_bFlash == true)
	{// �Â�����
		m_fBrightness += COL_DARK;
	}
	else if (m_bFlash == false)
	{// ���邭����
		m_fBrightness += COL_LIGHT;
	}

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] != NULL)
			{// �g�p����Ă���
				// �i���o�[�̍X�V����
				m_apNumber[nCntRank][nCntObj]->Update();

				if (nCntRank == YOUR_SCORE || nCntRank == m_nYourRank)
				{// ����̃X�R�A
					// �F�̐ݒ�
					m_apNumber[nCntRank][nCntObj]->SetCol(D3DXCOLOR(m_fBrightness, m_fBrightness, m_fBrightness, 1.0f));
				}

			}
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CRanking::Draw(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
		{
			if (m_apNumber[nCntRank][nCntObj] != NULL)
			{// �g�p����Ă���
				// �i���o�[�̕`�揈��
				m_apNumber[nCntRank][nCntObj]->Draw();
			}
		}
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CRanking::Set(void)
{
	FILE *pFile;
	int nTemp;							// �u�������p�̕ϐ�
	int aTexU[NUM_PLACE];				// �e�s�̐������i�[
	int nScore = m_aScore[YOUR_SCORE];	// ����̃X�R�A

	// �������J��Ԃ�
	for (int nCntRank = 0; nCntRank < YOUR_SCORE; nCntRank++)
	{
		// ���l���r����
		if (nScore >= m_aScore[nCntRank])
		{// �g�b�v5�Ƀ����N�C������
			nTemp = m_aScore[nCntRank];
			m_aScore[nCntRank] = nScore;

			if (m_nYourRank == - 1)
			{// �����N�C���̐ݒ�����Ă��Ȃ�
				CResult::GetRankIn()->Set(nCntRank);	// �X�V�����NNo.���X�V
				m_nYourRank = nCntRank;
			}

			// �������J��Ԃ�
			for (int nCount = nCntRank + 1; nCount < YOUR_SCORE; nCount++)
			{
				nScore = nTemp;
				nTemp = m_aScore[nCount];
				m_aScore[nCount] = nScore;
			}
		}
	}

	// �t�@�C�����J��
	pFile = fopen("data\\TXT\\ranking.txt", "w");

	if (pFile != NULL)
	{
		for (int nCntRank = 0; nCntRank < YOUR_SCORE; nCntRank++)
		{
			// �t�@�C�����當�����ǂݍ���
			fprintf(pFile, "%d\n", m_aScore[nCntRank]);
		}

		// �t�@�C�������
		fclose(pFile);
	}

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		aTexU[0] = m_aScore[nCntRank] % 100000000 / 10000000;
		aTexU[1] = m_aScore[nCntRank] % 10000000 / 1000000;
		aTexU[2] = m_aScore[nCntRank] % 1000000 / 100000;
		aTexU[3] = m_aScore[nCntRank] % 100000 / 10000;
		aTexU[4] = m_aScore[nCntRank] % 10000 / 1000;
		aTexU[5] = m_aScore[nCntRank] % 1000 / 100;
		aTexU[6] = m_aScore[nCntRank] % 100 / 10;
		aTexU[7] = m_aScore[nCntRank] % 10;

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			if (m_apNumber[nCntScore] != NULL)
			{// �g�p����Ă���
				// �i���o�[�ݒ�
				m_apNumber[nCntRank][nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}

//===============================================
// ���Z����
//===============================================
void CRanking::Add(const int nScore)
{
	int aTexU[NUM_PLACE];  // �e�s�̐������i�[

	m_aScore[YOUR_SCORE] = nScore;

	if (m_aScore[YOUR_SCORE] > MAX_SCORE)
	{// �ő�X�R�A�𒴂���
		m_aScore[YOUR_SCORE] = MAX_SCORE;
	}
	else if (m_aScore[YOUR_SCORE] < 0)
	{// �X�R�A���O���������
		m_aScore[YOUR_SCORE] = 0;
	}

	aTexU[0] = m_aScore[YOUR_SCORE] % 100000000 / 10000000;
	aTexU[1] = m_aScore[YOUR_SCORE] % 10000000 / 1000000;
	aTexU[2] = m_aScore[YOUR_SCORE] % 1000000 / 100000;
	aTexU[3] = m_aScore[YOUR_SCORE] % 100000 / 10000;
	aTexU[4] = m_aScore[YOUR_SCORE] % 10000 / 1000;
	aTexU[5] = m_aScore[YOUR_SCORE] % 1000 / 100;
	aTexU[6] = m_aScore[YOUR_SCORE] % 100 / 10;
	aTexU[7] = m_aScore[YOUR_SCORE] % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[YOUR_SCORE][nCntScore] != NULL)
		{// �g�p����Ă���
			// �i���o�[�ݒ�
			m_apNumber[YOUR_SCORE][nCntScore]->Set(aTexU[nCntScore]);
		}
	}

	// �ݒ菈��
	Set();
}