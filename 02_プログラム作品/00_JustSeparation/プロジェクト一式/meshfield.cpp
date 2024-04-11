//=========================================================
// 
// ���b�V���t�B�[���h�`�揈�� [meshfield.cpp]
// Author = �����đ�Y
// 
//=========================================================
#include "main.h"
#include "meshfield.h"
#include "texture.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"
#include "garbage.h"
#include "game.h"
#include "tutorial.h"
#include "target.h"
#include "title.h"
#include "renderer.h"
#include "time.h"

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

// FEVER
#define CREATEGABAGETIME_FEVER		(120)									// �t�B�[�o�[��Ԏ��ɃS�~�𐶐����鎞�ԊԊu
#define RAND_GABAGEPOSFEVER			(140)									// �t�B�[�o�[��Ԏ��ɃS�~�����������m��
#define ADD_RANDFEVER				(1)										// �t�B�[�o�[��Ԃ̃����_���̊m���㏸��

// �}�b�v�����_����������
#define MAX_COUNTX				(FIELD_COUNTX -1)	// ���̍ő�z�u���iX�j
#define MAX_COUNTY				(FIELD_COUNTY -1)	// ���̍ő�z�u���iY�j

#define FIRST_PATTERN			(0)			// �P�p�^�[����
#define SECOND_PATTERN			(1)			// �Q�p�^�[����
#define THIRD_PATTERN			(2)			// �R�p�^�[����

#define RAND_UP_MOVELEFT						(3)			// ��̍��i�߂�Ƃ���rand
#define RAND_UP_NOTMOVELEFT						(4)			// ��̍��i�߂Ȃ��Ƃ���rand
#define RAND_LEFTUP								(1)			// ����̂Ƃ���rand
#define RAND_LEFT_MOVEDOWN_MOVELEFT				(1)			// ���̉��i�߂鍶�i�߂�Ƃ���rand
#define RAND_LEFT_MOVEDOWN_NOTMOVELEFT			(1)			// ���̉��i�߂鍶�i�߂Ȃ��Ƃ���rand
#define RAND_LEFT_NOTMOVEDOWN					(1)			// ���̉��i�߂Ȃ����i�߂�Ƃ���rand
#define RAND_RIGHTDOWN							(3)			// �E���̂Ƃ���rand
#define RAND_RIGHT_MOVEDOWN						(4)			// �E�̉��i�߂�Ƃ���rand
#define RAND_RIGHT_NOTMOVEDOWN					(2)			// �E�̉��i�߂Ȃ��Ƃ���rand
#define RAND_DOWN_NOTMOVEDOWN_MOVELEFT			(1)			// ���̉��i�߂Ȃ������i�߂�Ƃ���rand
#define RAND_DOWN_NOTMOVEDOWN_NOTMOVELEFT		(1)			// ���̉��i�߂Ȃ������i�߂Ȃ��Ƃ���rand
#define RAND_INSIDE_MOVEDOWN_MOVELEFT			(3)			// �����̉��i�߂鍶�i�߂�Ƃ���rand
#define RAND_INSIDE_MOVEDOWN_NOTMOVELEFT		(3)			// �����̉��i�߂鍶�i�߂Ȃ��Ƃ���rand
#define RAND_INSIDE_NOTMOVEDOWN_MOVELEFT		(3)			// �����̉��i�߂Ȃ����i�߂�Ƃ���rand
#define RAND_INSIDE_NOTMOVEDOWN_NOTMOVELEFT		(3)			// �����̉��i�߂Ȃ����i�߂Ȃ��Ƃ���rand

// ���q�����Ă��邩�`�F�b�N
#define HALF_POS				(23)					// �����̈ʒu
#define TWO_BACK				(-2)					// �Q�O
#define HALF_COUNT				(FIELD_COUNTX / 2)		// ���̔����z�u���iX�j

#define STRAIGHT_UPH_POS		(12)			// �㔼���̏c�����Ōq�����Ă��邩�m�F�ʒu
#define STRAIGHT_DOWNH_POS		(30)			// �������̏c�����Ōq�����Ă��邩�m�F�ʒu
#define STRAIGHT_RIGHTW_POS		(2)			// �E�����̉������Ōq�����Ă��邩�m�F�ʒu
#define STRAIGHT_LEFTW_POS		(5)			// �������̉������Ōq�����Ă��邩�m�F�ʒu

// �S�~�̔z�u
#define RANDPOSITION_MAX		(3)			// �S�~�̃����_���ő吔

//===============================================
// �ÓI�����o�ϐ�
//===============================================
char CMeshField::m_aFileName[TYPE_MAX][MAX_NAME] = {};		// �e�N�X�`���t�@�C������ۑ�
int CMeshField::m_nIdxTexture[TYPE_MAX] = {};				// �g�p����e�N�X�`���̔ԍ�
LPDIRECT3DTEXTURE9 CMeshField::m_apTexture[TYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
CMeshField::TYPE CMeshField::m_aType[MAX_FIELD] = {};		// ���̎��
int CMeshField::m_nNumAll = 0;								// ����

//===============================================
// �R���X�g���N�^
//===============================================
CMeshField::CMeshField() : CObjectMesh(3)
{
	//�l���N���A����
	m_type = TYPE_STRAIGHT_H;
	m_destination = DESTINATION_NONE;
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
	m_type = TYPE_STRAIGHT_H;
	m_destination = DESTINATION_NONE;
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
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, TYPE nType, int nPriority)
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
	pObjectMesh->Set(pos, rot, nType);

	// �e�N�X�`���̊��蓖��
	pObjectMesh->BindTexture(m_nIdxTexture[nType]);

	return pObjectMesh;
}

//===============================================
// �ǂݍ��ݏ���
//===============================================
void CMeshField::Load(HWND hWnd)
{
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
		MessageBox(hWnd, "�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);	// �x���\��
	}

	//if (CManager::GetMode() == CScene::MODE_GAME || CManager::GetMode() == CScene::MODE_TITLE)
	//{// ���[�h���Q�[��
	//	// �}�b�v�����_����������
	//	RandArrange();
	//}
}

//===============================================
// �e�N�X�`���̔j��
//===============================================
void CMeshField::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//===============================================
// Script���ǂݍ��ݏ���
//===============================================
void CMeshField::Script(FILE *pFile)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();   //�f�o�C�X�̎擾

	char aStr[MAX_NAME] = {};
	int nCntTexture = 0;				// �e�N�X�`���������J�E���g

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TEXTURE_FILENAME") == 0)
		{// �e�N�X�`���ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%s", &m_aFileName[nCntTexture][0]);

			// �e�N�X�`���̐ݒ�
			m_nIdxTexture[nCntTexture] = CManager::GetTexture()->Regist((const char*)&m_aFileName[nCntTexture][0]);

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				(const char*)&m_aFileName[nCntTexture][0],
				&m_apTexture[nCntTexture]);

			nCntTexture++;	// �e�N�X�`���������J�E���g�A�b�v
		}
		else if (strcmp(&aStr[0], "FIELDSET") == 0)
		{// �����ǂݍ���
			FieldSet(pFile);
		}
		else if (nResult == -1 || strcmp(&aStr[0], "END_SCRIPT") == 0)
		{// �Ō�܂œǂݍ���
			break;
		}
	}
}

//===============================================
// Field���ǂݍ��ݏ���
//===============================================
void CMeshField::FieldSet(FILE *pFile)
{
	char aStr[MAX_NAME] = {};		// ��������擾
	TYPE type;						// ���
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
	D3DXVECTOR3 size;				// �T�C�Y

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "TEXTYPE") == 0)
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
		else if (strcmp(&aStr[0], "SIZE") == 0)
		{// �T�C�Y�ǂݍ���
			fscanf(pFile, "%s", &aStr[0]);	// (=)�ǂݍ���
			fscanf(pFile, "%f", &size.x);
			fscanf(pFile, "%f", &size.z);
		}

		if (strcmp(&aStr[0], "END_FIELDSET") == 0)
		{//�ǂݍ��񂾃��f�����ɒB����
			//CMeshField::Create(pos, rot, size.x, size.z, type, 3);
			break;
		}
	}
}

