//=========================================================
//
// ������@�\������ [uioperation.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _UIOPERATION_H_     // ���̃}�N����`������ĂȂ�������
#define _UIOPERATION_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object2D.h"

//===============================================
// �}�N����`
//===============================================

//===============================================
// ������@�\���N���X
//===============================================
class CUiOperation : public CObject2D
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CUiOperation();						// �f�t�H���g�R���X�g���N�^
	CUiOperation(int nPriority = 5);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CUiOperation();					// �f�X�g���N�^

	// �菇
	enum TEX
	{
		TEX_NOT_OPERATION = 0,	// �ړ�����ł��Ȃ�
		TEX_COLLECT,			// �^�[�Q�b�g���d�˂�
		TEX_TARGET,				// �^�[�Q�b�g�ړ�
		TEX_MATCHCOL,			// �F�𑵂��悤
		TEX_BURN,				// �R����S�~
		TEX_NONFLAMMABLE,		// �R���Ȃ��S�~
		TEX_RECYCLABLE,			// �����S�~
		TEX_GAGE,				// �Q�[�W
		TEX_DUMPSTER,			// �S�~�X�e�[�V����
		TEX_END,				// �I���m�F
		TEX_MAX
	};

	// �{�^��
	enum BUTTON
	{
		BUTTON_W = 0,	// W
		BUTTON_A,		// A
		BUTTON_S,		// S
		BUTTON_D,		// D
		BUTTON_MAX
	};

	static CUiOperation *Create(int nTex, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nTex, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const int nType);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static const char *m_apTextureData[TEX_MAX];	// �e�N�X�`���t�@�C����
	static int m_aIdxTexture[TEX_MAX];				// �g�p����e�N�X�`���̔ԍ�
	static int m_nProgress;							// �i��
	static int m_nTime;								// �o�ߎ���

	int m_nTypeTex;									// �e�N�X�`���̎��
	bool m_bPressInput[BUTTON_MAX];					// �{�^������������
	int m_nGarbage;								// �S�~�̗�
};

#endif