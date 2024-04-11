////=========================================================
////
//// ���b�V���E�H�[���`�揈�� [meshwall.h]
//// Author = �����đ�Y
////
////=========================================================
//#ifndef _MESHWALL_H_  // ���̃}�N����`������ĂȂ�������
//#define _MESHWALL_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����
//
//#include "objectMesh.h"
//
////===============================================
//// �}�N����`
////===============================================
//#define MAX_MESHWALL	(8)			// �ő�ǐ�
//#define WALL_COLISIONX	(-280.0f)	// �Ǔ����蔻��
//#define WALL_COLISIONZ	(4600.0f)	// �Ǔ����蔻��
//#define WALL_SIZE		(-1800.0f)	// �ǃT�C�Y
//
////===============================================
//// ���b�V���E�H�[���N���X
////===============================================
//class CMeshwall : CObjectMesh
//{
//public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
//	CMeshwall();		// �f�t�H���g�R���X�g���N�^
//	~CMeshwall();		// �f�X�g���N�^
//
//	// �ǂ̎��
//	typedef enum
//	{
//		MESHWALLTYPE_NORMAL = 0,	// �ʏ�
//		MESHWALLTYPE_INVISIBLE,		// ����
//		MESHWALLTYPE_MAX
//	}MESHWALLTYPE;
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type);
//
//private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
//	LPDIRECT3DTEXTURE9 g_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
//	LPDIRECT3DVERTEXBUFFER9 g_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
//	LPDIRECT3DINDEXBUFFER9 g_pldxBuff = NULL;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
//
//	D3DXVECTOR3 pos;				// �ʒu
//	D3DXVECTOR3 rot;				// ����
//	int nMeshwallType;				// �^�C�v
//	D3DXMATRIX mtxWorldMeshwall;	// ���[���h�}�g���b�N�X
//};
//
//#endif