//===============================================
// �����_���z�u����
//===============================================
void CMeshField::RandArrange(void)
{
	bool bMoveOn = false;					// ���̐�ɐi�߂邩
	int nCntPrevX = 1;						// Y����

	// ���̃����_����������
	for (int nCntZ = 0; nCntZ < FIELD_COUNTY; nCntZ++)
	{
		for (int nCntX = 0; nCntX < FIELD_COUNTX; nCntX++)
		{
			int nCntField = nCntZ * FIELD_COUNTY + nCntX;				// ���݂̏��ԍ�
			m_aType[nCntField] = TYPE_CROSSROADS;						// ���̎��
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���̈ʒu

			srand(timeGetTime());	// rand�̒l��������
			int nTypeRand = 0;		// rand�̒l���擾
			bool bNext = false;		// �אڂ̏����������q�����Ă��邩

			if (nCntZ == 0 && nCntX == 0)
			{// �E��p
				m_aType[nCntField] = TYPE_CURVE_UPRIGHT;	// ��ނ��J�[�u�i�E��p�j�ɐݒ�
			}
			else
			{// ����ȊO
				// �אڂ̏���ނɂ�鏰�ݒ菈��
				nCntPrevX = NextField(nCntField, nCntField - 1, (nCntZ - 1) * FIELD_COUNTY + nCntX, nCntPrevX);
			}
		}
	}

	for (int nCntZ = 0; nCntZ < FIELD_COUNTY; nCntZ++)
	{
		for (int nCntX = 0; nCntX < FIELD_COUNTX; nCntX++)
		{
			int nCntField = nCntZ * FIELD_COUNTY + nCntX;				// ���݂̏��ԍ�

			// ���̐���
			CMeshField::Create(D3DXVECTOR3(-1000.0f * nCntX, 0.0f, -1000.0f * nCntZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, m_aType[nCntField], 3);
		}
	}
}

//===============================================
// �אڂ̏���ނɂ�鏰�ݒ菈��
//===============================================
int CMeshField::NextField(int type, int prevTypeX, int prevTypeY, int nCntPrevX)
{
	int nCntPrev = nCntPrevX;	// ���

	srand(timeGetTime());	// rand�̒l��������
	int nTypeRand = 0;		// rand�̒l

	if (prevTypeY < 0)
	{// �����������猩�ĉ��̏������݂��Ȃ�
		if (type != MAX_COUNTX)
		{// ����p����Ȃ�
			// ���O�̏��̎�ޕʂɏ������s��
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// ���O�̏������i�߂��i�߂Ȃ�
				srand(timeGetTime() + type);				// rand�̒l��������
				nTypeRand = rand() % RAND_UP_MOVELEFT;		// rand�̒l���擾

				// �E�i�߂��i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_W;	// ������
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;	// �J�[�u����p
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_UP;	// T���H��
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;	// �s���~�܂荶
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// ���i�߂Ȃ���i�߂Ȃ�
				srand(timeGetTime() + type);					// rand�̒l��������
				nTypeRand = rand() % RAND_UP_NOTMOVELEFT;		// rand�̒l���擾

				// �E�i�߂Ȃ���i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_DEADEND_RIGHT;	// �s���~�܂�E
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_DEADEND_UP;	// �s���~�܂��
					break;

				default:
					m_aType[type] = TYPE_CURVE_UPRIGHT;	// �J�[�u�E��p
					break;
				}
			}
		}
		else
		{// ����p
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_UP
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// ���O�̏������i�߂��i�߂Ȃ�
				srand(timeGetTime() + type);		// rand�̒l��������
				nTypeRand = rand() % RAND_LEFTUP;	// rand�̒l���擾

				// ���i�߂Ȃ��E�i�߂��i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;	// �J�[�u����p
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;	// �s���~�܂荶
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// ���O�̏������i�߂Ȃ���i�߂Ȃ�
				// ���i�߂Ȃ��E�i�߂Ȃ���i�߂Ȃ����𐶐�
				m_aType[type] = TYPE_DEADEND_UP;	// �s���~�܂��
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 2)
	{// �����p�P�O
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// rand�̒l���擾

				if (m_aType[prevTypeY + 1] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN || m_aType[prevTypeY + 1] == TYPE_DEADEND_LEFT
					|| m_aType[prevTypeY + 1] == TYPE_STRAIGHT_H || m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
				{// ���̍���̏������ɐi�߂Ȃ����ɐi�߂Ȃ��A�������͍��i�߂Ȃ����i�߂�E�i�߂Ȃ�
					// ���ɐi�߂Ȃ��E�ɐi�߂��ɐi�߂鍶�ɐi�߂鏰�𐶐�
					m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
				}
				else
				{
					// ���i�߂Ȃ��E�i�߂��i�߂鏰���烉���_���Ő���
					switch (nTypeRand)
					{
					case FIRST_PATTERN:
						m_aType[type] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
						break;

					default:
						m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
						break;
					}
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂Ȃ�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// rand�̒l���擾

				if (m_aType[prevTypeY + 1] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN || m_aType[prevTypeY + 1] == TYPE_DEADEND_LEFT
					|| m_aType[prevTypeY + 1] == TYPE_STRAIGHT_H || m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
				{// ���̍���̏������ɐi�߂Ȃ����ɐi�߂Ȃ��A�������͍��i�߂Ȃ����i�߂�E�i�߂Ȃ�
					// ���ɐi�߂Ȃ��E�ɐi�߂Ȃ���ɐi�߂鍶�ɐi�߂鏰�𐶐�
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
				}
				else
				{
					// ���i�߂Ȃ��E�i�߂Ȃ���i�߂鏰���烉���_���Ő���
					switch (nTypeRand)
					{
					case FIRST_PATTERN:
						m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
						break;

					default:
						m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
						break;
					}
				}
			}			
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_NOTMOVELEFT;	// rand�̒l���擾

				if (m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
				{// ����̏����s���~�܂��
					m_aType[type] = TYPE_STRAIGHT_W;		// ������
				}
				else
				{
					// ���i�߂Ȃ��E�i�߂��i�߂Ȃ������烉���_���Ő���
					switch (nTypeRand)
					{
					case FIRST_PATTERN:
						m_aType[type] = TYPE_STRAIGHT_W;		// ������
						break;

					default:
						m_aType[type] = TYPE_DEADEND_LEFT;		// �s���~�܂荶
						break;
					}
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂Ȃ�
				// ���i�߂Ȃ��E�i�߂Ȃ���i�߂Ȃ����𐶐�
				m_aType[type] = TYPE_DEADEND_RIGHT;		// �s���~�܂荶
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 1)
	{// �����p
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂�
				// ���i�߂Ȃ����i�߂Ȃ��E�i�߂��i�߂鏰�𐶐�
				m_aType[type] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂Ȃ�
				// ���i�߂Ȃ����i�߂Ȃ��E�i�߂Ȃ���i�߂鏰�𐶐�
				m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
			}			
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂�
				// ���i�߂Ȃ����i�߂Ȃ��E�i�߂��i�߂Ȃ����𐶐�
				m_aType[type] = TYPE_DEADEND_LEFT;		// �s���~�܂荶
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂Ȃ�
				// ���i�߂Ȃ����i�߂Ȃ��E�i�߂Ȃ���i�߂Ȃ����𐶐��i����Ȃ��̂͂Ȃ��j
				m_aType[type] = TYPE_CROSSROADS;		// �\���H
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY)
	{// �O�����̈ʒu
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// rand�̒l���擾

				// ���i�߂Ȃ��E�i�߂��i�߂鏰���烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
					break;

				default:
					m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂Ȃ�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_MOVELEFT;	// rand�̒l���擾

				// ���i�߂Ȃ��E�i�߂Ȃ���i�߂鏰���烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
					break;

				default:
					m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
					break;
				}
			}			
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_DOWN_NOTMOVEDOWN_NOTMOVELEFT;	// rand�̒l���擾

				// ���i�߂Ȃ��E�i�߂��i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_W;		// ������
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// �s���~�܂荶
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT || m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ����ɐi�߂Ȃ�
				// ���i�߂Ȃ��E�i�߂Ȃ���i�߂Ȃ����𐶐�
				m_aType[type] = TYPE_DEADEND_RIGHT;		// �s���~�܂�E
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX -1)
	{// �O�����̈ʒu
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂�
				srand(timeGetTime() + type);						// rand�̒l��������
				nTypeRand = rand() % RAND_LEFT_MOVEDOWN_MOVELEFT;	// rand�̒l���擾

				// ���i�߂Ȃ��E�i�߂��i�߂鏰���烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
					break;

				default:
					m_aType[type] = TYPE_TJUNCTION_LEFT;	// T���H��
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ�
				//srand(timeGetTime() + type);							// rand�̒l��������
				//nTypeRand = rand() % RAND_LEFT_MOVEDOWN_NOTMOVELEFT;	// rand�̒l���擾

				//// ���i�߂Ȃ��E�i�߂Ȃ���i�߂鏰���烉���_���Ő���
				//switch (nTypeRand)
				//{
				//case FIRST_PATTERN:
				//	m_aType[type] = TYPE_STRAIGHT_H;		// �����c
				//	break;

				//default:
				//	m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
				//	break;
				//}

				m_aType[type] = TYPE_STRAIGHT_H;		// �����c
			}
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂�
				srand(timeGetTime() + type);				// rand�̒l��������
				nTypeRand = rand() % RAND_LEFT_NOTMOVEDOWN;	// rand�̒l���擾

				// ���i�߂Ȃ��E�i�߂��i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;		// �J�[�u����p
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// �s���~�܂荶
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ�
				// ���i�߂Ȃ��E�i�߂Ȃ���i�߂Ȃ����𐶐�
				m_aType[type] = TYPE_DEADEND_UP;		// �s���~�܂��
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
	{// �E���p
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			//srand(timeGetTime() + type);			// rand�̒l��������
			//nTypeRand = rand() % RAND_RIGHTDOWN;	// rand�̒l���擾

			//// ���i�߂Ȃ��E�i�߂Ȃ���i�߂鏰���烉���_���Ő���
			//switch (nTypeRand)
			//{
			//case FIRST_PATTERN:
			//	m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
			//	break;

			//default:
			//	m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
			//	break;
			//}

			m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			// ���i�߂Ȃ��E�i�߂Ȃ���i�߂Ȃ����𐶐�
			m_aType[type] = TYPE_DEADEND_RIGHT;		// �s���~�܂�E
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX)
	{// �O���E�̈ʒu
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			srand(timeGetTime() + type);				// rand�̒l��������
			nTypeRand = rand() % RAND_RIGHT_MOVEDOWN;	// rand�̒l���擾

			// �E�i�߂Ȃ���i�߂鏰���烉���_���Ő���
			switch (nTypeRand)
			{
			case FIRST_PATTERN:
				m_aType[type] = TYPE_STRAIGHT_H;		// �����c
				break;

			case SECOND_PATTERN:
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
				break;

			case THIRD_PATTERN:
				m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
				break;

			default:
				m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				break;
			}
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			srand(timeGetTime() + type);					// rand�̒l��������
			nTypeRand = rand() % RAND_RIGHT_NOTMOVEDOWN;	// rand�̒l���擾

			// �E�i�߂Ȃ���i�߂Ȃ������烉���_���Ő���
			switch (nTypeRand)
			{
			case FIRST_PATTERN:
				m_aType[type] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				break;

			case SECOND_PATTERN:
				m_aType[type] = TYPE_DEADEND_UP;		// �s���~�܂��
				break;

			default:
				m_aType[type] = TYPE_DEADEND_RIGHT;		// �s���~�܂�E
				break;
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);

		nCntPrev++;
	}
	else
	{// �����̏�
		// ���O�̏��̎�ޕʂɏ������s��
		if (m_aType[prevTypeY] == TYPE_STRAIGHT_H || m_aType[prevTypeY] == TYPE_CURVE_UPLEFT || m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeY] == TYPE_TJUNCTION_LEFT
			|| m_aType[prevTypeY] == TYPE_CROSSROADS || m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏������ɐi�߂�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_INSIDE_MOVEDOWN_MOVELEFT;		// rand�̒l���擾

				// �E�i�߂��i�߂鏰���烉���_���Ő���	
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_LEFT;	// T���H��
					break;

				default:
					m_aType[type] = TYPE_CROSSROADS;		// �\���H
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP || m_aType[prevTypeX] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_INSIDE_MOVEDOWN_NOTMOVELEFT;	// rand�̒l���擾

				// �E�i�߂Ȃ���i�߂鏰���烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_H;		// �����c
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T���H�E
					break;

				default:
					m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
					break;
				}
			}
		}
		else if (m_aType[prevTypeY] == TYPE_STRAIGHT_W || m_aType[prevTypeY] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY] == TYPE_CURVE_DOWNLEFT
			|| m_aType[prevTypeY] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY] == TYPE_DEADEND_DOWN
			|| m_aType[prevTypeY] == TYPE_DEADEND_LEFT)
		{// ��̏������ɐi�߂Ȃ�
			if (m_aType[prevTypeX] == TYPE_STRAIGHT_W || m_aType[prevTypeX] == TYPE_CURVE_UPRIGHT || m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT 
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_UP || m_aType[prevTypeX] == TYPE_TJUNCTION_RIGHT || m_aType[prevTypeX] == TYPE_TJUNCTION_DOWN
				|| m_aType[prevTypeX] == TYPE_CROSSROADS || m_aType[prevTypeX] == TYPE_DEADEND_RIGHT)
			{// �E�̏������ɐi�߂�
				srand(timeGetTime() + type);							// rand�̒l��������
				nTypeRand = rand() % RAND_INSIDE_NOTMOVEDOWN_MOVELEFT;	// rand�̒l���擾

				// �E�i�߂��i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_STRAIGHT_W;		// ������
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_UPLEFT;		// �J�[�u����p
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_TJUNCTION_UP;		// T���H��
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// �s���~�܂荶
					break;
				}
			}
			else if (m_aType[prevTypeX] == TYPE_STRAIGHT_H || m_aType[prevTypeX] == TYPE_CURVE_UPLEFT || m_aType[prevTypeX] == TYPE_CURVE_DOWNLEFT 
				|| m_aType[prevTypeX] == TYPE_TJUNCTION_LEFT || m_aType[prevTypeX] == TYPE_DEADEND_UP
				|| m_aType[prevTypeX] == TYPE_DEADEND_DOWN || m_aType[prevTypeX] == TYPE_DEADEND_LEFT)
			{// �E�̏������ɐi�߂Ȃ�
				srand(timeGetTime() + type);								// rand�̒l��������
				nTypeRand = rand() % RAND_INSIDE_NOTMOVEDOWN_NOTMOVELEFT;	// rand�̒l���擾
				
				// �E�i�߂Ȃ���i�߂Ȃ������烉���_���Ő���
				switch (nTypeRand)
				{
				case FIRST_PATTERN:
					m_aType[type] = TYPE_DEADEND_RIGHT;		// �s���~�܂�E
					break;

				case SECOND_PATTERN:
					m_aType[type] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
					break;

				case THIRD_PATTERN:
					m_aType[type] = TYPE_DEADEND_UP;		// �s���~�܂��
					break;

				default:
					m_aType[type] = TYPE_DEADEND_LEFT;		// �s���~�܂荶
					break;
				}
			}
		}

		// �����q�����Ă��邩�`�F�b�N
		ConnectCheck(type, prevTypeX, prevTypeY, nCntPrevX);
	}
	return nCntPrev;
}

