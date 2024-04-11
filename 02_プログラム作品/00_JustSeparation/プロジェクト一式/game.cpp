//=========================================================
//
// ゲーム画面処理 [game.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "game.h"
#include "player.h"
#include "score.h"
#include "time.h"
#include "object3D.h"
#include "objectX.h"
#include "garbage.h"
#include "meshfield.h"
#include "target.h"
#include "pause.h"
#include "input.h"
#include "ui.h"
#include "uigage.h"
#include "renderer.h"
#include "fade.h"
#include "uitarget.h"
#include "camera.h"
#include "debugproc.h"
#include "sound.h"
#include "justdust.h"
#include "fever.h"
#include "hint.h"
#include "gagefull.h"

//===============================================
// 静的メンバ変数
//===============================================
CPlayer *CGame::m_pPlayer = NULL;						// プレイヤークラスのポインタ
CNumber *CGame::m_pNumber = NULL;						// ナンバークラスのポインタ
CTime *CGame::m_pTime = NULL;							// タイムクラスのポインタ
CScore *CGame::m_pScore = NULL;							// スコアクラスのポインタ
CGarbage *CGame::m_pGarbage = NULL;						// ゴミクラスのポインタ
CMeshField *CGame::m_pMeshField = NULL;					// メッシュフィールドクラスのポインタ
CObject3D *CGame::m_pObject3D = NULL;					// オブジェクト3Dクラスのポインタ
CTarget *CGame::m_pTarget = NULL;						// ターゲットクラスのポインタ
CDumpster *CGame::m_pDumpster[MAX_DUMPSTER] = {};		// ゴミステーションクラスのポインタ
CPause *CGame::m_pPause = NULL;							// ポーズクラスのポインタ
CUi *CGame::m_pUi = NULL;								// UIクラスのポインタ
CUiGage *CGame::m_pUiGage = NULL;						// ゴミゲージクラスのポインタ
CUiTarget *CGame::m_pUiTarget = NULL;					// ターゲットUIクラスのポインタ
CJustDust *CGame::m_pJustDust = NULL;					// JustDust表示クラスのポインタ
CFever *CGame::m_pFever = NULL;							// Fever表示クラスのポインタ
CHint *CGame::m_pHint = NULL;							// ヒント表示クラスのポインタ
CGageFull *CGame::m_pGageFull = NULL;					// ゲージフル表示クラスのポインタ

bool CGame::m_bPause = false;				// ポーズ状態
bool CGame::m_bStateReady = false;			// GAMSESTATE_READYかどうか
bool CGame::m_bPauseCamera = false;			// ポーズ時のカメラ操作可能か

//===============================================
// コンストラクタ
//===============================================
CGame::CGame() : CScene()
{
	// 値のクリア
	m_state = STATE_NONE;
	m_nCounterState = 0;
	m_nTimeFever = 0;
}

