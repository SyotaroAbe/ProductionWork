//=========================================================
//
// ���b�V���E�H�[���`�揈�� [meshwall.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MESHWALL_H_  // ���̃}�N����`������ĂȂ�������
#define _MESHWALL_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectMesh.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_MESHWALL	(8)			// �ő�ǐ�
#define WALL_COLISIONX	(-280.0f)	// �Ǔ����蔻��
#define WALL_COLISIONZ	(4600.0f)	// �Ǔ����蔻��
#define WALL_SIZE		(-1800.0f)	// �ǃT�C�Y

//===============================================
// ���b�V���E�H�[���N���X
//===============================================
class CMeshwall : CObjectMesh
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CMeshwall();					// �f�t�H���g�R���X�g���N�^
	CMeshwall(int nPriority = 3);	// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CMeshwall();					// �f�X�g���N�^

	// �ǂ̎��
	typedef enum
	{
		MESHWALLTYPE_NORMAL = 0,	// �ʏ�
		MESHWALLTYPE_INVISIBLE,		// ����
		MESHWALLTYPE_MAX
	}MESHWALLTYPE;

	static CMeshwall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSizeX, float fSizeZ, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type);

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	static int m_nIdxTexture;					// �g�p����e�N�X�`���̔ԍ�
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3 m_PosMin;				// �ʒu�̍ŏ��l
	D3DXVECTOR3 m_PosMax;				// �ʒu�̍ő�l
};

#endif