//===============================================
// �����q�����Ă��邩�`�F�b�N
//===============================================
void CMeshField::ConnectCheck(int type, int prevTypeX, int prevTypeY, int nCntPrevX)
{
	if ((m_aType[type] == TYPE_CURVE_DOWNLEFT && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
		|| (m_aType[type] == TYPE_DEADEND_DOWN && m_aType[type - 1] == TYPE_DEADEND_DOWN && m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
		|| (m_aType[type] == TYPE_CURVE_DOWNLEFT && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY] == TYPE_DEADEND_UP && m_aType[prevTypeY - 1] == TYPE_DEADEND_UP)
		|| (m_aType[type] == TYPE_DEADEND_LEFT && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY] == TYPE_DEADEND_LEFT && m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
		|| (m_aType[type] == TYPE_CURVE_DOWNLEFT && m_aType[type - 1] == TYPE_DEADEND_RIGHT && m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - 1] == TYPE_DEADEND_RIGHT))
	{// �~��̌`���ł��Ă���
		if (prevTypeY <= MAX_COUNTX)
		{// ��̈ʒu
			if (prevTypeY == MAX_COUNTX)
			{// ����
				// �`�ʂɏ��̎�ނ�ς���
				if (m_aType[type] == TYPE_DEADEND_DOWN)
				{// �tU�^
					m_aType[type - 1] = TYPE_STRAIGHT_H;		// �c����
					m_aType[type] = TYPE_STRAIGHT_H;			// �c����
				}
				else if (m_aType[type] == TYPE_DEADEND_LEFT)
				{// �R�^
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
					m_aType[type] = TYPE_CURVE_UPLEFT;			// �J�[�u����p
				}
				else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
				{// �t�R�^
					m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
				}
				else
				{// U�^�A���^
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
				}
			}
			else if (prevTypeY - 1 == 0)
			{// �E��
				// �`�ʂɏ��̎�ނ�ς���
				if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
				{// U�^
					m_aType[type - 1] = TYPE_STRAIGHT_H;		// �c����
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T���H�E
				}
				else if (m_aType[type] == TYPE_DEADEND_LEFT)
				{// �R�^
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
					m_aType[type] = TYPE_TJUNCTION_UP;			// T���H��
				}
				else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
				{// �t�R�^
					m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
					m_aType[type] = TYPE_CROSSROADS;			// �\���H
				}
				else
				{// �tU�^�A���^
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
					m_aType[type] = TYPE_CROSSROADS;			// �\���H
				}
			}
			else
			{// �����
				// �`�ʂɏ��̎�ނ�ς���
				if (m_aType[type] == TYPE_DEADEND_DOWN)
				{// �tU�^
					m_aType[type - 1] = TYPE_TJUNCTION_LEFT;	// T���H��
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T���H�E
				}
				else if (m_aType[type] == TYPE_DEADEND_LEFT)
				{// �R�^
					m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
					m_aType[type] = TYPE_TJUNCTION_UP;			// T���H��
				}
				else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
				{// �t�R�^
					m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
				}
				else
				{// U�^�A���^
					m_aType[type - 1] = TYPE_CROSSROADS;		// �\���H
					m_aType[type] = TYPE_CROSSROADS;			// �\���H
				}
			}
		}
		else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// ����
			// ��̏��i���ʂ�Ȃ��j�̎�ޕʂɂ��̏�̏��̎�ނ�ς���
			switch (m_aType[prevTypeY - FIELD_COUNTY - 1])
			{
			case TYPE_STRAIGHT_W:		// ������
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_TJUNCTION_UP;		// T���H��
				break;

			case TYPE_CURVE_DOWNRIGHT:	// �J�[�u�E���p
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				break;

			case TYPE_CURVE_DOWNLEFT:	// �J�[�u�����p
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_TJUNCTION_LEFT;	// T���H��
				break;

			case TYPE_TJUNCTION_DOWN:	// T���H��
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_CROSSROADS;		// �\���H
				break;

			case TYPE_DEADEND_RIGHT:	// �s���~�܂�E
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				break;

			case TYPE_DEADEND_DOWN:		// �s���~�܂艺
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_STRAIGHT_H;		// �c����
				break;

			case TYPE_DEADEND_LEFT:		// �s���~�܂荶
				m_aType[prevTypeY - FIELD_COUNTY - 1] = TYPE_CURVE_UPLEFT;		// �J�[�u����p
				break;
			}

			// �`�ʂɏ��̎�ނ�ς���
			if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// U�^
				m_aType[prevTypeY - 1] = TYPE_STRAIGHT_H;		// �c����
			}
			else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
			{// �t�R�^
				m_aType[prevTypeY - 1] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
			}
			else
			{// �tU�^�A�R�^�A���^
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
			}
		}
		else if (type - 1 == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
		{// �E��
			// �`�ʂɏ��̎�ނ�ς���
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// �tU�^
				m_aType[prevTypeY] = TYPE_TJUNCTION_UP;			// T���H��
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;			// �J�[�u�E���p
			}
			else if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// U�^
				m_aType[prevTypeY] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				m_aType[type] = TYPE_TJUNCTION_DOWN;			// T���H��
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// �R�^
				m_aType[prevTypeY] = TYPE_STRAIGHT_W;			// ������
				m_aType[type] = TYPE_STRAIGHT_W;				// ������
			}
			else
			{// �t�R�^�A���^
				m_aType[prevTypeY] = TYPE_TJUNCTION_UP;			// T���H��
				m_aType[type] = TYPE_TJUNCTION_DOWN;			// T���H��
			}

			// ����̏��̎�ނ̕ύX
			if (m_aType[prevTypeY + 1] == TYPE_STRAIGHT_H)
			{// �c����
				m_aType[prevTypeY + 1] = TYPE_TJUNCTION_LEFT;		// T���H��
			}
			else if (m_aType[prevTypeY + 1] == TYPE_CURVE_UPRIGHT)
			{// �J�[�u�E��p
				m_aType[prevTypeY + 1] = TYPE_TJUNCTION_UP;			// T���H��
			}
			else if (m_aType[prevTypeY + 1] == TYPE_CURVE_DOWNRIGHT)
			{// �J�[�u�E���p
				m_aType[prevTypeY + 1] = TYPE_TJUNCTION_DOWN;		// T���H��
			}
			else if (m_aType[prevTypeY + 1] == TYPE_TJUNCTION_RIGHT)
			{// T���H�E
				m_aType[prevTypeY + 1] = TYPE_CROSSROADS;			// �\���H
			}
			else if (m_aType[prevTypeY + 1] == TYPE_DEADEND_UP)
			{// �s���~�܂��
				m_aType[prevTypeY + 1] = TYPE_CURVE_UPLEFT;			// �J�[�u����p
			}
			else if (m_aType[prevTypeY + 1] == TYPE_DEADEND_RIGHT)
			{// �s���~�܂�E
				m_aType[prevTypeY + 1] = TYPE_STRAIGHT_W;			// ������
			}
			else if (m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN)
			{// �s���~�܂艺
				m_aType[prevTypeY + 1] = TYPE_CURVE_DOWNLEFT;		// �J�[�u�����p
			}
		}
		else if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY)
		{// ���̈ʒu
			// �`�ʂɏ��̎�ނ�ς���
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// �tU�^
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// �R�^
				m_aType[type] = TYPE_STRAIGHT_W;		// ������
			}
			else
			{// U�^�A�t�R�^�A���^
				m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
			}
		}
		else if (type - 1 == (MAX_COUNTX * nCntPrevX) + nCntPrevX)
		{// �E�̈ʒu
			// �`�ʂɏ��̎�ނ�ς���
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// �tU�^
				m_aType[type - 1] = TYPE_STRAIGHT_H;		// �c����
				m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T���H�E
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// �R�^
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				m_aType[type] = TYPE_TJUNCTION_UP;			// T���H��
			}
			else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
			{// �t�R�^
				m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				m_aType[type] = TYPE_CROSSROADS;			// �\���H
			}
			else
			{// U�^�A���^
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				m_aType[type] = TYPE_CROSSROADS;			// �\���H
			}
		}
		else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1)
		{// ���̈ʒu
			// �`�ʂɏ��̎�ނ�ς���
			if (m_aType[type] == TYPE_DEADEND_DOWN)
			{// �tU�^
				m_aType[type - 1] = TYPE_STRAIGHT_H;		// �c����
				m_aType[type] = TYPE_STRAIGHT_H;			// �c����
			}
			else if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// U�^
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
			}
			else if (m_aType[type] == TYPE_DEADEND_LEFT)
			{// �R�^
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				m_aType[type] = TYPE_CURVE_UPLEFT;			// �J�[�u����p
			}
			else if (m_aType[type - 1] == TYPE_DEADEND_RIGHT)
			{// �t�R�^
				m_aType[type - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
			}
			else
			{// ���^
				m_aType[type - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
			}
		}
	}

	bool bMoveOn = false;	// �񂲂Ƃ̏��̎�ރ`�F�b�N

	// �����s���~�܂�
	if (type == FIELD_COUNTX * FIELD_COUNTY - 1)
	{// ��ԉ��̗�
		for (int nCntField = 0; nCntField < FIELD_COUNTX; nCntField++)
		{// �����Ŋm�F
			if (m_aType[type - nCntField] == TYPE_DEADEND_LEFT)
			{// �s���~�܂�̊J�n�ʒu������
				for (int nCntLine = nCntField; nCntLine < FIELD_COUNTX; nCntLine++)
				{					
					if (m_aType[type - nCntField] == TYPE_DEADEND_RIGHT)
					{// �s���~�܂�E
						if (bMoveOn == false)
						{// ��ɍs���鏰���P���Ȃ�
							// ���݂̏�̏��̎�ނ���ʂ̎�ނ֕ύX
							switch (m_aType[prevTypeY - nCntField])
							{
							case TYPE_STRAIGHT_W:			// ������
								m_aType[prevTypeY - nCntField] = TYPE_TJUNCTION_UP;		// T���H��
								break;

							case TYPE_CURVE_DOWNRIGHT:		// �J�[�u�E���p
								m_aType[prevTypeY - nCntField] = TYPE_TJUNCTION_RIGHT;	// T���H�E
								break;

							case TYPE_CURVE_DOWNLEFT:		// �J�[�u�����p
								m_aType[prevTypeY - nCntField] = TYPE_TJUNCTION_LEFT;	// T���H��
								break;

							case TYPE_TJUNCTION_DOWN:		// T���H��
								m_aType[prevTypeY - nCntField] = TYPE_CROSSROADS;		// �\���H
								break;
							
							case TYPE_DEADEND_RIGHT:		// �s���~�܂�E
								m_aType[prevTypeY - nCntField] = TYPE_CURVE_UPRIGHT;	// �J�[�u�E��p
								break;
							
							case TYPE_DEADEND_DOWN:			// �s���~�܂艺
								m_aType[prevTypeY - nCntField] = TYPE_STRAIGHT_W;		// �����c
								break;
							
							case TYPE_DEADEND_LEFT:			// �s���~�܂荶
								m_aType[prevTypeY - nCntField] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
								break;

							default:
								m_aType[prevTypeY - nCntField] = TYPE_CROSSROADS;		// �\���H�i�����o����o�O�j
								break;
							}

							// �s���~�܂荶�̏�����֌q����悤�ɕύX
							m_aType[type - nCntField] = TYPE_CURVE_DOWNLEFT;		// �J�[�u�����p
						}
					}
					else if(m_aType[type - nCntField] != TYPE_STRAIGHT_W && m_aType[type - nCntField] != TYPE_DEADEND_LEFT)
					{// �s���~�܂荶�A�s���~�܂�E�A����������Ȃ�
						bMoveOn = true;	// �t���O�𖳌���
					}
				}
				bMoveOn = false;	// �t���O��������
			}
		}
	}

	for (int nCntLine = 0; nCntLine < FIELD_COUNTX; nCntLine++)
	{// �c���Ŋm�F
		if (type == FIELD_COUNTX * nCntLine - 1)
		{// �����̈ʒu�̏�
			for (int nCntField = 0; nCntField < FIELD_COUNTX; nCntField++)
			{// �����Ŋm�F
				if (m_aType[type - nCntField] != TYPE_STRAIGHT_W && m_aType[type - nCntField] != TYPE_CURVE_DOWNLEFT && m_aType[type - nCntField] != TYPE_CURVE_DOWNRIGHT
					&& m_aType[type - nCntField] != TYPE_TJUNCTION_DOWN && m_aType[type - nCntField] != TYPE_DEADEND_DOWN && m_aType[type - nCntField] != TYPE_DEADEND_RIGHT
					&& m_aType[type - nCntField] != TYPE_DEADEND_LEFT)
				{// ���̏��̉��ɍs����
					bMoveOn = true;
				}
			}

			if (bMoveOn == false)
			{// ���ɍs���鏰���P���Ȃ�
				// ���݂̏��̎�ނ���ʂ̎�ނ֕ύX
				switch (m_aType[type])
				{
				case TYPE_CURVE_DOWNLEFT:	// �J�[�u�����p
					m_aType[type] = TYPE_TJUNCTION_LEFT;	// T���H��
					break;

				case TYPE_DEADEND_DOWN:		// �s���~�܂艺
					m_aType[type] = TYPE_DEADEND_DOWN;		// �s���~�܂艺
					break;

				case TYPE_DEADEND_LEFT:		// �s���~�܂荶
					m_aType[type] = TYPE_CURVE_UPLEFT;		// �J�[�u����p
					break;

				default:
					m_aType[type] = TYPE_CROSSROADS;		// �\���H
					break;
				}
			}
		}
		bMoveOn = false;	// �t���O�̏�����
	}

	// �P�^�S�͈͂Ŋm�F
	for (int nCntLine = 0; nCntLine < MAX_COUNTX; nCntLine++)
	{
		if (type == STRAIGHT_UPH_POS * nCntLine || type == STRAIGHT_DOWNH_POS * nCntLine)
		{// �㔼���̏c�����Ōq�����Ă��邩�m�F�ʒu �^ �������̏c�����Ōq�����Ă��邩�m�F�ʒu
			for (int nCntField = 0; nCntField < HALF_COUNT; nCntField++)
			{
				if (m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_STRAIGHT_W || m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_CURVE_UPRIGHT
					|| m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_CURVE_DOWNRIGHT || m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_TJUNCTION_RIGHT
					|| m_aType[type - (nCntField * FIELD_COUNTX)] == TYPE_DEADEND_RIGHT)
				{// ���ւ�����
					bMoveOn = true;
				}
			}

			if (bMoveOn == false)
			{// ���ւ����鏰���Ȃ�
				// ���ւ����鏰�����̎�ނ𐶐�
				switch (m_aType[type])
				{
				case TYPE_STRAIGHT_H:		// �c����
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T���H�E
					break;

				case TYPE_CURVE_UPLEFT:		// �J�[�u����p
					m_aType[type] = TYPE_TJUNCTION_UP;			// T���H��
					break;

				case TYPE_CURVE_DOWNLEFT:	// �J�[�u�����p
					m_aType[type] = TYPE_TJUNCTION_DOWN;		// T���H��
					break;

				case TYPE_TJUNCTION_LEFT:	// T���H��
					m_aType[type] = TYPE_CROSSROADS;			// �\���H
					break;

				case TYPE_DEADEND_UP:		// �s���~�܂��
					m_aType[type] = TYPE_CURVE_UPRIGHT;			// �J�[�u�E��p
					break;

				case TYPE_DEADEND_LEFT:		// �s���~�܂荶
					m_aType[type] = TYPE_STRAIGHT_W;			// ������
					break;

				case TYPE_DEADEND_DOWN:		// �s���~�܂艺
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;		// �J�[�u�E���p
					break;
				}
			}

			bMoveOn = false;	// �t���O�̏�����
		}
		if (type == STRAIGHT_RIGHTW_POS * nCntLine || type == STRAIGHT_LEFTW_POS * nCntLine)
		{// �E�����̉������Ōq�����Ă��邩�m�F�ʒu �^ �������̉������Ōq�����Ă��邩�m�F�ʒu
			for (int nCntField = 0; nCntField < HALF_COUNT; nCntField++)
			{
				if (m_aType[type - nCntField] == TYPE_STRAIGHT_H || m_aType[type - nCntField] == TYPE_CURVE_UPLEFT || m_aType[type - nCntField] == TYPE_CURVE_UPRIGHT
					|| m_aType[type - nCntField] == TYPE_TJUNCTION_UP || m_aType[type - nCntField] == TYPE_TJUNCTION_RIGHT || m_aType[type - nCntField] == TYPE_TJUNCTION_LEFT
					|| m_aType[type - nCntField] == TYPE_CROSSROADS || m_aType[type - nCntField] == TYPE_DEADEND_UP)
				{// ���ւ�����
					bMoveOn = true;
				}
			}

			if (bMoveOn == false)
			{// ���֍s���鏰���Ȃ�
				// ���ւ����鏰�����̎�ނ𐶐�
				switch (m_aType[type])
				{
				case TYPE_STRAIGHT_W:		// ������
					m_aType[type] = TYPE_TJUNCTION_UP;			// T���H��
					break;

				case TYPE_CURVE_DOWNRIGHT:	// �J�[�u�E���p
					m_aType[type] = TYPE_TJUNCTION_RIGHT;		// T���H�E
					break;

				case TYPE_CURVE_DOWNLEFT:	// �J�[�u�����p
					m_aType[type] = TYPE_TJUNCTION_LEFT;		// T���H��
					break;

				case TYPE_TJUNCTION_DOWN:	// T���H��
					m_aType[type] = TYPE_CROSSROADS;			// �\���H
					break;

				case TYPE_DEADEND_RIGHT:	// �s���~�܂�E
					m_aType[type] = TYPE_CURVE_UPRIGHT;			// �J�[�u�E��p
					break;

				case TYPE_DEADEND_LEFT:		// �s���~�܂荶
					m_aType[type] = TYPE_CURVE_UPLEFT;			// �J�[�u����p
					break;

				case TYPE_DEADEND_DOWN:		// �s���~�܂艺
					m_aType[type] = TYPE_STRAIGHT_H;			// �c����
					break;
				}
			}

			bMoveOn = false;	// �t���O�̏�����
		}
	}

	// ���̎�ޕʂɏ�������
	switch (m_aType[type])
	{
	case TYPE_STRAIGHT_H:		// �c����
		if (type == (MAX_COUNTX * (MAX_COUNTX - 1)) + (MAX_COUNTX - 1) - 1)
		{// �����p�̏�
			for (int nCntField = 1; nCntField < MAX_COUNTX; nCntField++)
			{
				if (m_aType[(MAX_COUNTX * nCntField) + nCntField - 1] != TYPE_STRAIGHT_H)
				{// �c���c��������Ȃ�
					bMoveOn = true;
				}

				if (bMoveOn == false)
				{// �c��񒼐�
					// �c���̐^�񒆈ʒu�̎�ނ�ύX
					m_aType[HALF_POS] = TYPE_TJUNCTION_LEFT;	// T���H��

					// �ύX���ꂽ���ɍ��킹�āA���̉E�̏��̎�ނ��ύX����
					switch (m_aType[HALF_POS - 1])
					{
					case TYPE_STRAIGHT_H:		// �c����
						m_aType[HALF_POS - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
						break;

					case TYPE_CURVE_UPLEFT:		// �J�[�u�i����p�j
						m_aType[HALF_POS - 1] = TYPE_TJUNCTION_UP;		// T���H��
						break;

					case TYPE_TJUNCTION_LEFT:	// T���H��
						m_aType[HALF_POS - 1] = TYPE_CROSSROADS;		// �\���H
						break;

					case TYPE_DEADEND_UP:		// �s���~�܂��
						m_aType[HALF_POS - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
						break;

					case TYPE_DEADEND_DOWN:		// �s���~�܂艺
						m_aType[HALF_POS - 1] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
						break;

					case TYPE_DEADEND_LEFT:		// �s���~�܂荶
						m_aType[HALF_POS - 1] = TYPE_STRAIGHT_W;		// ������
						break;
					}
				}
			}
		}
		break;

	case TYPE_STRAIGHT_W:		// ������
		if (type == MAX_COUNTX - 1)
		{// ����P�O
			for (int nCntField = 0; nCntField < MAX_COUNTX - 2; nCntField++)
			{
				if (m_aType[type - nCntField] != TYPE_STRAIGHT_W)
				{// ����񂪉���������Ȃ�
					bMoveOn = true;		// �t���O��܂�
				}
			}

			if (bMoveOn == false)
			{// �����������������̂�
				m_aType[type] = TYPE_STRAIGHT_W;	// ������
			}
		}
		break;

	case TYPE_CURVE_UPLEFT:		// �J�[�u�i����p�j
		break;

	case TYPE_CURVE_UPRIGHT:	// �J�[�u�i�E��p�j
		break;

	case TYPE_CURVE_DOWNRIGHT:	// �J�[�u�i�E���p�j
		// ��ƍ�������ăJ�[�u�ň�����Ă���ꍇ���g��ύX�B
		if (m_aType[prevTypeY] == TYPE_CURVE_UPRIGHT)
		{// ��̏����J�[�u�E��p
			if (m_aType[prevTypeY + 1] == TYPE_CURVE_UPLEFT)
			{// ����̏����J�[�u����p
				// ���̈ʒu�ʂɎ�ނ�ύX����
				if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY || type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
				{// �O�����̈ʒu�A�E���p
					m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
				}
				else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1)
				{// �O�����̈ʒu
					m_aType[type] = TYPE_STRAIGHT_H;		// �����c
				}
				else
				{// ����ȊO
					m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				}
			}
		}

		// �E������Ȃ��オ�s���~�܂��̏ꍇ�AT���H���֕ύX�B
		if (type != (MAX_COUNTX * nCntPrevX) + nCntPrevX)
		{// �E������Ȃ�
			if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
			{// �s���~�܂��
				m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
			}
			else if (m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT)
			{// T���H�E
				m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
			}

			if (m_aType[prevTypeY] == TYPE_DEADEND_UP || m_aType[prevTypeY] == TYPE_TJUNCTION_RIGHT)
			{
				// �E�̎�ނ����̎�ޕʂɕύX
				switch (m_aType[prevTypeX])
				{
				case TYPE_STRAIGHT_H:			// �c����
					m_aType[prevTypeX] = TYPE_TJUNCTION_RIGHT;		// T���H�E
					break;

				case TYPE_CURVE_UPLEFT:			// �J�[�u����p
					m_aType[prevTypeX] = TYPE_TJUNCTION_UP;			// T���H��
					break;

				case TYPE_CURVE_DOWNLEFT:		// �J�[�u�����p
					m_aType[prevTypeX] = TYPE_TJUNCTION_DOWN;		// T���H��
					break;

				case TYPE_TJUNCTION_LEFT:		// T���H��
					m_aType[prevTypeX] = TYPE_CROSSROADS;			// �\���H
					break;

				case TYPE_DEADEND_UP:			// �s���~�܂��
					m_aType[prevTypeX] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
					break;

				case TYPE_DEADEND_DOWN:			// �s���~�܂艺
					m_aType[prevTypeX] = TYPE_CURVE_DOWNRIGHT;		// �J�[�u�E���p
					break;

				case TYPE_DEADEND_LEFT:			// �s���~�܂荶
					m_aType[prevTypeX] = TYPE_STRAIGHT_W;			// ������
					break;
				}
			}
		}
		break;

	case TYPE_CURVE_DOWNLEFT:	// �J�[�u�i�����p�j
		// ���̈ʒu�ʂɎ�ނ�ύX����
		if (type == (FIELD_COUNTY * FIELD_COUNTX) - 3)
		{// �����p�Q�O
			if (m_aType[prevTypeY + 2] == TYPE_STRAIGHT_W || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeY + 2] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY + 2] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY + 2] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeY + 2] == TYPE_DEADEND_LEFT)
			{// �����p�̏オ���Ɍq�����Ă��Ȃ�
				m_aType[type] = TYPE_TJUNCTION_DOWN;		// T���H��
			}
		}
		else if(type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY && m_aType[type - 1] == TYPE_CURVE_DOWNRIGHT && type != (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// �O���� ���� �E�ׂ��J�[�u�E���p ���� ��������Ȃ�
			m_aType[type] = TYPE_TJUNCTION_DOWN;		// T���H��
		}

		if (m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && type != (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1 && type <= FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY)
		{// �オ�J�[�u����p�A�O��������Ȃ��A�O��������Ȃ�
			m_aType[type] = TYPE_CROSSROADS;		// �\���H
		}
		else if (m_aType[prevTypeY] == TYPE_CURVE_UPLEFT && m_aType[prevTypeY - MAX_COUNTX] == TYPE_CURVE_DOWNLEFT)
		{// �オ�J�[�u����p�A����ɍ��オ�J�[�u�����p
			m_aType[type] = TYPE_TJUNCTION_DOWN;	// T���H��
		}

		break;

	case TYPE_TJUNCTION_UP:		// T���H�i��j
		break;

	case TYPE_TJUNCTION_RIGHT:	// T���H�i�E�j
		break;

	case TYPE_TJUNCTION_DOWN:	// T���H�i���j
		if (m_aType[prevTypeX] == TYPE_CURVE_DOWNRIGHT && m_aType[prevTypeY - 1] == TYPE_DEADEND_UP && prevTypeY - 1 >= MAX_COUNTX)
		{// �E���J�[�u�E���p�A�E�オ�s���~�܂��A�O���ザ��Ȃ�
			m_aType[prevTypeY - 1] = TYPE_STRAIGHT_H;		// �c����

			switch (m_aType[prevTypeY - FIELD_COUNTX - 1])
			{
			case TYPE_STRAIGHT_W:			// ������
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_TJUNCTION_UP;		// T���H��
				break;

			case TYPE_CURVE_DOWNRIGHT:		// �J�[�u�E���p
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_TJUNCTION_RIGHT;	// T���H�E
				break;

			case TYPE_CURVE_DOWNLEFT:		// �J�[�u�����p
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_TJUNCTION_LEFT;	// T���H��
				break;

			case TYPE_TJUNCTION_DOWN:		// T���H��
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_CROSSROADS;		// �\���H
				break;

			case TYPE_DEADEND_RIGHT:		// �s���~�܂�E
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				break;

			case TYPE_DEADEND_DOWN:			// �s���~�܂艺
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_STRAIGHT_H;		// �c����
				break;

			case TYPE_DEADEND_LEFT:			// �s���~�܂荶
				m_aType[prevTypeY - FIELD_COUNTX - 1] = TYPE_CURVE_UPLEFT;		// �J�[�u����p
				break;
			}
		}
		break;

	case TYPE_TJUNCTION_LEFT:	// T���H�i���j
		break;

	case TYPE_CROSSROADS:		// �\���H
		break;

	case TYPE_DEADEND_UP:		// �s���~�܂�i��j
		if (type == MAX_COUNTX)
		{// ����̈ʒu
			// �P�O�̏������ނ�ύX����
			switch (m_aType[type - 1])
			{
			case TYPE_CURVE_UPLEFT:		// �J�[�u�i����p�j
				m_aType[type] = TYPE_CURVE_UPLEFT;		// �J�[�u�i����p�j
				m_aType[type - 1] = TYPE_TJUNCTION_UP;	// T���H�i��j
				break;
			
			case TYPE_DEADEND_UP:		// �s���~�܂�i��j
				m_aType[type] = TYPE_CURVE_UPLEFT;		// �J�[�u�i����p�j
				m_aType[type - 1] = TYPE_CURVE_UPRIGHT;	// �J�[�u�i�E��p�j
				break;

			case TYPE_DEADEND_LEFT:		// �s���~�܂�i���j
				m_aType[type] = TYPE_CURVE_UPLEFT;		// �J�[�u�i����p�j
				m_aType[type - 1] = TYPE_STRAIGHT_W;	// ������
				break;
			}
		}
		else
		{
			if (m_aType[prevTypeY - 1] == TYPE_CURVE_UPRIGHT)
			{// ���ݒn�̉E�オ�J�[�u�i�E��p�j
				// ���ݒn�Ə�̎�ނ�ύX���q��
				switch (m_aType[prevTypeY])
				{
				case TYPE_STRAIGHT_W:			// ������
					m_aType[prevTypeY] = TYPE_TJUNCTION_UP;			// T���H�i��j
					break;

				case TYPE_CURVE_DOWNLEFT:		// �J�[�u�i�����p�j
					m_aType[prevTypeY] = TYPE_TJUNCTION_LEFT;		// T���H�i���j
					break;

				case TYPE_TJUNCTION_DOWN:		// T���H�i���j
					m_aType[prevTypeY] = TYPE_CROSSROADS;			// �\���H
					break;

				case TYPE_DEADEND_LEFT:			// �s���~�܂�i���j
					m_aType[prevTypeY] = TYPE_CURVE_UPLEFT;			// �J�[�u�i����p�j
					break;
				}

				m_aType[type] = TYPE_STRAIGHT_H;	// �c����
			}

			if (m_aType[prevTypeY + 1] == TYPE_DEADEND_DOWN)
			{// ���ݒn�̍��オ�s���~�܂艺
				if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
				{// ���ݒn�̏オ�s���~�܂��
					// ���ݒn�ƑS�Čq����
					m_aType[prevTypeY] = TYPE_CURVE_UPRIGHT;		// ���ݒn�̏���J�[�u�E��p�֕ύX
					m_aType[prevTypeY + 1] = TYPE_CURVE_DOWNLEFT;	// ���ݒn�̍�����J�[�u�����p�֕ύX
				}
			}
		}
		break;

	case TYPE_DEADEND_RIGHT:	// �s���~�܂�i�E�j
		if (type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
		{// �E���̈ʒu
			// ���̎�ނ̕ύX
			m_aType[type] = TYPE_CURVE_DOWNRIGHT;		// �J�[�u�i�E���p�j

			// ��̏����A�ύX�������Ɍq����悤�ɂ���
			switch (m_aType[prevTypeY])
			{
			case TYPE_CURVE_DOWNRIGHT:	// �J�[�u�E���p
				m_aType[prevTypeY] = TYPE_TJUNCTION_RIGHT;	// T���H�E	
				break;

			case TYPE_DEADEND_RIGHT:	// �s���~�܂�E
				m_aType[prevTypeY] = TYPE_CURVE_UPRIGHT;	// �J�[�u�E��p
				break;

			case TYPE_DEADEND_DOWN:		// �s���~�܂艺
				m_aType[prevTypeY] = TYPE_STRAIGHT_H;		// �c����
				break;
			}
		}
		else if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY && type != (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// �O�����̈ʒu�i�����ł͂Ȃ��j
			// ���̎�ނ̕ύX
			m_aType[type] = TYPE_STRAIGHT_W;		// ������

			// �E�̏����A�ύX�������Ɍq����悤�ɂ���
			switch (m_aType[prevTypeX])
			{
			case TYPE_CURVE_DOWNLEFT:	// �J�[�u�����p
				m_aType[prevTypeX] = TYPE_TJUNCTION_DOWN;	// T���H��
				break;

			case TYPE_DEADEND_DOWN:		// �s���~�܂艺
				m_aType[prevTypeX] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
				break;

			case TYPE_DEADEND_LEFT:		// �s���~�܂荶
				m_aType[prevTypeX] = TYPE_STRAIGHT_W;		// ������
				break;
			}
		}

		if (m_aType[prevTypeY] == TYPE_DEADEND_RIGHT)
		{// �オ�s���~�܂�E
			m_aType[prevTypeY] = TYPE_STRAIGHT_W;		// ������

			// �E��̈ʒu�ʂɎ�ނ�ύX����
			switch (m_aType[prevTypeY - 1])
			{
			case TYPE_STRAIGHT_H:			// �c����
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_RIGHT;		// T���H�E
				break;

			case TYPE_CURVE_UPLEFT:			// �J�[�u����p
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_UP;			// T���H��
				break;

			case TYPE_CURVE_DOWNLEFT:		// �J�[�u�����p
				m_aType[prevTypeY - 1] = TYPE_TJUNCTION_DOWN;		// T���H��
				break;

			case TYPE_TJUNCTION_LEFT:		// T���H��
				m_aType[prevTypeY - 1] = TYPE_CROSSROADS;			// �\���H
				break;

			case TYPE_DEADEND_UP:			// �s���~�܂��
				m_aType[prevTypeY - 1] = TYPE_CURVE_UPRIGHT;		// �J�[�u�E��p
				break;

			case TYPE_DEADEND_DOWN:			// �s���~�܂艺
				m_aType[prevTypeY - 1] = TYPE_CURVE_DOWNRIGHT;		// �J�[�u�E���p
				break;

			case TYPE_DEADEND_LEFT:			// �s���~�܂荶
				m_aType[prevTypeY - 1] = TYPE_STRAIGHT_W;			// ������
				break;
			}
		}
		break;

	case TYPE_DEADEND_DOWN:		// �s���~�܂�i���j
		if (m_aType[prevTypeY] == TYPE_DEADEND_UP)
		{// ��̏����s���~�܂��
			// ���̈ʒu�ʂɎ�ނ�ύX����
			if (type > FIELD_COUNTY * FIELD_COUNTX - FIELD_COUNTY || type == (MAX_COUNTY * MAX_COUNTX) + MAX_COUNTY)
			{// �O�����̈ʒu�A�E���p
				m_aType[type] = TYPE_CURVE_DOWNRIGHT;	// �J�[�u�E���p
			}
			else if (type == (MAX_COUNTX * nCntPrevX) + nCntPrevX - 1)
			{// �O�����̈ʒu
				m_aType[type] = TYPE_STRAIGHT_H;		// �����c
			}
			else
			{// ����ȊO
				m_aType[type] = TYPE_TJUNCTION_RIGHT;	// T���H�E
			}
		}
		break;

	case TYPE_DEADEND_LEFT:		// �s���~�܂�i���j
		// ���̈ʒu�ʂɎ�ނ�ύX����
		if (type == (FIELD_COUNTY * FIELD_COUNTX) - 3)
		{// �����p�Q�O
			if (m_aType[prevTypeY + 2] == TYPE_STRAIGHT_W || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNRIGHT || m_aType[prevTypeY + 2] == TYPE_CURVE_DOWNLEFT
				|| m_aType[prevTypeY + 2] == TYPE_TJUNCTION_DOWN || m_aType[prevTypeY + 2] == TYPE_DEADEND_RIGHT || m_aType[prevTypeY + 2] == TYPE_DEADEND_DOWN
				|| m_aType[prevTypeY + 2] == TYPE_DEADEND_LEFT)
			{// �����p�̏オ���Ɍq�����Ă��Ȃ�
				m_aType[type] = TYPE_STRAIGHT_W;		// ������
			}
		}
		else if (type == (FIELD_COUNTY * FIELD_COUNTX) - 1)
		{// �����p
			// ���̎�ނ̕ύX
			m_aType[type] = TYPE_CURVE_DOWNLEFT;		// �J�[�u�i�����p�j

			// ��̏����A�ύX�������Ɍq����悤�ɂ���
			switch (m_aType[prevTypeY])
			{
			case TYPE_CURVE_DOWNLEFT:	// �J�[�u�����p
				m_aType[prevTypeY] = TYPE_TJUNCTION_LEFT;	// T���H��	
				break;

			case TYPE_DEADEND_DOWN:		// �s���~�܂艺
				m_aType[prevTypeY] = TYPE_STRAIGHT_H;		// �c����
				break;

			case TYPE_DEADEND_LEFT:		// �s���~�܂荶
				m_aType[prevTypeY] = TYPE_CURVE_DOWNLEFT;	// �J�[�u�����p
				break;
			}
		}
		break;
	}
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

	// �t�B�[�o�[����rand�̒l�̏�����
	m_nRandFever = RAND_GABAGEPOSFEVER;

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
	m_nTimeFever++;			// ���Ԃ��J�E���g

	// �I�u�W�F�N�g���b�V���̍X�V����
	CObjectMesh::Update();

	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 PlayerRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CManager::GetMode() == CScene::MODE_TITLE)
	{// �`���[�g���A��
		// �v���C���[�̏��擾
		PlayerPos = CTitle::GetPlayer()->GetPos();	// �ʒu
		PlayerRot = CTitle::GetPlayer()->GetRot();	// ����
	}
	else if (CManager::GetMode() == CScene::MODE_GAME)
	{// �Q�[�����[�h
		// �v���C���[�̏��擾
		PlayerPos = CGame::GetPlayer()->GetPos();	// �ʒu
		PlayerRot = CGame::GetPlayer()->GetRot();	// ����

		if (CGame::GetTime()->Get() <= TIME_FEVER)
		{// FEVER TIME
			//if ((m_nTimeFever % CREATEGABAGETIME_FEVER) == 0)
			//{// ��莞�Ԃ���
				srand(timeGetTime());					// rand�̒l��������
				int nRand = rand() % m_nRandFever;		// rand�̒l���擾

				if (nRand == 0)
				{
					// �ݒ菈��
					Set(m_pos, m_rot, m_type);
				}
			//}
		}
	}

	if (PlayerPos.x <= m_pos.x + m_fSizeX && PlayerPos.x >= m_pos.x - m_fSizeX
		&& PlayerPos.z <= m_pos.z + m_fSizeZ && PlayerPos.z >= m_pos.z - m_fSizeZ)
	{// �v���C���[�����͈͓̔��ɓ�����
		CManager::GetDebugProc()->Print(" ���͈͓̔��[\n");		// �f�o�b�O�\��

		if (PlayerRot.y <= TURN_ROT_DIFF && PlayerRot.y >= -TURN_ROT_DIFF
			&& PlayerPos.z >= m_pos.z + (m_fSizeZ * TURN_LENTH) - TURN_DIFF
			&& PlayerPos.z <= m_pos.z + (m_fSizeZ * TURN_LENTH) + TURN_DIFF)
		{// -��Z�����̊p
			// ��ޕʔ��菈��
			TurnInfo(D3DX_PI * ROT_UP);

			CManager::GetDebugProc()->Print(" ������[\n");		// �f�o�b�O�\��
		}
		else if ((PlayerRot.y <= D3DX_PI + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI - TURN_ROT_DIFF
			&& PlayerPos.z <= m_pos.z - (m_fSizeZ * TURN_LENTH) + TURN_DIFF
			&& PlayerPos.z >= m_pos.z - (m_fSizeZ * TURN_LENTH) - TURN_DIFF)
				|| (PlayerRot.y <= -D3DX_PI + TURN_ROT_DIFF && PlayerRot.y >= -D3DX_PI - TURN_ROT_DIFF
			&& PlayerPos.z <= m_pos.z - (m_fSizeZ * TURN_LENTH) + TURN_DIFF
			&& PlayerPos.z >= m_pos.z - (m_fSizeZ * TURN_LENTH) - TURN_DIFF))
		{// +��Z�����̊p
			// ��ޕʔ��菈��
			TurnInfo(D3DX_PI * ROT_DOWN);

			CManager::GetDebugProc()->Print(" �������[\n");		// �f�o�b�O�\��
		}
		else if (PlayerRot.y <= D3DX_PI * ROT_RIGHT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_RIGHT - TURN_ROT_DIFF
			&& PlayerPos.x >= m_pos.x + (m_fSizeX * TURN_LENTH) - TURN_DIFF
			&& PlayerPos.x <= m_pos.x + (m_fSizeX * TURN_LENTH) + TURN_DIFF)
		{// -��X�����̊p
			// ��ޕʔ��菈��
			TurnInfo(D3DX_PI * ROT_RIGHT);

			CManager::GetDebugProc()->Print(" �E�����[\n");		// �f�o�b�O�\��
		}
		else if (PlayerRot.y <= D3DX_PI * ROT_LEFT + TURN_ROT_DIFF && PlayerRot.y >= D3DX_PI * ROT_LEFT - TURN_ROT_DIFF
			&& PlayerPos.x <= m_pos.x - (m_fSizeX * TURN_LENTH) + TURN_DIFF
			&& PlayerPos.x >= m_pos.x - (m_fSizeX * TURN_LENTH) - TURN_DIFF)
		{// +��X�����̊p
			// ��ޕʔ��菈��
			TurnInfo(D3DX_PI * ROT_LEFT);
		
			CManager::GetDebugProc()->Print(" �������[\n");		// �f�o�b�O�\��
		}

#if _DEBUG
		// ���̐F�ύX
		//CObjectMesh::SetCol(CObjectMesh::COL_RED);
#endif
	}

	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{
		if (PlayerPos.z < m_pos.z - DELETE_POSITION)
		{// ������ʊO�ւ�����
			// �I������
			Uninit();
		}
	}
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
void CMeshField::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;
	m_rot = rot;
	m_type = type;

	srand(timeGetTime());										// rand�̒l��������
	int nPosRand = rand() % 5 + GABAGEPOS_CENTER;	// rand�̒l���擾�i�ʒu�j
	int nNumXRand = 1;											// rand�̒l���擾�i��X�j
	int nNumYRand = rand() % 2 + 1;								// rand�̒l���擾�i��Y�j

	if (CManager::GetMode() == CScene::MODE_GAME)
	{// �Q�[�����[�h
		if (/*type == TYPE_CURVE_UPRIGHT || type == TYPE_CURVE_DOWNLEFT ||*/ nPosRand > GABAGEPOS_EDGE)
		{
			nNumXRand = RANDPOSITION_MAX;				// rand�̒l���擾�i��X�j
			nNumYRand = RANDPOSITION_MAX;				// rand�̒l���擾�i��Y�j
		}
	}

	// �z�u�ݒ�
	switch (nPosRand)
	{
	case GABAGEPOS_CENTER:		// ���S
		nNumYRand = 1;
		nNumXRand = 1;
		break;

	case GABAGEPOS_EDGE:		// �[
		nNumXRand = 1;
		break;

	default:					// ������
		if (m_type == TYPE_STRAIGHT_H || m_type == TYPE_DEADEND_DOWN || m_type == TYPE_DEADEND_UP)
		{
			nNumYRand = 1;
		}
		else if (m_type == TYPE_STRAIGHT_W || m_type == TYPE_DEADEND_RIGHT || m_type == TYPE_DEADEND_LEFT)
		{
			nNumXRand = 1;
		}
		else
		{
			nNumYRand = 1;
			nNumXRand = 1;
		}
		break;
	}

	// �S�~�̃����_���z�u
	for (int nCntX = 0; nCntX < nNumXRand; nCntX++)
	{
		for (int nCntY = 0; nCntY < nNumYRand; nCntY++)
		{
			// ��ޕʂɏ������s��
			switch (m_type)
			{
			case TYPE_STRAIGHT_H:		// �c����
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
					break;
				}
				break;

			case TYPE_STRAIGHT_W:		// ������
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH,
						m_pos.y, m_pos.z + cosf(D3DX_PI * ROT_RIGHT) * nCntY * DISTANCE_WIDTH + cosf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH), 2);
					break;
				}
				break;

			case TYPE_CURVE_UPLEFT:		// �J�[�u�i����p�j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
					break;
				}
				break;

			case TYPE_CURVE_UPRIGHT:	// �J�[�u�i�E��p�j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_CURVE_DOWNRIGHT:	// �J�[�u�i�E���p�j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_CURVE_DOWNLEFT:	// �J�[�u�i�����p�j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_UP:		// T���H�i��j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_RIGHT:	// T���H�i�E�j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_DOWN:	// T���H�i���j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_TJUNCTION_LEFT:	// T���H�i���j
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_CROSSROADS:		// �\���H
				// �S�~�̈ʒu
				switch (nPosRand)
				{
				case GABAGEPOS_CENTER:		// ���S
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_UP) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_UP) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_UP) * LENTH_FAR), 2);
					break;

				case GABAGEPOS_EDGE:		// �[
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * LENTH_FAR), 2);
					break;

				default:		// ������
					CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * LENTH_FAR,
						m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * LENTH_FAR), 2);
					break;
				}
				break;

			case TYPE_DEADEND_UP:		// �s���~�܂�i��j
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
				break;

			case TYPE_DEADEND_RIGHT:	// �s���~�܂�i�E�j
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH), 2);
				break;

			case TYPE_DEADEND_DOWN:		// �s���~�܂�i���j
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_LEFT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_RIGHT) * DISTANCE_WIDTH), 2);
				break;

			case TYPE_DEADEND_LEFT:		// �s���~�܂�i���j
				CGarbage::Create(D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * ROT_RIGHT) * nCntX * DISTANCE_WIDTH + sinf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH,
					m_pos.y, m_pos.z + nCntY * DISTANCE_HEIGHT + cosf(D3DX_PI * ROT_LEFT) * DISTANCE_WIDTH), 2);
				break;
			}
		}
	}
}

