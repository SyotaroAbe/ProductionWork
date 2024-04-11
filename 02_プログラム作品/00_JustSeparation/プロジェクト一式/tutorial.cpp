//=========================================================
//
// チュートリアル画面処理 [tutorial.cpp]
// Author = 阿部翔大郎
//
//=========================================================
#include "tutorial.h"
#include "renderer.h"
#include "fade.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "score.h"
#include "time.h"
#include "garbage.h"
#include "meshfield.h"
#include "target.h"
#include "uigage.h"
#include "uitarget.h"
#include "dumpster.h"
#include "uioperation.h"
#include "justdust.h"
#include "sound.h"
#include "gagefull.h"

//===============================================
// マクロ定義
//===============================================
#define FIRSTSET_FIELD			(10)			// 床の初期配置数
#define DISTANCE_FIELD			(-1000.0f)		// 床の配置間隔

#define TIME_CREATEFIELD		(116)			// 床の生成時間間隔
#define TIME_CREATEDUMPSTER		(1300)			// ゴミステーションの生成時間間隔
#define POS_CREATEOBJ			(-9120.0f)		// 生成位置間隔

//===============================================
// 静的メンバ変数
//===============================================
CPlayer *CTutorial::m_pPlayer = NULL;			// プレイヤークラスのポインタ
CNumber *CTutorial::m_pNumber = NULL;			// ナンバークラスのポインタ
CScore *CTutorial::m_pScore = NULL;				// スコアクラスのポインタ
CGarbage *CTutorial::m_pGarbage = NULL;			// ゴミクラスのポインタ
CMeshField *CTutorial::m_pMeshField = NULL;		// メッシュフィールドクラスのポインタ
CTarget *CTutorial::m_pTarget = NULL;			// ターゲットクラスのポインタ
CDumpster *CTutorial::m_pDumpster = NULL;		// ゴミステーションクラスのポインタ
CUiGage *CTutorial::m_pUiGage = NULL;			// ゴミゲージクラスのポインタ
CUiTarget *CTutorial::m_pUiTarget = NULL;		// ターゲットUIクラスのポインタ
CUiOperation *CTutorial::m_pUiOperation = NULL;	// 操作方法表示クラスのポインタ
CJustDust *CTutorial::m_pJustDust = NULL;		// JustDust表示クラスのポインタ
CGageFull *CTutorial::m_pGageFull = NULL;		// ゲージフル表示クラスのポインタ

//===============================================
// コンストラクタ
//===============================================
CTutorial::CTutorial() : CScene()
{
	// 値のクリア
	m_nTime = 0;
	m_bFade = false;
}

//===============================================
// デストラクタ
//===============================================
CTutorial::~CTutorial()
{
	
}

//===============================================
// 初期化処理
//===============================================
HRESULT CTutorial::Init(HWND hWnd)
{
	// カメラの初期化処理
	CManager::GetCamera()->Init();

	// メッシュフィールドの生成
	//CMeshField::load(hWnd);
	for (int nCntField = 0; nCntField < FIRSTSET_FIELD; nCntField++)
	{
		CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f + nCntField * DISTANCE_FIELD), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, CMeshField::TYPE_STRAIGHT_H, 3);
	}

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 4);

	// ごみの生成
	CGarbage::Load();

	// ゴミステーションの読み込みと生成
	CDumpster::Load(hWnd);
	m_pDumpster = CDumpster::Create(D3DXVECTOR3(300.0f, 0.0f, POS_CREATEOBJ), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 2);

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

	// ゴミゲージの生成
	m_pUiGage = CUiGage::Create(6);

	// 操作方法表示の生成
	for (int nCntOperation = 0; nCntOperation < CUiOperation::TEX_MAX; nCntOperation++)
	{
		m_pUiOperation = CUiOperation::Create(nCntOperation, 6);
	}

	// サウンドの再生
	CManager::GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{
	if (m_pScore != NULL)
	{
		// スコアの終了処理
		m_pScore->Uninit();
		delete m_pScore;
		m_pScore = NULL;
	}

	if (m_pUiGage != NULL)
	{
		// ゴミゲージの終了処理
		delete m_pUiGage;
		m_pUiGage = NULL;
	}

	if (m_pUiTarget != NULL)
	{
		// ターゲットUIの終了処理
		delete m_pUiTarget;
		m_pUiTarget = NULL;
	}

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//===============================================
// 更新処理
//===============================================
void CTutorial::Update(void)
{
	m_nTime++;	// 時間をカウント

	if ((m_nTime % TIME_CREATEFIELD) == 0)
	{
		D3DXVECTOR3 playerPos = GetPlayer()->GetPos();		// プレイヤーの位置

		// メッシュフィールドの生成
		CMeshField::Create(D3DXVECTOR3(playerPos.x, playerPos.y, playerPos.z + POS_CREATEOBJ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 500.0f, 500.0f, CMeshField::TYPE_STRAIGHT_H, 2);
	}
	if ((m_nTime % TIME_CREATEDUMPSTER) == 0)
	{
		D3DXVECTOR3 playerPos = GetPlayer()->GetPos();		// プレイヤーの位置

		m_pDumpster = CDumpster::Create(D3DXVECTOR3(300.0f, playerPos.y, playerPos.z + POS_CREATEOBJ), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 2);
	}

	if (CManager::GetKeyboardInput()->GetTrigger(DIK_RETURN) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_START, 0) == true)
	{
		if (m_bFade == false)
		{// フェードバグ防止
			CRenderer::GetFade()->Set(CScene::MODE_GAME);	// ゲーム画面へ移行
			m_bFade = true;
		}
	}
	else if (CManager::GetKeyboardInput()->GetTrigger(DIK_BACKSPACE) == true
		|| CManager::GetInputGamePad()->GetTrigger(CInputGamePad::BUTTON_BACK, 0) == true)
	{
		if (m_bFade == false)
		{// フェードバグ防止
			CRenderer::GetFade()->Set(CScene::MODE_TUTORIAL);	// チュートリアル画面へ移行
			m_bFade = true;
		}
	}


	if (m_pScore != NULL)
	{
		// スコアの更新処理
		m_pScore->Update();
	}

	if (m_pUiGage != NULL)
	{
		// ゴミゲージの更新処理
		m_pUiGage->Update();
	}

	if (m_pUiTarget != NULL)
	{
		// ターゲットUIの更新処理
		m_pUiTarget->Update();
	}
}

//===============================================
// 描画処理
//===============================================
void CTutorial::Draw(void)
{
	if (m_pScore != NULL)
	{
		// スコアの描画処理
		m_pScore->Draw();
	}

	if (m_pUiGage != NULL)
	{
		// ゴミゲージの描画処理
		m_pUiGage->Draw();
	}
}