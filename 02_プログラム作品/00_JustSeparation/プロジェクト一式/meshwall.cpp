////=========================================================
////
//// ���b�V���E�H�[���`�揈�� [meshwall.cpp]
//// Author = �����đ�Y
////
////=========================================================
//#include "main.h"
//#include "meshwall.h"
//#include "bullet.h"
//#include "camera.h"
//
////===============================================
//// �R���X�g���N�^
////===============================================
//CMeshwall::CMeshwall() : CObjectMesh(1)
//{
//	//�l���N���A����
//	
//}
//
////===============================================
//// �R���X�g���N�^�i�I�[�o�[���[�h�j
////===============================================
//CMeshwall::CMeshwall(int nPriority) : CObjectMesh(nPriority)
//{
//	// �l���N���A����
//	
//}
//
////===============================================
//// �f�X�g���N�^
////===============================================
//CMeshwall::~CMeshwall()
//{
//
//}
//
////===============================================
//// ����������
////===============================================
//HRESULT CMeshwall::Init(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();   // �f�o�C�X�̎擾
//
//	// �e�N�X�`���̓ǂݍ���
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\Audience001.png",
//		&g_pTexture);
//
//	// �ϐ��̏�����
//	g_aMeshWall[nCntMeshWall].pos = D3DXVECTOR3(0.0f, 0.0f, 200.0f);
//	g_aMeshWall[nCntMeshWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	// ���_�o�b�t�@�̐���
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9 * MAX_MESHWALL,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_3D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuff,
//		NULL);
//
//	VERTEX_3D * pVtx;   // ���_���ւ̃|�C���^
//
//	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
//	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// ���_���W�̐ݒ�
//	pVtx[0].pos = D3DXVECTOR3(-1800.0f, 800.0f, 0.0f);
//	pVtx[1].pos = D3DXVECTOR3(0.0f, 800.0f, 0.0f);
//	pVtx[2].pos = D3DXVECTOR3(1800.0f, 800.0f, 0.0f);
//	pVtx[3].pos = D3DXVECTOR3(-1800.0f, 400.0f, 0.0f);
//	pVtx[4].pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
//	pVtx[5].pos = D3DXVECTOR3(1800.0f, 400.0f, 0.0f);
//	pVtx[6].pos = D3DXVECTOR3(-1800.0f, 0.0f, 0.0f);
//	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	pVtx[8].pos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
//
//	// �@���x�N�g���̐ݒ�
//	for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//	{
//		pVtx[nCntMesh].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//	}
//
//	// ���_�J���[�̐ݒ�
//	for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//	{
//		pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	}
//
//	// �e�N�X�`�����W�̐ݒ�
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
//	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
//	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
//	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
//	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuff->Unlock();
//
//	// �C���f�b�N�X�o�b�t�@�̐���
//	pDevice->CreateIndexBuffer(sizeof(WORD) * 14 * MAX_MESHWALL,
//		D3DUSAGE_WRITEONLY,
//		D3DFMT_INDEX16,
//		D3DPOOL_MANAGED,
//		&g_pldxBuff,
//		NULL);
//
//	WORD * pldx;	// �C���f�b�N�X���ւ̃|�C���^
//
//	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
//	g_pldxBuff->Lock(0, 0, (void**)&pldx, 0);
//
//	// ���_�ԍ��f�[�^�̐ݒ�
//	pldx[0] = 3;
//	pldx[1] = 0;
//	pldx[2] = 4;
//	pldx[3] = 1;
//	pldx[4] = 5;
//	pldx[5] = 2;
//	pldx[6] = 2;
//	pldx[7] = 6;
//	pldx[8] = 6;
//	pldx[9] = 3;
//	pldx[10] = 7;
//	pldx[11] = 4;
//	pldx[12] = 8;
//	pldx[13] = 5;
//
//	// �C���f�b�N�X�o�b�t�@���A�����b�N����
//	g_pldxBuff->Unlock();
//
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, -2600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, 4600.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(-1800.0f, 0.0f, 8200.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, -2600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, 4600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//	Set(D3DXVECTOR3(1300.0f, 0.0f, 8200.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), MESHWALLTYPE_NORMAL);
//}
//
////===============================================
//// �I������
////===============================================
//void CMeshwall::Uninit(void)
//{
//	// �e�N�X�`���̔j��
//	if (g_pTexture != NULL)
//	{
//		g_pTexture->Release();
//		g_pTexture = NULL;
//	}
//
//	// ���_�o�b�t�@�̔j��
//	if (g_pVtxBuff != NULL)
//	{
//		g_pVtxBuff->Release();
//		g_pVtxBuff = NULL;
//	}
//
//	// �C���f�b�N�X�o�b�t�@�̔j��
//	if (g_pldxBuff != NULL)
//	{
//		g_pldxBuff->Release();
//		g_pldxBuff = NULL;
//	}
//}
//
////===============================================
//// �X�V����
////===============================================
//void CMeshwall::Update(void)
//{
//
//}
//
////===============================================
//// �`�揈��
////===============================================
//void CMeshwall::Draw(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
//	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
//
//	// ���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&g_aMeshWall[nCntMeshWall].mtxWorld);
//
//	// �����𔽉f
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[nCntMeshWall].rot.y, g_aMeshWall[nCntMeshWall].rot.x, g_aMeshWall[nCntMeshWall].rot.z);
//	D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxRot);
//
//	// �ʒu�𔽉f
//	D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[nCntMeshWall].pos.x, g_aMeshWall[nCntMeshWall].pos.y, g_aMeshWall[nCntMeshWall].pos.z);
//	D3DXMatrixMultiply(&g_aMeshWall[nCntMeshWall].mtxWorld, &g_aMeshWall[nCntMeshWall].mtxWorld, &mtxTrans);
//
//	// ���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[nCntMeshWall].mtxWorld);
//
//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, g_pVtxBuff, 0, sizeof(VERTEX_3D));
//	pDevice->SetIndices(g_pldxBuff);
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// �e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, g_pTexture);
//
//	// �|���S���̕`��
//	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
//		0,
//		0,
//		9,
//		0,
//		12);
//}
//
////===============================================
//// �ݒ菈��
////===============================================
//void CMeshwall::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
//{
//	VERTEX_3D * pVtx;   // ���_���ւ̃|�C���^
//
//	// ���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
//	g_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	g_aMeshWall[nCntMeshWall].pos = pos;
//	g_aMeshWall[nCntMeshWall].rot = rot;
//	g_aMeshWall[nCntMeshWall].nType = type;
//
//	// GetWall(g_aMeshWall[0].pos + pVtx[0].pos, g_aMeshWall[0].pos + pVtx[8].pos, 0);
//
//	// GetWall(D3DXVECTOR3(g_aMeshWall[nCntMeshWall].pos.x + sinf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[0].pos.x,
//	// 					g_aMeshWall[nCntMeshWall].pos.y,
//	// 					g_aMeshWall[nCntMeshWall].pos.z + sinf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[0].pos.z),
//	// 		D3DXVECTOR3(g_aMeshWall[nCntMeshWall].pos.x + cosf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[8].pos.x,
//	// 					g_aMeshWall[nCntMeshWall].pos.y,
//	// 					g_aMeshWall[nCntMeshWall].pos.z + cosf(g_aMeshWall[nCntMeshWall].rot.y + (ROT_CAMERA * pCamera->rot.y)) * pVtx[8].pos.z),
//	// 					nCntMeshWall);
//
//	if (g_aMeshWall[nCntMeshWall].nType == MESHWALLTYPE_NORMAL)
//	{
//		// ���_�J���[�̐ݒ�
//		for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//		{
//			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		}
//
//		// �e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
//		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
//		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
//		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
//		pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		break;
//	}
//	else if (g_aMeshWall[nCntMeshWall].nType == MESHWALLTYPE_INVISIBLE)
//	{
//		// ���_�J���[�̐ݒ�
//		for (nCntMesh = 0; nCntMesh < 9; nCntMesh++)
//		{
//			pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
//		}
//
//		// �e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.5f, 1.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
//		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
//		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.5f);
//		pVtx[6].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[7].tex = D3DXVECTOR2(0.5f, 0.0f);
//		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
//
//		break;
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuff->Unlock();
//}