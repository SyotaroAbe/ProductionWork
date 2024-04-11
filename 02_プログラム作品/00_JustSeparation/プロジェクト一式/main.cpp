//=========================================================
//
// ���C���`�� [main.cpp]
// Author = �����đ�Y
//
//=========================================================
#include "main.h"
#include "manager.h"

//===============================================
// ���C�u�����̃����N
//===============================================
#pragma comment(lib, "d3d9.lib")     // �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")    // [d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")   // DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")    // �V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")  // ���͏����ɕK�v

//===============================================
// �}�N����`
//===============================================
#define CLASS_NAME        "WindowClass"			// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME       "JustSeparation"		// �E�C���h�E�̖��O�i�L���v�V�����ɕ\���j

//===============================================
// �v���g�^�C�v�錾
//===============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//===============================================
// ���C���֐�
//===============================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	// �I�����Ƀ��������[�N���o��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),               // WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,                       // �E�C���h�E�̃X�^�C��
		WindowProc,                       // �E�C���h�E�v���V�[�W��
		0,                                // �O�ɂ���i�ʏ�͎g�p���Ȃ��j
		0,                                // �O�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,                        // �C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),  // �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),      // �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),       // �N���C�A���g�̈�̔w�i�F
		NULL,                             // ���j���[�o�[
		CLASS_NAME,                       // �E�C���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION),  // �t�@�C���̃A�C�R��
	};

	HWND hWnd;											// �E�C���h�E�n���h���i���ʎq�j
	MSG msg;											// ���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// ��ʃT�C�Y�̍\����
	DWORD dwCurrentTime;								// ���ݎ���
	DWORD dwExecLastTime;								// �Ō�ɏ�����������
	DWORD dwFrameCount;									// �t���[���J�E���g
	DWORD dwFPSLastTime;								// �Ō��FPS���v����������

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,             // �g���E�C���h�E�X�^�C��
		CLASS_NAME,                      // �E�C���h�E�N���X�̖��O
		WINDOW_NAME,                     // �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,             // �E�C���h�E�X�^�C��
		CW_USEDEFAULT,                   // �E�C���h�E�̍���X���W
		CW_USEDEFAULT,                   // �E�C���h�E�̍���Y���W
		(rect.right - rect.left),        // �E�C���h�E�̕�
		(rect.bottom - rect.top),        // �E�C���h�E�̍���
		NULL,                            // �e�E�C���h�E�̃n���h��
		NULL,                            // ���j���[�n���h���܂��͎q�E�C���h�E
		hInstance,                       // �C���X�^���X�n���h��
		NULL);                           // �E�C���h�E����f�[�^

	CManager *pManager = NULL;
	pManager = new CManager;

	// �}�l�[�W���̏���������
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;                // ����������
	dwExecLastTime = timeGetTime();   // ���ݎ������擾�i�ۑ��j
	dwFrameCount = 0;                 // ����������
	dwFPSLastTime = timeGetTime();    // ���ݎ������擾�i�ۑ��j

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);    // �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);            // �N���C�A���g�̈���X�V

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);     // ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);      // �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();   // ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				// FPS���v��
				int nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				pManager->SetFPS(nCountFPS);
				dwFPSLastTime = dwCurrentTime;   // FPS���v������������ۑ�
				dwFrameCount = 0;                // �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// �U�O���̂P�b�o��
				dwExecLastTime = dwCurrentTime;		// �����J�n�̎����i���ݎ����j��ۑ�
				dwFrameCount++;						// �t���[���J�E���g���v�Z

				if (pManager != NULL)
				{// ���������g�p����Ă���
					// �}�l�[�W���̍X�V����
					pManager->Update();

					// �}�l�[�W���̕`�揈��
					pManager->Draw();
				}
			}
		}
	}

	if (pManager != NULL)
	{// ���������g�p����Ă���
		// �}�l�[�W���̏I������
		pManager->Uninit();

		// ���������J��
		delete pManager;
		pManager = NULL;
	}

	//����\��߂�
	timeBeginPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===============================================
// �E�C���h�E�v���V�[�W��
//===============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;            // �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:   // �E�C���h�E�j���̃��b�Z�[�W
		// WM_QUIT ���b�Z�[�W�𑗂�
		PostQuitMessage(1);
		break;
	case WM_KEYDOWN:   // �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:   // [ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				// �E�C���h�E��j������iWM_DESTROY���b�Z�[�W�𑗂�j
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_CLOSE:   // ����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			// �E�C���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;   // �O��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	case WM_LBUTTONDOWN:   // �}�E�X���N���b�N�̃��b�Z�[�W
						   // �ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);    // ����̏������J��Ԃ�
}