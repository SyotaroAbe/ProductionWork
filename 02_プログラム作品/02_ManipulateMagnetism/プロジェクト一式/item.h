//=========================================================
//
// �A�C�e������ [item.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _ITEM_H_  // ���̃}�N����`������ĂȂ�������
#define _ITEM_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"
#include "xfile.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_NAME	(256)	// �ő啶����

//===============================================
// �A�C�e���N���X
//===============================================
class CItem : public CObjectX
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CItem();						// �f�t�H���g�R���X�g���N�^
	CItem(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CItem();			// �f�X�g���N�^

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawXFile(int nIdx, CXFile::COL col);
	static bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	static bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

	void SetMagnet(const bool bMagnet) { m_bMagnet = bMagnet; }

protected:	// �h���N���X������A�N�Z�X�ł���

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxXFile[MODEL_MAX];			// �g�p����X�t�@�C���̔ԍ�
	
	D3DXVECTOR3 m_vtxMax;						// ���f���̍ő�l
	D3DXVECTOR3 m_vtxMin;						// ���f���̍ŏ��l
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_nIdxShadow;							// �g�p����e�̔ԍ�
	bool m_bMagnet;								// �v���C���[�Ƃ������Ă��邩
};

#endif