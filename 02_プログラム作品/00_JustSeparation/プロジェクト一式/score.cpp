//=========================================================
//
// �X�R�A���� [score.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "input.h"
#include "texture.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CNumber *CScore::m_apNumber[MAX_NUMBER] = {};		// �i���o�[�N���X�̃|�C���^
int CScore::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�
int CScore::m_nScore = 0;							// �X�R�A�̒l

//===============================================
// �}�N����`
//===============================================
#define NUM_PLACE		(8)				// �X�R�A�̌���
#define SCORE_SIZEX		(18)			// �X�R�A�̕��i�����j
#define SCORE_SIZEY		(40)			// �X�R�A�̍����i�����j

#define MAX_SCORE		(99999999)		// �ő�X�R�A

//===============================================
// �R���X�g���N�^
//===============================================
CScore::CScore()
{

}

//===============================================
// �R���X�g���N�^�i�I�[�o�[���[�h�j
//===============================================
CScore::CScore(int nPriority)
{
	
}

//===============================================
// �f�X�g���N�^
//===============================================
CScore::~CScore()
{

}

//===============================================
// ��������
//===============================================
CScore *CScore::Create(int nPriority)
{
	CScore *pScore;

	// �X�R�A�̐���
	pScore = new CScore(nPriority);

	// ����������
	pScore->Init(D3DXVECTOR3(900.0f, 50.0f, 0.0f), SCORE_SIZEX, SCORE_SIZEY, nPriority);

	return pScore;
}

//===============================================
// ����������
//===============================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nPriority)
{
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] == NULL)
		{// �g�p����Ă��Ȃ�
			// ����
			m_apNumber[nCntObj] = new CNumber(nPriority);

			// ��ނ̐ݒ�
			m_apNumber[nCntObj]->SetType(CNumber::TYPE_SCORE);

			// ������
			m_apNumber[nCntObj]->Init(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2, pos.y, pos.z), fSizeX, fSizeY, nPriority);
			m_apNumber[nCntObj]->SetPos(D3DXVECTOR3(pos.x + (nCntObj + 1) * fSizeX * 2, pos.y, pos.z), fSizeX, fSizeY);

			// �e�N�X�`���̐ݒ�
			m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\score000.png");

			// �e�N�X�`���̊��蓖��
			m_apNumber[nCntObj]->BindTexture(m_nIdxTexture);
		}
	}

	// �T�C�Y��ݒ�
	m_fSizeX = SCORE_SIZEX;
	m_fSizeY = SCORE_SIZEY;

	// �X�R�A�����ݒ�
	Set(0);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CScore::Uninit(void)
{
	// �i���o�[�̏I������
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{// �g�p����Ă���
			m_apNumber[nCntObj]->Uninit();
			delete m_apNumber[nCntObj];
			m_apNumber[nCntObj] = NULL;
		}
	}
}

//===============================================
// �X�V����
//===============================================
void CScore::Update(void)
{
	// �i���o�[�̍X�V����
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{// �g�p����Ă���
			m_apNumber[nCntObj]->Update();
		}
	}

#ifdef _DEBUG
	if (CManager::GetKeyboardInput()->GetPress(DIK_U) == true)
	{// U�L�[
		Add(100000);
	}
#endif
}

//===============================================
// �`�揈��
//===============================================
void CScore::Draw(void)
{
	// �i���o�[�̕`�揈��
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{// �g�p����Ă���
			m_apNumber[nCntObj]->Draw();
		}
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CScore::Set(const int nScore)
{
	int aTexU[NUM_PLACE];  // �e�s�̐������i�[

	m_nScore = nScore;

	aTexU[0] = m_nScore % 100000000 / 10000000;
	aTexU[1] = m_nScore % 10000000 / 1000000;
	aTexU[2] = m_nScore % 1000000 / 100000;
	aTexU[3] = m_nScore % 100000 / 10000;
	aTexU[4] = m_nScore % 10000 / 1000;
	aTexU[5] = m_nScore % 1000 / 100;
	aTexU[6] = m_nScore % 100 / 10;
	aTexU[7] = m_nScore % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// �g�p����Ă���
			CNumber::TYPE type = m_apNumber[nCntScore]->GetType();

			if (type == CNumber::TYPE_SCORE)
			{// �X�R�A�̏ꍇ
				// �i���o�[�ݒ�
				m_apNumber[nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}

//===============================================
// ���Z����
//===============================================
void CScore::Add(const int nScore)
{
	int aTexU[NUM_PLACE];  // �e�s�̐������i�[

	m_nScore += nScore;

	if (m_nScore > MAX_SCORE)
	{// �ő�X�R�A�𒴂���
		m_nScore = MAX_SCORE;
	}
	else if (m_nScore < 0)
	{// �X�R�A���O���������
		m_nScore = 0;
	}

	aTexU[0] = m_nScore % 100000000 / 10000000;
	aTexU[1] = m_nScore % 10000000 / 1000000;
	aTexU[2] = m_nScore % 1000000 / 100000;
	aTexU[3] = m_nScore % 100000 / 10000;
	aTexU[4] = m_nScore % 10000 / 1000;
	aTexU[5] = m_nScore % 1000 / 100;
	aTexU[6] = m_nScore % 100 / 10;
	aTexU[7] = m_nScore % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// �g�p����Ă���
			CNumber::TYPE type = m_apNumber[nCntScore]->GetType();

			if (type == CNumber::TYPE_SCORE)
			{// �X�R�A�̏ꍇ
				// �i���o�[�ݒ�
				m_apNumber[nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}