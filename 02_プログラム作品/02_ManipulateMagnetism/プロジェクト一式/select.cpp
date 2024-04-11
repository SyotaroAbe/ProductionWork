//=========================================================
//
// セレクト画面処理 [select.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "select.h"
#include "renderer.h"
#include "fade.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "bg.h"
#include "object2D.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================

//===============================================
// 静的メンバ変数
//===============================================
CBg *CSelect::m_pBg = nullptr;						// 背景クラスのポインタ
CObject2D* CSelect::m_pObject2D = nullptr;			// オブジェクト2Dクラスのポインタ

//===============================================
// コンストラクタ
//===============================================
CSelect::CSelect() : CScene()
{
	// 値のクリア
	m_pTutorial = nullptr;
	m_pFirstStage = nullptr;
	m_pSecondStage = nullptr;
	m_pThirdStage = nullptr;
	m_nTime = 0;
	m_nSelectStage = STAGE_TUTORIAL;
	m_bFade = false;
}

//===============================================
// デストラクタ
//===============================================
CSelect::~CSelect()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CSelect::Init(HWND hWnd)
{
	m_nSelectStage = STAGE_TUTORIAL;

	// カメラの初期化処理
	CManager::GetInstance()->GetCamera()->Init();

	// 背景の生成
	m_pBg = CBg::Create(CBg::TYPE_TITLE);

	// Tutorialを表示
	m_pTutorial = new CObject2D(3);
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->Init({ 300.0f, 150.0f, 0.0f });
		m_pTutorial->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_Tutorial.png"));
		m_pTutorial->SetSize(200.0f, 50.0f);
		m_pTutorial->SetVtx();
	}

	// ステージ１
	m_pFirstStage = new CObject2D(3);
	if (m_pFirstStage != nullptr)
	{
		m_pFirstStage->Init({ 500.0f, 300.0f, 0.0f });
		m_pFirstStage->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_FirstStage.png"));
		m_pFirstStage->SetSize(200.0f, 50.0f);
		m_pFirstStage->SetVtx();
	}

	// ステージ２
	m_pSecondStage = new CObject2D(3);
	if (m_pSecondStage != nullptr)
	{
		m_pSecondStage->Init({ 700.0f, 450.0f, 0.0f });
		m_pSecondStage->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_SecondStage.png"));
		m_pSecondStage->SetSize(200.0f, 50.0f);
		m_pSecondStage->SetVtx();
	}

	// ステージ３
	m_pThirdStage = new CObject2D(3);
	if (m_pThirdStage != nullptr)
	{
		m_pThirdStage->Init({ 900.0f, 600.0f, 0.0f });
		m_pThirdStage->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ui_ThirdStage.png"));
		m_pThirdStage->SetSize(200.0f, 50.0f);
		m_pThirdStage->SetVtx();
	}

	// サウンドの再生
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CSelect::Uninit(void)
{
	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//===============================================
// 更新処理
//===============================================
void CSelect::Update(void)
{
	m_nTime++;	// 時間をカウント

	// 色をリセット
	if (m_pTutorial != nullptr)
	{
		m_pTutorial->SetCol({0.0f, 0.0f, 0.0f, 0.0f});
	}
	if (m_pFirstStage != nullptr)
	{
		m_pFirstStage->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	if (m_pSecondStage != nullptr)
	{
		m_pSecondStage->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	if (m_pThirdStage != nullptr)
	{
		m_pThirdStage->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	// ステージ選択
	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_S) == true
		|| CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_DOWN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_DOWN, 0) == true)
	{
		m_nSelectStage++;
	}
	else if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_W) == true
		|| CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_UP) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_UP, 0) == true)
	{
		m_nSelectStage--;
	}

	if (m_nSelectStage >= STAGE_MAX)
	{
		m_nSelectStage = STAGE_TUTORIAL;
	}
	else if (m_nSelectStage < STAGE_TUTORIAL)
	{
		m_nSelectStage = STAGE_MAX -1;
	}

	switch (m_nSelectStage)
	{
	case STAGE_TUTORIAL:	// チュートリアル
		if (m_pTutorial != nullptr)
		{
			m_pTutorial->Brightness(0.05f);
			m_pFirstStage->SetVtx();
			m_pSecondStage->SetVtx();
			m_pThirdStage->SetVtx();
		}
		break;

	case STAGE_1:			// ステージ１
		if (m_pFirstStage != nullptr)
		{
			m_pTutorial->SetVtx();
			m_pFirstStage->Brightness(0.05f);
			m_pSecondStage->SetVtx();
			m_pThirdStage->SetVtx();
		}
		break;

	case STAGE_2:			// ステージ２
		if (m_pSecondStage != nullptr)
		{
			m_pTutorial->SetVtx();
			m_pFirstStage->SetVtx();
			m_pSecondStage->Brightness(0.05f);
			m_pThirdStage->SetVtx();
		}
		break;

	case STAGE_3:			// ステージ３
		if (m_pThirdStage != nullptr)
		{
			m_pTutorial->SetVtx();
			m_pFirstStage->SetVtx();
			m_pSecondStage->SetVtx();
			m_pThirdStage->Brightness(0.05f);
		}
		break;
	}

	if (CManager::GetInstance()->GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInstance()->GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_A, 0) == true)
	{
		if (m_bFade == false)
		{// フェードバグ防止
			m_bFade = true;

			// 選択中のステージによって遷移先を変える
			switch (m_nSelectStage)
			{
			case STAGE_TUTORIAL:	// チュートリアル
				CRenderer::GetFade()->Set(CScene::MODE_TUTORIAL);	// チュートリアル画面へ移行
				break;

			default:				// ステージ１以降
				CRenderer::GetFade()->Set(CScene::MODE_GAME);	// ゲーム画面へ移行
				break;
			}

			CManager::GetInstance()->SetStage(m_nSelectStage);	// managerにステージ情報を保存

			if (CManager::GetMode() == MODE_GAME)
			{
				m_bFade = true;		// 入んない
			}
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CSelect::Draw(void)
{
	
}