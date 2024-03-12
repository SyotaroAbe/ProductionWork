//=========================================================
//
// �|�[�Y���� [pause.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _PAUSE_H_  // ���̃}�N����`������ĂȂ�������
#define _PAUSE_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//===============================================
// �O���錾
//===============================================
class CObject2D;

//===============================================
// �}�N����`
//===============================================

//===============================================
// �|�[�Y�N���X
//===============================================
class CPause
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CPause();					// �f�t�H���g�R���X�g���N�^
	CPause(int nPriority = 5);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CPause();					// �f�X�g���N�^;

	// �Q�[���̏��
	enum MENU
	{
		MENU_CONTINUE = 0,	// �Q�[���ɖ߂�
		MENU_RETRY,			// �Q�[������蒼��
		MENU_QUIT,			// �^�C�g����ʂɖ߂�
		MENU_MAX
	};

	// �e�N�X�`���̎��
	enum TEX
	{
		TEX_BG = 0,		// �w�i
		TEX_CONTINU,	// �Q�[���ɖ߂�
		TEX_RETRY,		// ���g���C
		TEX_QUIT,		// �^�C�g���ɖ߂�
		TEX_LOGO,		// �uPause�v
		TEX_MAX
	};

	static CPause *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CObject2D *m_apObject2D[TEX_MAX];		// �I�u�W�F�N�g2D�N���X�̃|�C���^
	static int m_aIdxTexture[TEX_MAX];				// �g�p����e�N�X�`���̔ԍ�
	static const char *m_apTextureData[TEX_MAX];	// �e�N�X�`���t�@�C����

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_aPos[TEX_MAX];						// �ʒu
	D3DXVECTOR3 m_aSize[TEX_MAX];						// �T�C�Y
	MENU m_Menu;										// �|�[�Y���j���[
	int m_nPosCursor;									// �J�[�\���̈ʒu
	bool m_EnterFade;									// �J�ڒ��R�}���h�h�~
};

#endif