//===============================================
// ��ޕʔ��菈��
//===============================================
void CMeshField::TurnInfo(float fRot)
{
	// ��ޕʂɏ������s��
	switch (m_type)
	{
	case TYPE_STRAIGHT_H:		// �c����
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// �܂���Ȃ�
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// �܂���Ȃ�
		}
		break;

	case TYPE_STRAIGHT_W:		// ������
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// �܂���Ȃ�
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// �܂���Ȃ�
		}
		break;

	case TYPE_CURVE_UPLEFT:		// �J�[�u�i����p�j
		if (fRot == D3DX_PI * ROT_DOWN)
		{// �����ʒu���猩�ď�֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// �����ʒu���猩�č��֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
		}
		break;

	case TYPE_CURVE_UPRIGHT:	// �J�[�u�i�E��p�j
		if (fRot == D3DX_PI * ROT_DOWN)
		{// �����ʒu���猩�ď�֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
		}
		else if (fRot == D3DX_PI * ROT_LEFT)
		{// �����ʒu���猩�ĉE�֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
		}
		break;

	case TYPE_CURVE_DOWNRIGHT:	// �J�[�u�i�E���p�j
		if (fRot == D3DX_PI * ROT_UP)
		{// �����ʒu���猩�ď�֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
		}
		else if (fRot == D3DX_PI * ROT_LEFT)
		{// �����ʒu���猩�ĉE�֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
		}
		break;

	case TYPE_CURVE_DOWNLEFT:	// �J�[�u�i�����p�j
		if (fRot == D3DX_PI * ROT_UP)
		{// �����ʒu���猩�ĉ��֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
			}
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// �����ʒu���猩�č��֐N��
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
		}
		break;

	case TYPE_TJUNCTION_UP:		// T���H�i��j
		if (fRot == D3DX_PI * ROT_LEFT)
		{// �����ʒu���猩�ĉE�֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// �^�������s���ĉE�̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// �����ʒu���猩�č��֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// �^�������s���č��̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_DOWN)
		{// �����ʒu���猩�ď�֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_CURVE);			// �^�����������Ȃ������E�Ȃ��邱�Ƃ͂ł���
		}
		break;

	case TYPE_TJUNCTION_RIGHT:	// T���H�i�E�j
		if (fRot == D3DX_PI * ROT_DOWN)
		{// �����ʒu���猩�ď�֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// �^�������s���č��̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_UP)
		{// �����ʒu���猩�ĉ��֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// �^�������s���ĉE�̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_LEFT)
		{// �����ʒu���猩�ĉE�֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_CURVE);			// �^�����������Ȃ������E�Ȃ��邱�Ƃ͂ł���
		}
		break;

	case TYPE_TJUNCTION_DOWN:	// T���H�i���j
		if (fRot == D3DX_PI * ROT_LEFT)
		{// �����ʒu���猩�ĉE�֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// �^�������s���č��̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// �����ʒu���猩�č��֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// �^�������s���ĉE�̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_UP)
		{// �����ʒu���猩�ĉ��֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_CURVE);			// �^�����������Ȃ������E�Ȃ��邱�Ƃ͂ł���
		}
		break;

	case TYPE_TJUNCTION_LEFT:	// T���H�i���j
		if (fRot == D3DX_PI * ROT_DOWN)
		{// �����ʒu���猩�ď�֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_RIGHT);	// �^�������s���ĉE�̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_UP)
		{// �����ʒu���猩�ĉ��֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_STRAIGHT_LEFT);	// �^�������s���č��̂݋Ȃ��邱�Ƃ��ł���
		}
		else if (fRot == D3DX_PI * ROT_RIGHT)
		{// �����ʒu���猩�č��֐N��
			// ���͕����ύX����
			TurnInput(DESTINATION_CURVE);			// �^�����������Ȃ������E�Ȃ��邱�Ƃ͂ł���
		}
		break;

	case TYPE_CROSSROADS:		// �\���H
		// ���͕����ύX����
		TurnInput(DESTINATION_STRAIGHT_CURVE);		// �^�����������č��E�Ȃ��邱�Ƃ��ł���
		break;

	case TYPE_DEADEND_UP:		// �s���~�܂�i��j
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		break;

	case TYPE_DEADEND_RIGHT:	// �s���~�܂�i�E�j
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		break;

	case TYPE_DEADEND_DOWN:		// �s���~�܂�i���j
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		break;

	case TYPE_DEADEND_LEFT:		// �s���~�܂�i���j
		if (CManager::GetMode() == CScene::MODE_TITLE)
		{
			CTitle::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		else if (CManager::GetMode() == CScene::MODE_GAME)
		{
			CGame::GetPlayer()->SetTurn(CPlayer::TURN_BACK);		// �܂�Ԃ�
		}
		break;
	}
}

//===============================================
// ���͕����ύX����
//===============================================
void CMeshField::TurnInput(DESTINATION destination)
{
	if (CManager::GetKeyboardInput()->GetPress(DIK_Q) == true || CManager::GetKeyboardInput()->GetPress(DIK_Z) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_LEFT, 0))
	{// A�L�[����
		if (destination == DESTINATION_STRAIGHT_CURVE || destination == DESTINATION_STRAIGHT_LEFT || destination == DESTINATION_CURVE || destination == DESTINATION_LEFT)
		{// ���ɋȂ����
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// ���֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// ���֋Ȃ���
			}
		}
	}
	else if (CManager::GetKeyboardInput()->GetPress(DIK_E) == true || CManager::GetKeyboardInput()->GetPress(DIK_C) == true
		|| CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RB, 0) || CManager::GetInputGamePad()->GetPress(CInputGamePad::BUTTON_RIGHT, 0))
	{// D�L�[����
		if (destination == DESTINATION_STRAIGHT_CURVE || destination == DESTINATION_STRAIGHT_RIGHT || destination == DESTINATION_CURVE || destination == DESTINATION_RIGHT)
		{// �E�ɋȂ����
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
			}
		}
	}
	else
	{// ���͂Ȃ�
		if (destination == DESTINATION_STRAIGHT_CURVE || destination == DESTINATION_STRAIGHT
			|| destination == DESTINATION_STRAIGHT_RIGHT || destination == DESTINATION_STRAIGHT_LEFT)
		{// �^�������i�߂�
			if (CManager::GetMode() == CScene::MODE_TITLE)
			{
				CTitle::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// �܂���Ȃ�
			}
			else if (CManager::GetMode() == CScene::MODE_GAME)
			{
				CGame::GetPlayer()->SetTurn(CPlayer::TURN_NONE);		// �܂���Ȃ�
			}
		}
		else
		{// �i�߂Ȃ�
			switch (destination)
			{
			case DESTINATION_LEFT:		// �^�����������Ȃ������ɂ͋Ȃ����
				if (CManager::GetMode() == CScene::MODE_TITLE)
				{
					CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// ���֋Ȃ���
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{
					CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);		// ���֋Ȃ���
				}
				break;

			case DESTINATION_RIGHT:		// �^�����������Ȃ����E�ɂ͋Ȃ����
				if (CManager::GetMode() == CScene::MODE_TITLE)
				{
					CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
				}
				else if (CManager::GetMode() == CScene::MODE_GAME)
				{
					CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
				}
				break;

			case DESTINATION_CURVE:		// �^�����������Ȃ������E�Ȃ����
				// �����_���ō��E�ǂ���ɋȂ��邩����
				srand(timeGetTime());											// rand�̒l��������
				int nRand = rand() % CPlayer::TURN_LEFT + CPlayer::TURN_RIGHT;	// rand�̒l���擾

				switch (nRand)
				{
				case CPlayer::TURN_RIGHT:		// �E�֋Ȃ���
					if (CManager::GetMode() == CScene::MODE_TITLE)
					{
						CTitle::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
					}
					else if (CManager::GetMode() == CScene::MODE_GAME)
					{
						CGame::GetPlayer()->SetTurn(CPlayer::TURN_RIGHT);	// �E�֋Ȃ���
					}
					break;

				case CPlayer::TURN_LEFT:		// ���֋Ȃ���
					if (CManager::GetMode() == CScene::MODE_TITLE)
					{
						CTitle::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
					}
					else if (CManager::GetMode() == CScene::MODE_GAME)
					{
						CGame::GetPlayer()->SetTurn(CPlayer::TURN_LEFT);	// ���֋Ȃ���
					}
					break;
				}
				break;
			}
		}
	}
}