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
		LABEL_BGM_FEVER,		// BGM�i�t�B�[�o�[�j
		LABEL_BGM_SCOREATTACK,	// BGM�i�X�R�A�A�^�b�N�j
		LABEL_BGM_RANKING,		// BGM�i�����L���O�j
		LABEL_SE_CURVE,			// �J�[�u
		LABEL_SE_SEPARATION,	// ����
		LABEL_SE_JUSTDUST,		// JUSTDUST
		LABEL_SE_SUCCESS,		// ����
		LABEL_SE_FAILURE,		// ���s
		LABEL_SE_ROCKON,		// ���b�N�I��
		LABEL_SE_ABLE,			// ���b�N�I����
		LABEL_SE_DISABLE,		// ���b�N�I���s��
		LABEL_SE_DUMPSTER,		// �S�~�X�e�[�V����
		LABEL_SE_FEVER,			// �t�B�[�o�[
		LABEL_SE_PAUSE,			// �|�[�Y�N��
		LABEL_SE_PAUSE_CANCEL,	// �|�[�Y�L�����Z��
		LABEL_SE_PAUSE_CURSOR,	// �|�[�Y�̃J�[�\���ړ�
		LABEL_SE_PAUSE_ENTER,	// �|�[�Y����
		LABEL_SE_TITLE_ENTER,	// �^�C�g������
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

	IXAudio2 *m_pXAudio2 = NULL;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;		// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX] = {};						// �I�[�f�B�I�f�[�^�T�C�Y
	static const INFO m_aInfo[LABEL_MAX];					// �T�E���h�̏��
};

#endif