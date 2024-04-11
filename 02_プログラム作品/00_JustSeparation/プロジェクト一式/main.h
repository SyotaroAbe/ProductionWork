//=========================================================
//
// ���C���`�� [main.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _MAIN_H_     // ���̃}�N����`������ĂȂ�������
#define _MAIN_H_     // 2�d�C���N���[�h�h�~�̃}�N����`����

//===============================================
// �}�N����`
//===============================================
#define SCREEN_WIDTH			(1280)                            // �E�C���h�E�̕�
#define SCREEN_HEIGHT			(720)                             // �E�C���h�E�̍���
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D			(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//===============================================
// ���_���[2D]�̍\���̂��`
//===============================================
typedef struct
{
	D3DXVECTOR3 pos;   // ���_���W
	float rhw;         // ���W�ϊ��p�֐��i1.0f�ŌŒ�j
	D3DCOLOR col;      // ���_�J���[
	D3DXVECTOR2 tex;   // �e�N�X�`�����W
}VERTEX_2D;

//===============================================
// ���_���[3D]�̍\���̂��`
//===============================================
typedef struct
{
	D3DXVECTOR3 pos;   // ���_���W
	D3DXVECTOR3 nor;   // �@���x�N�g��
	D3DCOLOR col;      // ���_�J���[
	D3DXVECTOR2 tex;   // �e�N�X�`�����W
}VERTEX_3D;


#endif