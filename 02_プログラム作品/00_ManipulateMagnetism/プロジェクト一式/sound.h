//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �����đ�Y
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=========================================================
// �T�E���h�N���X
//=========================================================
class CSound
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CSound();		// �f�t�H���g�R���X�g���N�^
	~CSound();		// �f�X�g���N�^

	// �T�E���h�ꗗ
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// BGM�i�^�C�g���j
		LABEL_BGM_TUTORIAL,		// BGM�i�`���[�g���A���j
		LABEL_BGM_GAME,			// BGM�i�Q�[���j
		LABEL_BGM_RANKING,		// BGM�i�����L���O�j
		LABEL_SE_PAUSE,			// �|�[�Y�N��
		LABEL_SE_PAUSE_CANCEL,	// �|�[�Y�L�����Z��
		LABEL_SE_PAUSE_CURSOR,	// �|�[�Y�̃J�[�\���ړ�
		LABEL_SE_PAUSE_ENTER,	// �|�[�Y����
		LABEL_SE_TITLE_ENTER,	// �^�C�g������
		LABEL_SE_BULLET,	// �^�C�g������
		LABEL_SE_DASH,	// �^�C�g������
		LABEL_SE_EXPLOSION,	// �^�C�g������
		LABEL_SE_HIPDROP,	// �^�C�g������
		LABEL_SE_JUMP,	// �^�C�g������
		LABEL_MAX,
	} LABEL;

	// �T�E���h���̍\���̒�`
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} INFO;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);
	bool GetPlay(LABEL label);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = nullptr;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = nullptr;		// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y
	static const INFO m_aInfo[LABEL_MAX];					// �T�E���h�̏��
};

#endif