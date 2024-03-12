//=========================================================
//
// ���΃u���b�N���� [magnet.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MAGNET_H_  // ���̃}�N����`������ĂȂ�������
#define _MAGNET_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"
#include "xfile.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_NAME	(256)	// �ő啶����

//===============================================
// ���΃u���b�N�N���X
//===============================================
class CMagnet : public CObjectX
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CMagnet();						// �f�t�H���g�R���X�g���N�^
	CMagnet(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CMagnet();			// �f�X�g���N�^

	enum STATE
	{
		STATE_NONE = 0,	// ����
		STATE_N,
		STATE_S,
		STATE_MAX
	};

	static CMagnet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL type, int nPriority = 3);

	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void DrawXFile(int nIdx, CXFile::COL col);
	static bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);
	static bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pRot, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

protected:	// �h���N���X������A�N�Z�X�ł���

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_aIdxXFile[MODEL_MAX];			// �g�p����X�t�@�C���̔ԍ�
	
	STATE m_state;								// ���΂̋ɏ��
	D3DXVECTOR3 m_vtxMax;						// ���f���̍ő�l
	D3DXVECTOR3 m_vtxMin;						// ���f���̍ŏ��l
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_nIdxShadow;							// �g�p����e�̔ԍ�
};

#endif