//===============================================
// デストラクタ
//===============================================
CGame::~CGame()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGame::Init(HWND hWnd)
{
	m_bPause = false;
	m_bStateReady = true;		// 待機状態にする
	m_bPauseCamera = false;

	// カメラの初期化処理
	CManager::GetCamera()->Init();

	// メッシュフィールドの生成
	CMeshField::RandArrange();

	// オブジェクトXファイルの生成
	CObjectX::Load(hWnd);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);

	// ごみの生成
	CGarbage::Load();

	// ゴミステーションの読み込みと生成
	CDumpster::Load(hWnd);

	// ターゲットの生成
	m_pTarget = CTarget::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CGarbage::SEPARATION_NONE, -1, 5);

	// ターゲットUIの生成
	m_pUiTarget = CUiTarget::Create(5);

	// ゲージフルの生成
	m_pGageFull = CGageFull::Create(6);

	// JustDustの生成
	m_pJustDust = CJustDust::Create(5);

	// スコアの生成
	m_pScore = CScore::Create(6);

	// タイムの生成
	m_pTime = CTime::Create(6);

	// ゴミゲージの生成
	m_pUiGage = CUiGage::Create(6);

	// ヒント表示の生成
	m_pHint = CHint::Create();

	// ポーズの生成
	m_pPause = CPause::Create(6);

	// 通常状態に設定
	m_state = STATE_NORMAL;
	m_nCounterState = 0;

	// サウンドの再生
	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGame::Uninit(void)
{
	// タイムの終了処理
	m_pTime->Uninit();
	delete m_pTime;
	m_pTime = NULL;

	// スコアの終了処理
	m_pScore->Uninit();
	delete m_pScore;
	m_pScore = NULL;

	// ゴミゲージの終了処理
	delete m_pUiGage;
	m_pUiGage = NULL;

	// ターゲットUIの終了処理
	delete m_pUiTarget;
	m_pUiTarget = NULL;

	// ポーズの終了処理
	m_pPause->Uninit();
	delete m_pPause;
	m_pPause = NULL;

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//===============================================
// 更新処理
//===============================================
void CGame::Update(void)
{
	if (CManager::GetKeyboardInput()->GetTrigger(DIK_P) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{// ポーズ入力
		m_bPause = m_bPause ? false : true;		// ポーズ状態切り替え

		if (m_bPause == true)
		{
			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE);
		}
		else
		{
			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE_CANCEL);
		}
	}

#if _DEBUG
	if (m_bPause == true && CManager::GetKeyboardInput()->GetTrigger(DIK_F3) == true)
	{// ポーズ中カメラ操作
		m_bPauseCamera = m_bPauseCamera ? false : true;		// ポーズ状態切り替え
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{// BackSpace
		CRenderer::GetFade()->Set(CScene::MODE_RESULT);		// リザルト画面へ移動
	}
#endif

	if (m_bStateReady == false)
	{// 待機状態じゃない
		if (m_bPause == false)
		{// ポーズ状態じゃない
			// タイムの更新処理
			m_pTime->Update();

			// スコアの更新処理
			m_pScore->Update();

			// ゴミゲージの更新処理
			m_pUiGage->Update();

			// ターゲットUIの更新処理
			m_pUiTarget->Update();
		}
	}
	else if (m_bStateReady == true)
	{// 待機状態のフラグが立っている
		if (m_state == STATE_NORMAL)
		{
			// 待機状態へ切り替える
			m_state = STATE_READY;
			m_nCounterState = TIME_STATEREADY;
		}
	}

	if (m_bPause == true && m_bPauseCamera == false)
	{// ポーズ状態
		// ポーズの更新処理
		m_pPause->Update();
	}

	switch (m_state)
	{
	case STATE_NORMAL:  // 通常状態
		m_nCounterState--;

		if (m_pTime->Get() == TIME_FEVER)
		{
			m_state = STATE_FEVER;				// フィーバー状態に設定

			// Feverの生成
			m_pFever = CFever::Create();

			// サウンドの停止
			CManager::GetSound()->Stop(CSound::LABEL_BGM_GAME);

			// サウンドの再生
			CManager::GetSound()->Play(CSound::LABEL_BGM_FEVER);
		}
		break;

	case STATE_READY:	// 開始待機状態
		if (m_bPause == false)
		{// ポーズ状態じゃない
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NORMAL;				// 通常状態に設定
				m_nCounterState = TIME_STATEFEVER;	// 次の状態への時間を設定
				m_bStateReady = false;
			}
		}
		break;

	case STATE_FEVER:	// フィーバー
		m_nTimeFever++;	// 時間をカウント
		break;

	case STATE_END:     // 終了状態
		if (m_bPause == false)
		{// ポーズ状態じゃない
			m_nCounterState--;

			if (m_nCounterState <= 0)
			{
				m_state = STATE_NONE;  // 何もしていない状態に設定
			}
		}
		break;
	}

	// デバッグ表示
	CManager::GetDebugProc()->Print(" 現在の状態：%d\n", m_state);
}

//===============================================
// 描画処理
//===============================================
void CGame::Draw(void)
{
	// タイムの描画処理
	m_pTime->Draw();

	// スコアの描画処理
	m_pScore->Draw();

	// ゴミゲージの描画処理
	m_pUiGage->Draw();
}

//===============================================
// ポーズ状態の設定
//===============================================
void CGame::SetEnablePause(const bool bPause)
{
	m_bPause = bPause;
}