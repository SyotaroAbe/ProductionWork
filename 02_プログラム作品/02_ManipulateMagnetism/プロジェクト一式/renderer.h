//=========================================================
//
// �����_���[���� [renderer.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _RENDERER_H_     // ���̃}�N����`������ĂȂ�������
#define _RENDERER_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"

//===============================================
// �O���錾
//===============================================
class CFade;

//===============================================
// �����_���[�N���X
//===============================================
class CRenderer
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CRenderer();		// �f�t�H���g�R���X�g���N�^
	~CRenderer();		// �f�X�g���N�^

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	static CFade *GetFade() { return m_pFade; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	LPDIRECT3D9 m_pD3D;						// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// Direct3D�f�o�C�X�ւ̃|�C���^

	static CFade *m_pFade;							// �t�F�[�h�N���X�̃|�C���^
};

#endif