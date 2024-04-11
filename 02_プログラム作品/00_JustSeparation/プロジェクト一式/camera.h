//=========================================================
//
// �J�������� [camera.h]
// Author = �����đ�Y
//
//=========================================================
#ifndef _CAMERA_H_  // ���̃}�N����`������ĂȂ�������
#define _CAMERA_H_  // �Q�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//===============================================
// �}�N����`
//===============================================
#define ROT_DIFF_PI			(2.0f)	// �p�x�̒l�̕␳
#define ROT_CAMERA			(1.0f)	// �J�����̌����ɍ��킹�ē�����
#define ROT_CAMERA_DIFF		(0.3f)	// �p�x�̕␳�W���i�����傫�����f�����ς��j

//===============================================
// �J�����N���X
//===============================================
class CCamera
{
public:		// �N�ł��A�N�Z�X�\ [�A�N�Z�X�w��q]
	CCamera();	// �f�t�H���g�R���X�g���N�^
	~CCamera();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Set();

	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

private:	// �����̂݃A�N�Z�X�\ [�A�N�Z�X�w��q]
	D3DXMATRIX m_mtxView;		// �r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 m_posV;			// ���_
	D3DXVECTOR3 m_posVDest;		// �ړI�̎��_
	D3DXVECTOR3 m_posR;			// �����_
	D3DXVECTOR3 m_posRDest;		// �ړI�̒����_
	D3DXVECTOR3 m_vecU;			// ������x�N�g��
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_rotDest;		// �ړI�̌���

	int m_nViewMapCounter;		// �}�b�v�^�C���J�E���^
	bool m_bMapCamera;			// �}�b�v�J������Ԃ�
};

#endif