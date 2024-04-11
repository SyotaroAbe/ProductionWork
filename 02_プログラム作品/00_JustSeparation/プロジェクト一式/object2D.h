//=========================================================
//
// �I�u�W�F�N�g2D���� [object2D.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _OBJECT2D_H_     // ���̃}�N����`������ĂȂ�������
#define _OBJECT2D_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object.h"

//===============================================
// �I�u�W�F�N�g2D�N���X
//===============================================
class CObject2D : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CObject2D();					// �f�t�H���g�R���X�g���N�^
	CObject2D(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObject2D();			// �f�X�g���N�^

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void UpdatePos(D3DXVECTOR3 pos, float fSizeX, float fSizeY);
	void UpdatePosAngle(D3DXVECTOR3 pos);
	void UpdateAnim(int nPatternX, int nPatternY, int nSpeed);
	void UpdateTex(const float fTexU, const float fSubU, const float fAddU, const float fTexV, const float fSubV, const float fAddV);
	bool RevisionPos(const D3DXVECTOR3 pos, float fMalti, bool bWidth = false);
	void DrawEffect(D3DXCOLOR col);
	void Brightness(float fBrightness);

	void BindTexture(int nIdx);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	virtual D3DXVECTOR3 GetPosOld(void) { return m_posOld; };
	void SetMove(const D3DXVECTOR3 move);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetCol(const D3DXCOLOR col);
	void SetNumber(const int nNumber);
	void SetSize(const float fSizeX, const float fSizeY);
	float GetSizeX(void) { return m_fSizeX; }
	float GetSizeY(void) { return m_fSizeY; }
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f); }

protected:	// �h���N���X������A�N�Z�X�ł��� [�A�N�Z�X�w��q]
	LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����
	float m_fLength;		// �Ίp���̒���
	float m_fAngle;			// �Ίp���̊p�x
	float m_fSizeX;			// ��
	float m_fSizeY;			// ����

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	float m_fBrightness;				// ���邳�̒l
	bool m_bFlash;						// ���邭���邩�Â����邩
};

#endif