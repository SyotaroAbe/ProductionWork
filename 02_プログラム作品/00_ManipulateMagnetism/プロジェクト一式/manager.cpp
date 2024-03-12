//=========================================================
//
// マネージャ処理 [manager.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "xfile.h"
#include "texture.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "meshfield.h"
#include "fileload.h"
#include "game.h"
#include "time.h"
#include "result.h"
#include "ranking.h"
#include "select.h"

//===============================================
// 静的メンバ変数
//===============================================
CManager *CManager::m_pManager = nullptr;				// マネージャクラスのポインタ
CRenderer *CManager::m_pRenderer = nullptr;			// レンダラークラスのポインタ
CInputKeyboard *CManager::m_pKeyboardInput = nullptr;	// キーボード入力クラスのポインタ
CInputGamePad *CManager::m_pInputGamePad = nullptr;	// ゲームパッド入力クラスのポインタ
CDebugProc *CManager::m_pDebugProc = nullptr;			// デバッグ表示クラスのポインタ
CSound *CManager::m_pSound = nullptr;					// サウンドクラスのポインタ
CCamera *CManager::m_pCamera = nullptr;				// カメラクラスのポインタ
CLight *CManager::m_pLight = nullptr;					// ライトクラスのポインタ
CTexture *CManager::m_pTexture = nullptr;				// テクスチャクラスのポインタ
CXFile *CManager::m_pXFile = nullptr;					// Xファイルクラスのポインタ
CFileLoad *CManager::m_pFileLoad = nullptr;			// ファイルロードクラスのポインタ
CScene * CManager::m_pScene = nullptr;					// シーンクラスのポインタ
HWND CManager::m_hWnd = nullptr;						// ウインドウ保存用

CScene::MODE CScene::m_modeOld = CScene::MODE_TITLE;	// 前回の画面モード
CScene::MODE CManager::m_mode = CScene::MODE_TITLE;		// 現在の画面モード
int CScene::m_nTime = 0;								// タイム

//***********************************************************
// シーンクラス
//***********************************************************
//===============================================
// コンストラクタ
//===============================================
CScene::CScene()
{
	
}

//===============================================
// デストラクタ
//===============================================
CScene::~CScene()
{

}

//===============================================
// 生成処理
//===============================================
CScene *CScene::Create(HWND hWnd, MODE mode)
{
	CScene *pScene = nullptr;
	MODE sceneMode = CManager::GetMode();

	// シーンの生成
	switch (mode)
	{
	case MODE_TITLE:	// タイトル画面
		pScene = new CTitle;

		if (sceneMode == MODE_RESULT)
		{
			// サウンドの再生
			CManager::GetInstance()->GetSound()->Stop(CSound::LABEL_BGM_RANKING);
			CManager::GetInstance()->GetSound()->Stop(CSound::LABEL_BGM_TITLE);
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);

			//if (CManager::GetSound()->GetPlay(CSound::LABEL_BGM_TITLE) == false)
			//{// タイトルのBGMが再生されていない
			//	// サウンドの再生
			//	CManager::GetSound()->Play(CSound::LABEL_BGM_TITLE);
			//}
		}
		else
		{
			// サウンドの停止
			CManager::GetInstance()->GetSound()->Stop();

			// サウンドの再生
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);
		}
		break;

	case MODE_SELECT:	// セレクト画面
		pScene = new CSelect;
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
		pScene = new CTutorial;
		break;

	case MODE_GAME:		// ゲーム画面
		pScene = new CGame;
		break;

	case MODE_RESULT:	// リザルト画面
		pScene = new CResult;

		// サウンドの停止
		CManager::GetInstance()->GetSound()->Stop();

		// サウンドの再生
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RANKING);
		break;
	}

	if (pScene != nullptr)
	{// 使用されている
		// シーンの初期化処理
		pScene->Init(hWnd);

		// タイムの情報を渡す
		if (sceneMode == MODE_GAME && m_modeOld == MODE_GAME)
		{// ゲームからゲームへ
			CGame::GetTime()->Set(m_nTime);
		}
		else if (sceneMode == CScene::MODE_RESULT && m_modeOld == CScene::MODE_GAME)
		{// 次のモードがリザルト
			// ランキングの設定
			CResult::GetRanking()->Add(m_nTime);
		}
		else
		{
			m_nTime = 0;	// タイムを初期化
		}

		// モードの設定
		pScene->SetMode(mode);
	}

	return pScene;
}

