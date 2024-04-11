//=========================================================
//
// �^�C������ [time.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "time.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "texture.h"
#include "fade.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CNumber *CTime::m_apNumber[MAX_NUMBER] = {};		// �i���o�[�N���X�̃|�C���^
int CTime::m_nIdxTexture = 0;						// �g�p����e�N�X�`���̔ԍ�
int CTime::m_nTime = 0;								// �^�C���̒l

//===============================================
// �}�N����`
//===============================================
#define SET_TIME		(120)		// �ݒ�^�C��
#define NUM_PLACE		(3)			// �^�C���̌���
#define TIME_SIZEX		(27)		// �^�C���̕��i�����j
#define TIME_SIZEY		(60)		// �^�C���̍����i�����j

//===============================================
// �R���X�g���N�^
//===============================================
CTime::CTime()
{
	// �l�̃N���A
	m_nTime = 0;
	m_dwTime = 0;
	m_dwtime = 0;
	m_bFade = false;
}

//===============================================
// �f�X�g���N�^
//===============================================
CTime::~CTime()
{

}

//===============================================
// ��������
//===============================================
CTime *CTime::Create(int nPriority)
{
	CTime *pTime;

	// �^�C���̐���
	pTime = new CTime;

	// ����������
	pTime->Init(nPriority);

	return pTime;
}

//===============================================
// ����������
//===============================================
HRESULT CTime::Init(int nPriority)
{
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] == NULL)
		{
			// ����
			m_apNumber[nCntObj] = new CNumber(nPriority);

			// ��ނ̐ݒ�
			m_apNumber[nCntObj]->SetType(CNumber::TYPE_TIME);

			// ������
			m_apNumber[nCntObj]->Init(D3DXVECTOR3(550.0f + (nCntObj + 1) * TIME_SIZEX * 2, 50.0f, 0.0f), TIME_SIZEX, TIME_SIZEY, nPriority);
			m_apNumber[nCntObj]->SetPos(D3DXVECTOR3(550.0f + (nCntObj + 1) * TIME_SIZEX * 2, 50.0f, 0.0f), TIME_SIZEX, TIME_SIZEY);

			// �e�N�X�`���̐ݒ�
			m_nIdxTexture = CManager::GetTexture()->Regist("data\\TEXTURE\\time000.png");

			// �e�N�X�`���̊��蓖��
			m_apNumber[nCntObj]->BindTexture(m_nIdxTexture);
		}
	}

	// �T�C�Y��ݒ�
	m_fSizeX = TIME_SIZEX;
	m_fSizeY = TIME_SIZEY;

	// �^�C�������ݒ�
	Set(SET_TIME);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CTime::Uninit(void)
{
	// �i���o�[�̏I������
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{
			m_apNumber[nCntObj]->Uninit();
			delete m_apNumber[nCntObj];
			m_apNumber[nCntObj] = NULL;
		}
	}
}

//===============================================
// �X�V����
//===============================================
void CTime::Update(void)
{
	m_dwtime = timeGetTime();	// ���ݎ������擾�i�ۑ��j

	if ((m_dwtime - m_dwTime) >= 1000)
	{// �P�b�o��
		m_dwTime = m_dwtime;

		if (m_nTime > 0)
		{// �̂��莞�Ԃ��O�b����Ȃ�
			m_nTime--;
			Set(m_nTime);
		}
		else if(m_nTime <= 0)
		{
			if (m_bFade == false)
			{// �t�F�[�h�o�O�h�~
				CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// ���U���g��ʂֈړ�
				m_bFade = true;
			}
		}
	}

	// �i���o�[�̍X�V����
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{
			m_apNumber[nCntObj]->Update();
		}
	}
}

//===============================================
// �`�揈��
//===============================================
void CTime::Draw(void)
{
	// �i���o�[�̕`�揈��
	for (int nCntObj = 0; nCntObj < NUM_PLACE; nCntObj++)
	{
		if (m_apNumber[nCntObj] != NULL)
		{
			m_apNumber[nCntObj]->Draw();
		}
	}
}

//===============================================
// �ݒ菈��
//===============================================
void CTime::Set(const int nTime)
{
	int aTexU[NUM_PLACE] = {};  // �e�s�̐������i�[

	m_nTime = nTime;

	aTexU[0] = m_nTime % 1000 / 100;
	aTexU[1] = m_nTime % 100 / 10;
	aTexU[2] = m_nTime % 10;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{// �g�p����Ă���
			CNumber::TYPE type = m_apNumber[nCntScore]->GetType();

			if (type == CNumber::TYPE_TIME)
			{// �^�C���̏ꍇ
				// �i���o�[�ݒ�
				m_apNumber[nCntScore]->Set(aTexU[nCntScore]);
			}
		}
	}
}