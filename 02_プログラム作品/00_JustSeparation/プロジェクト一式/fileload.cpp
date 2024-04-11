//=========================================================
//
// �t�@�C���ǂݍ��ݏ��� [fileload.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "fileload.h"

//===============================================
// �R���X�g���N�^
//===============================================
CFileLoad::CFileLoad()
{
	// �l�̃N���A
	m_nNumModel = 0;
	m_nMotionType = 0;
}

//===============================================
// �f�X�g���N�^
//===============================================
CFileLoad::~CFileLoad()
{

}

//===============================================
// ����������
//===============================================
HRESULT CFileLoad::Init(HWND hWnd)
{
	// �t�@�C�����ǂݍ���
	Name(hWnd, "data\\TXT\\player00.txt");

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CFileLoad::Uninit(void)
{

}

//===============================================
// �t�@�C�����ǂݍ��ݏ���
//===============================================
void CFileLoad::Name(HWND hWnd, const char* pFileName)
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// �ǂݍ��ݐ���
		char aStr[MAX_NAME] = {};		// ������ǂݎ��

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
void CFileLoad::Script(FILE *pFile)
{
	char aStr[MAX_NAME] = {};		// ������ǂݎ��
	int nCntModel = 0;				// ���f�������J�E���g

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "NUM_MODEL") == 0)
		{// ���f�����ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &m_nNumModel);
		}
		else if (strcmp(&aStr[0], "MODEL_FILENAME") == 0
			&& m_nNumModel > nCntModel)
		{// �e���f���̃C���f�b�N�X��ݒ�
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%s", &m_ModelInfo[nCntModel].aFileName[0]);

			// [char] ���� [const char*] �֒u������
			m_ModelInfo[nCntModel].apFileName = (const char*)&m_ModelInfo[nCntModel].aFileName[0];
			nCntModel++;
		}
		else if (strcmp(&aStr[0], "CHARACTERSET") == 0)
		{// CharacterSet���ǂݍ���
			CharacterSet(pFile);
		}
		else if (strcmp(&aStr[0], "MOTIONSET") == 0)
		{// MotionSet���ǂݍ���
			MotionSet(pFile);
			m_nMotionType++;
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// �Ō�܂œǂݍ���
			break;
		}
	}
}

//===============================================
// Character���ǂݍ��ݏ���
//===============================================
void CFileLoad::CharacterSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};		// ������ǂݎ��
	int nCntModel = 0;				// ���f�������J�E���g
	bool bNumModel = false;

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "PARTSSET") == 0)
		{// PartsSet���ǂݍ���
			PartsSet(pFile);
			nCntModel++;
			bNumModel = true;
		}

		if (nCntModel == m_nNumModel
			&& bNumModel == true
			|| strcmp(&aStr[0], "END_CHARACTERSET") == 0)
		{// �ǂݍ��񂾃��f�����ɒB����
			break;
		}
	}
}

//===============================================
// PartsSet���ǂݍ��ݏ���
//===============================================
void CFileLoad::PartsSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};		// ������ǂݎ��
	int nIdx;

	// �e�p�[�c�̊K�w�\���ݒ�
	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "INDEX") == 0)
		{// �C���f�b�N�X
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &nIdx);
		}
		else if (strcmp(&aStr[0], "PARENT") == 0)
		{// �e���f���̃C���f�b�N�X��ݒ�
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &m_ModelInfo[nIdx].aParent);
		}
		if (strcmp(&aStr[0], "POS") == 0)
		{// �ʒu�i�I�t�Z�b�g�j�̏����ʒu
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &m_ModelInfo[nIdx].pos.x);
			fscanf(pFile, "%f", &m_ModelInfo[nIdx].pos.y);
			fscanf(pFile, "%f", &m_ModelInfo[nIdx].pos.z);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// �����̏����ʒu
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &m_ModelInfo[nIdx].rot.x);
			fscanf(pFile, "%f", &m_ModelInfo[nIdx].rot.y);
			fscanf(pFile, "%f", &m_ModelInfo[nIdx].rot.z);
		}
		else if (strcmp(&aStr[0], "END_PARTSSET") == 0)
		{// �ǂݍ��ݏI��
			break;
		}
	}
}

//===============================================
// Motion���ǂݍ��ݏ���
//===============================================
void CFileLoad::MotionSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};		// ������ǂݎ��
	int nCntKey = 0;
	bool bNumKey = false;
	int nLoop = 0;

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "LOOP") == 0)
		{// ���[�v���邩
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &nLoop);

			// [int] ���� [bool] �֒u������
			if (nLoop == 0)
			{// ���[�v���Ȃ�
				m_aInfo[m_nMotionType].bLoop = false;
			}
			else
			{// ���[�v����
				m_aInfo[m_nMotionType].bLoop = true;
			}
		}

		if (strcmp(&aStr[0], "NUM_KEY") == 0)
		{// �L�[��
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &m_aInfo[m_nMotionType].nNumKey);
			bNumKey = true;
		}
		if (strcmp(&aStr[0], "KEYSET") == 0)
		{// KeySet���ǂݍ���
			KeySet(pFile, nCntKey);
			nCntKey++;
		}

		if (nCntKey == m_aInfo[m_nMotionType].nNumKey
			&& bNumKey == true
			|| strcmp(&aStr[0], "END_MOTIONSET") == 0)
		{// �ǂݍ��񂾃L�[���ɒB����
			break;
		}
	}
}

//===============================================
// KeySet���ǂݍ��ݏ���
//===============================================
void CFileLoad::KeySet(FILE *pFile, int nCntKey)
{
	char aStr[MAX_NAME] = {};		// ������ǂݎ��
	int nCntModel = 0;				// ���f�������J�E���g
	bool bFrame = false;

	while (1)
	{// �L�[���ɒB����܂ŌJ��Ԃ�
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "FRAME") == 0)
		{// �t���[����
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%d", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].nFrame);
			bFrame = true;
		}
		else if (strcmp(&aStr[0], "KEY") == 0 && bFrame == true)
		{// Key���ǂݍ���
			Key(pFile, nCntKey, nCntModel);
			nCntModel++;
		}

		if (nCntModel == m_nNumModel
			|| strcmp(&aStr[0], "END_KEYSET") == 0)
		{// �ǂݍ��񂾃��f�����ɒB����
			break;
		}
	}
}

//===============================================
// Key���ǂݍ��ݏ���
//===============================================
void CFileLoad::Key(FILE *pFile, int nCntKey, int nCntModel)
{
	char aStr[MAX_NAME] = {};		// ������ǂݎ��

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "POS") == 0)
		{// �ʒu
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fPosX);
			fscanf(pFile, "%f", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fPosY);
			fscanf(pFile, "%f", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fPosZ);
		}
		else if (strcmp(&aStr[0], "ROT") == 0)
		{// ����
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fRotX);
			fscanf(pFile, "%f", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fRotY);
			fscanf(pFile, "%f", &m_aInfo[m_nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].fRotZ);
		}
		else if (strcmp(&aStr[0], "END_KEY") == 0)
		{// �ǂݍ��ݏI��
			break;
		}
	}
}