//===============================================
// シーン設定処理
//===============================================
void CScene::SetMode(MODE mode)
{
	MODE sceneMode = CManager::GetMode();

	m_modeOld = sceneMode;
	//m_mode = mode;
}

//===============================================
// 時間設定処理
//===============================================
void CScene::SetTime(int nTime)
{
	m_nTime = nTime;
}

//***********************************************************
// マネージャクラス
//***********************************************************
//===============================================
// コンストラクタ
//===============================================
CManager::CManager()
{
	// 値のクリア
	m_nCountFPS = 0;
	m_nCntDeathGame = 0;
	m_nStage = 0;
	m_bEdit = false;
}

//===============================================
// デストラクタ
//===============================================
CManager::~CManager()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// ウインドウを保存
	m_hWnd = hWnd;

	// レンダラーの生成
	m_pRenderer = new CRenderer;

	// レンダラーの初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// キーボード入力の生成
	m_pKeyboardInput = new CInputKeyboard;

	// キーボード入力の初期化処理
	if (FAILED(m_pKeyboardInput->Init(hInstance, hWnd)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// ゲームパッド入力の生成
	m_pInputGamePad = new CInputGamePad;

	// ゲームパッド入力の初期化処理
	if (FAILED(m_pInputGamePad->Init(hInstance, hWnd)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// サウンドの生成
	m_pSound = new CSound;

	// サウンドの初期化処理
	if (FAILED(m_pSound->Init(hWnd)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// カメラの生成
	m_pCamera = new CCamera;

	// カメラの初期化処理
	if (FAILED(m_pCamera->Init()))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// ライトの生成
	m_pLight = new CLight;

	// ライトの初期化処理
	if (FAILED(m_pLight->Init()))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// テクスチャの生成
	m_pTexture = new CTexture;

	// テクスチャの読み込み処理
	if (FAILED(m_pTexture->Load()))
	{// 読み込み処理が失敗した場合
		return -1;
	}

	// ロードの生成
	m_pFileLoad = new CFileLoad;

	if (m_pFileLoad != nullptr)
	{// 使用されている
	 // ロードの読み込み処理
		if (FAILED(m_pFileLoad->Init(hWnd)))
		{// 読み込み処理が失敗した場合
			return -1;
		}
	}

	// Xファイルの生成
	m_pXFile = new CXFile;

	// Xファイルの読み込み処理
	if (FAILED(m_pXFile->Load()))
	{// 読み込み処理が失敗した場合
		return -1;
	}

	// シーン生成
	m_pScene = CScene::Create(hWnd, m_mode);

	// デバッグ表示の生成
	m_pDebugProc = new CDebugProc;

	// デバッグ表示の初期化処理
	m_pDebugProc->Init();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CManager::Uninit(void)
{
	if (m_pScene != nullptr)
	{// 使用されている
		// シーンの終了処理
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	if (m_pRenderer != nullptr)
	{// メモリが使用されている
		// キーボード入力の終了処理
		m_pKeyboardInput->Uninit();
		delete m_pKeyboardInput;
		m_pKeyboardInput = nullptr;

		// ゲームパッド入力の終了処理
		m_pInputGamePad->Uninit();
		delete m_pInputGamePad;
		m_pInputGamePad = nullptr;

		// レンダラーの終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;

		// サウンドの終了処理
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;

		// カメラの終了処理
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;

		// ライトの終了処理
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;

		// テクスチャの破棄
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;

		// ファイル読み込みの終了処理
		m_pFileLoad->Uninit();
		delete m_pFileLoad;
		m_pFileLoad = nullptr;

		// Xファイルの破棄
		m_pXFile->Unload();
		delete m_pXFile;
		m_pXFile = nullptr;

		// デバッグ表示の終了処理
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;

		// メモリを開放
		delete m_pRenderer;
		m_pRenderer = nullptr;

		if (m_pManager != nullptr)
		{
			delete m_pManager;
			m_pManager = nullptr;
		}
	}
}

//===============================================
// 更新処理
//===============================================
void CManager::Update(void)
{
	if (m_pRenderer != nullptr)
	{// メモリが使用されている
		// FPSの表示
		m_pDebugProc->Print(" FPS：%d\n\n", m_nCountFPS);

		// キーボード入力の更新処理
		m_pKeyboardInput->Update();
		
		//if (CManager::GetKeyboardInput()->GetTrigger(DIK_R) == true)
		//{// ENTERキー入力
		//	// リセット
		//	CObject::Reset();
		//}
		//else if(CManager::GetKeyboardInput()->GetTrigger(DIK_F2) == true)
		//{// F2キー入力
		//	// エディット切り替え
		//	m_bEdit = m_bEdit ? false : true;
		//	m_pEdit->Set(m_bEdit);
		//}

		// ゲームパッド入力の更新処理
		m_pInputGamePad->Update();

		// レンダラーの更新処理
		m_pRenderer->Update();

		if ((CManager::GetMode() == CScene::MODE_GAME && (CGame::GetPauseState() == false || CGame::GetPauseCamera() == true)
			|| CManager::GetMode() == CScene::MODE_TUTORIAL || CManager::GetMode() == CScene::MODE_TITLE))
		{// ポーズ状態
			// カメラの更新処理
			m_pCamera->Update();
		}

		// ライトの更新処理
		m_pLight->Update();

		// デバッグ表示の更新処理
		m_pDebugProc->Update();

		if (m_pScene != nullptr)
		{
			// シーンの更新処理
			m_pScene->Update();

			//CScene::MODE mode = CScene::GetMode();		// 現在のモードを取得

			if (m_mode == CScene::MODE_TITLE)
			{
				m_pDebugProc->Print(" 現在のモード：タイトル");
			}
			if (m_mode == CScene::MODE_TUTORIAL)
			{
				m_pDebugProc->Print(" 現在のモード：チュートリアル");
			}
			if (m_mode == CScene::MODE_GAME)
			{
				m_pDebugProc->Print(" 現在のモード：ゲーム");
			}
			if (m_mode == CScene::MODE_RESULT)
			{
				m_pDebugProc->Print(" 現在のモード：リザルト");
			}
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{// メモリが使用されている
		// レンダラーの描画処理
		m_pRenderer->Draw();

		if (m_pScene != nullptr)
		{
			// シーンの描画処理
			m_pScene->Draw();
		}
	}
}

//===============================================
// リセット処理
//===============================================
void CManager::Reset(void)
{
	// 敵の生成
	//m_pEnemy->Create(3)->SetPos(D3DXVECTOR3(700.0f, 250.0f, 0.0f));
	//m_pEnemy->Create(3)->SetPos(D3DXVECTOR3(800.0f, 250.0f, 0.0f));

	// アイテムの生成
	//CItem::Create(4)->Set(CItem::TYPE_TREASURE, D3DXVECTOR3(700.0f, 450.0f, 0.0f));
	//CItem::Create(4)->Set(CItem::TYPE_COIN, D3DXVECTOR3(800.0f, 450.0f, 0.0f));
}

//===============================================
// FPS設定処理
//===============================================
void CManager::SetFPS(int nCountFPS)
{
	m_nCountFPS = nCountFPS;
}

//===============================================
// 死亡回数カウント処理
//===============================================
void CManager::AddCountDeath(CScene::MODE mode)
{
	if (mode == CScene::MODE_GAME)
	{// ゲームモード中に死亡
		m_nCntDeathGame++;
	}
}

//===============================================
// シングルトン
//===============================================
CManager* CManager::GetInstance(void)
{
	if (m_pManager == nullptr)
	{
		return m_pManager = new CManager;
	}
	else
	{
		return m_pManager;
	}
}

//===============================================
// シーン設定処理
//===============================================
void CManager::SetMode(CScene::MODE mode)
{
	CScene::MODE modeOld = m_mode;
	m_mode = mode;

	//if (mode == CScene::MODE_TUTORIAL || mode == CScene::MODE_GAME)
	//{
		// サウンドの停止
		GetInstance()->GetSound()->Stop();
	//}

	// シーンを代入
	CScene *pScenePrev = m_pScene;

	//if (mode == CScene::MODE_RESULT && modeOld != CScene::MODE_TITLE)
	//{// 次のモードがリザルト
	//	m_nTime = CGame::GetTime()->Get();
	//}

	// 現在のモードの破棄
	if (pScenePrev != nullptr)
	{
		pScenePrev->Uninit();
	}

	// 新しいモードの生成
	m_pScene = CScene::Create(m_hWnd, mode);

	// メモリの開放
	delete pScenePrev;
	pScenePrev = nullptr;
}

//===============================================
// シーン取得処理
//===============================================
CScene::MODE CManager::GetMode(void)
{
	return m_mode;
}