//=========================================================
//
// �r���{�[�h���� [objectBillboard.cpp]
// Author = �����đ�Y
//
//=========================================================
#ifndef _OBJECTBILLBOARD_H_  // ���̃}�N����`������ĂȂ�������
#define _OBJECTBILLBOARD_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "object3D.h"

//===============================================
// �}�N����`
//===============================================
#define SIZE_DIFF			(0.4f)		// �T�C�Y�̒l�̕␳

//===============================================
// �r���{�[�h�N���X
//===============================================
class CObjectBillboard : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObjectBillboard();						// �f�t�H���g�R���X�g���N�^
	CObjectBillboard(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CObjectBillboard();					// �f�X�g���N�^

	static CObjectBillboard *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdatePos(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	void RevisionPos(const D3DXVECTOR3 pos, const float fMalti = 1.0f);
	bool ScalingSize(const float fSize, float fMalti = SIZE_DIFF);
	void DrawEffect(void);
	void Brightness(float fBrightness);

	void BindTexture(int nIdx);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMove(const D3DXVECTOR3 move);
	void SetCol(const D3DXCOLOR col);
	void SetSize(const float fSizeX, const float fSizeZ);
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, 0.0f, m_fSizeZ); }

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_rot;							// ����
	float m_fSizeX;								// ��
	float m_fSizeZ;								// ����

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	float m_fBrightness;				// ���邳�̒l
	bool m_bFlash;						// ���邭���邩�Â����邩
};

#endif