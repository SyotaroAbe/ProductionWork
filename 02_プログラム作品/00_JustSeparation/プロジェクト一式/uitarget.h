//=========================================================
//
// �^�[�Q�b�gUI���� [uitarget.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _UITARGET_H_     // ���̃}�N����`������ĂȂ�������
#define _UITARGET_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object.h"

//===============================================
// �O���錾
//===============================================
class CObjectBillboard;

//===============================================
// �}�N����`
//===============================================
//#define NUM_TEX		(3)				// �e�N�X�`���̐�

//===============================================
// �^�[�Q�b�gUI�N���X
//===============================================
class CUiTarget
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CUiTarget();						// �f�t�H���g�R���X�g���N�^
	CUiTarget(int nPriority = 5);		// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CUiTarget();						// �f�X�g���N�^

	enum TYPE
	{
		TYPE_BURN,			// �R����S�~
		TYPE_NONFLAMMABLE,	// �R���Ȃ��S�~
		TYPE_RECYCLABLE,		// �����S�~
		TYPE_MAX
	};

	static CUiTarget *Create(int nIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CObjectBillboard *m_apObjectBillboard[TYPE_MAX];		// �I�u�W�F�N�g�r���{�[�h�N���X�̃|�C���^
	static const char *m_apTextureData[TYPE_MAX];				// �e�N�X�`���t�@�C����
	static int m_aIdxTexture[TYPE_MAX];							// �g�p����e�N�X�`���̔ԍ�
	D3DXVECTOR3 m_aPos[TYPE_MAX];								// ���v�ʒu
};

#endif