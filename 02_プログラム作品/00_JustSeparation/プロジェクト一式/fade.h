//=========================================================
//
// �t�F�[�h���� [fade.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _FADE_H_  // ���̃}�N����`������ĂȂ�������
#define _FADE_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "manager.h"

//===============================================
// �t�F�[�h�N���X
//===============================================
class CFade
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]

	// �t�F�[�h�̏��
	typedef enum
	{
		STATE_NONE = 0,  // �������Ă��Ȃ����
		STATE_IN,        // �t�F�[�h�C�����
		STATE_OUT,       // �t�F�[�h�A�E�g���
		STATE_MAX
	}STATE;

	CFade();						// �f�t�H���g�R���X�g���N�^
	~CFade();						// �f�X�g���N�^

	static CFade *Create(CScene::MODE modeNext, int nPriority);

	HRESULT Init(CScene::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(CScene::MODE modeNext);
	STATE Get(void) { return m_state; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	STATE m_state;							// �t�F�[�h�̏��
	CScene::MODE m_modeNext;				// ���̉�ʁi���[�h�j
	D3DXCOLOR m_color;						// �|���S���i�t�F�[�h�j�̐F
};

#endif