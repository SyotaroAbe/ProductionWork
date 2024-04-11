//=========================================================
//
// ���d�̔w�i���� [multibg.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MULTIBG_H_     // ���̃}�N����`������ĂȂ�������
#define _MULTIBG_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

#include "main.h"
#include "object.h"

//===============================================
// �O���錾
//===============================================
class CObject2D;

//===============================================
// �}�N����`
//===============================================
#define NUM_BG		(3)				// �w�i�̐�

//===============================================
// ���d�̔w�i�N���X
//===============================================
class CMultiBg : public CObject
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CMultiBg();		// �f�t�H���g�R���X�g���N�^
	~CMultiBg();	// �f�X�g���N�^

	static CMultiBg *Create(int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const float TexU, const float TexV);
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_pos; }
	void SetMove(const D3DXVECTOR3 move);
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f); }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static CObject2D *m_apObject2D[MAX_OBJECT];			// �I�u�W�F�N�g2D�N���X�̃|�C���^
	static int m_aIdxTexture[NUM_BG];					// �g�p����e�N�X�`���̔ԍ�
	static const char *m_apTextureData[NUM_BG];			// �e�N�X�`���t�@�C����
	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_rot;									// ����
	float m_fTexU;										// �e�N�X�`�����W�̊J�n�ʒu�iU�l�j
	float m_fTexV;										// �e�N�X�`�����W�̊J�n�ʒu�iV�l�j
};

#endif