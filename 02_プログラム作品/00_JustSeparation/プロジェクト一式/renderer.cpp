//=========================================================
//
// �����_���[���� [renderer.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"
#include "game.h"
#include "fade.h"

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CFade * CRenderer::m_pFade = NULL;					// �t�F�[�h�N���X�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CRenderer::CRenderer()
{
	// �l���N���A����
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//===============================================
// �f�X�g���N�^
//===============================================
CRenderer::~CRenderer()
{

}

//===============================================
// �������ݒ�
//===============================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;          // �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;   // �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));                            // �p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;                         // �Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                       // �Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;                        // �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;                                    // �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                     // �_�u���o�b�t�@�̐؂�ւ��i�f���p�l���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;                          // �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                    // �f�v�X�o�b�t�@�Ƃ��ĂP�Ubit���g��
	d3dpp.Windowed = bWindow;                                     // �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;   // ���t���b�V�����[�h
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;     // �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �t�F�[�h�̐���
	m_pFade = CFade::Create(CScene::MODE_TITLE, 7);

	// �e��I�u�W�F�N�g�̏���������
	return S_OK;
}

//===============================================
// �I������
//===============================================
void CRenderer::Uninit(void)
{// �e��I�u�W�F�N�g�̏I������
	if (m_pFade != NULL)
	{// �g�p����Ă���
		// �t�F�[�h�̏I������
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	
	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//===============================================
// �X�V����
//===============================================
void CRenderer::Update(void)
{
	if ((CManager::GetMode() == CScene::MODE_GAME && CGame::GetPauseState() == false) || CManager::GetMode() == CScene::MODE_TUTORIAL || CManager::GetMode() == CScene::MODE_TITLE)
	{// �|�[�Y��Ԃ���Ȃ�
		// �S�ẴI�u�W�F�N�g�̍X�V����
		CObject::UpdateAll();
	}

	if (m_pFade != NULL)
	{
		// �t�F�[�h�̍X�V����
		m_pFade->Update();
	}
}

//===============================================
// �`�揈��
//===============================================
void CRenderer::Draw(void)
{
	// ��ʃN���A�i�o�b�N�o�b�t�@�Ƃy�o�b�t�@�̃N���A�j
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ
		// �S�ẴI�u�W�F�N�g�̕`�揈��
		CObject::DrawAll();

		if (m_pFade != NULL)
		{
			// �t�F�[�h�̕`�揈��
			m_pFade->Draw();
		}

		// �f�o�b�O�\���̕`�揈��
		CManager::GetDebugProc()->Draw();

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}